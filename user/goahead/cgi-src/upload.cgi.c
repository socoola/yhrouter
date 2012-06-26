#include "upload.cgi.h"

#define REFRESH_TIMEOUT		"40000"		/* 40000 = 40 secs*/

#define RFC_ERROR "RFC1867 error"

void *memmem(const void *buf, size_t buf_len, const void *byte_line, size_t byte_line_len)
{
    unsigned char *bl = (unsigned char *)byte_line;
    unsigned char *bf = (unsigned char *)buf;
    unsigned char *p  = bf;

    while (byte_line_len <= (buf_len - (p - bf))){
        unsigned int b = *bl & 0xff;
        if ((p = (unsigned char *) memchr(p, b, buf_len - (p - bf))) != NULL){
            if ( (memcmp(p, byte_line, byte_line_len)) == 0)
                return p;
            else
                p++;
        }else{
            break;
        }
    }
    return NULL;
}

#define MEM_SIZE	1024
#define MEM_HALF	512
int findStrInFile(char *filename, int offset, unsigned char *str, int str_len)
{
	int pos = 0, rc;
	FILE *fp;
	unsigned char mem[MEM_SIZE];

	if(str_len > MEM_HALF)
		return -1;
	if(offset <0)
		return -1;

	fp = fopen(filename, "rb");
	if(!fp)
		return -1;

	rewind(fp);
	fseek(fp, offset + pos, SEEK_SET);
	rc = fread(mem, 1, MEM_SIZE, fp);
	while(rc){
		unsigned char *mem_offset;
		mem_offset = (unsigned char*)memmem(mem, rc, str, str_len);
		if(mem_offset){
			fclose(fp);	//found it
			return (mem_offset - mem) + pos + offset;
		}

		if(rc == MEM_SIZE){
			pos += MEM_HALF;	// 8
		}else
			break;
		
		rewind(fp);
		fseek(fp, offset+pos, SEEK_SET);
		rc = fread(mem, 1, MEM_SIZE, fp);
	}

	fclose(fp);
	return -1;
}

/*
 *  ps. callee must free memory...
 */
void *getMemInFile(char *filename, int offset, int len)
{
    void *result;
    FILE *fp;
    if( (fp = fopen(filename, "r")) == NULL ){
        return NULL;
    }
	fseek(fp, offset, SEEK_SET);
    result = malloc(sizeof(unsigned char) * len );
	if(!result)
		return NULL;
    if( fread(result, 1, len, fp) != len){
        free(result);
        return NULL;
    }
    return result;
}



#if defined (UPLOAD_FIRMWARE_SUPPORT)

/*
 *  taken from "mkimage -l" with few modified....
 */
int check(char *imagefile, int offset, int len, char *err_msg)
{
	struct stat sbuf;

	int  data_len;
	char *data;
	unsigned char *ptr;
	unsigned long checksum;

	image_header_t header;
	image_header_t *hdr = &header;

	int ifd;

	if ((unsigned)len < sizeof(image_header_t)) {
		sprintf (err_msg, "Bad size: \"%s\" is no valid image\n", imagefile);
		return 0;
	}

	ifd = open(imagefile, O_RDONLY);
	if(!ifd){
		sprintf (err_msg, "Can't open %s: %s\n", imagefile, strerror(errno));
		return 0;
	}

	if (fstat(ifd, &sbuf) < 0) {
		close(ifd);
		sprintf (err_msg, "Can't stat %s: %s\n", imagefile, strerror(errno));
		return 0;
	}

	ptr = (unsigned char *) mmap(0, sbuf.st_size, PROT_READ, MAP_SHARED, ifd, 0);
	if ((caddr_t)ptr == (caddr_t)-1) {
		close(ifd);
		sprintf (err_msg, "Can't mmap %s: %s\n", imagefile, strerror(errno));
		return 0;
    }
	ptr += offset;

	/*
	 *  handle Header CRC32
	 */
    memcpy (hdr, ptr, sizeof(image_header_t));

    if (ntohl(hdr->ih_magic) != IH_MAGIC) {
		munmap(ptr, len);
		close(ifd);
		sprintf (err_msg, "Bad Magic Number: \"%s\" is no valid image\n", imagefile);
		return 0;
	}

	data = (char *)hdr;

    checksum = ntohl(hdr->ih_hcrc);
    hdr->ih_hcrc = htonl(0);	/* clear for re-calculation */

    if (crc32 (0, data, sizeof(image_header_t)) != checksum) {
		munmap(ptr, len);
		close(ifd);
		sprintf (err_msg, "*** Warning: \"%s\" has bad header checksum!\n", imagefile);
		return 0;
    }

	/*
	 *  handle Data CRC32
	 */
    data = (char *)(ptr + sizeof(image_header_t));
    data_len  = len - sizeof(image_header_t) ;

    if (crc32 (0, data, data_len) != ntohl(hdr->ih_dcrc)) {
		munmap(ptr, len);
		close(ifd);
		sprintf (err_msg, "*** Warning: \"%s\" has corrupted data!\n", imagefile);
		return 0;
    }

#if 1
	/*
	 * compare MTD partition size and image size
	 */
#if defined (CONFIG_RT2880_ROOTFS_IN_RAM)
	if(len > getMTDPartSize("\"Kernel\"")){
		munmap(ptr, len);
		close(ifd);
		sprintf(err_msg, "*** Warning: the image file(0x%x) is bigger than Kernel MTD partition.\n", len);
		return 0;
	}
#elif defined (CONFIG_RT2880_ROOTFS_IN_FLASH)
  #ifdef CONFIG_ROOTFS_IN_FLASH_NO_PADDING
	if(len > getMTDPartSize("\"Kernel_RootFS\"")){
		munmap(ptr, len);
		close(ifd);
		sprintf(err_msg, "*** Warning: the image file(0x%x) is bigger than Kernel_RootFS MTD partition.\n", len);
		return 0;
	}
  #else
	if(len < CONFIG_MTD_KERNEL_PART_SIZ){
		munmap(ptr, len);
		close(ifd);
		sprintf(err_msg, "*** Warning: the image file(0x%x) size doesn't make sense.\n", len);
		return 0;
	}

	if((len - CONFIG_MTD_KERNEL_PART_SIZ) > getMTDPartSize("\"RootFS\"")){
		munmap(ptr, len);
		close(ifd);
		sprintf(err_msg, "*** Warning: the image file(0x%x) is bigger than RootFS MTD partition.\n", len - CONFIG_MTD_KERNEL_PART_SIZ);
		return 0;
	}
  #endif
#else
#error "goahead: no CONFIG_RT2880_ROOTFS defined!"
#endif
#endif

	munmap(ptr, len);
	close(ifd);

	return 1;
}


#endif /* UPLOAD_FIRMWARE_SUPPORT */

/*
 * arguments: ifname  - interface name
 *            if_addr - a 16-byte buffer to store ip address
 * description: fetch ip address, netmask associated to given interface name
 */
int getIfIp(char *ifname, char *if_addr)
{
    struct ifreq ifr;
    int skfd = 0;

    if((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("getIfIp: open socket error");
        return -1;
    }

    strncpy(ifr.ifr_name, ifname, IF_NAMESIZE);
    if (ioctl(skfd, SIOCGIFADDR, &ifr) < 0) {
        printf("getIfIp: ioctl SIOCGIFADDR error for %s", ifname);
        return -1;
    }
    strcpy(if_addr, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));

    close(skfd);
    return 0;
}

/*
 * I'm too lazy to use popen() instead of system()....
 * ( note:  static buffer used)
 */
#define DEFAULT_LAN_IP "10.10.10.254"
char *getLanIP(void)
{
	static char buf[64];
	char *nl;
	FILE *fp;

	memset(buf, 0, sizeof(buf));
	if( (fp = popen("nvram_get 2860 lan_ipaddr", "r")) == NULL )
		goto error;

	if(!fgets(buf, sizeof(buf), fp)){
		pclose(fp);
		goto error;
	}

	if(!strlen(buf)){
		pclose(fp);
		goto error;
	}
	pclose(fp);

	if(nl = strchr(buf, '\n'))
		*nl = '\0';

	return buf;

error:
	fprintf(stderr, "warning, cant find lan ip\n");
	return DEFAULT_LAN_IP;
}


void javascriptUpdate(int success)
{
    printf("<script language=\"JavaScript\" type=\"text/javascript\">");
    if(success){
        printf(" \
function refresh_all(){	\
  top.location.href = \"http://%s\"; \
} \
function update(){ \
  self.setTimeout(\"refresh_all()\", %s);\
}", getLanIP(), REFRESH_TIMEOUT);
    }else{
        printf("function update(){ parent.menu.setUnderFirmwareUpload(0);}");
    }
    printf("</script>");
}

inline void webFoot(void)
{
    printf("</body></html>\n");
}


int main (int argc, char *argv[])
{
    int file_begin, file_end;
    int line_begin, line_end;
    char err_msg[256];
    char *boundary; int boundary_len;
    char *filename = getenv("UPLOAD_FILENAME");

    printf(
"\
Server: %s\n\
Pragma: no-cache\n\
Content-type: text/html\n",
getenv("SERVER_SOFTWARE"));

    printf("\n\
<html>\n\
<head>\n\
<TITLE>Upload Firmware</TITLE>\n\
<link rel=stylesheet href=/style/normal_ws.css type=text/css>\n\
<meta http-equiv=\"content-type\" content=\"text/html; charset=iso-8859-1\">\n\
</head>\n\
<body onload=\"update()\"> <h1> Upload Firmware</h1>");

      line_begin = 0;
      if((line_end = findStrInFile(filename, line_begin, "\r\n", 2)) == -1){
          printf("%s %d", RFC_ERROR, 1);
          return -1;
      }
      boundary_len = line_end - line_begin;
      boundary = getMemInFile(filename, line_begin, boundary_len);
  //  printf("boundary:%s\n", boundary);

      // sth like this..
      // Content-Disposition: form-data; name="filename"; filename="\\192.168.3.171\tftpboot\a.out"
      //
      char *line, *semicolon, *user_filename;
      line_begin = line_end + 2;
      if((line_end = findStrInFile(filename, line_begin, "\r\n", 2)) == -1){
          printf("%s %d", RFC_ERROR, 2);
          goto err;
      }
      line = getMemInFile(filename, line_begin, line_end - line_begin);
      if(strncasecmp(line, "content-disposition: form-data;", strlen("content-disposition: form-data;"))){
          printf("%s %d", RFC_ERROR, 3);
          goto err;
      }
      semicolon = line + strlen("content-disposition: form-data;") + 1;
      if(! (semicolon = strchr(semicolon, ';'))  ){
          printf("We dont support multi-field upload.\n");
          goto err;
      }
      user_filename = semicolon + 2;
      if( strncasecmp(user_filename, "filename=", strlen("filename="))  ){
          printf("%s %d", RFC_ERROR, 4);
          goto err;
      }
      user_filename += strlen("filename=");
      //until now we dont care about what the true filename is.
      free(line);

      // We may check a string  "Content-Type: application/octet-stream" here,
      // but if our firmware extension name is the same with other known ones, 
      // the browser would use other content-type instead.
      // So we dont check Content-type here...
      line_begin = line_end + 2;
      if((line_end = findStrInFile(filename, line_begin, "\r\n", 2)) == -1){
          printf("%s %d", RFC_ERROR, 5);
          goto err;
      }

      line_begin = line_end + 2;
      if((line_end = findStrInFile(filename, line_begin, "\r\n", 2)) == -1){
          printf("%s %d", RFC_ERROR, 6);
          goto err;
      }

      file_begin = line_end + 2;

      if( (file_end = findStrInFile(filename, file_begin, boundary, boundary_len)) == -1){
          printf("%s %d", RFC_ERROR, 7);
          goto err;
      }
      file_end -= 2;		// back 2 chars.(\r\n);

      // printf("file:%s, file_begin:%d, len:%d<br>\n", filename, file_begin, file_end - file_begin);

    // examination
#if defined (UPLOAD_FIRMWARE_SUPPORT)
    if(!check(filename, file_begin, file_end - file_begin, err_msg) ){
        printf("Not a valid firmware. %s", err_msg);
        javascriptUpdate(0);
        goto err;
    }

    /*
     * write the current linux version into flash.
     */
    write_flash_kernel_version(filename, file_begin);
//#if 0
#ifdef CONFIG_RT2880_DRAM_8M
    system("killall goahead");
#endif

    system("start_reboot_timer.sh network_reboot_stop");
    system("start_reboot_timer.sh stop");
    system("power_led.sh fast &");
    // flash write
    if( mtd_write_firmware(filename, file_begin, file_end - file_begin) == -1){
        printf("mtd_write fatal error! The corrupted image has ruined the flash!!");
        javascriptUpdate(0);
	
    	system("power_led.sh normal &");
        goto err;
    }

    system("power_led.sh normal &");
	 system("killall pppd"); //add 2010-07-02
    system("echo \"killall pppd\" > /dev/console"); //add 2010-07-02
        sleep(2);

#elif defined (UPLOAD_BOOTLOADER_SUPPORT)
    system("power_led.sh fast &");
    mtd_write_bootloader(filename, file_begin, file_end - file_begin);
    system("power_led.sh normal &");
#else
#error "no upload support defined!"
#endif

//#if 0
#ifdef CONFIG_RT2880_DRAM_8M
    system("sleep 3 && reboot &");
#endif
    printf("Done...rebooting");
    javascriptUpdate(1);
    webFoot();
    free(boundary);
    exit(0);

err:
    webFoot();
    free(boundary);
    exit(-1);
}

