#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "ft_common.h"
/* 
func : pcba
*/

#define SC_PCBA_MTD_NAME "0:MFINFO"
#define SC_PCBA_OFFS 0x1020
#define SC_PCBA_LEN 12


/*100	*/	int ftcmd_set_pcba(char * name, char * para)
{
	/* FIXME */
	/* use SC_PCBA_LEN or strlen(para) ??? */
	char file[32];
	int num = get_mtd_num_by_mtd_name(SC_PCBA_MTD_NAME);
	memset(file,'\0',sizeof(file));
	sprintf(file,"/dev/mtdblock%d",num);
	if(mtd_write(file,SC_PCBA_OFFS,strlen(para),para) == SC_FTCMD_OK)
		printf("RET : write done!\n");
	return SC_FTCMD_OK;
}

/*400	*/	int ftcmd_get_pcba(char * name, char * para)
{
	/* Tips : use buf[] or malloc() */
	char * rbuf =NULL;
	if((rbuf = (char *)malloc(sizeof(char)*(SC_PCBA_LEN+1))) == NULL){
		printf("DEBUG : malloc faile!\n");
		return SC_FTCMD_NG;
	}
	char file[32];
	int num = get_mtd_num_by_mtd_name(SC_PCBA_MTD_NAME);
	memset(file,'\0',sizeof(file));
	sprintf(file,"/dev/mtdblock%d",num);
	memset(rbuf,'\0',SC_PCBA_LEN+1);
	//mtd_read(SC_MAC_FILE,SC_MAC_OFFS,SC_MAC_LEN,rbuf);
	mtd_read(file,SC_PCBA_OFFS,SC_PCBA_LEN,rbuf);
	//printf("DEBUG : get rbuf %s\n",rbuf);
	printf("%s\n",rbuf);
	if(rbuf)    free(rbuf);


	return SC_FTCMD_OK;
}
