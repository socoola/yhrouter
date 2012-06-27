OPTION_FILE=/etc/pptp_option
DAEMON_PID_FILE=/tmp/.pptp_daemon_pid

echo $$ > $DAEMON_PID_FILE
while true;do

	PID=""
	PID=`ps |grep -v grep | grep pppd | grep $OPTION_FILE |sed 's/^ *//' |sed 's/ .*//' `
	if [ "$PID" == "" ];then 
		pppd file $OPTION_FILE&
	fi

	sleep 10
done
