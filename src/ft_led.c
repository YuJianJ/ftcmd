#include <stdio.h>
#include "ft_common.h"

#if 1
#include <unistd.h>

extern int open_node_file(void);
extern int close_node_file(void);

/*105  */  int ftcmd_set_all_led(char * name , char * para)
{
	open_node_file();
	gpio_set_dir(25,SC_GPIO_OUT);
	gpio_set_dir(26,SC_GPIO_OUT);
	gpio_set_dir(27,SC_GPIO_OUT);
	switch(para[1]){
		case 'n'://on
			gpio_set_val(25,SC_GPIO_HI);
			gpio_set_val(26,SC_GPIO_HI);
			gpio_set_val(27,SC_GPIO_HI);
			break;
		case 'f'://off
			gpio_set_val(25,SC_GPIO_LO);
			gpio_set_val(26,SC_GPIO_LO);
			gpio_set_val(27,SC_GPIO_LO);
			break;
		default:
			ft_sys_show_help();
			printf("\nError : input led para error !!!\n\n");
	}
	close_node_file();
	return SC_FTCMD_OK;
}

/*106	*/  int ftcmd_set_all_red_led(char * name , char * para)
{
	open_node_file();
	gpio_set_dir(27,SC_GPIO_OUT);
	switch(para[1]){
		case 'n'://on
			gpio_set_val(27,SC_GPIO_HI);
			break;
		case 'f'://off
			gpio_set_val(27,SC_GPIO_LO);
			break;
		default:
			ft_sys_show_help();
			printf("\nError : input led para error !!!\n\n");
	}
	close_node_file();
	return SC_FTCMD_OK;
}

/*107	*/  int ftcmd_set_all_green_led(char * name , char * para)
{
	open_node_file();
	gpio_set_dir(26,SC_GPIO_OUT);
	switch(para[1]){
		case 'n'://on
			gpio_set_val(26,SC_GPIO_HI);
			break;
		case 'f'://off
			gpio_set_val(26,SC_GPIO_LO);
			break;
		default:
			ft_sys_show_help();
			printf("\nError : input led para error !!!\n\n");
	}
	close_node_file();
	return SC_FTCMD_OK;
}

/*108	*/  int ftcmd_set_all_blue_led(char * name , char * para)
{
	open_node_file();
	gpio_set_dir(25,SC_GPIO_OUT);
	switch(para[1]){
		case 'n'://on
			gpio_set_val(25,SC_GPIO_HI);
			break;
		case 'f'://off
			gpio_set_val(25,SC_GPIO_LO);
			break;
		default:
			ft_sys_show_help();
			printf("\nError : input led para error !!!\n\n");
	}
	close_node_file();
	return SC_FTCMD_OK;
}

#else
/*105	*/  int ftcmd_set_all_led(char * name , char * para)
{
	printf("DEBUG : func reserved , %s()\n",__func__);
	return SC_FTCMD_OK;
}

/*106	*/  int ftcmd_set_all_red_led(char * name , char * para)
{
	printf("DEBUG : func reserved , %s()\n",__func__);
	return SC_FTCMD_OK;
}

/*107*/  int ftcmd_set_all_green_led(char * name , char * para)
{
	printf("DEBUG : func reserved , %s()\n",__func__);
	return SC_FTCMD_OK;
}

/*108	*/  int ftcmd_set_all_blue_led(char * name , char * para)
{
	printf("DEBUG : func reserved , %s()\n",__func__);
	return SC_FTCMD_OK;
}

/*109	*/  int ftcmd_set_all_orange_led(char * name , char * para)
{
	printf("DEBUG : func reserved , %s()\n",__func__);
	return SC_FTCMD_OK;
}

/*110	*/  int ftcmd_set_all_yellow_led(char * name , char * para)
{
	printf("DEBUG : func reserved , %s()\n",__func__);
	return SC_FTCMD_OK;
}
#endif
