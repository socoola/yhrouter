#!/bin/sh

PPP_3G_FILE=/etc_ro/ppp/peers/3g
init=`nvram_get 2860 g3_initial_cmd`
conn_num=`nvram_get 2860 g3_dial_num`
opmode=`nvram_get 2860 wan_3g_opmode`
optime=`nvram_get 2860 wan_3g_optime`
g3_dialup_device=`nvram_get 2860 wan_3g_dev`

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
          PASSWORD="password $2" 
    	  shift ;;
        "-u")
          USERNAME="user $2" 
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
echo "opengt" > $PPP_3G_CONN_FILE
echo "set com 115200n81" >> $PPP_3G_CONN_FILE
echo "set senddelay 0.05" >> $PPP_3G_CONN_FILE
echo "waitquiet 1 0.2" >> $PPP_3G_CONN_FILE
echo "send \"ATZ^m\"" >> $PPP_3G_CONN_FILE
echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
echo "if % = -1 goto timeerror" >>$PPP_3G_CONN_FILE
echo "if % = 0 goto next1" >> $PPP_3G_CONN_FILE
echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE
echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE
echo ":next1" >> $PPP_3G_CONN_FILE

if [ "$init" != "" ]; then
      	echo "send \"$init\"^m\"" >> $PPP_3G_CONN_FILE
			echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE

	echo "if % = -1 goto timeerror" >> $PPP_3G_CONN_FILE
	echo "if % = 0 goto next2" >> $PPP_3G_CONN_FILE
	echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE
	echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE
else
	echo "goto next2" >> $PPP_3G_CONN_FILE
fi

echo ":next2" >> $PPP_3G_CONN_FILE



if [ "$g3_dialup_device" == "HUAWEI-EM770" ]; then
        echo "config-3g-ppp.sh:use  device HUAWEI-EM770 "
        echo "send \"AT\\^SYSCFG=$WLNETWORK,0,3FFFFFFF,2,4^m\"" >> $PPP_3G_CONN_FILE
        echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
        echo "if % = -1 goto timeerror" >> $PPP_3G_CONN_FILE
        echo "if % = 0 goto next21" >> $PPP_3G_CONN_FILE
        echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE
        echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE
elif [ "$g3_dialup_device" == "HUAWEI-EM660" ]; then
        echo "config-3g-ppp.sh:use  device HUAWEI-EM660 "
        echo "send \"AT\\^PREFMODE=$WLNETWORK^m\"" >> $PPP_3G_CONN_FILE
        echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
        echo "if % = -1 goto timeerror" >> $PPP_3G_CONN_FILE
        echo "if % = 0 goto next21" >> $PPP_3G_CONN_FILE
        echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE
        echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE
else
        echo "config-3g-ppp.sh:use  device NONE "
        echo "goto error" >> $PPP_3G_CONN_FILE
fi



#echo "send \"AT\\^PREFMODE=$WLNETWORK^m\"" >> $PPP_3G_CONN_FILE
#echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
#echo "if % = -1 goto timeerror" >> $PPP_3G_CONN_FILE
#echo "if % = 0 goto next21" >> $PPP_3G_CONN_FILE
#echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE
#echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE




echo ":next21" >> $PPP_3G_CONN_FILE
echo "system \"3gdialup_led.sh fast\"" >> $PPP_3G_CONN_FILE
echo "send \"ATD$conn_num^m\"" >> $PPP_3G_CONN_FILE
echo "waitfor 10 \"CONNECT\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
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
echo "logfile /var/pppd.log" >> $PPP_3G_FILE
echo "modem" >> $PPP_3G_FILE
echo "crtscts" >> $PPP_3G_FILE
echo "noauth" >> $PPP_3G_FILE
echo "defaultroute" >> $PPP_3G_FILE
echo "noipdefault" >> $PPP_3G_FILE
echo "nopcomp" >> $PPP_3G_FILE
echo "noaccomp" >> $PPP_3G_FILE
echo "novj" >> $PPP_3G_FILE
echo "nobsdcomp" >> $PPP_3G_FILE
echo "usepeerdns" >> $PPP_3G_FILE
if [ $opmode == "G3ModeAol" ]; then
	echo "persist" >> $PPP_3G_FILE
elif [ $opmode == "G3ModeDod" ]; then
	optime=`expr $optime \* 60`
	echo "demand" >> $PPP_3G_FILE
	echo "idle $optime" >> $PPP_3G_FILE
fi
echo "nodeflate" >> $PPP_3G_FILE 
echo "connect \"/bin/comgt -d $MODEM -s $PPP_3G_CONN_FILE\"" >> $PPP_3G_FILE 
echo "disconnect \"/bin/comgt -d $MODEM -s /etc_ro/ppp/3g/$DISCONN\"" >> $PPP_3G_FILE



