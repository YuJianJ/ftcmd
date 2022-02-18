#include <stdio.h>
#include "ft_common.h"
/*
 func : caldata/art 
*/

#include <stdlib.h>
#include <string.h>

#define SC_ART_MTD_NAME "0:ART"

#define SC_ART_SOC_OFFS 0x1000
#define SC_ART_SOC_LEN 32

#define SC_ART_SCAN_OFFS 0x33000
#define SC_ART_SCAN_LEN 32

/* cmd : get */
/*403	*/  int ftcmd_get_caldata(char * name , char * para)
{
	char * rbuf =NULL;
	char sc_cal_file[32];
	int num = get_mtd_num_by_mtd_name(SC_ART_MTD_NAME);
	int idx = 0;
	int is_art_soc_exist = SC_FTCMD_NG;
	int is_art_scan_exist = SC_FTCMD_NG;
	int ret = SC_FTCMD_OK;
	/* 1.check scanning */
	if((rbuf = (char *)malloc(sizeof(char)*(SC_ART_SOC_LEN+1))) == NULL){
		printf("DEBUG : malloc faile!\n");
		return SC_FTCMD_NG;
	}
	memset(rbuf,'\0',SC_ART_SOC_LEN+1);
	//mtd_read(SC_CSN_FILE,SC_ART_SOC_OFFS,SC_ART_SOC_LEN,rbuf);
	memset(sc_cal_file , '\0' , sizeof(sc_cal_file));
	sprintf(sc_cal_file , "/dev/mtdblock%d",num);
	mtd_read(sc_cal_file,SC_ART_SOC_OFFS,SC_ART_SOC_LEN,rbuf);
	/* show info */
	printf("DEBUG: ART_SOC: 0x");
	for(idx=0;idx<SC_ART_SOC_LEN;idx++){
		printf("%x",rbuf[idx]);
	}
	printf("\n");
	/* check ret */
	for(idx=0;idx<SC_ART_SOC_LEN;idx++){
		if(rbuf[idx] == 0xff){
			continue;
		}else{
			is_art_soc_exist = SC_FTCMD_OK;
			break;
		}
	}
	if(is_art_soc_exist == SC_FTCMD_OK){
		printf("ART_SOC RET : OK\n");
	}else{
		printf("ART_SOC RET : NG\n");
		ret = SC_FTCMD_NG;
	}
	if(rbuf)    free(rbuf);
	/* 2.check scanning */
	if((rbuf = (char *)malloc(sizeof(char)*(SC_ART_SCAN_LEN+1))) == NULL){
		printf("DEBUG : malloc faile!\n");
		return SC_FTCMD_NG;
	}
	memset(rbuf,'\0',SC_ART_SCAN_LEN+1);
	//mtd_read(SC_CSN_FILE,SC_ART_SOC_OFFS,SC_ART_SOC_LEN,rbuf);
	memset(sc_cal_file , '\0' , sizeof(sc_cal_file));
	sprintf(sc_cal_file , "/dev/mtdblock%d",num);
	mtd_read(sc_cal_file,SC_ART_SCAN_OFFS,SC_ART_SCAN_LEN,rbuf);
	/* show info */
	printf("DEBUG: ART_SCAN: 0x");
	for(idx=0;idx<SC_ART_SCAN_LEN;idx++){
		printf("%x",rbuf[idx]);
	}
	printf("\n");
	/* check ret */
	for(idx=0;idx<SC_ART_SCAN_LEN;idx++){
		if(rbuf[idx] == 0xff){
			continue;
		}else{
			is_art_scan_exist = SC_FTCMD_OK;
			break;
		}
	}
	if(is_art_scan_exist == SC_FTCMD_OK){
		printf("ART_SCAN RET : OK\n");
	}else{
		printf("ART_SCAN RET : NG\n");
		ret = SC_FTCMD_NG;
	}
	if(rbuf)    free(rbuf);
	return ret==SC_FTCMD_OK?SC_FTCMD_OK:SC_FTCMD_OK;
}


