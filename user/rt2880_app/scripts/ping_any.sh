c=0
count=0
secCount=0
while [ "$c" -lt "1" ]
do 
    check_vpn
    ret=$?
    if [ "$ret" -eq "255" ];then
        # vpn is not enable or 3g is not up
        exit
    fi
    
    if [ "$ret" -eq "0" ];then  
        # vpn is ok
        echo "1">/var/vpn2.log
    else 
        # vpn is not working
        #count=`expr $count + 1`
        #secCount=`expr $secCount + 1`
        echo "1">>/var/vpn2.log
    fi
    sanmao=`cat /var/vpn2.log`
    length=`expr length "$sanmao"`
    # length include the new line character
    if [ "$length" -eq "11" -o "$length" -eq "22" -o "$length" -eq "33" -o "$length" -eq "44" ];then
        # every 5 times failed, try to start vpn
        killall start_vpn
        killall start_vpn
        sleep 1
        setkey -FP
        setkey -FP
        start_vpn&
    fi

    if [ "$length" -eq "55" ];then
        # start vpn 5 times, but failed
        reboot
    fi
    ping -c 2 -I br0 8.8.8.8
    sleep 10
    #echo "result">>/var/vpn2.log
    #echo $count>>/var/vpn2.log
 
    
    
done
