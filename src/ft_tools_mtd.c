#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/* uintX_t */
#include <stdint.h>
#include <sys/types.h>
/* file ctl */
#include <fcntl.h>
#include <unistd.h>
/* for safe mtd ctl */
#include <sys/ioctl.h> 
#include <mtd/mtd-abi.h>
/* malloc */
#include <malloc.h>

#include "ft_common.h"

/* 
func : public func about mtd_write and mtd_read (safe mode is default )
*/

//#define MTD_DEBUG

typedef struct sc_mtd_opt_and_mtd_info_t{
	struct mtd_info_user mtd;
	int total_blk;
	char file_mtd[9+1];		//strlen("/dev/mtdX") == 9 -> FIX 9 + '\0'
	long int offs;
	char file_mtdblock[14+1];	//strlen("/dev/mtdblockX") == 14 + '\0'
	long int len;
	char * buf;
}sc_cmd_t;

typedef struct sc_bad_block_info_t{
	unsigned int num;
	unsigned int bad;//BBT_YES/BBT_NO
	long int start;
	long int end;
}bbt_info_t;

static bbt_info_t *bbt=NULL;
enum bbt_sta{
	BBT_NO=0,
	BBT_YES=1,
	BBT_END
};


/************************* private func api ************************/
static char mtd_path_tmp[20];
static sc_cmd_t cmd ;
/* func */
/* 1. init cmd info and create bbt */
static int init_cmd_info(sc_cmd_t *cmd,char *file,long int offs,long int len,char *buf);
static int init_bbt(sc_cmd_t *cmd);
/* tools */
/* change mtdblock_to_mtd or mtd_to_mtdblock*/
static char * mtdblock_to_mtd(char * mtd_path);
static char * mtd_to_mtdblock(char * mtd_path);
static int get_mtd_info(char * mtd_path , struct mtd_info_user * mtd);
/* check bad blk */
static int get_good_blk_by_offs(sc_cmd_t *cmd);
static int free_all(sc_cmd_t *cmd);

/************************* public func api ************************/
/*
 * func		:	serach mtd_num by 'name'
 * input	:	name	-> name of MTD
 */
/* EG: get_mtd_num_by_mtd_name("0:ART")  */
int get_mtd_num_by_mtd_name(char * name){
	/* FIXME : use ioctl() or /proc/mtd */
	FILE *fp = NULL;
	char rbuf[64];
	char *pbuf = NULL;
	int i = 0; // index 
	int flag_find_mtd = 0;
	int num = -1;
	int len = strlen(name);

	fp = fopen("/proc/mtd","r");
	if(fp == NULL ){
		printf("ERROR : open /proc/mtd fail!\n");
		exit(-1);
	}
	fseek(fp,0,SEEK_SET);
	while(fgets(rbuf,64,fp)!=NULL){
		if(strstr(rbuf,name) != NULL){
			pbuf = rbuf;
			//printf("DEBUG : rbuf = %s\n",rbuf);
			//printf("DEBUG : pbuf = %s\n",pbuf);
			i = 0;
			/* find start offs */
			while(rbuf[i++]!='"');
			//printf("DEBUG : rbuf[i] = %c\n",rbuf[i]);
			//printf("DEBUG : strlen(rbuf) - i - 1 - 1 = %d\n",strlen(rbuf) - i - 1 - 1);
			//printf("DEBUG : len = %d\n",len);
			/* check len equal or not */
			if(strlen(rbuf) - i - 1 - 1 != len){
				continue;
			}
			/* cmp input_name and mtd_name */
			if(strncmp(name,pbuf+i,len) == 0){
				flag_find_mtd = 1;
				break;
			}else{
				continue;
			}
		}
	}

	/* close FILE */
	if(fp!=NULL)
		fclose(fp);

	if(flag_find_mtd == 1){
		/* get num from buffer */
		//printf("DEBUG : rbuf = %s\n",rbuf);
		i = 3;//mtd10: 00080000 00010000 "0:ART"
		num = 0;
		while(rbuf[i] != ':'){
			if(rbuf[i] >='0' && rbuf[i] <= '9'){
				num = num * 10 + (rbuf[i] - '0');
			}
			i++;
		}
		//printf("DEBUG : get mtd_num = %d\n",num);
		if(num>=0 && num <= 999){// mtd_num >=0 && mtd_num <= 999
			return num ;
		}else{
			printf("Error , mtd num should between 0 and 999!!! \n");
			exit(-1);
		}
	}else{
		printf("Error , can not find mtd_num named \'%s\'!!!\n",name);
		exit(-1);
		//return SC_FTCMD_NG;
	}
}

/*
 * func		:	write data into mtd flash
 * input	:	file	-> /dev/mtdblockX or /dev/mtdX or X
 *				off		-> offset in file like 0x10000
 *				len		-> the length of buffer
 *				wbuf	-> buffer for writing
 */
/* EG: mtd_write("/dev/mtdblock9"	,0x20	,0x10	,wbuf)  */
/* EG: mtd_write("/dev/mtdblock9"	,32		,16		,wbuf)  */
int mtd_write(char * file ,long int off ,long int len,char *wbuf){
	uint8_t fd;
	int w_len = 0;
	int blk = 0;
	/* init cmd  and bbt */
	init_cmd_info(&cmd,file,off,len,wbuf);
	init_bbt(&cmd);
	if(cmd.len < strlen(wbuf)){
		printf("Waring : The actual write length is less than the length of the buffer.\n");
		printf("Waring : len < the length of the wbuf.\n");
	}
	if((fd=open(cmd.file_mtdblock,O_RDWR))<0){
		printf("ERROR : open mtd fail!\n");
		if(cmd.buf)	free(cmd.buf);
		free_all(&cmd);
		return SC_FTCMD_NG;
	}
	//cmd.mtd.erasesize = 8;//DEBUG 
	while(cmd.len>0){
		blk = get_good_blk_by_offs(&cmd);
#ifdef MTD_DEBUG
		printf("DEBUG :  bbt[blk].start : 0x%x\n", bbt[blk].start);
		printf("DEBUG :  cmd.offs		: 0x%x\n", cmd.offs);
#endif
		w_len = (cmd.len < (bbt[blk].end - cmd.offs + 0x1)) ? cmd.len : (bbt[blk].end - cmd.offs + 0x1);
#ifdef MTD_DEBUG
		printf("DEBUG : write offs  is 0x%x\n",cmd.offs);
		printf("DEBUG : write w_len is 0x%x\n",w_len);
#endif
		lseek(fd,cmd.offs,SEEK_SET);
		if(write(fd,cmd.buf,w_len)<0){
			printf("ERROR : write into mtd fail!\n"); 
			if(cmd.buf)	free(cmd.buf);
			free_all(&cmd);
			return SC_FTCMD_NG;
		}
		cmd.offs += w_len;
		cmd.buf += w_len;
		cmd.len -= w_len;
	}

	if(fd)
		close(fd);
	free_all(&cmd);
	return SC_FTCMD_OK;
}

/*
 * func		:	read data from mtd flash
 * input	:	file	-> /dev/mtdblockX
 *				off		-> offset in file like 0x10000
 *				len		-> the length of buffer
 *				rbuf	-> buffer for writing
 * ruturn	:	SC_FTCMD_OK /SC_FTCMD_NG
 */
/* EG: mtd_read("/dev/mtdblock9"	,0x20	,0x10	,rbuf)  */
/* EG: mtd_read("/dev/mtdblock9"	,32		,16		,rbuf)  */
int mtd_read(char * file , long int off,long int len , char * rbuf){
	uint8_t fd;
	int r_len =0;
	int blk=0;
	/* init cmd  and bbt */
	init_cmd_info(&cmd,file,off,len,rbuf);
	init_bbt(&cmd);
	if((fd=open(cmd.file_mtdblock,O_RDWR))<0){
		printf("ERROR : open mtd fail!\n");
		free_all(&cmd);
		return SC_FTCMD_NG;
	}
	while(cmd.len>0){
		blk = get_good_blk_by_offs(&cmd);
#ifdef MTD_DEBUG
		printf("DEBUG :  bbt[blk].start : 0x%x\n", bbt[blk].start);
		printf("DEBUG :  cmd.offs       : 0x%x\n", cmd.offs);
#endif
		r_len = (cmd.len < (bbt[blk].end - cmd.offs + 0x1)) ? cmd.len : (bbt[blk].end - cmd.offs + 0x1);
#ifdef MTD_DEBUG
		printf("DEBUG : read offs  is 0x%x\n",cmd.offs);
		printf("DEBUG : read r_len is 0x%x\n",r_len);
#endif
		lseek(fd,cmd.offs,SEEK_SET);
		if(read(fd,cmd.buf,r_len)<0){
			printf("ERROR : read mtd fail!\n"); 
			free_all(&cmd);
			return SC_FTCMD_NG;
		}
		//printf("DEBUG : read buf is %s \n",cmd.buf);
		cmd.offs += r_len;
		cmd.buf += r_len;
		cmd.len -= r_len;
	}
	if(fd)
		close(fd);

#ifdef MTD_DEBUG
	printf("DEBUG : read buf is %s \n",cmd.buf);
#endif
	free_all(&cmd);
	return SC_FTCMD_OK;
}


/************************* private func api ************************/
/* 
 * func		:	create cmd(info about cmd_opt and mtd_info) 
 * input	:	sc_cmd_t  , file_path , offs , len , buf
 * ruturn	:	SC_FTCMD_OK /SC_FTCMD_NG
 */
static int init_cmd_info(sc_cmd_t *cmd,char *file,long int offs,long int len, char *buf)
{
	memset(cmd,'\0',sizeof(sc_cmd_t));
	memcpy(cmd->file_mtd		,	mtdblock_to_mtd(file)	,	sizeof(cmd->file_mtd)	);// 9+1
	memcpy(cmd->file_mtdblock	,	mtd_to_mtdblock(file)	,	sizeof(cmd->file_mtdblock)	);//14+1
	cmd->offs = offs;
	cmd->len = len;
	get_mtd_info(cmd->file_mtd,&(cmd->mtd));
	cmd->total_blk = cmd->mtd.size / cmd->mtd.erasesize;
	/* parameters check */
	if((offs >= cmd->mtd.size) || (len >= cmd->mtd.size) || offs + len > cmd->mtd.size ||
			access(cmd->file_mtd,F_OK) || access(cmd->file_mtdblock,F_OK))
	{
		printf("Waring : input para illgal ! %s %s()\n",__FILE__,__func__);
		free_all(cmd);
		exit(-1);
	}
	if(buf[0]!='\0'){//write
		cmd->buf = buf;
	}else{//read
		memset(buf,'\0',len);
		cmd->buf = buf;
	}

#ifdef MTD_DEBUG //debug show cmd_info
	printf("\nDEBUG : show cmd_info\n");
	printf("DEBUG : cmd->total_blk         : %d\n",cmd->total_blk);
	printf("DEBUG : cmd->mtd.size          : 0x%x\n",cmd->mtd.size);
	printf("DEBUG : cmd->mtd.erasesize     : 0x%x\n",cmd->mtd.erasesize);
	printf("DEBUG : cmd->file_mtd      : %s\n",cmd->file_mtd);
	printf("DEBUG : cmd->file_mtdblock : %s\n",cmd->file_mtdblock);
	printf("DEBUG : cmd->offs          : %ld\n",cmd->offs);
	printf("DEBUG : cmd->len           : %ld\n",cmd->len);
	printf("DEBUG : cmd->len           : 0x%x\n",cmd->len);
	printf("DEBUG : cmd->buf           : %s\n",cmd->buf);
#endif
	return SC_FTCMD_OK;
}
/* 
 * func		:	create bbt(bad block table) 
 * input	:	sc_cmd_t
 * return	:	SC_FTCMD_OK / SC_FTCMD_NG
*/
static int init_bbt(sc_cmd_t *cmd){
	//int total_block_of_mtd = cmd->mtd.size / cmd->mtd.erasesize ;
	int idx=0;
	//bbt = (bbt_info_t *)malloc(sizeof(bbt_info_t)*(total_block_of_mtd));
	bbt = (bbt_info_t *)malloc(sizeof(bbt_info_t)*(cmd->total_blk));
	/* get mtd_info then create bad block tables */
	int fd;
	long int offs = 0;//TIP 1
	if ((fd = open(cmd->file_mtd, O_RDWR)) < 0) {
		printf("ERROR : can not open file %s\n",cmd->file_mtd);
		free_all(cmd);
		exit(-1);
	}

	for(idx = 0;idx < cmd->total_blk ; idx++){
		if(ioctl(fd, MEMGETBADBLOCK, (idx *cmd->mtd.erasesize)) > 0){
			printf("WARING : find bad blk at 0x%08x ...\n",idx * cmd->mtd.erasesize);
			bbt[idx].num = idx;
			bbt[idx].bad = BBT_YES;
		}else{
			//printf("DEBUG : find good blk at 0x%08x ...\n",idx *cmd->mtd.erasesize);
			bbt[idx].num = idx;
			bbt[idx].bad = BBT_NO;
		}
		bbt[idx].end	= (idx+1) * cmd->mtd.erasesize - 0x1;
		bbt[idx].start	= bbt[idx].end - (cmd->mtd.erasesize - 0x1);
	}
	if(fd)	close(fd);
#ifdef MTD_DEBUG //debug show bbt info 
	int i=0;
	for(i=0;i<cmd->total_blk;i++){
		printf("DEBUG : check bbt[%d].bad   : %d\n",i,bbt[i].bad);
		printf("DEBUG : check bbt[%d].start : 0x%x\n",i,bbt[i].start);
		printf("DEBUG : check bbt[%d].end   : 0x%x\n",i,bbt[i].end);
	}
#endif
	return SC_FTCMD_OK;
}
static int free_all(sc_cmd_t * cmd){
	/* FIXME */
	//if(cmd->buf)	free(cmd->buf);
	if(bbt)	free(bbt);
}
/********************** other tools *********************/
static char * mtdblock_to_mtd(char * mtd_path)
{
	/* change mtdblock -> mtd */
	int tmp_len = strlen(mtd_path);
	if(tmp_len <= 0){
		printf("Error , get mtd_path NULL!!!\n");
		exit(-1);
	}
	memset(mtd_path_tmp,'\0',sizeof(mtd_path_tmp));
	if(mtd_path[tmp_len-1]>='0' && mtd_path[tmp_len-1]<='9'){
		if(mtd_path[tmp_len-2]>='0' && mtd_path[tmp_len-2]<='9'){
			sprintf(mtd_path_tmp,"/dev/mtd%c%c",mtd_path[tmp_len-2],mtd_path[tmp_len-1]);
		}else{
			sprintf(mtd_path_tmp,"/dev/mtd%c",mtd_path[tmp_len-1]);
		}
	}else{
		printf("ERROR : use illegal mtd_path\n");
		exit(-1);
	}
	//printf("LIAM : mtd_path_tmp = %s\n",mtd_path_tmp);
	return mtd_path_tmp;
}
static char * mtd_to_mtdblock(char * mtd_path)
{
	/* change mtdblock -> mtd */
	int tmp_len = strlen(mtd_path);
	if(tmp_len <= 0){
		printf("Error , get mtd_path NULL!!!\n");
		exit(-1);
	}
	memset(mtd_path_tmp,'\0',sizeof(mtd_path_tmp));
	if(mtd_path[tmp_len-1]>='0' && mtd_path[tmp_len-1]<='9'){
		if(mtd_path[tmp_len-2]>='0' && mtd_path[tmp_len-2]<='9'){
			sprintf(mtd_path_tmp,"/dev/mtdblock%c%c",mtd_path[tmp_len-2],mtd_path[tmp_len-1]);
		}else{
			sprintf(mtd_path_tmp,"/dev/mtdblock%c",mtd_path[tmp_len-1]);
		}
	}else{
		printf("ERROR : use illegal mtd_path\n");
		exit(-1);
	}
	//printf("LIAM : mtd_path_tmp = %s\n",mtd_path_tmp);
	return mtd_path_tmp;
}

/* 
 * func		:	get info about mtd 
 * input	:	/dev/mtdx 
 *			:	srtuct mtd_info_user
 * retrurn	:	SC_FTCMD_OK / SC_FTCMD_NG
 */
static int get_mtd_info(char * mtd_path, struct mtd_info_user * mtd)
{
	int fd;
	//loff_t offs = 0;
	long int offs = 0;
	if ((fd = open(mtd_path, O_RDWR)) < 0) {
		printf("ERROR : can not open file %s\n",mtd_path);
		exit(-1);
	}
	ioctl(fd, MEMGETINFO,mtd);
	if(fd)	close(fd);
	return SC_FTCMD_OK;
}


/*
 * func   : skip bad block  
 * input  : sc_cmd_t 
 * output : return the num of first good blk
 */
static int get_good_blk_by_offs(sc_cmd_t *cmd){
	long int offs = 0;
	int i = (cmd->offs+1) / cmd->mtd.erasesize;
#ifdef MTD_DEBUG
	printf("DEBUG : get cmd->offs           : 0x%x\n",cmd->offs);
	printf("DEBUG : get cmd->mtd.erasesize  : 0x%x\n",cmd->mtd.erasesize);
	printf("DEBUG : set i                   : %d\n",i);
#endif
	for(;i<cmd->total_blk;i++){
		if(bbt[i].bad == BBT_YES){
			printf("Tips : skip bad blk at 0x%x\n",bbt[i].num * cmd->mtd.erasesize);
			offs += cmd->mtd.erasesize;
		}else{
#ifdef MTD_DEBUG
			printf("DEBUG : --> find good blk at 0x%x\n",bbt[i].num * cmd->mtd.erasesize);
#endif
			break;
		}
	}
	if(i >= cmd->total_blk){
		printf("Error : can not find good blk on this mtd!!!\n");
		free_all(cmd);
		exit(-1);
	}
	return i;
}
