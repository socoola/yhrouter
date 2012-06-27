#only the pppd of l2tpd is sync nodetach...
OPTION_FILE="sync nodetach noaccomp nobsdcomp nodeflate"
DAEMON_PID_FILE=/tmp/.l2tp_daemon_pid

echo $$ > $DAEMON_PID_FILE
while true;do

	PID=""
	PID=`ps |grep -v grep | grep pppd | grep "$OPTION_FILE" |sed 's/^ *//' |sed 's/ .*//' `
	if [ "$PID" == "" ];then 
		#pppd file $OPTION_FILE&
		server=`cat /etc/l2tp/l2tp.conf |grep "peer " | sed 's/peer //'`
		killall l2tpd >/dev/null 2>&1
		sleep 1
		l2tpd
		l2tp-control "start-session $server" >/dev/null 2>&1
	fi

	sleep 10
done
