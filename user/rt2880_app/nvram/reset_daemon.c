#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include <nvram.h>

#ifdef CONFIG_RALINK_GPIO
#include "ralink_gpio.h"
#define GPIO_DEV "/dev/gpio"
#endif


#ifdef CONFIG_RT2880_L2_MANAGE
int ramad_start(void);
#endif
static char *saved_pidfile;

void loadDefault(void)
{
 system("ralink_init clear 2860");
#if defined CONFIG_LAN_WAN_SUPPORT || defined CONFIG_MAC_TO_MAC_MODE
        system("ralink_init renew 2860 /etc_ro/Wireless/RT2860AP/RT2860_default_vlan");
#elif defined(CONFIG_ICPLUS_PHY)
        system("ralink_init renew 2860 /etc_ro/Wireless/RT2860AP/RT2860_default_oneport");
#else
        system("ralink_init renew 2860 /etc_ro/Wireless/RT2860AP/RT2860_default_novlan");
#endif
#if defined (CONFIG_INIC_MII) || defined (CONFIG_INIC_USB) || defined (CONFIG_INIC_PCI)
        system("ralink_init clear inic");
#if defined CONFIG_LAN_WAN_SUPPORT || defined CONFIG_MAC_TO_MAC_MODE
        system("ralink_init renew inic /etc_ro/Wireless/RT2860AP/RT2860_default_vlan");
#elif defined(CONFIG_ICPLUS_PHY)
        system("ralink_init renew inic /etc_ro/Wireless/RT2860AP/RT2860_default_oneport");
#else
        system("ralink_init renew inic /etc_ro/Wireless/RT2860AP/RT2860_default_novlan");
#endif
#endif
#if defined (CONFIG_RT2561_AP) || defined (CONFIG_RT2561_AP_MODULE)
        system("ralink_init clear 2561");
        system("ralink_init renew 2561 /etc_ro/Wireless/RT61AP/RT2561_default");
#endif


}

/*
 * gpio interrupt handler -
 *   SIGUSR1 - notify goAhead to start WPS (by sending SIGUSR1)
 *   SIGUSR2 - restore default value
 */
static void reset_irq_handler(int signum)
{
	if (signum == SIGUSR1) {
		printf("reboot ...\n");
		system("reboot");

	} else if (signum == SIGUSR2) {
	
		printf("load default and reboot..\n");


		loadDefault();
		system("reboot");
	}
}

/*
 * init gpio interrupt -
 *   1. config gpio interrupt mode
 *   2. register my pid and request gpio pin 0
 *   3. issue a handler to handle SIGUSR1 and SIGUSR2
 */
int initGpio(void)
{
#ifndef CONFIG_RALINK_GPIO
	signal(SIGUSR1, reset_irq_handler);
	signal(SIGUSR2, reset_irq_handler);
	return 0;
#else
	int fd;
	ralink_gpio_reg_info info;

	info.pid = getpid();
#ifdef CONFIG_RALINK_RT2880
	info.irq = 0;
#else
	//RT2883, RT3052 use gpio 10 for load-to-default
#if defined CONFIG_RALINK_I2S || defined CONFIG_RALINK_I2S_MODULE	
	info.irq = 43;
#else
//	info.irq = 10;  
	info.irq = 20;  //reset key gpio 20
#endif	
#endif

	fd = open(GPIO_DEV, O_RDONLY);
	if (fd < 0) {
		perror(GPIO_DEV);
		return -1;
	}
	//set gpio direction to input
	if (ioctl(fd, RALINK_GPIO_SET_DIR_IN, (1<<info.irq)) < 0)
		goto ioctl_err;
	//enable gpio interrupt
	if (ioctl(fd, RALINK_GPIO_ENABLE_INTP) < 0)
		goto ioctl_err;

	//register my information
	if (ioctl(fd, RALINK_GPIO_REG_IRQ, &info) < 0)
		goto ioctl_err;
	close(fd);

	//issue a handler to handle SIGUSR1 and SIGUSR2
	signal(SIGUSR1, reset_irq_handler);
	signal(SIGUSR2, reset_irq_handler);
	return 0;

ioctl_err:
	perror("ioctl");
	close(fd);
	return -1;
#endif
}



int main(int argc,char **argv)
{
	pid_t pid;
	int fd;



	if (initGpio() != 0)
		exit(EXIT_FAILURE);




	while (1) {
		pause();
	}


	exit(EXIT_SUCCESS);
}

