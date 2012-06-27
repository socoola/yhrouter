#!/bin/sh

srv=`nvram_get 2860 DDNSProvider`
ddns=`nvram_get 2860 DDNS`
u=`nvram_get 2860 DDNSAccount`
pw=`nvram_get 2860 DDNSPassword`

killall -q inadyn

if [ "$srv" = "" -o "$srv" = "none" ]; then
	exit 0
fi
if [ "$ddns" = "" -o "$u" = "" -o "$pw" = "" ]; then
	exit 0
fi

# debug
echo "srv=$srv"
echo "ddns=$ddns"
echo "u=$u"
echo "pw=$pw"


if [ "$srv" = "dyndns.org" ]; then
	inadyn -u $u -p $pw -a $ddns --dyndns_system dyndns@$srv &
elif [ "$srv" = "freedns.afraid.org" ]; then
	inadyn -u $u -p $pw -a $ddns --dyndns_system default@$srv &
elif [ "$srv" = "zoneedit.com" ]; then
	inadyn -u $u -p $pw -a $ddns --dyndns_system default@$srv &
elif [ "$srv" = "no-ip.com" ]; then
	inadyn -u $u -p $pw -a $ddns --dyndns_system default@$srv &
elif [ "$srv" = "3322.org" ]; then
	connected_mode=`nvram_get 2860 wanConnectionMode`
	if [ "$connected_mode" = "DHCP" ];then
		ez-ipupdate -i eth2.2 -h $ddns -S qdns -u $u:$pw &
	elif [ "$connected_mode" = "STATIC" ];then
		ez-ipupdate -i eth2.2 -h $ddns -S qdns -u $u:$pw &
	else
		#add in ip-up	
		echo "3322.org ddns add in ip-up"
		#ez-ipupdate -i $interface -h $ddns -S qdns -u $u:$pw &
	fi
else
	echo "$0: unknown DDNS provider: $srv"
	exit 1
fi

