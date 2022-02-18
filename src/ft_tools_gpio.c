#include <stdio.h>
#include "ft_common.h"
/* access() */
#include <unistd.h>
/* exit */
#include <stdlib.h>
/* file */
#include <fcntl.h>

/*
func : gpio_get/set_val/dir
*/

#if 1

#define SC_GPIO_NODE_PATH "/dev/sc_gpio"
static int fd=-1;

int open_node_file(void);
int close_node_file(void);

/* */   int gpio_get_dir(int num)
{
	int dir;
	printf("DEBUG : func reserved , %s()\n",__func__);
	return (dir == SC_GPIO_OUT ? SC_GPIO_OUT:SC_GPIO_IN);
}
/* */   int gpio_get_val(int num)
{
	unsigned char buf[3];
	int val;
	buf[0] = 2;
	buf[1] = num;
	buf[2] = 0;
	write(fd, buf, 3);
	read(fd, &val, 1);
	//printf("DEBUG : get val %d\n",val);
	return (val == SC_GPIO_HI ? SC_GPIO_HI:SC_GPIO_LO);
}
/* */   int gpio_set_dir(int num , int dir)
{
	unsigned char buf[3];
	buf[0] = 0;
	buf[1] = num;
	switch(dir){
		case SC_GPIO_IN :
			buf[2] = 0;break;
		case SC_GPIO_OUT:
			buf[2] = 1;break;
		default:
			printf("Error : input gpio dir illegal!!!\n");
			close_node_file();
			exit(-1);
	}
	write(fd,buf,3);
	return SC_FTCMD_OK;
}
/* */   int gpio_set_val(int num , int val)
{
	unsigned char buf[3];
	buf[0] = 1;
	buf[1] = num;
	switch(val){
		case SC_GPIO_HI:
			buf[2] = 1;break;
		case SC_GPIO_LO:
			buf[2] = 0;break;
		default:
			printf("Error : input gpio val illegal!!!\n");
			close_node_file();
			exit(-1);
	}
	write(fd,buf,3);
	return SC_FTCMD_OK;
}

int open_node_file(void)
{
	if(access(SC_GPIO_NODE_PATH,F_OK) == 0){
		unsigned char buf[3];
		fd = open(SC_GPIO_NODE_PATH,O_RDWR);
		if(fd < 0){
			printf("Error : open sc_gpio node failed!!!\n");
			exit(-1);
		}
	}else{
		printf("Error : cannot find sc_gpio node !!!\n");
	}
	return SC_FTCMD_OK;
}
int close_node_file(void)
{
	close(fd);
	return SC_FTCMD_OK;
}


#else //others

/* */   int gpio_get_dir(int num , int dir)
{
	printf("DEBUG : func reserved , %s()\n",__func__);
	return SC_FTCMD_OK;
}
/* */   int gpio_get_val(int num , int val)
{
	printf("DEBUG : func reserved , %s()\n",__func__);
	return SC_FTCMD_OK;
}
/* */   int gpio_set_dir(int num , int dir)
{
	printf("DEBUG : func reserved , %s()\n",__func__);
	return SC_FTCMD_OK;
}
/* */   int gpio_set_val(int num , int val)
{
	printf("DEBUG : func reserved , %s()\n",__func__);
	return SC_FTCMD_OK;
}
#endif
