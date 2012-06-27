#!/bin/sh
#
# $Id: lan.sh,v 1.24.2.1 2009-04-21 12:49:37 michael Exp $
#
# usage: wan.sh
#

display_shell_name.sh  lan.sh start &
. /sbin/global.sh




# stop all
killall -q udhcpd
killall -q lld2d
killall -q igmpproxy
killall -q upnpd
killall -q radvd
killall -q pppoe-relay
killall -q dnsmasq
rm -rf /var/run/lld2d-*
echo "" > /var/udhcpd.leases

# ip address
ip=`nvram_get 2860 lan_ipaddr`
nm=`nvram_get 2860 lan_netmask`
ifconfig $lan_if down
ifconfig $lan_if $ip netmask $nm
opmode=`nvram_get 2860 OperationMode`
if [ "$opmode" = "0" ]; then
	gw=`nvram_get 2860 wan_gateway`
	pd=`nvram_get 2860 wan_primary_dns`
	sd=`nvram_get 2860 wan_secondary_dns`
	route del default
	route add default gw $gw
	config-dns.sh $pd $sd
fi

ifconfig "br0:9" down
ifconfig "eth2:9" down
lan2enabled=`nvram_get 2860 Lan2Enabled`
if [ "$lan2enabled" = "1" ]; then
	ip_2=`nvram_get 2860 lan2_ipaddr`
	nm_2=`nvram_get 2860 lan2_netmask`
	if [ "$opmode" = "0" ]; then
		ifconfig "br0:9" "$ip_2" netmask "$nm_2"
		echo "ifconfig "br0:9" "$ip_2" netmask "$nm_2""
	elif [ "$opmode" = "1" ]; then
		ifconfig "br0:9" "$ip_2" netmask "$nm_2"
		echo "ifconfig "br0:9" "$ip_2" netmask "$nm_2""
	elif [ "$opmode" = "2" ]; then
		ifconfig "eth2:9" "$ip_2" netmask "$nm_2"
		echo "ifconfig "eth2:9" "$ip_2" netmask "$nm_2""
	elif [ "$opmode" = "3" ]; then
		ifconfig "br0:9" "$ip_2" netmask "$nm_2"
		echo "ifconfig "br0:9" "$ip_2" netmask "$nm_2""
	fi
fi

# hostname
host=`nvram_get 2860 HostName`
if [ "$host" = "" ]; then
	host="ralink"
	nvram_set 2860 HostName ralink
fi
hostname $host
echo "127.0.0.1 localhost.localdomain localhost" > /etc/hosts
echo "$ip $host.ralinktech.com $host" >> /etc/hosts

# dhcp server
dhcp=`nvram_get 2860 dhcpEnabled`
if [ "$dhcp" = "1" ]; then
	start=`nvram_get 2860 dhcpStart`
	end=`nvram_get 2860 dhcpEnd`
	mask=`nvram_get 2860 dhcpMask`
	pd=`nvram_get 2860 dhcpPriDns`
	sd=`nvram_get 2860 dhcpSecDns`
	gw=`nvram_get 2860 dhcpGateway`
	lease=`nvram_get 2860 dhcpLease`
	static1=`nvram_get 2860 dhcpStatic1 | sed -e 's/;/ /'`
	static2=`nvram_get 2860 dhcpStatic2 | sed -e 's/;/ /'`
	static3=`nvram_get 2860 dhcpStatic3 | sed -e 's/;/ /'`
	config-udhcpd.sh -s $start
	config-udhcpd.sh -e $end
	config-udhcpd.sh -i $lan_if
	config-udhcpd.sh -m $mask
	if [ "$pd" != "" -o "$sd" != "" ]; then
		config-udhcpd.sh -d $pd $sd
	fi
	if [ "$gw" != "" ]; then
		config-udhcpd.sh -g $gw
	fi
	if [ "$lease" != "" ]; then
		config-udhcpd.sh -t $lease
	fi
	if [ "$static1" != "" ]; then
		config-udhcpd.sh -S $static1
	fi
	if [ "$static2" != "" ]; then
		config-udhcpd.sh -S $static2
	fi
	if [ "$static3" != "" ]; then
		config-udhcpd.sh -S $static3
	fi
	#config-udhcpd.sh -r 1
	config-udhcpd.sh -r 0
fi

# stp
if [ "$wan_if" = "br0" -o "$lan_if" = "br0" ]; then
	stp=`nvram_get 2860 stpEnabled`
	if [ "$stp" = "1" ]; then
		brctl setfd br0 15
		brctl stp br0 1
	else
		brctl setfd br0 1
		brctl stp br0 0
	fi
fi

# lltd
lltd=`nvram_get 2860 lltdEnabled`
if [ "$lltd" = "1" ]; then
	lld2d $lan_if
fi

# igmpproxy
igmp=`nvram_get 2860 igmpEnabled`
if [ "$igmp" = "1" ]; then
	config-igmpproxy.sh $wan_if $lan_if
fi

# upnp
if [ "$opmode" = "0" -o "$opmode" = "1" ]; then
	upnp=`nvram_get 2860 upnpEnabled`
	if [ "$upnp" = "1" ]; then
		route add -net 239.0.0.0 netmask 255.0.0.0 dev $lan_if
		upnp_xml.sh $ip
		upnpd -f $wan_ppp_if $lan_if &
	fi
fi

# radvd
radvd=`nvram_get 2860 radvdEnabled`
ifconfig sit0 down
echo "0" > /proc/sys/net/ipv6/conf/all/forwarding
if [ "$radvd" = "1" ]; then
	echo "1" > /proc/sys/net/ipv6/conf/all/forwarding
	ifconfig sit0 up
	ifconfig sit0 add 2002:1101:101::1101:101/16
	route -A inet6 add 2000::/3 gw ::17.1.1.20 dev sit0
	route -A inet6 add 2002:1101:101:0::/64 dev br0
	radvd -C /etc_ro/radvd.conf -d 1 &
fi

# pppoe-relay
pppr=`nvram_get 2860 pppoeREnabled`
if [ "$pppr" = "1" ]; then
	pppoe-relay -S $wan_if -B $lan_if
fi

# dns proxy
dnsp=`nvram_get 2860 dnsPEnabled`
if [ "$dnsp" = "1" ]; then
	dnsmasq &
fi

display_shell_name.sh  lan.sh end &


