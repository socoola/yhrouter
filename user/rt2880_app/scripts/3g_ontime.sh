echo "3g:pppd call 3g (dot mode)"
killall crond
pppd call 3g
sleep 10

while true;do


	if [ -f /tmp/3G_LINK ];then
		cell_up_sign=`cat /tmp/3G_LINK`	
		if [ "$cell_up_sign" = "3g_link_up" ];then
			cell_up=1
			break
		else
			cell_up=0
		fi
	fi
	sleep 1

done

count=0
#if [ "$cell_up" = "1" ];then
	while [ "$cell_up" = "1" -a $count -lt 20 ];do
		ntp.sh > /tmp/.ntplog
		sleep 5
		ntp_info=`cat /tmp/.ntplog |grep day`
		ntp_info=`echo $ntp_info |grep elapsed`
		if [ "$ntp_info" != "" ];then
			ntp_info=get_time_ok
			break
		fi
		sleep 5
		count=`expr $count + 1`
	done
#fi






if [ "$ntp_info" = "get_time_ok" ];then

	
		mkdir -p /var/spool/cron/crontabs/
		tpsel=`nvram_get 2860 DOT_select_index`
		if [ $tpsel -ge 0 -a $tpsel -lt 4 ]; then
				tpsh=`nvram_get 2860 tp_sh_${tpsel}`
				tpsm=`nvram_get 2860 tp_sm_${tpsel}`
				tpeh=`nvram_get 2860 tp_eh_${tpsel}`
				tpem=`nvram_get 2860 tp_em_${tpsel}`
				thnow=`date +%H`
				tmnow=`date +%M`
				#echo "$tpsm $tpsh * * * /sbin/internet.sh" > /var/spool/cron/crontabs/yinghua868
				#echo "$tpem $tpeh * * * /sbin/internet.sh" >> /var/spool/cron/crontabs/yinghua868
				echo "$tpsm $tpsh * * * /sbin/3g_up_command.sh" > /var/spool/cron/crontabs/yinghua868
				echo "$tpem $tpeh * * * /sbin/3g_down_command.sh" >> /var/spool/cron/crontabs/yinghua868

				#for reboot_neterr in 3g on timer mode
				echo "$tpsm $tpsh * * * /bin/reboot_neterr_up_for_crond.sh" >> /var/spool/cron/crontabs/yinghua868
				echo "$tpem $tpeh * * * /bin/reboot_neterr_down_for_crond.sh" >> /var/spool/cron/crontabs/yinghua868

				crond
				#comparetime $tpsh $tpsm $thnow $tmnow
				ret=`comparetime.sh $thnow $tmnow $tpsh $tpsm`
				if [ "$ret" == "4" ]; then
					3g_down_command.sh
					/bin/reboot_neterr_down_for_crond.sh
					#killall pppd					
					echo "3g:killall pppd"
				elif [ "$ret" == "6" ];then
					ret2=`comparetime.sh $thnow $tmnow $tpeh $tpem`
					if [ "$ret2" == "6" ]; then
						3g_down_command.sh
						/bin/reboot_neterr_down_for_crond.sh
						#killall pppd					
						echo "3g:killall pppd"
					fi		
				fi		
		else
			3g_down_command.sh
			/bin/reboot_neterr_down_for_crond.sh
			echo "3g:killall pppd"
			#killall pppd
		fi	


fi	

