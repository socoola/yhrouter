#!/bin/sh

echo "config-3g-ppp.sh begin"
PPP_3G_FILE=/etc_ro/ppp/peers/3g
init=`nvram_get 2860 g3_initial_cmd`
conn_num=`nvram_get 2860 g3_dial_num`
opmode=`nvram_get 2860 wan_3g_opmode`
optime=`nvram_get 2860 g3idletime`
g3_dialup_device=`nvram_get 2860 wan_3g_dev`
localip=`nvram_get 2860 G3LocalIp`
cell_mtu=`nvram_get 2860 cell_mtu`
auth_type=`nvram_get 2860 G3auth_type`

usage () {
  echo "usage: config-3g-ppp.sh [option]..."
  echo "options:"
  echo "  -h              : print this help"
  echo "  -p password     : set password"
  echo "  -u username     : set username"
  echo "  -b baud         : Set baudrate"
  echo "  -m dev 	  : set modem device"
  echo "  -c conn         : set connect AT script"
  echo "  -d disconn	  : set disconnect AT script"
  exit
}


for arg in $*
  do
    if [ "$1" != "" ] 
    then
      case "$1" in
        "-p")
	  pwd_for_auth=$2
	  if [ "$auth_type" == "1" ];then 
         	PASSWORD="" 
	  elif [ "$auth_type" == "2" ];then 
         	PASSWORD="" 
	  else
         	PASSWORD="password $2" 
	  fi
    	  shift ;;
        "-u")
	  user_for_auth=$2
	  if [ "$auth_type" == "1" ];then 
          	USERNAME="name $2" 
	  elif [ "$auth_type" == "2" ];then 
          	USERNAME="name $2" 
	  else
          	USERNAME="user $2" 
	  fi
    	  shift ;;
        "-b")
          BAUD="$2"
	  shift ;;
        "-m")
          MODEM="/dev/$2" 
	  shift ;;
        "-c")
          CONN="$2" 
	  shift ;;
        "-d")
          DISCONN="$2" 
	  shift ;;
        "-h")
	  usage ;;
        *) 
	  echo "illegal option -- $2" 
	  usage ;;
      esac
      shift
  fi
  done

PPP_3G_CONN_FILE=/etc/$CONN
echo "user 3g conn file:"$PPP_3G_CONN_FILE
echo "opengt" > $PPP_3G_CONN_FILE
echo "set com 115200n81" >> $PPP_3G_CONN_FILE
echo "set senddelay 0.05" >> $PPP_3G_CONN_FILE
echo "waitquiet 1 0.2" >> $PPP_3G_CONN_FILE





if [[ "$g3_dialup_device" == "HUAWEI-EM560" -o "$g3_dialup_device" == "ZTE-MU301" -o "$g3_dialup_device" == "F3607gw" ]]; then


	if [ "$g3_dialup_device" == "ZTE-MU301" ];then
		echo "system \"3GInfo -d $MODEM >/dev/null 2>&1 \" " >> $PPP_3G_CONN_FILE
	fi


echo "send \"ATE1\"" >> $PPP_3G_CONN_FILE
echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
echo "if % = -1 goto timeerror" >>$PPP_3G_CONN_FILE
echo "if % = 0 goto next11" >> $PPP_3G_CONN_FILE
echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE
echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE
echo ":next11" >> $PPP_3G_CONN_FILE
echo "send \"AT+CFUN=1\"" >> $PPP_3G_CONN_FILE
echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
echo "if % = -1 goto timeerror" >>$PPP_3G_CONN_FILE
echo "if % = 0 goto next12" >> $PPP_3G_CONN_FILE
echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE
echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE
echo ":next12" >> $PPP_3G_CONN_FILE
echo "waitfor 3" >> $PPP_3G_CONN_FILE
echo "goto next1" >> $PPP_3G_CONN_FILE


elif [ "$g3_dialup_device" == "MC5728" ]; then

echo "send \"ATZ\"" >> $PPP_3G_CONN_FILE
echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
echo "if % = -1 goto timeerror" >>$PPP_3G_CONN_FILE
echo "if % = 0 goto next11" >> $PPP_3G_CONN_FILE
echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE
echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE

echo ":next11" >> $PPP_3G_CONN_FILE
echo "send \"ATE1\"" >> $PPP_3G_CONN_FILE
echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
echo "if % = -1 goto timeerror" >>$PPP_3G_CONN_FILE
echo "if % = 0 goto next1" >> $PPP_3G_CONN_FILE
echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE
echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE



elif [ "$g3_dialup_device" == "SIMCOM-SIM700" ]; then

echo "waitfor 20" >> $PPP_3G_CONN_FILE
echo "send \"ATZ\"" >> $PPP_3G_CONN_FILE
echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
echo "if % = -1 goto timeerror" >>$PPP_3G_CONN_FILE
echo "if % = 0 goto next11" >> $PPP_3G_CONN_FILE
echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE
echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE
echo ":next11" >> $PPP_3G_CONN_FILE
echo "send \"AT+CFUN=1\"" >> $PPP_3G_CONN_FILE
echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
echo "if % = -1 goto timeerror" >>$PPP_3G_CONN_FILE
echo "if % = 0 goto next12" >> $PPP_3G_CONN_FILE
echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE
echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE
echo ":next12" >> $PPP_3G_CONN_FILE
echo "waitfor 3" >> $PPP_3G_CONN_FILE
echo "goto next1" >> $PPP_3G_CONN_FILE

elif [ "$g3_dialup_device" == "SYNCWISER-801/401" ]; then


echo "goto next1" >> $PPP_3G_CONN_FILE


else

echo "send \"ATZ\"" >> $PPP_3G_CONN_FILE
echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
echo "if % = -1 goto timeerror" >>$PPP_3G_CONN_FILE
echo "if % = 0 goto next1" >> $PPP_3G_CONN_FILE
echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE
echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE

fi
echo ":next1" >> $PPP_3G_CONN_FILE

#echo -e	"waitquiet 1 0.1\n"\
#	"putenv  \"SIMSTATE=\\\"Not Ready\\\"\"\n"\
#	"system \"echo \$SIMSTATE >/var/sim\"\n"\
#	"send \"AT+CPIN?\"\n"\
#	"waitfor 5 \"READY\",\"ERR\"\n"\
#	"if % != 0 goto error\n"\
#	"putenv  \"SIMSTATE=Inserted\"\n"\
#	"system \"echo \$SIMSTATE >/var/sim\"\n"\
#	>>$PPP_3G_CONN_FILE	
cat /etc_ro/ppp/3g/imei >>$PPP_3G_CONN_FILE
cat /etc_ro/ppp/3g/sim >>$PPP_3G_CONN_FILE	

#echo "waitquiet 1 0.1">>$PPP_3G_CONN_FILE
#echo "system \"echo xx>/var/signal\"" >>$PPP_3G_CONN_FILE
#echo "send \"AT+CSQ?\"" >>$PPP_3G_CONN_FILE
#echo "get 2 \"^m\" \$s" >>$PPP_3G_CONN_FILE
#echo "get 2 \"^m\" \$s" >>$PPP_3G_CONN_FILE
#echo "let a=len(\$s)" >>$PPP_3G_CONN_FILE
#echo "let a=a-6" >>$PPP_3G_CONN_FILE
#echo "let \$s=\$right(\$s,a)" >>$PPP_3G_CONN_FILE
#echo "putenv \"SIGNAL=\"\$s" >>$PPP_3G_CONN_FILE
#echo "system \"echo \$SIGNAL >/var/signal\"" >>$PPP_3G_CONN_FILE
cat /etc_ro/ppp/3g/signal >>$PPP_3G_CONN_FILE

user=`nvram_get 2860 G3UserName`
password=`nvram_get 2860 G3Password`
#if [ "$init" != "" ]; then
	
	if [ "$g3_dialup_device" == "GTM681W" -o "$g3_dialup_device" == "SIERRA-MC8785" ]; then
		init="at+cgdcont=1,\\\"IP\\\",""\\\""$init"\\\""
	elif [ "$g3_dialup_device" == "HUAWEI-EM770" ]; then
		init="at+cgdcont=1,\\\"IP\\\",""\\\""$init"\\\""
	elif [[ "$g3_dialup_device" == "HUAWEI-EM660" -o "$g3_dialup_device" == "KSE-360" -o "$g3_dialup_device" == "IE901D" ]]; then
		init="at\\^pppcfg=\\\""$user"\\\",\\\""$password"\\\"" 
	elif [[ "$g3_dialup_device" == "HUAWEI-EM560" -o "$g3_dialup_device" == "ZTE-MU301" -o "$g3_dialup_device" == "F3607gw" ]]; then
		init="at+cgdcont=1,\\\"IP\\\",""\\\""$init"\\\""
	elif [ "$g3_dialup_device" == "THINKWILL-MI600" ]; then
		init="at\\^pppcfg=\\\""$user"\\\",\\\""$password"\\\"" 
	elif [ "$g3_dialup_device" == "SYNCWISER-801/401" ]; then
		init="at\\^pppcfg=\\\""$user"\\\",\\\""$password"\\\""
	elif [ "$g3_dialup_device" == "LONGSUNG-C5300" ]; then
		init="at\\^pppcfg=\\\""$user"\\\",\\\""$password"\\\""
	elif [ "$g3_dialup_device" == "LONGSUNG-U6300/U5300" ]; then
		init="at+cgdcont=1,\\\"IP\\\",""\\\""$init"\\\""
	elif [ "$g3_dialup_device" == "GAORAN-280" ]; then
		init="at+cgdcont=1,\\\"IP\\\",""\\\""$init"\\\""

	elif [ "$g3_dialup_device" == "TW-W1M100" ]; then
		init="at+cgdcont=1,\\\"IP\\\",""\\\""$init"\\\""
	elif [ "$g3_dialup_device" == "MC5728" ]; then
		init="at\\^pppcfg=\\\""$user"\\\",\\\""$password"\\\""
	elif [ "$g3_dialup_device" == "SIMCOM-SIM700" ]; then
		init="at+cgdcont=1,\\\"IP\\\",""\\\""$init"\\\""
	elif [ "$g3_dialup_device" == "ZTE-MF210V" ]; then
		init="at+cgdcont=1,\\\"IP\\\",""\\\""$init"\\\""

	elif [ "$g3_dialup_device" == "ZX-600" ]; then
		init="at+cgdcont=1,\\\"IP\\\",""\\\""$init"\\\""

   elif [ "$g3_dialup_device" == "AD3812" ]; then
		init="at+cgdcont=1,\\\"IP\\\",""\\\""$init"\\\""
      

	else
		init="at+cgdcont=1,\\\"IP\\\",""\\\""$init"\\\""
	fi

      	echo "send \"$init\"" >> $PPP_3G_CONN_FILE
			echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE

	echo "if % = -1 goto timeerror" >> $PPP_3G_CONN_FILE
	echo "if % = 0 goto next2" >> $PPP_3G_CONN_FILE
	echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE
	echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE
#else
	#echo "goto next2" >> $PPP_3G_CONN_FILE
#fi



echo ":next2" >> $PPP_3G_CONN_FILE



if [ "$g3_dialup_device" == "GTM681W" -o "$g3_dialup_device" == "SIERRA-MC8785" ]; then

        echo "goto next21" >> $PPP_3G_CONN_FILE

elif [ "$g3_dialup_device" == "HUAWEI-EM770" ]; then
        echo "config-3g-ppp.sh:use  device HUAWEI-EM770 "
        echo "send \"AT\\^SYSCFG=$WLNETWORK,0,3FFFFFFF,2,4\"" >> $PPP_3G_CONN_FILE
        echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
        echo "if % = -1 goto timeerror" >> $PPP_3G_CONN_FILE
        echo "if % = 0 goto next21" >> $PPP_3G_CONN_FILE
        echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE
        echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE
elif [[ "$g3_dialup_device" == "HUAWEI-EM660" -o "$g3_dialup_device" == "KSE-360" -o "$g3_dialup_device" == "IE901D" ]]; then
        echo "config-3g-ppp.sh:use  device HUAWEI-EM660  or kse-360 or ie901d"
        echo "send \"AT\\^PREFMODE=$WLNETWORK\"" >> $PPP_3G_CONN_FILE
        echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
        echo "if % = -1 goto timeerror" >> $PPP_3G_CONN_FILE
        echo "if % = 0 goto next21" >> $PPP_3G_CONN_FILE
        echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE
        echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE
elif [[ "$g3_dialup_device" == "HUAWEI-EM560" -o "$g3_dialup_device" == "ZTE-MU301" -o "$g3_dialup_device" == "F3607gw" ]]; then
        echo "config-3g-ppp.sh:use  device HUAWEI-EM560  or ZTE-MU301 or F3607gw"
        echo "send \"AT\\^SYSCONFIG=$WLNETWORK,0,1,2\"" >> $PPP_3G_CONN_FILE
        echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
        echo "if % = -1 goto timeerror" >> $PPP_3G_CONN_FILE
        echo "if % = 0 goto next21" >> $PPP_3G_CONN_FILE
        echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE
        echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE
elif [ "$g3_dialup_device" == "THINKWILL-MI600" ]; then
        echo "config-3g-ppp.sh:use  device THINKWILL-MI600 "
        echo "send \"AT\\^PREFMODE=$WLNETWORK\"" >> $PPP_3G_CONN_FILE
        echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
        echo "if % = -1 goto timeerror" >> $PPP_3G_CONN_FILE
        echo "if % = 0 goto next21" >> $PPP_3G_CONN_FILE
        echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE
        echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE
elif [ "$g3_dialup_device" == "SYNCWISER-801/401" ]; then
        echo "config-3g-ppp.sh:use  device SYNCWISER-801/401 "
        echo "send \"AT*NAM*SVCMD=$WLNETWORK\"" >> $PPP_3G_CONN_FILE
        echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
        echo "if % = -1 goto timeerror" >> $PPP_3G_CONN_FILE
        echo "if % = 0 goto next21" >> $PPP_3G_CONN_FILE
        echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE
        echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE
elif [ "$g3_dialup_device" == "LONGSUNG-C5300" ]; then
        echo "config-3g-ppp.sh:use  device LONGSUNG-C5300 "
        echo "send \"AT\\^PREFMODE=$WLNETWORK\"" >> $PPP_3G_CONN_FILE
        echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
        echo "if % = -1 goto timeerror" >> $PPP_3G_CONN_FILE
        echo "if % = 0 goto next21" >> $PPP_3G_CONN_FILE
        echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE
        echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE
elif [ "$g3_dialup_device" == "LONGSUNG-U6300/U5300" ]; then
        echo "config-3g-ppp.sh:use  device LONGSUNG-U6300/U5300"
        echo "send \"AT+MODODR=$WLNETWORK\"" >> $PPP_3G_CONN_FILE
        echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
        echo "if % = -1 goto timeerror" >> $PPP_3G_CONN_FILE
        echo "if % = 0 goto next21" >> $PPP_3G_CONN_FILE
        echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE
        echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE
elif [ "$g3_dialup_device" == "GAORAN-280" ]; then
        echo "config-3g-ppp.sh:use  device GAORAN-280"
        echo "waitfor 5" >> $PPP_3G_CONN_FILE
        echo "send \"AT+ZSNT=$WLNETWORK,0,0\"" >> $PPP_3G_CONN_FILE
        echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
        echo "if % = -1 goto timeerror" >> $PPP_3G_CONN_FILE
        echo "if % = 0 goto next211" >> $PPP_3G_CONN_FILE
        echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE
        echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE



	echo ":next211" >> $PPP_3G_CONN_FILE
        echo "waitfor 15" >> $PPP_3G_CONN_FILE
        echo "goto next21" >> $PPP_3G_CONN_FILE
	


elif [ "$g3_dialup_device" == "TW-W1M100" ]; then
        echo "config-3g-ppp.sh:use  device TW-W1M100"
        echo "send \"AT+MODODR=$WLNETWORK\"" >> $PPP_3G_CONN_FILE
        echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
        echo "if % = -1 goto timeerror" >> $PPP_3G_CONN_FILE
        echo "if % = 0 goto next21" >> $PPP_3G_CONN_FILE
        echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE
        echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE
elif [ "$g3_dialup_device" == "AD3812" ]; then
        echo "config-3g-ppp.sh:use  device MC5728"
        echo "goto next21" >> $PPP_3G_CONN_FILE
elif [ "$g3_dialup_device" == "MC5728" ]; then
        echo "config-3g-ppp.sh:use  device MC5728"
        echo "goto next21" >> $PPP_3G_CONN_FILE

elif [ "$g3_dialup_device" == "SIMCOM-SIM700" ]; then
        echo "config-3g-ppp.sh:use  device SIMCOM-SIM700"
        echo "goto next21" >> $PPP_3G_CONN_FILE
elif [ "$g3_dialup_device" == "ZTE-MF210V" ]; then
        echo "config-3g-ppp.sh:use  device ZTE-MF210V"
        echo "send \"AT+ZSNT=$WLNETWORK\"" >> $PPP_3G_CONN_FILE
        echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
        echo "if % = -1 goto timeerror" >> $PPP_3G_CONN_FILE
        echo "if % = 0 goto next21" >> $PPP_3G_CONN_FILE
        echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE
        echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE

elif [ "$g3_dialup_device" == "ZX-600" ]; then
        echo "config-3g-ppp.sh:use  device ZX-600"
        echo "send \"AT+PHPREF=$WLNETWORK\"" >> $PPP_3G_CONN_FILE
        echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
        echo "if % = -1 goto timeerror" >> $PPP_3G_CONN_FILE
        echo "if % = 0 goto next21" >> $PPP_3G_CONN_FILE
        echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE
        echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE
else
        echo "config-3g-ppp.sh:use  device NONE "
        echo "goto error" >> $PPP_3G_CONN_FILE
fi




echo ":next21" >> $PPP_3G_CONN_FILE
echo "system \"3gdialup_led.sh fast\"" >> $PPP_3G_CONN_FILE
echo "send \"ATD$conn_num\"" >> $PPP_3G_CONN_FILE
echo "waitfor 20 \"CONNECT\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
echo "if % = -1 goto timeerror" >> $PPP_3G_CONN_FILE
echo "if % = 0 goto next3" >> $PPP_3G_CONN_FILE
echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE
echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE
echo ":next3" >> $PPP_3G_CONN_FILE
echo "print \"CONNECTED\n\"" >> $PPP_3G_CONN_FILE
echo "system \"3gdialup_led.sh on\""  >> $PPP_3G_CONN_FILE
echo "exit 0" >> $PPP_3G_CONN_FILE
echo ":error" >> $PPP_3G_CONN_FILE
echo "print \"CONNECT ERROR\n\"" >> $PPP_3G_CONN_FILE
echo "system \"3gdialup_led.sh off\"" >> $PPP_3G_CONN_FILE
echo "exit 1" >> $PPP_3G_CONN_FILE
echo ":timeerror" >> $PPP_3G_CONN_FILE
echo "print \"CONNECT TIMEOUT\n\"" >> $PPP_3G_CONN_FILE
echo "system \"3gdialup_led.sh off\"" >> $PPP_3G_CONN_FILE
echo "exit 1" >> $PPP_3G_CONN_FILE

echo $MODEM > $PPP_3G_FILE
echo $BAUD >> $PPP_3G_FILE
echo $USERNAME >> $PPP_3G_FILE
echo $PASSWORD >> $PPP_3G_FILE
echo "debug" >> $PPP_3G_FILE
echo "logfile /flash/pppd.log" >> $PPP_3G_FILE
echo "modem" >> $PPP_3G_FILE
echo "crtscts" >> $PPP_3G_FILE
echo "noauth" >> $PPP_3G_FILE
echo "defaultroute" >> $PPP_3G_FILE
if [ "$localip" == "" ]; then
	echo "noipdefault" >> $PPP_3G_FILE
else
	echo "$localip:" >> $PPP_3G_FILE
fi
echo "nopcomp" >> $PPP_3G_FILE
echo "noaccomp" >> $PPP_3G_FILE
echo "novj" >> $PPP_3G_FILE
echo "nobsdcomp" >> $PPP_3G_FILE
echo "usepeerdns" >> $PPP_3G_FILE
if [ $opmode == "G3ModeAol" ]; then
	echo "persist" >> $PPP_3G_FILE
elif [ $opmode == "G3ModeDot" ]; then
	echo "persist" >> $PPP_3G_FILE
elif [ $opmode == "G3ModeDod" ]; then
	optime=`expr $optime \* 60`
	echo "demand" >> $PPP_3G_FILE
	echo "idle $optime" >> $PPP_3G_FILE
fi
echo "nodeflate" >> $PPP_3G_FILE 

if [ "$cell_mtu" != "" ];then
	echo "mtu $cell_mtu" >> $PPP_3G_FILE
fi


echo "connect \"/bin/comgt -d $MODEM -s $PPP_3G_CONN_FILE\"" >> $PPP_3G_FILE 

if [ "$g3_dialup_device" == "THINKWILL-MI600" ]; then
	echo "disconnect \"/bin/comgt -d /dev/ttyUSB4 -s /etc_ro/ppp/3g/$DISCONN\"" >> $PPP_3G_FILE
elif [ "$g3_dialup_device" == "ZTE-MU301" ];then
	echo "disconnect \"/bin/comgt -d /dev/ttyUSB2 -s /etc_ro/ppp/3g/$DISCONN\"" >> $PPP_3G_FILE
else
	echo "disconnect \"/bin/comgt -d $MODEM -s /etc_ro/ppp/3g/$DISCONN\"" >> $PPP_3G_FILE
fi

cat /etc_ro/ppp/3g/sim >>/etc_ro/ppp/3g/$DISCONN	
cat /etc_ro/ppp/3g/signal >>/etc_ro/ppp/3g/$DISCONN	
#cat /etc_ro/ppp/3g/imei>>/etc_ro/ppp/3g/$DISCONN


if [ "$auth_type" == "0" ];then 
	rm -rf /etc_ro/ppp/chap-secrets
	rm -rf /etc_ro/ppp/pap-secrets
    echo "$user_for_auth * $pwd_for_auth  " > /etc_ro/ppp/pap-secrets
	chmod 700 /etc_ro/ppp/pap-secrets
    echo "$user_for_auth * $pwd_for_auth  " > /etc_ro/ppp/chap-secrets
	chmod 700 /etc_ro/ppp/chap-secrets
elif [ "$auth_type" == "2" ];then 
	rm -rf /etc_ro/ppp/chap-secrets
	echo "$user_for_auth * $pwd_for_auth  " > /etc_ro/ppp/pap-secrets
	chmod 700 /etc_ro/ppp/pap-secrets
elif [ "$auth_type" == "1" ];then 
	rm -rf /etc_ro/ppp/pap-secrets
	echo "$user_for_auth * $pwd_for_auth  " > /etc_ro/ppp/chap-secrets
	chmod 700 /etc_ro/ppp/chap-secrets
fi
echo "config-3g-ppp.sh end"
