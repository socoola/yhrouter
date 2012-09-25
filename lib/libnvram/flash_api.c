#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>

int mtd_open(const char *name, int flags)
{
	FILE *fp;
	char dev[80];
	int i, ret;

	sprintf(dev, "/etc/nvram/%s", name);
	ret = open(dev, flags);
	return ret;
}

int flash_read_mac(char *buf)
{
	int fd, ret;

	if (!buf)
		return -1;
	fd = mtd_open("Factory", O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "Could not open mtd device Factory to read\n");
		return -1;
	}
	lseek(fd, 0x2E, SEEK_SET);
	ret = read(fd, buf, 6);
	close(fd);
	return ret;
}

int flash_read_NicConf(char *buf)
{
	int fd, ret;

	if (!buf)
		return -1;
	fd = mtd_open("Factory", O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "Could not open mtd device Factory to read\n");
		return -1;
	}
	lseek(fd, 0x34, SEEK_SET);
	ret = read(fd, buf, 6);
	close(fd);
	return ret;
}

int flash_read(char *buf, off_t from, size_t len)
{
	int fd, ret;
	struct stat info;

	fd = mtd_open("Config", O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "Could not open mtd device Config to read\n");
		return -1;
	}

	if (fstat(fd, &info)) {
		fprintf(stderr, "Could not get mtd device info\n");
		close(fd);
		return -1;
	}
	if (len > info.st_size) {
		fprintf(stderr, "Too many bytes - %d > %d bytes\n", len, info.st_size);
		close(fd);
		return -1;
	}

	lseek(fd, from, SEEK_SET);
	ret = read(fd, buf, len);
	if (ret == -1) {
		fprintf(stderr, "Reading from mtd failed\n");
		close(fd);
		return -1;
	}

	close(fd);
	return ret;
}

#define min(x,y) ({ typeof(x) _x = (x); typeof(y) _y = (y); (void) (&_x == &_y); _x < _y ? _x : _y; })

int flash_write(char *buf, off_t to, size_t len)
{
	int fd, ret = 0;

	fd = mtd_open("Config", O_RDWR | O_SYNC);
	if (fd < 0) {
		fprintf(stderr, "Could not open mtd device Config to read and write\n");
		return -1;
	}

	lseek(fd, to, SEEK_SET);
	ret = write(fd, buf, len);
	if (ret == -1) {
		fprintf(stderr, "Writing to mtd failed\n");
		close(fd);
		return -1;
	}

	close(fd);
	return ret;
}

