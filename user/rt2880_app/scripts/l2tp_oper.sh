OPTION_FILE=/etc/l2tp_option
L2TPD_CONFIG_DIR=/etc/l2tp
L2TPD_CONFIG_FILE=/etc/l2tp/l2tp.conf
HOSTNAME=yinghua
L2TP_DAEMON_PID_FILE=/tmp/.l2tp_daemon_pid

server=`nvram_get l2tp_server`


usage()
{
	echo "usage:l2tp_oper.sh start|stop"
}
make_l2tp_conf()
{
	if [ ! -d $L2TPD_CONFIG_DIR ];then
		mkdir -p $L2TPD_CONFIG_DIR
	fi
	echo "global" > $L2TPD_CONFIG_FILE
	echo "load-handler \"sync-pppd.so\"" >> $L2TPD_CONFIG_FILE
	echo "load-handler \"cmd.so\"" >> $L2TPD_CONFIG_FILE
	echo "listen-port 1701" >> $L2TPD_CONFIG_FILE
	echo "section sync-pppd" >> $L2TPD_CONFIG_FILE
	echo "lac-pppd-opts \"file $OPTION_FILE\"" >> $L2TPD_CONFIG_FILE
	echo "section peer" >> $L2TPD_CONFIG_FILE
	echo "hostname $HOSTNAME" >> $L2TPD_CONFIG_FILE
	echo "peer $server" >> $L2TPD_CONFIG_FILE
	echo "port 1701" >> $L2TPD_CONFIG_FILE
	echo "lac-handler sync-pppd" >> $L2TPD_CONFIG_FILE
	#echo "persist yes" >> $L2TPD_CONFIG_FILE
	#echo "maxfail 32767" >> $L2TPD_CONFIG_FILE
	#echo "holdoff 30" >> $L2TPD_CONFIG_FILE
	echo "hide-avps no" >> $L2TPD_CONFIG_FILE
	echo "section cmd" >> $L2TPD_CONFIG_FILE
	
	
}
make_option_file()
{
	user=`nvram_get l2tp_user`
	password=`nvram_get l2tp_password`

	remote_lan=`nvram_get l2tp_remote_lan`
	remote_mask_str=`nvram_get l2tp_remote_mask`
	remote_mask=`masktoprefix.sh $remote_mask_str`
	l2tp_localip_mode=`nvram_get l2tp_localip_mode`
	l2tp_local_ip=`nvram_get l2tp_local_ip`
	l2tp_mppe_encryption=`nvram_get l2tp_mppe_encryption`
	
	
	echo "debug" > $OPTION_FILE
	echo "lock" >> $OPTION_FILE
	echo "noauth" >> $OPTION_FILE
	echo "novj" >> $OPTION_FILE
	echo "nobsdcomp" >> $OPTION_FILE
	echo "nodeflate" >> $OPTION_FILE
	echo "unit 2000" >> $OPTION_FILE
	#echo "lcp-echo-interval 10" >> $OPTION_FILE
	#echo "lcp-echo-failure 6" >> $OPTION_FILE

	echo "user \"$user\" " >> $OPTION_FILE
	echo "password \"$password\"" >> $OPTION_FILE

#	echo "pty 'l2tp $server --nolaunchpppd' " >> $OPTION_FILE

	echo "ipparam \"l2tp:$remote_lan/$remote_mask\"" >> $OPTION_FILE

	if [ "$l2tp_localip_mode" == "static" ];then
		echo "$l2tp_local_ip:" >> $OPTION_FILE
	else
		echo "noipdefault" >> $OPTION_FILE
	fi

	if [ "$l2tp_mppe_encryption" == "on" ];then
		echo "refuse-eap" >> $OPTION_FILE
		echo "refuse-chap" >> $OPTION_FILE
		echo "refuse-pap" >> $OPTION_FILE
		echo "refuse-mschap" >> $OPTION_FILE
		l2tp_mppe_40=`nvram_get l2tp_mppe_40`
		if [ "$l2tp_mppe_40" == "on" ];then 
			echo "require-mppe-40" >> $OPTION_FILE
		else
			echo "require-mppe-128" >> $OPTION_FILE
		fi
		l2tp_mppe_refuse_stateless=`nvram_get l2tp_mppe_refuse_stateless`
		if [ "l2tp_mppe_refuse_stateless" == "on" ];then 
			echo "mppe-stateful">> $OPTION_FILE
		else
			echo "nomppe-stateful" >> $OPTION_FILE
		fi
	else
		echo "nomppe" >> $OPTION_FILE
	fi

	#it can make memory small
	#echo "persist" >> $OPTION_FILE
	
	
}


operate_stop()
{

	if [ -f $L2TP_DAEMON_PID_FILE ];then
		PID=`cat $L2TP_DAEMON_PID_FILE`
		if [ "$PID" != "" ];then
			kill $PID >/dev/null 2>&1
		fi
		rm -rf $L2TP_DAEMON_PID_FILE
	fi



	killall  l2tpd >/dev/null 2>&1
}


case $1 in
	start)
		operate_stop
		
		l2tp_status=`nvram_get l2tp_active`
		if [ "$l2tp_status" != "on" ];then
			#echo "l2tp status is off!"
			logger "l2tp status is off!"
			exit 0
		fi
		make_l2tp_conf
		make_option_file
		l2tp_daemon.sh&
		#sleep 1
		#l2tpd
		#l2tp-control "start-session $server"
	;;
	stop)
		operate_stop
	;;
	*)
		usage
	;;
esac

