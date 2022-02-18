#ifndef __SC_FTCMD_COMMON_H__
#define __SC_FTCMD_COMMON_H__

/******************* include ********************/
#include "ft_api.h"	// DO NOE EDIT

/******************* version info ********************/
// like : 1.1.20201120
#define __SC_VER_DRIVER_NAME__ "liam_wgj@163.com"
#define __SC_VER_RELEASE__  0x1				//If some of the cmd_opt have changed , release the official version after debug.
#define __SC_VER_RELEASE_DEBUG__ 0x3		//If somw of the cmd_name have changed , release the official version after debug.
#define __SC_VER_RELEASE_DEBUG_DATE__ 20220218	//A temporary version of debug.

/* add commit message */
/*
 * 1.1.20201120 : M mtd_xxx by path to mtd_xxx by mtd_name ;
 * 1.2.20210311 : show version with show_usage()
 * 1.3.20210803 : FIX mtd_file is NULL
 * 1.3.20220218 : M email,coFLAG ; D Makefile toolchain with QCA/BRCM 
 */

/******************* cmd table ********************/
static cmd_info_t cmd_info[]={

	/* 0~99 for debug reserved*/
	/* 100~199 for cmd_set */
	/* 200~299 for cmd_set with RD_add*/
	/* 300~399 for cmd_get */
	/* 400~499 for cmd_get whit RD_add*/
	/* 500~599 for cmd_show */
	/* 600~999 reserved */

	/* idx		,	opt			,		name			,	para			,	func					*/
	/* 1. cmd_set */
	{100		,	SC_SET		,		"pcba"			,	"pcba1234"		,	ftcmd_set_pcba				},
	{101		,	SC_SET		,		"mac"			,	"pcba1234"		,	ftcmd_set_mac				},
	{102		,	SC_SET		,		"csn"			,	"sn123456"		,	ftcmd_set_csn				},
	//{103		,	SC_SET		,		"bi_target"		,	"xxxxx"			,	ftcmd_set_bi_target			},
	{104		,	SC_SET		,		"csn"			,	"xxxxx"			,	ftcmd_set_bi_mode			},
	{105		,	SC_SET		,		"all_led"		,	"on | off"		,	ftcmd_set_all_led			},
	{106		,	SC_SET		,		"all_red_led"	,	"on | off"		,	ftcmd_set_all_red_led		},
	{107		,	SC_SET		,		"all_green_led"	,	"on | off"		,	ftcmd_set_all_green_led		},
	{108		,	SC_SET		,		"all_blue_led"	,	"on | off"		,	ftcmd_set_all_blue_led		},
	//{109		,	SC_SET		,		"all_orange_led",	"on | off"		,	ftcmd_set_all_orange_led	},
	//{110		,	SC_SET		,		"all_yellow_led",	"on | off"		,	ftcmd_set_all_yellow_led	},
	//{111		,	SC_SET		,		"Download_mode"	,	"on | off"		,	ftcmd_set_download_mode		},

	/* 1.2 RD add : cmd_get */
	//{200		,	SC_SET		,		"caldata"		,	"mtd_num"		,	ftcmd_set_caldata			},
	{201		,	SC_SET		,		"usb"			,	"write|read|test",	ftcmd_set_usb				},

	/* 2. cmd_get */
	{300		,	SC_GET		,		"asinfo"		,	"NULL"			,	ftcmd_get_asinfo			},
	{301		,	SC_GET		,		"btn_status"	,	"NULL"			,	ftcmd_get_btn_status		},
	{302		,	SC_GET		,		"mt_ver"		,	"NULL"			,	ftcmd_get_mt_ver			},
	//{303		,	SC_GET		,		"bi_info"		,	"NULL"			,	ftcmd_get_bi_info			},
	//{304		,	SC_GET		,		"gpio_state"	,	"NULL"			,	ftcmd_get_gpio_state		},
	//{305		,	SC_GET		,		"temperatur"	,	"NULL"			,	ftcmd_get_temperature		},
	//{306		,	SC_GET		,		"Download_mode"	,	"NULL"			,	ftcmd_get_download_mode		},

	/* 2.1 RD add : cmd_get */
	{401		,	SC_GET		,		"pcba"			,	"NULL"			,	ftcmd_get_pcba				},
	{402		,	SC_GET		,		"mac"			,	"NULL"			,	ftcmd_get_mac				},
	{403		,	SC_GET		,		"caldata"		,	"NULL"			,	ftcmd_get_caldata			},
	//{404		,	SC_GET		,		"usb"			,	"NULL"			,	ftcmd_get_usb				},
	{405		,	SC_GET		,		"csn"			,	"NULL"			,	ftcmd_get_csn				},
	{406/*=403*/,	SC_GET		,		"RFModule_info"	,	"NULL"			,	ftcmd_get_caldata			},
	{407		,	SC_GET		,		"rst_btn"		,	"NULL"			,	ftcmd_get_btn_status		},

	/* 3. cmd_show */
	{500		,	SC_SHOW		,		"version"		,	"NULL"			,	ftcmd_show_version			},
	//{501		,	SC_SHOW		,		"gpio"			,	"NULL"			,	ftcmd_show_gpio				},
	//{502		,	SC_SHOW		,		"cmd_list"		,	"NULL"			,	ftcmd_show_cmd_list			},

	/* Do NOT edit this line */
	{SC_CMD_NUM_MAX	,	0		,		"NULL"			,	"NULL"			,	NULL						},
};

#endif
