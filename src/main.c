#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* file ctl */
#include <fcntl.h>
#include <unistd.h>
/* uintX_t */
#include <stdint.h>
#include <sys/types.h>
#include "ft_common.h"

//#define DEBUG_MAIN

static int is_cmd_opt_legal(char * input , cmd_info_t *cmd);
static int is_cmd_name_legal(char * input , cmd_info_t *cmd);
static int run_func_by_idx(cmd_info_t *cmd , char * para);

/* 
func	:	main
input	:	cmd_name , cmd_para
output	:	SC_FTCMD_OK	
*/
int main(int argc, char *argv[]){
	//printf("DEBUG : run ftcmd_main \n");
	/* init */
	/*Using cmd_info_t may make the program look cleaner.*/
	cmd_info_t cmd={
		SC_FTCMD_NG,
		SC_HELP
	};
	/* 0. check argc */
	if(argc<3 || argc>4){
		ft_sys_show_help();
		if(argc<3){
			printf("\nWARING : too less parameters\n\n");
		}else{
			printf("\nWARING : too many parameters\n\n");
		}
		return 0;
	}
	/* 1.1 check cmd_opt  && get input cmd opt */
	is_cmd_opt_legal(argv[1],&cmd);
	/* 1.2 check cmd_name && get input cmd idx and cmd name */
	is_cmd_name_legal(argv[2],&cmd);
	/* FIXME Maybe we should check the parameters. */
#ifdef DEBUG_MAIN
	printf("DEBUG : input opt = %d - %s\n",cmd.opt,opt_table[cmd.opt].name);
	printf("DEBUG : input name = %s\n",cmd.name);
	printf("DEBUG : input idx = %d \n",cmd.idx);
#endif
	/* 2. run func */
	if(argc==4){
		run_func_by_idx(&cmd,argv[3]);
	}else if(argc=3){
		run_func_by_idx(&cmd,NULL);
	}else{
		//should not jump here!
	}
	return SC_FTCMD_OK;
}

/* 
func	:	check input cmd_opt
input	:	cmd_info_t 
output	:	SC_FTCMD_OK	
*/
static int is_cmd_opt_legal(char * input , cmd_info_t *cmd)
{
	/* set / get / show / debug / .etc */
	int i = 0;
	for(i= 0 ;i < SC_CMD_OPT_NUM_MAX ; i++)
	{
		if((input[0] == opt_table[i].name[0]) && (strlen(input) == strlen(opt_table[i].name)))
		{
			if(strcmp(input,opt_table[i].name)==0){
				//printf("DEBUG : opt check pass!\n");
				cmd->opt = opt_table[i].opt;
				return SC_FTCMD_OK;
			}
		}
	}
	ft_sys_show_help();
	printf("\nWaring : input cmd_opt error!!!\n\n");
	exit(-1);
}
static int is_cmd_name_legal(char * input , cmd_info_t *cmd)
{
	int i = 0;
	while(cmd_info[i].idx != SC_CMD_NUM_MAX){
		if(cmd_info[i].opt == cmd->opt){
			if((cmd_info[i].name[0] == input[0]) && (strlen(cmd_info[i].name) == strlen(input))){
				if(strcmp(input,cmd_info[i].name)==0){
					//printf("DEBUG : find name , %s \n",cmd_info[i].name);
					strncpy(cmd->name,cmd_info[i].name,strlen(cmd_info[i].name));
					/* XXX get idx */
					cmd->idx = cmd_info[i].idx;
					return SC_FTCMD_OK;
				}
			}
		}
		i++;
	}
	ft_sys_show_help();
	printf("\nWaring : input cmd_name error!!!\n\n");
	exit(-1);
}
static int run_func_by_idx(cmd_info_t *cmd , char *para){
	//printf("DEBUG : run %s()\n",__func__);
	int para_en = SC_FTCMD_DISABLE;
	int idx = 0;
	switch(cmd->opt){
		case SC_SET:
			if(para == NULL){
				ft_sys_show_help();
				printf("\nWARING : input cmd_para/cmd_opt error!!!\n\n");
				exit(-1);
			}else{
				para_en = SC_FTCMD_ENABLE;
			}
			break;
		case SC_GET:
		case SC_SHOW:
		case SC_DEBUG:
		case SC_HELP:
			break;
		default:
			/* should not jump here */
			printf("ERROR : check cmd.opt !!!\n");
			break;
	}
	/* find idx */
	while(cmd_info[idx].idx != SC_CMD_NUM_MAX){
		if(cmd_info[idx].idx == cmd->idx )	break;
		idx++;
	}
#ifdef DEBUG_MAIN
	printf("DEBUG : idx  = %d\n",idx);
	printf("DEBUG : name = %s\n",cmd->name);
	printf("DEBUG : para = %s\n",para);
#endif
	/* 2.1 Transfer para or not*/
	if(para_en == SC_FTCMD_ENABLE){
		cmd_info[idx].func(cmd->name,para);
	}else{
		cmd_info[idx].func(cmd->name,NULL);
	}
	return SC_FTCMD_OK;
}
