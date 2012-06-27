
if [ $# -ne 1 ];then
	echo "usage:set_sim_pin.sh modem"	
	exit 1
fi

echo "set_sim_pin.sh $1"

modem=/dev/$1
SIM_PIN_FLAG=/tmp/.set_sim_pin_flag
PPP_3G_CONN_FILE=/tmp/.set_sim_pin.scr
CPIN_FILE=/tmp/.cpin_file






if [ -f $SIM_PIN_FLAG ];then
	flag=`cat $SIM_PIN_FLAG |grep "SUCCEED"`
	if [ "$flag" = "SUCCEED" ];then
		echo "sim pin had been set Succeed ,do not need to set again!" 
		exit 1
	fi
fi

	echo "" > $CPIN_FILE

	echo "opengt" > $PPP_3G_CONN_FILE
	echo "set com 115200n81" >> $PPP_3G_CONN_FILE
	echo "set senddelay 0.05" >> $PPP_3G_CONN_FILE
	echo "waitquiet 1 0.2" >> $PPP_3G_CONN_FILE

        echo "send \"AT+CPIN?\"" >> $PPP_3G_CONN_FILE
        echo "let i=0" >> $PPP_3G_CONN_FILE
        echo "open file \"$CPIN_FILE\"" >> $PPP_3G_CONN_FILE
        echo ":get_next" >> $PPP_3G_CONN_FILE
        echo "get 1 \"^m\" \$s" >> $PPP_3G_CONN_FILE
        echo "fprint \$s" >> $PPP_3G_CONN_FILE
        echo "let \$a=\$s" >> $PPP_3G_CONN_FILE
        echo "if len(\$a)>=3 let \$b=\$right(\$a,2)" >> $PPP_3G_CONN_FILE
        echo "if \$b=\"OK\" goto exit" >> $PPP_3G_CONN_FILE
        echo "inc i" >> $PPP_3G_CONN_FILE
        echo "if i<10 goto get_next" >> $PPP_3G_CONN_FILE
        echo ":exit" >> $PPP_3G_CONN_FILE
        echo "fprint \"\n\"" >> $PPP_3G_CONN_FILE
        echo "close file" >> $PPP_3G_CONN_FILE

#	echo "print \"SUCCEED\"" >> $PPP_3G_CONN_FILE

	comgt -d $modem -s $PPP_3G_CONN_FILE
	res=`cat $CPIN_FILE`
	logger $res


	res1=`cat $CPIN_FILE |grep "SIM PIN"`
	res2=`cat $CPIN_FILE |grep "SIM PUK"`
	if [ "$res1" = "" -a "$res2" = "" ];then
		res3=`cat $CPIN_FILE |grep "READY"`
		if [ "$res3" != "" ];then
			echo "SIM  is READY!" 
			logger "SIM  is READY!" 
			echo "SUCCEED" > $SIM_PIN_FLAG
		fi
		exit 1
	fi


	sim_pin=`nvram_get 2860 g3_sim_pin`
	if [ "$sim_pin" = "" ];then
		echo "sim pin is none in wan.asp!" 
		exit 1
	fi

	sleep 1

		echo "opengt" > $PPP_3G_CONN_FILE
		echo "set com 115200n81" >> $PPP_3G_CONN_FILE
		echo "set senddelay 0.05" >> $PPP_3G_CONN_FILE
		echo "waitquiet 1 0.2" >> $PPP_3G_CONN_FILE

		echo "send \"AT+CPIN=$sim_pin\"" >> $PPP_3G_CONN_FILE
		echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
		echo "if % = -1 goto timeerror" >> $PPP_3G_CONN_FILE
		echo "if % = 0 goto succeed" >> $PPP_3G_CONN_FILE
		echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE
		echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE

		echo ":succeed" >> $PPP_3G_CONN_FILE
		echo "print \"SUCCEED\"" >> $PPP_3G_CONN_FILE
		echo "exit 1" >> $PPP_3G_CONN_FILE
		echo ":timeout" >> $PPP_3G_CONN_FILE
		echo "print \"TIMEOUT\"" >> $PPP_3G_CONN_FILE
		echo "exit 1" >> $PPP_3G_CONN_FILE
		echo ":error" >> $PPP_3G_CONN_FILE
		echo "print \"ERROR\"" >> $PPP_3G_CONN_FILE
		echo "exit 1" >> $PPP_3G_CONN_FILE
	
echo "killall pppd in set_sim_pin.sh!" 
3g_down_command.sh
sleep 2
echo "set sim pin ...!" 
resule=`comgt -d $modem -s $PPP_3G_CONN_FILE`
if [ "$resule" = "SUCCEED" ];then
	echo "SUCCEED" > $SIM_PIN_FLAG
	echo "set sim pin code succeed!" 
	logger "set sim pin code succeed!" 
else
	echo "ERROR" > $SIM_PIN_FLAG
	echo "set sim pin code error!" 
	logger "set sim pin code error!" 
fi

sleep 1 


exit 1
