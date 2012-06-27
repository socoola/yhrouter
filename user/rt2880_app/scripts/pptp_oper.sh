OPTION_FILE=/etc/pptp_option
PPTP_DAEMON_PID_FILE=/tmp/.pptp_daemon_pid



usage()
{
	echo "usage:pptp_oper.sh start|stop"
}
make_option_file()
{
	user=`nvram_get pptp_user`
	password=`nvram_get pptp_password`
	server=`nvram_get pptp_server`
	remote_lan=`nvram_get pptp_remote_lan`
	remote_mask_str=`nvram_get pptp_remote_mask`
	remote_mask=`masktoprefix.sh $remote_mask_str`
	pptp_localip_mode=`nvram_get pptp_localip_mode`
	pptp_local_ip=`nvram_get pptp_local_ip`
	pptp_mppe_encryption=`nvram_get pptp_mppe_encryption`
	
	
	echo "debug" > $OPTION_FILE
	echo "lock" >> $OPTION_FILE
	echo "noauth" >> $OPTION_FILE
	echo "novj" >> $OPTION_FILE
	echo "nobsdcomp" >> $OPTION_FILE
	echo "nodeflate" >> $OPTION_FILE
	echo "unit 1000" >> $OPTION_FILE
	#echo "lcp-echo-interval 10" >> $OPTION_FILE
	#echo "lcp-echo-failure 6" >> $OPTION_FILE

	echo "user \"$user\" " >> $OPTION_FILE
	echo "password \"$password\"" >> $OPTION_FILE
	echo "pty 'pptp $server --nolaunchpppd' " >> $OPTION_FILE
	echo "ipparam \"pptp:$remote_lan/$remote_mask\"" >> $OPTION_FILE

	if [ "$pptp_localip_mode" == "static" ];then
		echo "$pptp_local_ip:" >> $OPTION_FILE
	else
		echo "noipdefault" >> $OPTION_FILE
	fi

	if [ "$pptp_mppe_encryption" == "on" ];then
		echo "refuse-eap" >> $OPTION_FILE
		echo "refuse-chap" >> $OPTION_FILE
		echo "refuse-pap" >> $OPTION_FILE
		echo "refuse-mschap" >> $OPTION_FILE
		pptp_mppe_40=`nvram_get pptp_mppe_40`
		if [ "$pptp_mppe_40" == "on" ];then 
			echo "require-mppe-40" >> $OPTION_FILE
		else
			echo "require-mppe-128" >> $OPTION_FILE
		fi
		pptp_mppe_refuse_stateless=`nvram_get pptp_mppe_refuse_stateless`
		if [ "pptp_mppe_refuse_stateless" == "on" ];then 
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
	if [ -f $PPTP_DAEMON_PID_FILE ];then
		PID=`cat $PPTP_DAEMON_PID_FILE`
		if [ "$PID" != "" ];then
			kill $PID
		fi
		rm -rf $PPTP_DAEMON_PID_FILE
	fi


	PID=`ps |grep -v grep | grep pppd | grep $OPTION_FILE |sed 's/^ *//' |sed 's/ .*//' `
	if [ "$PID" != "" ];then
		kill $PID
	fi
}


case $1 in
	start)
		operate_stop
		pptp_status=`nvram_get pptp_active`
		if [ "$pptp_status" != "on" ];then
			#echo "pptp status is off!"
			logger "pptp status is off!"
			exit 0
		fi

		make_option_file

		pptp_daemon.sh&
	#	pppd file $OPTION_FILE &
		
	;;
	stop)
		operate_stop
	;;
	*)
		usage
	;;
esac

