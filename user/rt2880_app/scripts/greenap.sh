setGreenAP()
{
	start=`nvram_get 2860 GreenAPStart1`
	end=`nvram_get 2860 GreenAPEnd1`
	action=`nvram_get 2860 GreenAPAction1`
	if [ "$action" = "WiFiOFF" ]; then
		echo "$start * * * ifconfig ra0 down" >> /var/spool/cron/crontabs/admin
		echo "$end * * * ifconfig ra0 up" >> /var/spool/cron/crontabs/admin
	elif [ "$action" = "TX25" ]; then
		echo "$start * * * greenap.sh txpower 25" >> /var/spool/cron/crontabs/admin
		echo "$end * * * greenap.sh txpower normal" >> /var/spool/cron/crontabs/admin
	elif [ "$action" = "TX50" ]; then
		echo "$start * * * greenap.sh txpower 50" >> /var/spool/cron/crontabs/admin
		echo "$end * * * greenap.sh txpower normal" >> /var/spool/cron/crontabs/admin
	elif [ "$action" = "TX75" ]; then
		echo "$start * * * greenap.sh txpower 75" >> /var/spool/cron/crontabs/admin
		echo "$end * * * greenap.sh txpower normal" >> /var/spool/cron/crontabs/admin
	fi
	start=`nvram_get 2860 GreenAPStart2`
	end=`nvram_get 2860 GreenAPEnd2`
	action=`nvram_get 2860 GreenAPAction2`
	if [ "$action" = "WiFiOFF" ]; then
		echo "$start * * * ifconfig ra0 down" >> /var/spool/cron/crontabs/admin
		echo "$end * * * ifconfig ra0 up" >> /var/spool/cron/crontabs/admin
	elif [ "$action" = "TX25" ]; then
		echo "$start * * * greenap.sh txpower 25" >> /var/spool/cron/crontabs/admin
		echo "$end * * * greenap.sh txpower normal" >> /var/spool/cron/crontabs/admin
	elif [ "$action" = "TX50" ]; then
		echo "$start * * * greenap.sh txpower 50" >> /var/spool/cron/crontabs/admin
		echo "$end * * * greenap.sh txpower normal" >> /var/spool/cron/crontabs/admin
	elif [ "$action" = "TX75" ]; then
		echo "$start * * * greenap.sh txpower 75" >> /var/spool/cron/crontabs/admin
		echo "$end * * * greenap.sh txpower normal" >> /var/spool/cron/crontabs/admin
	fi
	start=`nvram_get 2860 GreenAPStart3`
	end=`nvram_get 2860 GreenAPEnd3`
	action=`nvram_get 2860 GreenAPAction3`
	if [ "$action" = "WiFiOFF" ]; then
		echo "$start * * * ifconfig ra0 down" >> /var/spool/cron/crontabs/admin
		echo "$end * * * ifconfig ra0 up" >> /var/spool/cron/crontabs/admin
	elif [ "$action" = "TX25" ]; then
		echo "$start * * * greenap.sh txpower 25" >> /var/spool/cron/crontabs/admin
		echo "$end * * * greenap.sh txpower normal" >> /var/spool/cron/crontabs/admin
	elif [ "$action" = "TX50" ]; then
		echo "$start * * * greenap.sh txpower 50" >> /var/spool/cron/crontabs/admin
		echo "$end * * * greenap.sh txpower normal" >> /var/spool/cron/crontabs/admin
	elif [ "$action" = "TX75" ]; then
		echo "$start * * * greenap.sh txpower 75" >> /var/spool/cron/crontabs/admin
		echo "$end * * * greenap.sh txpower normal" >> /var/spool/cron/crontabs/admin
	fi
	start=`nvram_get 2860 GreenAPStart4`
	end=`nvram_get 2860 GreenAPEnd4`
	action=`nvram_get 2860 GreenAPAction4`
	if [ "$action" = "WiFiOFF" ]; then
		echo "$start * * * ifconfig ra0 down" >> /var/spool/cron/crontabs/admin
		echo "$end * * * ifconfig ra0 up" >> /var/spool/cron/crontabs/admin
	elif [ "$action" = "TX25" ]; then
		echo "$start * * * greenap.sh txpower 25" >> /var/spool/cron/crontabs/admin
		echo "$end * * * greenap.sh txpower normal" >> /var/spool/cron/crontabs/admin
	elif [ "$action" = "TX50" ]; then
		echo "$start * * * greenap.sh txpower 50" >> /var/spool/cron/crontabs/admin
		echo "$end * * * greenap.sh txpower normal" >> /var/spool/cron/crontabs/admin
	elif [ "$action" = "TX75" ]; then
		echo "$start * * * greenap.sh txpower 75" >> /var/spool/cron/crontabs/admin
		echo "$end * * * greenap.sh txpower normal" >> /var/spool/cron/crontabs/admin
	fi
}

case $1 in
	"init")
		killall -q crond
		mkdir -p /var/spool/cron/crontabs
		rm -f /var/spool/cron/crontabs/admin
		cronebl="0"
		action=`nvram_get 2860 GreenAPAction1`
		if [ "$action" != "Disable" -a "$action" != "" ]; then
			start=`nvram_get 2860 GreenAPStart1`
			cronebl="1"
			greenap.sh setchk $start
		fi
		action=`nvram_get 2860 GreenAPAction2`
		if [ "$action" != "Disable" -a "$action" != "" ]; then
			start=`nvram_get 2860 GreenAPStart2`
			cronebl="1"
			greenap.sh setchk $start
		fi
		action=`nvram_get 2860 GreenAPAction3`
		if [ "$action" != "Disable" -a "$action" != "" ]; then
			start=`nvram_get 2860 GreenAPStart3`
			cronebl="1"
			greenap.sh setchk $start
		fi
		action=`nvram_get 2860 GreenAPAction4`
		if [ "$action" != "Disable" -a "$action" != "" ]; then
			start=`nvram_get 2860 GreenAPStart4`
			cronebl="1"
			greenap.sh setchk $start
		fi
		if [ "$cronebl" = "1" ]; then
			crond
		fi
		;;
	"setchk")
		if [ "$2" -lt "1" ]; then
			if [ "$3" -lt "1" ]; then
				hour=23
			else
				hour=`expr $3 - 1`
			fi
			minute=`expr 60 + $2 - 1`
		else
			hour=$3
			minute=`expr $2 - 1`
		fi
		echo "$minute $hour * * * greenap.sh chkntp" >> /var/spool/cron/crontabs/admin
		;;
	"chkntp")
		cat /var/spool/cron/crontabs/admin | sed '/ifconfig/d' > /var/spool/cron/crontabs/admin
		cat /var/spool/cron/crontabs/admin | sed '/txpower/d' > /var/spool/cron/crontabs/admin
		index=1
		while [ "$index" -le 10 ]
		do
			ntpvalid=`nvram_get 2860 NTPValid`
			if [ "$ntpvalid" = "1" ]; then
				setGreenAP
				break;
			else
				index=`expr $index + 1`
				sleep 5
			fi
		done
		killall -q crond
		crond
		;;
	"txpower")
		if [ "$2" = "normal" ]; then
			ralink_init gen 2860
			ifconfig ra0 down
			ifconfig ra0 up
		else
			cat /etc/Wireless/RT2860/RT2860.dat | sed '/TxPower/d' > /etc/Wireless/RT2860/RT2860.dat
			echo "TxPower=$2" >> /etc/Wireless/RT2860/RT2860.dat
			ifconfig ra0 down
			ifconfig ra0 up
		fi
		;;
esac
