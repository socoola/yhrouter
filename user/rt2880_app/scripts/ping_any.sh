c=0
count=0
secCount=0
while [ "$c" -lt "1" ]
do      
    ping -c 2 -I br0 8.8.8.8
    sleep 10
    
    check_vpn
    if [ "$?" -eq "0" ];then  
        echo "1">/var/vpn2.log
    else 
        #count=`expr $count + 1`
        #secCount=`expr $secCount + 1`
        echo "1">>/var/vpn2.log
    fi
    sanmao=`cat /var/vpn2.log`
    length=`expr length "$sanmao"`
    if [ "$length" -eq "7" ];then
        reboot
    fi
    
    #echo "result">>/var/vpn2.log
    #echo $count>>/var/vpn2.log
 
    
    
done
