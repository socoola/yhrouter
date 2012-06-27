#!/bin/sh
#
# $Id: storage.sh,v 1.30 2008-12-18 06:20:09 chhung Exp $
#
# usage: storage.sh
#
#
PART1=""
#PART1="/var"
for part in a b c d e f g h i j k l m n o p q r s t u v w x y z
do
	for index in 1 2 3 4 5 6 7 8 9
	do
		if [ -e "/media/sd$part$index" ]; then
			PART1="/media/sd$part$index"
			break;
		fi
	done
	if [ "$PART1" != "" ]; then
		break;
	fi
done

setUser()
{
	mkdir -p "$PART1/home/"
	for index in 1 2 3 4 5 6 7 8
	do
		user=`nvram_get 2860 "User$index"`
		base=500
		id=`expr $base + $index`
		if [ "$user" ]; then
			echo "$user::$id:$id:$user:$PART1/home/$user:/bin/sh" >> /etc/passwd
			echo "$user:x:$id:$user" >> /etc/group
			mkdir -p "$PART1/home/$user"
			chmod 777 "$PART1/home/$user"
		fi
	done
}

setFtp()
{
	ftpport=`nvram_get 2860 FtpPort`
	maxuser=`nvram_get 2860 FtpMaxUsers`
	loginT=`nvram_get 2860 FtpLoginTimeout`
	stayT=`nvram_get 2860 FtpStayTimeout`
	echo "stupid-ftpd-common.sh "$ftpport" "$maxuser" "$loginT" "$stayT""
	stupid-ftpd-common.sh "$ftpport" "$maxuser" "$loginT" "$stayT"
	admID=`nvram_get 2860 Login`
	admPW=`nvram_get 2860 Password`
	echo "stupid-ftpd-user.sh "$admID" "$admPW" / 3 A"
	stupid-ftpd-user.sh "$admID" "$admPW" / 3 A
	anonymous=`nvram_get 2860 FtpAnonymous`
	if [ "$anonymous" = "1" ]; then
		echo "stupid-ftpd-user.sh anonymous "*" /tmp 3 D"
		stupid-ftpd-user.sh anonymous "*" /tmp 3 D	
	fi
	if [ -e "$PART1" ]; then
		for index in 1 2 3 4 5 6 7 8
		do
			user=`nvram_get 2860 "User$index"`
			ftpuser=`nvram_get 2860 "FtpUser$index"`
			if [ "$user" -a "$ftpuser" = "1" ]; then
				pw=`nvram_get 2860 "UserPasswd$index"`
				max=`nvram_get 2860 "FtpMaxLogins$index"`
				mode=`nvram_get 2860 "FtpMode$index"`
				echo "stupid-ftpd-user.sh "$user" "$pw" "$PART1/home/$user" "$max" "$mode""
				stupid-ftpd-user.sh "$user" "$pw" "$PART1/home/$user" "$max" "$mode"
			fi
		done
	fi
}

setSmb()
{
	smbnetbios=`nvram_get 2860 SmbNetBIOS`
	smbwg=`nvram_get 2860 HostName`
	echo "samba.sh "$smbnetbios" "$smbwg""
	samba.sh "$smbnetbios" "$smbwg" 
	admID=`nvram_get 2860 Login`
	admPW=`nvram_get 2860 Password`
	echo "smbpasswd -a "$admID" "$admPW""
	smbpasswd -a "$admID" "$admPW"
	allusers="$admID"
	if [ -e "$PART1" ]; then
		for index in 1 2 3 4 5 6 7 8
		do
			user=`nvram_get 2860 "User$index"`
			smbuser=`nvram_get 2860 "SmbUser$index"`
			if [ "$user" -a "$smbuser" = "1" ]; then
				pw=`nvram_get 2860 "UserPasswd$index"`
				echo "smbpasswd -a "$user" "$pw""
				smbpasswd -a "$user" "$pw"
				allusers="$allusers $user"
			fi
		done
		if [ ! -e "$PART1/public" ]; then
			echo "storage.sh adddir "$PART1/public""
			storage.sh adddir "$PART1/public"
		fi
		echo "samba_add_dir.sh Public "$PART1/public" "$allusers""
		samba_add_dir.sh Public "$PART1/public" "$allusers"
	fi
}

case $1 in
	"admin")
		admID=`nvram_get 2860 Login`
		admPW=`nvram_get 2860 Password`
		echo "$admID::0:0:Adminstrator:/:/bin/sh" > /etc/passwd
		echo "$admID:x:0:$admID" > /etc/group
		chpasswd.sh $admID $admPW
		chmod 777 /tmp
		if [ -e "$PART1" ]; then
			setUser
		fi
		;;
	"adddir")
		if [ -n "$2" ]; then
			mkdir -p "$2"
			chmod 777 "$2"
		fi
		;;
	"deldir")
		if [ -n "$2" ]; then
			rm -rf "$2"
		fi
		;;
	"reparted")
		echo "fdisk -D /dev/sda"
		fdisk -D /dev/sda
		sleep 1
		if [ "$2" -gt "0" ]; then
			echo "fdisk /dev/sda -p 1 -v $2 "
			fdisk /dev/sda -p 1 -v $2 
		fi
		sleep 1
		if [ "$3" -gt "0" ]; then
			echo "fdisk /dev/sda -p 2 -v $3"
			fdisk /dev/sda -p 2 -v $3 
		fi
		sleep 1
		if [ "$4" -gt "0" ]; then
			echo "fdisk /dev/sda -p 3 -v $4"
			fdisk /dev/sda -p 3 -v $4 
		fi
		sleep 1
		if [ "$5" -gt "0" ]; then
			echo "fdisk /dev/sda -p 4 -v $5"
			fdisk /dev/sda -p 4 -v $5 
		fi
		sleep 1
		echo "fdisk -r /dev/sda"
		fdisk -r /dev/sda
		sleep 1
		echo "mkdosfs -F 32 /dev/sda1"
		mkdosfs -F 32 /dev/sda1
		sleep 1
		echo "mkdosfs -F 32 /dev/sda2"
		mkdosfs -F 32 /dev/sda2
		sleep 1
		echo "mkdosfs -F 32 /dev/sda3"
		mkdosfs -F 32 /dev/sda3
		sleep 1
		echo "mkdosfs -F 32 /dev/sda4"
		mkdosfs -F 32 /dev/sda4
		sleep 1
		reboot
		;;	
	"format")
		echo "umount -l $2"
		umount -l $2
		echo "mkdosfs -F 32 $2"
		mkdosfs -F 32 $2 
		echo "mount $2 $3"
		mount $2 $3
		;;
	"ftp")
		killall -q stupid-ftpd
		ftpenabled=`nvram_get 2860 FtpEnabled`
		if [ "$ftpenabled" = "1" ]; then
			setFtp
			echo "stupid-ftpd"
			stupid-ftpd
		fi
		;;
	"samba")
		killall -q nmbd
		killall -q smbd
		smbenabled=`nvram_get 2860 SmbEnabled`
		if [ "$smbenabled" = "1" ]; then
			setSmb
		fi
		;;
	"media")
		killall -q ushare
		media_enabled=`nvram_get 2860 mediaSrvEnabled`
		media_name=`nvram_get 2860 mediaSrvName`
		if [ "$media_enabled" = "1" ]; then
			echo "ushare.sh $media_name "$2" "$3" "$4" "$5""
			ushare.sh $media_name "$2" "$3" "$4" "$5"
			echo "ushare -D"
			ushare -D
		fi
		;;
esac
