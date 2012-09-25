#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/autoconf.h>

#include "nvram.h"

#ifdef CONFIG_RALINK_GPIO
#include "ralink_gpio.h"
#define GPIO_DEV "/dev/gpio"
#endif


#define STATUS_NORMAL_BLINK 0
#define STATUS_FAST_BLINK 1
#define STATUS_SLOW_BLINK 2


static char *saved_pidfile;


static int sign_last_status;






static void pidfile_delete(void)
{
	if (saved_pidfile) unlink(saved_pidfile);
}

int pidfile_acquire(const char *pidfile)
{
	int pid_fd;
	if (!pidfile) return -1;

	pid_fd = open(pidfile, O_CREAT | O_WRONLY, 0644);
	if (pid_fd < 0) {
		printf("Unable to open pidfile %s: %m\n", pidfile);
	} else {
		lockf(pid_fd, F_LOCK, 0);
		if (!saved_pidfile)
			atexit(pidfile_delete);
		saved_pidfile = (char *) pidfile;
	}
	return pid_fd;
}

void pidfile_write_release(int pid_fd)
{
	FILE *out;

	if (pid_fd < 0) return;

	if ((out = fdopen(pid_fd, "w")) != NULL) {
		fprintf(out, "%d\n", getpid());
		fclose(out);
	}
	lockf(pid_fd, F_UNLCK, 0);
	close(pid_fd);
}


static void change_led_fast_blink(int signum)
{

	 if(sign_last_status==STATUS_FAST_BLINK)
		{
			//system("gpio l 19 10 10 4000 1 4000");  //normal blink
			system("power_led.sh normal &");
			sign_last_status=STATUS_NORMAL_BLINK;
		}
	else 
		{
			//system("gpio l 19 5 5 4000 1 4000");  //fast blink
			system("power_led.sh fast &");
			sign_last_status=STATUS_FAST_BLINK;
		}

}


static void change_led_slow_blink(int signum)
{


	 if(sign_last_status==STATUS_SLOW_BLINK)
		{
			//system("gpio l 19 10 10 4000 1 4000");  //normal blink
			system("power_led.sh normal &");
			sign_last_status=STATUS_NORMAL_BLINK;
		}
	else 
		{
			//system("gpio l 19 5 10 4000 1 4000");  //slow blink
			system("power_led.sh slow &");
			sign_last_status=STATUS_SLOW_BLINK;
		}
}



int main(int argc,char **argv)
{
	pid_t pid;
	int fd;
	
	//system("gpio l 19 10 10 4000 1 4000");  //normal
	system("power_led.sh normal &");

	fd = pidfile_acquire("/var/run/power_led_daemon.pid");
	pidfile_write_release(fd);


	sign_last_status=STATUS_NORMAL_BLINK;


	signal(SIGUSR1, change_led_fast_blink);
	signal(SIGUSR2, change_led_slow_blink);


	while (1) {
		pause();
	}


	exit(EXIT_SUCCESS);
}

