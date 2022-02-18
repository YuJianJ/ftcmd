#include <stdio.h>
#include "ft_common.h"

/* 
func : show_XXX
*/

/************************* func api ***************************/
/*500	*/	int ftcmd_show_gpio(char * name , char * para)
{
	printf("/*---------------- GPIO INFO -------------------*/\n");
	printf(" btn_power    :  X\n");
	printf(" led_sta      :  X\n");
	printf(" watchdog_en  :  X\n");
	printf(" watchdog_sig :  X\n");
	printf("/*----------------------------------------------*/\n");
	return SC_FTCMD_OK;
}

/*501	*/	int ftcmd_show_cmd_list(char * name , char * para)
{
	//printf("DEBUG : func reserved , %s()\n",__func__);
	printf("/*---------------- cmd list -------------------*/\n");
	// memtest
	printf("==> memtest\n");
	// check burnin ret
	printf("==> check burnin ret\n");
	printf("/*----------------------------------------------*/\n");
	return SC_FTCMD_OK;
}

/************************* sys api ***************************/
/* DO NOT MODIFY the following API if it is not necessary. */
/*502	*/	int ftcmd_show_version(char * name ,char * para){
	//printf("DEBUG : call show_version \n");
	printf("/*-------------------------------------------*/\n");
	printf("  driver name     : %s\n",__SC_VER_DRIVER_NAME__);
	printf("  release version : r%d\n",__SC_VER_RELEASE__);
	printf("  debug version   : r%d.%d\n",__SC_VER_RELEASE__,__SC_VER_RELEASE_DEBUG__);
	printf("  tmp version     : r%d.%d.%d\n",__SC_VER_RELEASE__,__SC_VER_RELEASE_DEBUG__,__SC_VER_RELEASE_DEBUG_DATE__);
	printf("/*-------------------------------------------*/\n");
	return 0;
}
int ft_sys_show_help(void){
	unsigned int i ;
	printf("/*------------ usage -----------------*/\n");
	/* SC_SET */
	printf("SET : \n");
	i = 0;
	while(cmd_info[i].idx < SC_CMD_NUM_MAX){
		if(cmd_info[i].opt == SC_SET){
			printf("\tftcmd \t set \t %s \t %s \n",cmd_info[i].name,cmd_info[i].para);
		}
		i++;
	}
	/* SC_GET */
	printf("GET : \n");
	i = 0;
	while(cmd_info[i].idx < SC_CMD_NUM_MAX){
		if(cmd_info[i].opt == SC_GET){
			printf("\tftcmd \t get \t %s \n",cmd_info[i].name);
		}
		i++;
	}

	/* SC_SHOW */
	printf("SHOW : \n");
	i = 0;
	while(cmd_info[i].idx < SC_CMD_NUM_MAX){
		if(cmd_info[i].opt == SC_SHOW){
			printf("\tftcmd \t show \t %s \n",cmd_info[i].name);
		}
		i++;
	}
	printf("/*------------------------------------*/\n");

	return 0;
}
