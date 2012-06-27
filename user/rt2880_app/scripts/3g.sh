#!/bin/sh


display_shell_name.sh  3g.sh start &
if [ "$1" != "" ]; then
        dev=$1
else
        dev=`nvram_get 2860 wan_3g_dev`
fi
echo $dev > /tmp/.3G_DEVICE
user=`nvram_get 2860 G3UserName`

password=`nvram_get 2860 G3Password`
g3opmode=`nvram_get 2860 wan_3g_opmode`

echo "3g:device=$dev"
echo "3g:user=$user"
echo "3g:password=$password"
echo "3g:opmode=$g3opmode"


if [ "$dev" != "OPTION-ICON225" ]; then
	echo ""
elif [ "$dev" = "OPTION-ICON225" ]; then
	rmmod hso
	sleep 3
	insmod hso
	sleep 3
fi

modem_f=ttyUSB0

comparetime()
{
	if [ $1 -gt $3 ];then
		return 2
	elif [ $1 -eq $3 ];then
		if [ $2 -gt $4 ];then
			return 2
		elif [ $2 -eq $4 ];then
			return 1
		else
			return 0
		fi
	else
		return 
	fi
}

if [ "$dev" = "HUAWEI-EM660" -o "$dev" = "KSE-360" ]; then
	modem_f=ttyUSB0
	g3_network=`nvram_get 2860 g3_network_type`
	export WLNETWORK=8
	if [ $g3_network = 0 ];then
		WLNETWORK=8
	elif [ $g3_network = 1 ];then
		WLNETWORK=4
	elif [ $g3_network = 2 ];then
		WLNETWORK=2
	else
		WLNETWORK=8
	fi
fi

if [ "$dev" = "IE901D" ]; then
        modem_f=ttyUSB0
        g3_network=`nvram_get 2860 g3_network_type`
        export WLNETWORK=8
        if [ $g3_network = 0 ];then
                WLNETWORK=8
        elif [ $g3_network = 1 ];then
                WLNETWORK=4
        elif [ $g3_network = 2 ];then
                WLNETWORK=2
        else
                WLNETWORK=8
        fi
fi


if [ "$dev" = "HUAWEI-EM770" ]; then
	modem_f=ttyUSB0
	g3_network=`nvram_get 2860 g3_network_type`
	export WLNETWORK=2
	if [ $g3_network = 0 ];then
		WLNETWORK=2
	elif [ $g3_network = 1 ];then
		WLNETWORK=14
	elif [ $g3_network = 2 ];then
		WLNETWORK=13
	elif [ $g3_network = 3 ];then
		WLNETWORK=16
	else
		WLNETWORK=2
	fi
fi
if [ "$dev" = "F3607gw" ]; then
	modem_f=ttyACM0
	g3_network=`nvram_get 2860 g3_network_type`
	export WLNETWORK=2
	if [ $g3_network = 0 ];then
		WLNETWORK=2
	elif [ $g3_network = 1 ];then
		WLNETWORK=14
	elif [ $g3_network = 2 ];then
		WLNETWORK=13
	elif [ $g3_network = 3 ];then
		WLNETWORK=16
	else
		WLNETWORK=2
	fi
fi
if [ "$dev" = "HUAWEI-EM560" ]; then
	modem_f=ttyACM0
	g3_network=`nvram_get 2860 g3_network_type`
	export WLNETWORK=2
	if [ $g3_network = 0 ];then
		WLNETWORK=2
	elif [ $g3_network = 1 ];then
		WLNETWORK=15
	elif [ $g3_network = 2 ];then
		WLNETWORK=14
	elif [ $g3_network = 3 ];then
		WLNETWORK=13
	else
		WLNETWORK=2
	fi
fi

if [ "$dev" = "THINKWILL-MI600" ]; then
	modem_f=ttyUSB2
	g3_network=`nvram_get 2860 g3_network_type`
	export WLNETWORK=8
	if [ $g3_network = 0 ];then
		WLNETWORK=8
	elif [ $g3_network = 1 ];then
		WLNETWORK=4
	elif [ $g3_network = 2 ];then
		WLNETWORK=2
	else
		WLNETWORK=8
	fi
fi

if [ "$dev" = "SYNCWISER-801/401" ]; then
	modem_f=ttyUSB0
	g3_network=`nvram_get 2860 g3_network_type`
	export WLNETWORK=2
	if [ $g3_network = 0 ];then
		WLNETWORK=2
	elif [ $g3_network = 1 ];then
		WLNETWORK=1
	elif [ $g3_network = 2 ];then
		WLNETWORK=0
	else
		WLNETWORK=2
	fi
fi

if [ "$dev" = "LONGSUNG-C5300" ]; then
	modem_f=ttyUSB0
	g3_network=`nvram_get 2860 g3_network_type`
	export WLNETWORK=8
	if [ $g3_network = 0 ];then
		WLNETWORK=8
	elif [ $g3_network = 1 ];then
		WLNETWORK=4
	elif [ $g3_network = 2 ];then
		WLNETWORK=2
	else
		WLNETWORK=8
	fi
fi

if [ "$dev" = "LONGSUNG-U6300/U5300" ]; then
	modem_f=ttyUSB2
	g3_network=`nvram_get 2860 g3_network_type`
	export WLNETWORK=2
	if [ $g3_network = 0 ];then
		WLNETWORK=2
	elif [ $g3_network = 1 ];then
		WLNETWORK=1
	elif [ $g3_network = 2 ];then
		WLNETWORK=4
	elif [ $g3_network = 3 ];then
		WLNETWORK=3
	else
		WLNETWORK=2
	fi
fi

if [ "$dev" = "GAORAN-280" ]; then
#	usbSwitch 23 & 
	modem_f=ttyUSB1
	g3_network=`nvram_get 2860 g3_network_type`
	export WLNETWORK=0
	if [ $g3_network = 0 ];then
		WLNETWORK=0
	elif [ $g3_network = 1 ];then
		WLNETWORK=2
	elif [ $g3_network = 2 ];then
		WLNETWORK=1
	else
		WLNETWORK=0
	fi
fi

if [ "$dev" = "TW-W1M100" ]; then
	modem_f=ttyUSB2
	g3_network=`nvram_get 2860 g3_network_type`
	export WLNETWORK=2
	if [ $g3_network = 0 ];then
		WLNETWORK=2
	elif [ $g3_network = 1 ];then
		WLNETWORK=1
	elif [ $g3_network = 2 ];then
		WLNETWORK=4
	elif [ $g3_network = 3 ];then
		WLNETWORK=3
	else
		WLNETWORK=2
	fi
fi
if [ "$dev" = "MC5728" ]; then
	modem_f=ttyUSB0
fi
if [ "$dev" = "SIMCOM-SIM700" ]; then
	modem_f=ttyACM0
fi

if [ "$dev" = "ZTE-MU301" ]; then
	3GInfo -d /dev/ttyUSB0
	3GInfo -d /dev/ttyUSB2
	modem_f=ttyUSB0
	g3_network=`nvram_get 2860 g3_network_type`
	export WLNETWORK=2
	if [ $g3_network = 0 ];then
		WLNETWORK=2
	elif [ $g3_network = 1 ];then
		WLNETWORK=15
	elif [ $g3_network = 2 ];then
		WLNETWORK=14
	elif [ $g3_network = 3 ];then
		WLNETWORK=13
	else
		WLNETWORK=2
	fi
fi

if [ "$dev" = "ZTE-MF210V" ]; then
	#modem_f=ttyUSB0
	modem_f=ttyUSB3
	g3_network=`nvram_get 2860 g3_network_type`
	export WLNETWORK="0,0,2"
	if [ $g3_network = 0 ];then
		WLNETWORK="0,0,2"
	elif [ $g3_network = 1 ];then
		WLNETWORK="2,0,0"
	elif [ $g3_network = 2 ];then
		WLNETWORK="1,0,0"
	else
		WLNETWORK="0,0,2"
	fi
fi
if [ "$dev" = "ZX-600" ]; then
	modem_f=ttyUSB0
	g3_network=`nvram_get 2860 g3_network_type`
	export WLNETWORK=0
	if [ $g3_network = 0 ];then
		WLNETWORK=0
	elif [ $g3_network = 1 ];then
		WLNETWORK=4
	elif [ $g3_network = 2 ];then
		WLNETWORK=2
	elif [ $g3_network = 3 ];then
		WLNETWORK=3
	elif [ $g3_network = 4 ];then
		WLNETWORK=1
	else
		WLNETWORK=2
	fi
fi
if [ "$dev" = "GTM681W" ]; then
        #modem_f=ttyUSB0
        modem_f=ttyUSB2
        g3_network=`nvram_get 2860 g3_network_type`
        export WLNETWORK=""
        if [ $g3_network = 0 ];then
                WLNETWORK=""
        fi

fi
if [ "$dev" = "SIERRA-MC8785" ]; then
        #modem_f=ttyUSB0
        modem_f=ttyUSB4
        g3_network=`nvram_get 2860 g3_network_type`
        export WLNETWORK=""
        if [ $g3_network = 0 ];then
                WLNETWORK=""
        fi

fi

if [ "$dev" = "AD3812" ]; then
        #modem_f=ttyUSB0
        modem_f=ttyUSB3
        g3_network=`nvram_get 2860 g3_network_type`
        export WLNETWORK=""
        if [ $g3_network = 0 ];then
                WLNETWORK=""
        fi

fi


#SIGN_FOR_CHECK_3G 

if [ "$dev" != "OPTION-ICON225" ]; then
	config-3g-ppp.sh -p $password -u $user -m $modem_f -c Generic_conn.scr -d Generic_disconn.scr
	if [ $g3opmode == "G3ModeAol" ]; then
		#dialup 3g led is config in config-3g.sh
		set_sim_pin.sh $modem_f
		echo "3g:pppd call 3g (aol mode)"
		pppd call 3g
	elif [ $g3opmode == "G3ModeDod" ]; then
		set_sim_pin.sh $modem_f
		echo "3g:pppd call 3g (dod mode)"
		pppd call 3g
	elif [ $g3opmode == "G3ModeDot" ]; then
		set_sim_pin.sh $modem_f
		echo "3g:do 3g_ontme.sh"
		cellontimepid=`ps |grep "3g_ontime.sh" |  grep -v "grep" |sed 's/^ *//' | sed 's/ .*//'`	
		if [ "$cellontimepid" != "" ];then
			kill $cellontimepid
			sleep 1
		fi
		3g_ontime.sh &
	fi
	
elif [ "$dev" = "OPTION-ICON225" ]; then
	echo "APN=internet" > /etc/conninfo.ini
	hso_connect.sh up
fi

sleep 30
echo "do (start_msg_cmd)"
start_msg_cmd&
display_shell_name.sh  3g.sh end &

