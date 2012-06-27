#!/bin/sh

. /sbin/config.sh


# WAN interface name -> $wan_if
getWanIfName()
{
	wan_mode=`nvram_get 2860 wanConnectionMode`
	if [ "$opmode" = "0" ]; then
		wan_if="br0"
	elif [ "$opmode" = "1" ]; then
		if [ "$CONFIG_RAETH_ROUTER" = "y" -o "$CONFIG_MAC_TO_MAC_MODE" = "y" -o "$CONFIG_RT_3052_ESW" = "y" ]; then
			wan_if="eth2.2"
		else
			wan_if="eth2"
		fi
	elif [ "$opmode" = "2" ]; then
		wan_if="ra0"
	elif [ "$opmode" = "3" ]; then
		wan_if="apcli0"
	fi

	if [ "$wan_mode" = "PPPOE" -o  "$wan_mode" = "L2TP" -o "$wan_mode" = "PPTP"  ]; then
		wan_ppp_if="ppp0"
	else
		wan_ppp_if=$wan_if
	fi
}

# LAN interface name -> $lan_if
getLanIfName()
{
	bssidnum=`nvram_get 2860 BssidNum`

	if [ "$opmode" = "0" ]; then
		lan_if="br0"
	elif [ "$opmode" = "1" ]; then
		if [ "$CONFIG_RAETH_ROUTER" = "y" -o "$CONFIG_MAC_TO_MAC_MODE" = "y" -o "$CONFIG_RT_3052_ESW" = "y" ]; then
			lan_if="br0"
		elif [ "$CONFIG_ICPLUS_PHY" = "y" ]; then 
			if [ "$CONFIG_RT2860V2_AP_MBSS" = "y" -a "$bssidnum" != "1" ]; then
				lan_if="br0"
			else
				lan_if="ra0"
			fi
		else
			lan_if="ra0"
		fi
	elif [ "$opmode" = "2" ]; then
		lan_if="eth2"
	elif [ "$opmode" = "3" ]; then
		lan_if="br0"
	fi
}

# ethernet converter enabled -> $ethconv "y"
getEthConv()
{
	ec=`nvram_get 2860 ethConvert`
	if [ "$opmode" = "0" -a "$CONFIG_RT2860V2_STA_DPB" = "y" -a "$ec" = "1" ]; then
		ethconv="y"
	else
		ethconv="n"
	fi
}

# station driver loaded -> $stamode "y"
getStaMode()
{
	if [ "$opmode" = "2" -o "$ethconv" = "y" ]; then
		stamode="y"
	else
		stamode="n"
	fi
}

opmode=`nvram_get 2860 OperationMode`
wanmode=`nvram_get 2860 wanConnectionMode`
ethconv="n"
stamode="n"
wan_if="br0"
wan_ppp_if="br0"
lan_if="br0"
getWanIfName
getLanIfName
getEthConv
getStaMode

# debug
#echo "opmode=$opmode"
#echo "wanmode=$wanmode"
#echo "ethconv=$ethconv"
#echo "stamode=$stamode"
#echo "wan_if=$wan_if"
#echo "lan_if=$lan_if"

