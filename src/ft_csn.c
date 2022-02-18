#include <stdio.h>
#include "ft_common.h"

#include <stdlib.h>
#include <string.h>

/*
 func :  csn  
*/

#define SC_CSN_MTD_NAME "0:MFINFO"
#define SC_CSN_OFFS 0x1010
#define SC_CSN_LEN 12

/* cmd : set */
/*102  */   int ftcmd_set_csn(char * name , char * para)
{
	//if(mtd_write(SC_CSN_FILE,SC_CSN_OFFS,strlen(para),para) == SC_FTCMD_OK)
	char sc_csn_file[32];
	int num = get_mtd_num_by_mtd_name(SC_CSN_MTD_NAME);
	memset(sc_csn_file , '\0' , sizeof(sc_csn_file));
	sprintf(sc_csn_file , "/dev/mtdblock%d",num);
	if(mtd_write(sc_csn_file,SC_CSN_OFFS,strlen(para),para) == SC_FTCMD_OK)
		printf("RET : write done!\n");
	return SC_FTCMD_OK;
}

/* cmd : get */
/*405	*/  int ftcmd_get_csn(char * name , char * para)
{
	char * rbuf =NULL;
	char sc_csn_file[32];
	int num = get_mtd_num_by_mtd_name(SC_CSN_MTD_NAME);
	if((rbuf = (char *)malloc(sizeof(char)*(SC_CSN_LEN+1))) == NULL){
		printf("DEBUG : malloc faile!\n");
		return SC_FTCMD_NG;
	} 
	memset(rbuf,'\0',SC_CSN_LEN+1);
	//mtd_read(SC_CSN_FILE,SC_CSN_OFFS,SC_CSN_LEN,rbuf);
	memset(sc_csn_file , '\0' , sizeof(sc_csn_file));
	sprintf(sc_csn_file , "/dev/mtdblock%d",num);
	mtd_read(sc_csn_file,SC_CSN_OFFS,SC_CSN_LEN,rbuf);
	//printf("DEBUG : get rbuf %s\n",rbuf);
	printf("%s\n",rbuf);
	if(rbuf)    free(rbuf); 
	return SC_FTCMD_OK;
}


