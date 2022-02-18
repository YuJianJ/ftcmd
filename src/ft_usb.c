#include <stdio.h>
#include "ft_common.h"
/*
 func : usb 
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* cmd : set */
static int set_usb_write(void);
static int set_usb_read(void);
static int set_usb_test(void);
static int init_usb(void);
static int is_mount_already(void);

static char dev_f[12];//path of usb dev
static const char w_buf[18]="0123456789abcdefg";
static char r_buf[18];

static int set_usb_write(void){
	FILE *fp = NULL;
	int ret = -1;
	if((fp=fopen("/mnt/usb_test/ftcmd.log","w+")) == NULL){
		printf("Error , can not open file %s!\n",dev_f);
		//return SC_FTCMD_NG;
		exit(-1);
	}
	ret = fwrite(w_buf , 1 , sizeof(w_buf) , fp);
	fclose(fp);
	if(ret == sizeof(w_buf)){
		//printf("LIAM : write down!\n");
		printf("DEBUG : write buf = %s\n",w_buf);
		return SC_FTCMD_OK;
	}else{
		return SC_FTCMD_NG;
	}
}
static int set_usb_read(void){
	FILE *fp = NULL;
	int ret = -1;
	if((fp=fopen("/mnt/usb_test/ftcmd.log","r+")) == NULL){
		printf("Error , can not open file %s!\n",dev_f);
		//return SC_FTCMD_NG;
		exit(-1);
	}
	ret = fread(r_buf , 1 , sizeof(r_buf) , fp);
	fclose(fp);
	if(ret == sizeof(r_buf)){
		//printf("LIAM : read down!\n");
		printf("DEBUG : read buf = %s\n",r_buf);
		return SC_FTCMD_OK;
	}else{
		return SC_FTCMD_NG;
	}
}
static int set_usb_test(void){
	return SC_FTCMD_OK;
}

static int is_mount_already(void){
	int ret = -1;
	char rbuf[128];
	FILE *fp_mount = NULL;
	if((fp_mount=fopen("/proc/mounts","r")) == NULL){
		printf("Error , can not open file /proc/mounts!\n");
		//return SC_FTCMD_NG;
		exit(-1);
	}
	ret = SC_FTCMD_NG;
	fseek(fp_mount , 0 , SEEK_SET);
	while(fgets(rbuf , 128 , fp_mount) != NULL){
		//printf("LIAM : rbuf = %s",rbuf);
		if(strstr(rbuf , dev_f)!=NULL){
			ret = SC_FTCMD_OK;
			break;
		}
	}
	fclose(fp_mount);
	return ret==SC_FTCMD_OK?SC_FTCMD_OK:SC_FTCMD_NG;
}
static int init_usb(void){
	/* FIXME : read /proc/diskstats */
	FILE *fp = NULL;
	char rbuf[128];
	char * pbuf = NULL;
	char usb_buf[4];
	int ret = SC_FTCMD_NG;
	if((fp=fopen("/proc/diskstats","r")) == NULL){
		printf("Error , can not open file /proc/diskstats!\n");
		//return SC_FTCMD_NG;
		exit(-1);
	}
	fseek(fp , 0 , SEEK_SET);
	while(fgets(rbuf , 128 , fp) != NULL){
		//printf("LIAM : rbuf = %s",rbuf);
		if((pbuf=strstr(rbuf , "sd"))!=NULL){
			memset(usb_buf , '\0' ,sizeof(usb_buf));
			memcpy(usb_buf , pbuf , 3);//sdx
			//printf("find usb dri , name = sd%c\n",usb_buf[2]);
			ret = SC_FTCMD_OK;
			break;
		}
	}
	fclose(fp);
	/* check /dev/sdx  exist or not */
	if(ret == SC_FTCMD_OK && usb_buf[2] >='a' &&  usb_buf[2] <= 'g'){
		//char dev_f[12];
		memset(dev_f , '\0' , sizeof(dev_f));
		sprintf(dev_f , "/dev/sd%c",usb_buf[2]);
		if(!access(dev_f , F_OK)){
			//printf("find usb dev , path = %s\n",dev_f);
		}else{
			ret = SC_FTCMD_NG;
		}
	}
	// exit 
	if(ret!=SC_FTCMD_OK){
		printf("Error , can not find USB device !\n");
		exit(-1);
	}
	// mount 
	if( (ret=is_mount_already()) != SC_FTCMD_OK){
		char cmd[128];
		system("mkdir -p /mnt/usb_test");
		memset(cmd , '\0' , sizeof(cmd));
		sprintf(cmd , "mount %s /mnt/usb_test",dev_f);
		system(cmd);
	}
	// check mount sucessful
	if( (ret=is_mount_already()) != SC_FTCMD_OK){
		printf("Error , can not mount use dri !!!\n");
		exit(-1);
	}else{
		return SC_FTCMD_OK;
	}
	return ret==SC_FTCMD_OK?SC_FTCMD_OK:SC_FTCMD_NG;
}

/*201	*/  int ftcmd_set_usb(char * name, char * para)
{
	int opt = -1;
	int ret = SC_FTCMD_NG;
	init_usb();//exist , init dri
	/* para : write/read/test */
	switch(para[0]){
		case 'w':
			if((ret = set_usb_write()) == SC_FTCMD_NG){
				printf("RET : usb write NG!\n");
			}else{
				printf("RET : usb write OK!\n");
			}
			break;
		case 'r':
			if((ret = set_usb_write()) == SC_FTCMD_NG){
				printf("RET : usb write NG!\n");
			}
			if((ret = set_usb_read()) == SC_FTCMD_NG){
				printf("RET : usb read NG!\n");
			}else{
				printf("RET : usb read OK!\n");
			}
			break;
		case 't':
			ret = set_usb_test();
			break;
		default:
			ft_sys_show_help();
			printf("Waring , illegal para named \'%s\' \n",para);
			exit(-1);
	}

	//printf("DEBUG : func reserved , %s()\n",__func__);
	return ret == SC_FTCMD_OK?SC_FTCMD_OK:SC_FTCMD_NG;
}
/*404	*/  int ftcmd_get_usb(char * name, char * para)
{
	printf("DEBUG : func reserved , %s()\n",__func__);
	return SC_FTCMD_OK;
}


