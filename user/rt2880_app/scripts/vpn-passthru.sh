#!/bin/sh

. /sbin/config.sh
. /sbin/global.sh

l2tp_pt=`nvram_get 2860 l2tpPassThru`
ipsec_pt=`nvram_get 2860 ipsecPassThru`
pptp_pt=`nvram_get 2860 pptpPassThru`


# note: they must be removed in order
if [ "$CONFIG_NF_CONNTRACK_SUPPORT" = "y" ]; then
	rmmod nf_nat_pptp
	rmmod nf_conntrack_pptp
	rmmod nf_nat_proto_gre
	rmmod nf_conntrack_proto_gre
else
	rmmod ip_nat_pptp
	rmmod ip_conntrack_pptp
fi

if [ "$pptp_pt" = "1" -o "$l2tp_pt" = "1" -o "$ipsec_pt" = "1" ]; then
if [ "$CONFIG_NF_CONNTRACK_SUPPORT" = "y" ]; then
	insmod -q nf_conntrack_proto_gre
	insmod -q nf_nat_proto_gre

	if [ "$pptp_pt" = "1" ]; then
		insmod -q nf_conntrack_pptp
		insmod -q nf_nat_pptp
	fi
else
	insmod -q ip_conntrack_pptp
	insmod -q ip_nat_pptp
fi 
fi 
