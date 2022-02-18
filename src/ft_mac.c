#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ft_common.h"
/*
 func : mac  
*/

#define SC_MAC_MTD_NAME "0:MFINFO"
#define SC_MAC_OFFS 0x1000
#define SC_MAC_LEN 12

/* cmd : set */
/*101	*/   int ftcmd_set_mac(char * name , char * para)
{
	char file[32];
	int num = get_mtd_num_by_mtd_name(SC_MAC_MTD_NAME);
	memset(file,'\0',sizeof(file));
	sprintf(file,"/dev/mtdblock%d",num);
	//if(mtd_write(SC_MAC_FILE,SC_MAC_OFFS,strlen(para),para) == SC_FTCMD_OK)
	if(mtd_write(file,SC_MAC_OFFS,strlen(para),para) == SC_FTCMD_OK)
		printf("RET : write done!\n");
	return SC_FTCMD_OK;
}

/* cmd : get */
/*401	*/  int ftcmd_get_mac(char * name , char * para)
{
	char * rbuf =NULL;
	if((rbuf = (char *)malloc(sizeof(char)*(SC_MAC_LEN+1))) == NULL){
		printf("DEBUG : malloc faile!\n");
		return SC_FTCMD_NG;
	}
	char file[32];
	int num = get_mtd_num_by_mtd_name(SC_MAC_MTD_NAME);
	memset(file,'\0',sizeof(file));
	sprintf(file,"/dev/mtdblock%d",num);
	memset(rbuf,'\0',SC_MAC_LEN+1);
	//mtd_read(SC_MAC_FILE,SC_MAC_OFFS,SC_MAC_LEN,rbuf);
	mtd_read(file,SC_MAC_OFFS,SC_MAC_LEN,rbuf);
	//printf("DEBUG : get rbuf %s\n",rbuf);
	printf("%s\n",rbuf);
	if(rbuf)    free(rbuf);
	return SC_FTCMD_OK;
}


