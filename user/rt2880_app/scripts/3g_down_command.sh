#used in 3g_ontime.sh
PPPPID=`ps |grep "pppd.*3g.*" |  grep -v "grep" |sed 's/^ *//' | sed 's/ .*//'`
if [ "$PPPPID" != "" ];then
	kill $PPPPID
	3gdialup_led.sh off
fi
