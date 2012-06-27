#!/bin/sh
#
#
# usage: wan.sh
#


display_shell_name.sh  wan.sh start &
. /sbin/global.sh




# stop all
killall -q syslogd
killall -q udhcpc
#killall -q pppd
3g_down_command.sh
#killall -q l2tpd
#killall -q openl2tpd


clone_en=`nvram_get 2860 macCloneEnabled`
clone_mac=`nvram_get 2860 macCloneMac`
#MAC Clone: bridge mode doesn't support MAC Clone
if [ "$opmode" != "0" -a "$clone_en" = "1" ]; then
	ifconfig $wan_if down
	if [ "$opmode" = "2" ]; then
		rmmod rt2860v2_sta
		insmod rt2860v2_sta mac=$clone_mac
	else
		ifconfig $wan_if hw ether $clone_mac
	fi
	ifconfig $wan_if up
fi

if [ "$wanmode" = "STATIC" -o "$opmode" = "0" ]; then
	#always treat bridge mode having static wan connection
	ip=`nvram_get 2860 wan_ipaddr`
	nm=`nvram_get 2860 wan_netmask`
	gw=`nvram_get 2860 wan_gateway`
	pd=`nvram_get 2860 wan_primary_dns`
	sd=`nvram_get 2860 wan_secondary_dns`

	#lan and wan ip should not be the same except in bridge mode
	if [ "$opmode" != "0" ]; then
		lan_ip=`nvram_get 2860 lan_ipaddr`
		if [ "$ip" = "$lan_ip" ]; then
			echo "wan.sh: warning: WAN's IP address is set identical to LAN"
			exit 0
		fi
	else
		#use lan's ip address instead
		ip=`nvram_get 2860 lan_ipaddr`
		nm=`nvram_get 2860 lan_netmask`
	fi
	ifconfig $wan_if $ip netmask $nm
	route del default
	if [ "$gw" != "" ]; then
	route add default gw $gw
	fi
	config-dns.sh $pd $sd
elif [ "$wanmode" = "DHCP" ]; then
	udhcpc -i $wan_if -s /sbin/udhcpc.sh -p /var/run/udhcpc.pid &
elif [ "$wanmode" = "PPPOE" ]; then
	u=`nvram_get 2860 wan_pppoe_user`
	pw=`nvram_get 2860 wan_pppoe_pass`
	pppoe_opmode=`nvram_get 2860 wan_pppoe_opmode`
	pppoe_mtu=`nvram_get 2860 pppoe_mtu`
	if [ "$pppoe_opmode" = "" ]; then
		echo "pppoecd $wan_if -u $u -p $pw"
		pppoecd $wan_if -u "$u" -p "$pw"
	else
		pppoe_optime=`nvram_get 2860 wan_pppoe_optime`
		config-pppoe.sh $u $pw $wan_if $pppoe_opmode $pppoe_optime $pppoe_mtu
	fi
elif [ "$wanmode" = "L2TP" ]; then
	srv=`nvram_get 2860 wan_l2tp_server`
	u=`nvram_get 2860 wan_l2tp_user`
	pw=`nvram_get 2860 wan_l2tp_pass`
	mode=`nvram_get 2860 wan_l2tp_mode`
	l2tp_opmode=`nvram_get 2860 wan_l2tp_opmode`
	l2tp_optime=`nvram_get 2860 wan_l2tp_optime`
	if [ "$mode" = "0" ]; then
		ip=`nvram_get 2860 wan_l2tp_ip`
		nm=`nvram_get 2860 wan_l2tp_netmask`
		gw=`nvram_get 2860 wan_l2tp_gateway`
		if [ "$gw" = "" ]; then
			gw="0.0.0.0"
		fi
		config-l2tp.sh static $wan_if $ip $nm $gw $srv $u $pw $l2tp_opmode $l2tp_optime
	else
		config-l2tp.sh dhcp $wan_if $srv $u $pw $l2tp_opmode $l2tp_optime
	fi
elif [ "$wanmode" = "PPTP" ]; then
	srv=`nvram_get 2860 wan_pptp_server`
	u=`nvram_get 2860 wan_pptp_user`
	pw=`nvram_get 2860 wan_pptp_pass`
	mode=`nvram_get 2860 wan_pptp_mode`
	pptp_opmode=`nvram_get 2860 wan_pptp_opmode`
	pptp_optime=`nvram_get 2860 wan_pptp_optime`
	if [ "$mode" = "0" ]; then
		ip=`nvram_get 2860 wan_pptp_ip`
		nm=`nvram_get 2860 wan_pptp_netmask`
		gw=`nvram_get 2860 wan_pptp_gateway`
		if [ "$gw" = "" ]; then
			gw="0.0.0.0"
		fi
		config-pptp.sh static $wan_if $ip $nm $gw $srv $u $pw $pptp_opmode $pptp_optime
	else
		config-pptp.sh dhcp $wan_if $srv $u $pw $pptp_opmode $pptp_optime
	fi
elif [ "$wanmode" = "3G" ]; then
        dev=`nvram_get 2860 wan_3g_dev`
        3g.sh $dev &
else
	echo "wan.sh: unknown wan connection type: $wanmode"
	exit 1
fi

#add by zsf 2010-12-09 v1.4.28
start_reboot_timer.sh network_reboot_start &

display_shell_name.sh  wan.sh end &

