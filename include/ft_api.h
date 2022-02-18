#ifndef __SC_FTCMD_API_H__
#define __SC_FTCMD_API_H__

/********************* #define **********************/
/* DO NOT MODIFY the following define if it is not necessary. */
#define SC_CMD_NUM_MAX 1000
#define SC_CMD_OPT_NUM_MAX sizeof(opt_table)/sizeof(opt_table[0]) 
enum cmd_opt{
	SC_GET,		//"get"
	SC_SET,		//"set"
	SC_SHOW,	//"show"
	SC_DEBUG,	//"debug"
	SC_HELP		//"help"
};
typedef struct sc_ftcmd_cmd_opt_t
{
	int opt;
	char name[10];
} opt_t;
static opt_t opt_table[]={
	{SC_GET		,	"get"	},
	{SC_SET		,	"set"	},
	{SC_SHOW	,	"show"	},
	{SC_DEBUG	,	"debug"	},
	{SC_HELP	,	"help"	}
};
enum ret_sta{
	SC_FTCMD_NG=-1,
	SC_FTCMD_OK
};
enum init_sta{
	SC_FTCMD_DISABLE=-1,
	SC_FTCMD_ENABLE
};
enum ret_gpio_dir_sta{
	SC_GPIO_IN = 0,
	SC_GPIO_OUT = 1
};
enum ret_gpio_val_sta{
	SC_GPIO_LO = 0,
	SC_GPIO_HI = 1
};

/********************* struct **********************/
/* DO NOT MODIFY the following struct if it is not necessary. */
typedef struct sc_ftcmd_cmd_info_t
{
	unsigned int idx;
	unsigned int opt;
	char name[32];
	char para[32];
	int (*func)(char * name , char * para);
}cmd_info_t;

/******************* ftcmd func API ********************/
/* cmd : set */
/*100	*/	int ftcmd_set_pcba(char * , char *);
/*101	*/	int ftcmd_set_mac(char * , char *);
/*102	*/	int ftcmd_set_csn(char * , char *);
/*103	*/	int ftcmd_set_bi_target(char * , char *);
/*104	*/	int ftcmd_set_bi_mode(char * , char *);
/*105	*/	int ftcmd_set_all_led(char * , char *);
/*106	*/	int ftcmd_set_all_red_led(char * , char *);
/*107	*/	int ftcmd_set_all_green_led(char * , char *);
/*108	*/	int ftcmd_set_all_blue_led(char * , char *);
/*109	*/	int ftcmd_set_all_orange_led(char * , char *);
/*110	*/	int ftcmd_set_all_yellow_led(char * , char *);
/*111	*/	int ftcmd_set_download_mode(char * , char *);
/*112	*/	int ftcmd_set_wifi_2g(char * , char *);
/*113	*/	int ftcmd_set_wifi_5g(char * , char *);
/*114	*/	int ftcmd_set_gpio_state(char * , char *);
/*RD add*/
/*200	*/	int ftcmd_set_caldata(char * , char *);		// SXF save caldata 
/*201	*/	int ftcmd_set_usb(char * , char *); 

/* cmd : get */
/*300	*/	int ftcmd_get_asinfo(char * , char *);
/*301	*/	int ftcmd_get_btn_status(char * , char *);
/*302	*/	int ftcmd_get_mt_ver(char * , char *);
/*303	*/	int ftcmd_get_bi_info(char * , char *);
/*304	*/	int ftcmd_get_gpio_state(char * , char *);
/*305	*/	int ftcmd_get_temperature(char * , char *);
/*306	*/	int ftcmd_get_download_mode(char * , char *);
/*RD add*/
/*400	*/	int ftcmd_get_pcba(char * , char *);
/*401	*/	int ftcmd_get_mac(char * , char *);
/*402	*/	int ftcmd_get_csn(char * , char *);
/*403	*/	int ftcmd_get_caldata(char * , char *);
/*404	*/	int ftcmd_get_usb(char * , char *); 

/* cmd : show */
/*500	*/	int ftcmd_show_gpio(char * , char * );
/*501	*/	int ftcmd_show_cmd_list(char * , char * );
/*502	*/	int ftcmd_show_version(char * , char *);

/******************* public system API ********************/
/* file : ft_show.c */
/* */	int ft_sys_show_help(void);//system API

/******************* public tools API ********************/
/* file : ft_tools_mtd.c */
/* */	int get_mtd_num_by_mtd_name(char *name);
/* */	int mtd_read(char * file , long int off , long int len , char * rbuf);
/* */	int mtd_write(char * file , long int off , long int len , char * wbuf);
/* file : ft_tools_gpio.c */
/* */	int gpio_get_dir(int num);
/* */	int gpio_get_val(int num);
/* */	int gpio_set_dir(int num , int dir);
/* */	int gpio_set_val(int num , int val);

#endif
