#!/bin/sh
IPSEC_COUNT_FILE=/tmp/.ipsec_count


if [ ! -f $IPSEC_COUNT_FILE ];then
	echo "0" > $IPSEC_COUNT_FILE
fi

if [ "$1" == "phase1_up" ];then
	vpn_count=`cat $IPSEC_COUNT_FILE`
	vpn_count=`expr $vpn_count + 1`
	echo "$vpn_count" > $IPSEC_COUNT_FILE
	gpio l 18 4000 0 1 1 4000 
	logger "ipsec_count=$vpn_count"
elif [ "$1" == "phase1_down" ];then
	vpn_count=`cat $IPSEC_COUNT_FILE`
	if [ $vpn_count -gt 0 ];then
		vpn_count=`expr $vpn_count - 1`
		echo "$vpn_count" > $IPSEC_COUNT_FILE
	fi
	if [ $vpn_count -eq 0 ];then
		gpio l 18 0 4000  1 1 4000 
	fi
	logger "ipsec_count=$vpn_count"
fi


