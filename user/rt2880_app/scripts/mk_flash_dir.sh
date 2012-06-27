FLASH_DIR=/flash
FLASH_BDEV=/dev/mtdblock3
FLASH_CDEV=/dev/mtd3

if [ ! -d $FLASH_DIR ];then
	mkdir $FLASH_DIR
	chmod 777 $FLASH_DIR
	ret=`mount -t jffs2 $FLASH_BDEV $FLASH_DIR 2>&1`
	if [ "$ret" != "" ];then
		eraseall $FLASH_CDEV	
	fi
fi
