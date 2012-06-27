#!/bin/sh
#
# $Id: config-pptp.sh,v 1.6 2009-01-07 09:48:14 steven Exp $
#
# usage: config-pptp.sh <mode> <mode_params> <server> <user> <password>
#

. /sbin/config.sh
. /sbin/global.sh

usage()
{
	echo "Usage:"
	echo "  $0 <mode> <mode_params> <server> <user> <password>"
	echo "Modes:"
	echo "  static - <mode_params> = <wan_if_name> <wan_ip> <wan_netmask> <gateway>"
	echo "  dhcp - <mode_params> = <wan_if_name>"
	echo "Example:"
	echo "  $0 static eth2.2 10.10.10.254 255.255.255.0 10.10.10.253 192.168.1.1 user pass"
	echo "  $0 dhcp eth2.2 192.168.1.1 user pass"
	exit 1
}

if [ "$5" = "" ]; then
	echo "$0: insufficient arguments"
	usage $0
fi

if [ "$1" = "static" ]; then
	if [ "$7" = "" ]; then
		echo "$0: insufficient arguments"
		usage $0
	fi
	ifconfig $2 $3 netmask $4
	route del default
	if [ "$5" != "0.0.0.0" ]; then
		route add default gw $5
	fi
	pptp_srv=$6
	pptp_u=$7
	pptp_pw=$8
	pptp_opmode=$9
	pptp_optime=${10}
elif [ "$1" = "dhcp" ]; then
	killall -q udhcpc
	udhcpc -i $2 -s /sbin/udhcpc.sh -p /var/run/udhcpd.pid &
	pptp_srv=$3
	pptp_u=$4
	pptp_pw=$5
	pptp_opmode=$6
	pptp_optime=$7
else
	echo "$0: unknown connection mode: $1"
	usage $0
fi

if [ "$CONFIG_PPPOPPTP" == "y" ]; then
accel-pptp.sh $pptp_u $pptp_pw $pptp_srv $pptp_opmode $pptp_optime
else
pptp.sh $pptp_u $pptp_pw $pptp_srv $pptp_opmode $pptp_optime
fi
pppd file /etc/options.pptp  &
