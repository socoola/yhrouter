#!/bin/sh
#
# $Id: internet.sh,v 1.71 2009-02-25 01:31:35 steven Exp $
#
# usage: internet.sh
#


display_shell_name.sh  internet.sh start & 



. /sbin/config.sh
. /sbin/global.sh




lan_ip=`nvram_get 2860 lan_ipaddr`
stp_en=`nvram_get 2860 stpEnabled`
nat_en=`nvram_get 2860 natEnabled`
bssidnum=`nvram_get 2860 BssidNum`
radio_off=`nvram_get 2860 RadioOff`

set_vlan_map()
{
	if [ "$CONFIG_RAETH_QOS_PORT_BASED" = "y" ]; then
	# vlan priority tag => skb->priority mapping
	vconfig set_ingress_map $1 0 0
	vconfig set_ingress_map $1 1 1
	vconfig set_ingress_map $1 2 2
	vconfig set_ingress_map $1 3 3
	vconfig set_ingress_map $1 4 4
	vconfig set_ingress_map $1 5 5
	vconfig set_ingress_map $1 6 6
	vconfig set_ingress_map $1 7 7

	# skb->priority => vlan priority tag mapping
	vconfig set_egress_map $1 0 0
	vconfig set_egress_map $1 1 1
	vconfig set_egress_map $1 2 2
	vconfig set_egress_map $1 3 3
	vconfig set_egress_map $1 4 4
	vconfig set_egress_map $1 5 5
	vconfig set_egress_map $1 6 6
	vconfig set_egress_map $1 7 7
	fi
}

ifRaxWdsxDown()
{
	ifconfig ra0 down
	ifconfig ra1 down
	ifconfig ra2 down
	ifconfig ra3 down
	ifconfig ra4 down
	ifconfig ra5 down
	ifconfig ra6 down
	ifconfig ra7 down

	ifconfig wds0 down
	ifconfig wds1 down
	ifconfig wds2 down
	ifconfig wds3 down

	ifconfig apcli0 down

	ifconfig mesh0 down
}

addBr0()
{
	brctl addbr br0
	brctl addif br0 ra0
}

addRax2Br0()
{
	if [ "$bssidnum" = "2" ]; then
		brctl addif br0 ra1
	elif [ "$bssidnum" = "3" ]; then
		brctl addif br0 ra1
		brctl addif br0 ra2
	elif [ "$bssidnum" = "4" ]; then
		brctl addif br0 ra1
		brctl addif br0 ra2
		brctl addif br0 ra3
	elif [ "$bssidnum" = "5" ]; then
		brctl addif br0 ra1
		brctl addif br0 ra2
		brctl addif br0 ra3
		brctl addif br0 ra4
	elif [ "$bssidnum" = "6" ]; then
		brctl addif br0 ra1
		brctl addif br0 ra2
		brctl addif br0 ra3
		brctl addif br0 ra4
		brctl addif br0 ra5
	elif [ "$bssidnum" = "7" ]; then
		brctl addif br0 ra1
		brctl addif br0 ra2
		brctl addif br0 ra3
		brctl addif br0 ra4
		brctl addif br0 ra5
		brctl addif br0 ra6
	elif [ "$bssidnum" = "8" ]; then
		brctl addif br0 ra1
		brctl addif br0 ra2
		brctl addif br0 ra3
		brctl addif br0 ra4
		brctl addif br0 ra5
		brctl addif br0 ra6
		brctl addif br0 ra7
	fi
}

addWds2Br0()
{
	wds_en=`nvram_get 2860 WdsEnable`
	if [ "$wds_en" != "0" ]; then
		ifconfig wds0 up
		ifconfig wds1 up
		ifconfig wds2 up
		ifconfig wds3 up
		brctl addif br0 wds0
		brctl addif br0 wds1
		brctl addif br0 wds2
		brctl addif br0 wds3
	fi
}

addMesh2Br0()
{
	meshenabled=`nvram_get 2860 MeshEnabled`
	if [ "$meshenabled" = "1" ]; then
		ifconfig mesh0 up
		brctl addif br0 mesh0
		meshhostname=`nvram_get 2860 MeshHostName`
		iwpriv mesh0 set  MeshHostName="$meshhostname"
	fi
}

addRaix2Br0()
{
	#modify 2010-12-8 v1.4.28
	#inic_bssnum=`nvram_get inic BssidNum`
	inic_bssnum=`nvram_get 2860 BssidNum`
	if [ "$CONFIG_RT2880_INIC" == "" -a "$CONFIG_INIC_MII" == "" -a "$CONFIG_INIC_PCI" == "" -a "$CONFIG_INIC_USB" == "" ]; then
		return
	fi
	brctl addif br0 rai0

	if [ "$inic_bssnum" = "2" ]; then
		ifconfig rai1 up
		brctl addif br0 rai1
	elif [ "$inic_bssnum" = "3" ]; then
		ifconfig rai1 up
		ifconfig rai2 up
		brctl addif br0 rai1
		brctl addif br0 rai2
	elif [ "$inic_bssnum" = "4" ]; then
		ifconfig rai1 up
		ifconfig rai2 up
		ifconfig rai3 up
		brctl addif br0 rai1
		brctl addif br0 rai2
		brctl addif br0 rai3
	elif [ "$inic_bssnum" = "5" ]; then
		ifconfig rai1 up
		ifconfig rai2 up
		ifconfig rai3 up
		ifconfig rai4 up
		brctl addif br0 rai1
		brctl addif br0 rai2
		brctl addif br0 rai3
		brctl addif br0 rai4
	elif [ "$inic_bssnum" = "6" ]; then
		ifconfig rai1 up
		ifconfig rai2 up
		ifconfig rai3 up
		ifconfig rai4 up
		ifconfig rai5 up
		brctl addif br0 rai1
		brctl addif br0 rai2
		brctl addif br0 rai3
		brctl addif br0 rai4
		brctl addif br0 rai5
	elif [ "$inic_bssnum" = "7" ]; then
		ifconfig rai1 up
		ifconfig rai2 up
		ifconfig rai3 up
		ifconfig rai4 up
		ifconfig rai5 up
		ifconfig rai6 up
		brctl addif br0 rai1
		brctl addif br0 rai2
		brctl addif br0 rai3
		brctl addif br0 rai4
		brctl addif br0 rai5
		brctl addif br0 rai6
	elif [ "$inic_bssnum" = "8" ]; then
		ifconfig rai1 up
		ifconfig rai2 up
		ifconfig rai3 up
		ifconfig rai4 up
		ifconfig rai5 up
		ifconfig rai6 up
		ifconfig rai7 up
		brctl addif br0 rai1
		brctl addif br0 rai2
		brctl addif br0 rai3
		brctl addif br0 rai4
		brctl addif br0 rai5
		brctl addif br0 rai6
		brctl addif br0 rai7
	fi
}

addInicWds2Br0()
{
	if [ "$CONFIG_RT2880_INIC" == "" -a "$CONFIG_INIC_MII" == "" -a "$CONFIG_INIC_PCI" == "" -a "$CONFIG_INIC_USB" == "" ]; then
		return
	fi
	#modify v1.4.28
	#wds_en=`nvram_get inic WdsEnable`
	wds_en=`nvram_get 2860 WdsEnable`
	if [ "$wds_en" != "0" ]; then
		ifconfig wdsi0 up
		ifconfig wdsi1 up
		ifconfig wdsi2 up
		ifconfig wdsi3 up
		brctl addif br0 wdsi0
		brctl addif br0 wdsi1
		brctl addif br0 wdsi2
		brctl addif br0 wdsi3
	fi
}

addRaL02Br0()
{
	if [ "$CONFIG_RT2561_AP" != "" ]; then
		brctl addif br0 raL0
	fi
}

genSysFiles()
{
	#login=`nvram_get 2860 Login`
	#pass=`nvram_get 2860 Password`
	#if [ "$login" != "" -a "$pass" != "" ]; then
	#echo "$login::0:0:Adminstrator:/:/bin/sh" > /etc/passwd
	#echo "$login:x:0:$login" > /etc/group
	#	chpasswd.sh $login $pass
	#fi


	echo "admin868::0:0:Adminstrator:/:/bin/sh" > /etc/passwd
	echo "admin868:x:0:admin868" > /etc/group
	chpasswd.sh admin868 lylq-268

	echo "yh_user::1000:1000:Adminstrator:/:/bin/sh" >> /etc/passwd
	echo "yh_user:x:1000:yh_user">> /etc/group
	chpasswd.sh yh_user yh_123
	
	chmod 751 /etc/passwd
	chmod 751 /etc/passwd-
	chmod 751 /etc/group

	if [ "$CONFIG_PPPOL2TP" == "y" ]; then
	echo "l2tp 1701/tcp l2f" > /etc/services
	echo "l2tp 1701/udp l2f" >> /etc/services
	fi
}

genDevNode()
{
#Linux2.6 uses udev instead of devfs, we have to create static dev node by myself.
if [ "$CONFIG_DWC_OTG" == "m" -a "$CONFIG_HOTPLUG" == "y" ]; then
	mounted=`mount | grep mdev | wc -l`
	if [ $mounted -eq 0 ]; then
	mount -t ramfs mdev /dev
	mkdir /dev/pts
	mount -t devpts devpts /dev/pts
        mdev -s

        mknod   /dev/video0      c       81      0
        mknod   /dev/spiS0       c       217     0
        mknod   /dev/i2cM0       c       218     0
        mknod   /dev/rdm0        c       254     0
        mknod   /dev/flash0      c       200     0
        mknod   /dev/swnat0      c       210     0
        mknod   /dev/hwnat0      c       220     0
        mknod   /dev/acl0        c       230     0
        mknod   /dev/ac0         c       240     0
        mknod   /dev/mtr0        c       250     0
        mknod   /dev/gpio        c       252     0
	mknod   /dev/PCM         c       233     0
	mknod   /dev/I2S         c       234     0
	fi

	echo "# <device regex> <uid>:<gid> <octal permissions> [<@|$|*> <command>]" > /etc/mdev.conf
        echo "# The special characters have the meaning:" >> /etc/mdev.conf
        echo "# @ Run after creating the device." >> /etc/mdev.conf
        echo "# $ Run before removing the device." >> /etc/mdev.conf
        echo "# * Run both after creating and before removing the device." >> /etc/mdev.conf
        echo "sd[a-z][1-9] 0:0 0660 */sbin/automount.sh \$MDEV" >> /etc/mdev.conf

        #enable usb hot-plug feature
        echo "/sbin/mdev" > /proc/sys/kernel/hotplug

fi
}

# opmode adjustment:
#   if AP client was not compiled and operation mode was set "3" -> set $opmode "1"
#   if Station was not compiled and operation mode was set "2" -> set $opmode "1"
if [ "$opmode" = "3" -a "$CONFIG_RT2860V2_AP_APCLI" != "y" ]; then
	nvram_set 2860 OperationMode 1
	opmode="1"
fi
if [ "$opmode" = "2" -a "$CONFIG_RT2860V2_STA" == "" ]; then
	nvram_set 2860 OperationMode 1
	opmode="1"
fi

genSysFiles
genDevNode

if [ "$CONFIG_DWC_OTG" == "m" ]; then
usbmod_exist=`mount | grep dwc_otg | wc -l`

if [ $usbmod_exist == 0 ]; then
insmod -q lm
insmod -q dwc_otg
fi
fi

# insmod all
insmod -q bridge
insmod -q mii
insmod -q raeth
ifconfig eth2 0.0.0.0

ifRaxWdsxDown
rmmod rt2860v2_ap
rmmod rt2860v2_sta
sleep 1
ralink_init make_wireless_config rt2860
sleep 2
if [ "$stamode" = "y" ]; then
	insmod -q rt2860v2_sta
else
	if [ "$CONFIG_RT2860V2_AP_DFS" = "y" ]; then
		insmod -q rt_timer
	fi
	insmod -q rt2860v2_ap
fi
vpn-passthru.sh

# INIC support
if [ "$CONFIG_INIC_MII" != "" -o "$CONFIG_INIC_PCI" != ""  -o "$CONFIG_INIC_USB" != "" ]; then
	#modify v1.4.28
        #iNIC_Mii_en=`nvram_get inic InicMiiEnable`
        #iNIC_USB_en=`nvram_get inic InicUSBEnable`
        iNIC_Mii_en=`nvram_get 2860 InicMiiEnable`
        iNIC_USB_en=`nvram_get 2860 InicUSBEnable`
        ifconfig rai0 down
        ralink_init make_wireless_config inic
if [ "$iNIC_USB_en" == "1" ]; then
        rmmod iNIC_usb
        insmod -q iNIC_usb 
        ifconfig rai0 up
elif [ "$CONFIG_INIC_PCI" != "" ]; then
        rmmod iNIC_pci
        insmod -q iNIC_pci 
        ifconfig rai0 up
fi
fi

# RT2561(Legacy) support
if [ "$CONFIG_RT2561_AP" != "" ]; then
	ifconfig raL0 down
	rmmod rt2561ap
	ralink_init make_wireless_config rt2561
	insmod -q rt2561ap
	ifconfig raL0 up
fi

# config interface
ifconfig ra0 0.0.0.0
if [ "$ethconv" = "y" ]; then
	iwpriv ra0 set EthConvertMode=dongle
fi
if [ "$radio_off" = "1" ]; then
	iwpriv ra0 set RadioOn=0
fi
if [ "$bssidnum" = "2" ]; then
	ifconfig ra1 0.0.0.0
elif [ "$bssidnum" = "3" ]; then
	ifconfig ra1 0.0.0.0
	ifconfig ra2 0.0.0.0
elif [ "$bssidnum" = "4" ]; then
	ifconfig ra1 0.0.0.0
	ifconfig ra2 0.0.0.0
	ifconfig ra3 0.0.0.0
elif [ "$bssidnum" = "5" ]; then
	ifconfig ra1 0.0.0.0
	ifconfig ra2 0.0.0.0
	ifconfig ra3 0.0.0.0
	ifconfig ra4 0.0.0.0
elif [ "$bssidnum" = "6" ]; then
	ifconfig ra1 0.0.0.0
	ifconfig ra2 0.0.0.0
	ifconfig ra3 0.0.0.0
	ifconfig ra4 0.0.0.0
	ifconfig ra5 0.0.0.0
elif [ "$bssidnum" = "7" ]; then
	ifconfig ra1 0.0.0.0
	ifconfig ra2 0.0.0.0
	ifconfig ra3 0.0.0.0
	ifconfig ra4 0.0.0.0
	ifconfig ra5 0.0.0.0
	ifconfig ra6 0.0.0.0
elif [ "$bssidnum" = "8" ]; then
	ifconfig ra1 0.0.0.0
	ifconfig ra2 0.0.0.0
	ifconfig ra3 0.0.0.0
	ifconfig ra4 0.0.0.0
	ifconfig ra5 0.0.0.0
	ifconfig ra6 0.0.0.0
	ifconfig ra7 0.0.0.0
fi
if [ "$CONFIG_RAETH_ROUTER" = "y" -o "$CONFIG_MAC_TO_MAC_MODE" = "y" -o "$CONFIG_RT_3052_ESW" = "y" ]; then
	insmod -q 8021q
	vconfig add eth2 1
	set_vlan_map eth2.1
	vconfig add eth2 2
	set_vlan_map eth2.2
	ifconfig eth2.2 down
	wan_mac=`nvram_get 2860 WAN_MAC_ADDR`
	if [ "$wan_mac" != "FF:FF:FF:FF:FF:FF" ]; then
	ifconfig eth2.2 hw ether $wan_mac
	fi
	ifconfig eth2.1 0.0.0.0
	ifconfig eth2.2 0.0.0.0
elif [ "$CONFIG_ICPLUS_PHY" = "y" ]; then
	#remove ip alias
	# it seems busybox has no command to remove ip alias...
	ifconfig eth2:1 0.0.0.0 1>&2 2>/dev/null
fi

ifconfig lo 127.0.0.1
ifconfig br0 down
brctl delbr br0

# stop all
iptables --flush
iptables --flush -t nat
iptables --flush -t mangle


#
# init ip address to all interfaces for different OperationMode:
#   0 = Bridge Mode
#   1 = Gateway Mode
#   2 = Ethernet Converter Mode
#   3 = AP Client
#
if [ "$opmode" = "0" ]; then
	addBr0
	if [ "$CONFIG_RAETH_ROUTER" = "y" -a "$CONFIG_LAN_WAN_SUPPORT" = "y" ]; then
		echo "##### restore IC+ to dump switch #####"
		config-vlan.sh 0 0
	elif [ "$CONFIG_MAC_TO_MAC_MODE" = "y" ]; then
		echo "##### restore Vtss to dump switch #####"
		config-vlan.sh 1 0
	elif [ "$CONFIG_RT_3052_ESW" = "y" ]; then
		if [ "$CONFIG_P5_RGMII_TO_MAC_MODE" = "y" ]; then
			echo "##### restore RT3052 to dump switch #####"
			config-vlan.sh 2 0
			echo "##### restore Vtss to dump switch #####"
			config-vlan.sh 1 0
		else
			echo "##### restore RT3052 to dump switch #####"
			config-vlan.sh 2 0
		fi
	fi
	brctl addif br0 eth2
	if [ "$CONFIG_RT2860V2_AP_MBSS" = "y" -a "$bssidnum" != "1" ]; then
		addRax2Br0
	fi

	#start mii iNIC after network interface is working
	#v1.4.28
	#iNIC_Mii_en=`nvram_get inic InicMiiEnable`
	iNIC_Mii_en=`nvram_get 2860 InicMiiEnable`
	if [ "$iNIC_Mii_en" == "1" ]; then
	     ifconfig rai0 down
	     rmmod iNIC_mii
	     insmod -q iNIC_mii miimaster=eth2
	     ifconfig rai0 up
	fi

	addWds2Br0
	addMesh2Br0
	addRaix2Br0
	addInicWds2Br0
	addRaL02Br0
	wan.sh
	lan.sh
	echo 0 > /proc/sys/net/ipv4/ip_forward
elif [ "$opmode" = "1" ]; then
	if [ "$CONFIG_RAETH_ROUTER" = "y" -o "$CONFIG_MAC_TO_MAC_MODE" = "y" -o "$CONFIG_RT_3052_ESW" = "y" ]; then
		if [ "$CONFIG_RAETH_ROUTER" = "y" -a "$CONFIG_LAN_WAN_SUPPORT" = "y" ]; then
			if [ "$CONFIG_WAN_AT_P0" = "y" ]; then
				echo '##### config IC+ vlan partition (WLLLL) #####'
				config-vlan.sh 0 WLLLL
			else
				echo '##### config IC+ vlan partition (LLLLW) #####'
				config-vlan.sh 0 LLLLW
			fi
		fi
		if [ "$CONFIG_MAC_TO_MAC_MODE" = "y" ]; then
			echo '##### config Vtss vlan partition #####'
			config-vlan.sh 1 1
		fi
		if [ "$CONFIG_RT_3052_ESW" = "y" -a "$CONFIG_LAN_WAN_SUPPORT" = "y" ]; then
			if [ "$CONFIG_P5_RGMII_TO_MAC_MODE" = "y" ]; then
				echo "##### restore RT3052 to dump switch #####"
				config-vlan.sh 2 0
				echo "##### config Vtss vlan partition #####"
				config-vlan.sh 1 1
			else
				if [ "$CONFIG_WAN_AT_P0" = "y" ]; then
					echo '##### config RT3052 vlan partition (WLLLL) #####'
					config-vlan.sh 2 WLLLL
				else
					echo '##### config RT3052 vlan partition (LLLLW) #####'
					config-vlan.sh 2 LLLLW
				fi
			fi
		fi
		addBr0
		brctl addif br0 eth2.1
		if [ "$CONFIG_RT2860V2_AP_MBSS" = "y" -a "$bssidnum" != "1" ]; then
			addRax2Br0
		fi
		addWds2Br0
		addMesh2Br0
		addRaix2Br0
		addInicWds2Br0
		addRaL02Br0
	fi

	# IC+ 100 PHY (one port only)
	if [ "$CONFIG_ICPLUS_PHY" = "y" ]; then
		echo '##### connected to one port 100 PHY #####'
		if [ "$CONFIG_RT2860V2_AP_MBSS" = "y" -a "$bssidnum" != "1" ]; then
			addBr0
			addRax2Br0
		fi
		addWds2Br0
		addMesh2Br0

		#
		# setup ip alias for user to access web page.
		#
		ifconfig eth2:1 172.32.1.254 netmask 255.255.255.0 up
	fi

	wan.sh
	lan.sh
	nat.sh
elif [ "$opmode" = "2" ]; then
	# if (-1 == initStaProfile())
	#   error(E_L, E_LOG, T("internet.c: profiles in nvram is broken"));
	# else
	#   initStaConnection();
	if [ "$CONFIG_RAETH_ROUTER" = "y" -a "$CONFIG_LAN_WAN_SUPPORT" = "y" ]; then
		echo "##### restore IC+ to dump switch #####"
		config-vlan.sh 0 0
	fi
	if [ "$CONFIG_MAC_TO_MAC_MODE" = "y" ]; then
		echo "##### restore Vtss to dump switch #####"
		config-vlan.sh 1 0
	fi
	if [ "$CONFIG_RT_3052_ESW" = "y" ]; then
		if [ "$CONFIG_P5_RGMII_TO_MAC_MODE" = "y" ]; then
			echo "##### restore RT3052 to dump switch #####"
			config-vlan.sh 2 0
			echo "##### restore Vtss to dump switch #####"
			config-vlan.sh 1 0
		else
			echo "##### restore RT3052 to dump switch #####"
			config-vlan.sh 2 0
		fi
	fi
	wan.sh
	lan.sh
	nat.sh
elif [ "$opmode" = "3" ]; then
	if [ "$CONFIG_RAETH_ROUTER" = "y" -o "$CONFIG_MAC_TO_MAC_MODE" = "y" -o "$CONFIG_RT_3052_ESW" = "y" ]; then
		if [ "$CONFIG_RAETH_ROUTER" = "y" ]; then
			echo "##### restore IC+ to dump switch #####"
			config-vlan.sh 0 0
		fi
		if [ "$CONFIG_MAC_TO_MAC_MODE" = "y" ]; then
			echo "##### restore Vtss to dump switch #####"
			config-vlan.sh 1 0
		fi
		if [ "$CONFIG_RT_3052_ESW" = "y" ]; then
			if [ "$CONFIG_P5_RGMII_TO_MAC_MODE" = "y" ]; then
				echo "##### restore RT3052 to dump switch #####"
				config-vlan.sh 2 0
				echo "##### restore Vtss to dump switch #####"
				config-vlan.sh 1 0
			else
				echo "##### restore RT3052 to dump switch #####"
				config-vlan.sh 2 0
			fi
		fi
		addBr0
		brctl addif br0 eth2
	fi
	wan.sh
	lan.sh
	nat.sh
else
	echo "unknown OperationMode: $opmode"
	exit 1
fi

# INIC support
if [ "$CONFIG_RT2880_INIC" != "" ]; then
	ifconfig rai0 down
	rmmod rt_pci_dev
	ralink_init make_wireless_config inic
	insmod -q rt_pci_dev
	ifconfig rai0 up
	RaAP&
	sleep 3
fi

# in order to use broadcast IP address in L2 management daemon
if [ "$CONFIG_ICPLUS_PHY" = "y" ]; then
	route add -host 255.255.255.255 dev $wan_if
else
	route add -host 255.255.255.255 dev $lan_if
fi


m2uenabled=`nvram_get 2860 M2UEnabled`
if [ "$m2uenabled" = "1" ]; then
	iwpriv ra0 set IgmpSnEnable=1
	echo "iwpriv ra0 set IgmpSnEnable=1"
fi
#restart8021XDaemon(RT2860_NVRAM);
#firewall_init();
#management_init();




display_shell_name.sh  internet.sh end &

