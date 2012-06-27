#!/bin/sh                                                      

DEVICE_FILE=/tmp/.3G_DEVICE                                                               
CMD_FILE=sms_command.sh                                           
PPP_3G_CONN_FILE=/tmp/.sms_command_script.scr                     
INTERFACE=$1 
MODEM1="HUAWEI-EM660"
MODEM2="HUAWEI-EM770"  
dev=
if [ -f $DEVICE_FILE ];then
	dev=`cat $DEVICE_FILE`
else
	dev=`nvram_get 2860 wan_3g_dev`
fi

if [ "$dev" = "" ];then
	dev=$MODEM2
fi

usage_send()                                                        
{                                                              
        	echo "EM660  usage:$CMD_FILE device send phone_number ascii_text"                 
        	echo "EM770  usage:$CMD_FILE device send pdu_text"                 
}    
usage_read()                                                        
{                                                              
        echo "usage:$CMD_FILE device read"                 
}   
usage_del()                                                        
{                                                              
        echo "usage:$CMD_FILE device del index"                 
}                                                             
send_handle_em660()
{
	shift
	shift
	PHONE=$1
	shift
	SMS_TEXT=$*                                 
	echo "opengt" > $PPP_3G_CONN_FILE                              
	echo "set com 115200n81" >> $PPP_3G_CONN_FILE                  
	echo "set senddelay 0.05" >> $PPP_3G_CONN_FILE                 
	echo "waitquiet 1 0.2" >> $PPP_3G_CONN_FILE                    

        echo "send \"ATE0\"" >> $PPP_3G_CONN_FILE
        echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
        echo "if % = -1 goto timeerror" >>$PPP_3G_CONN_FILE            
        echo "if % = 0 goto next11" >> $PPP_3G_CONN_FILE               
        echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE                
        echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE

        echo ":next11" >> $PPP_3G_CONN_FILE
        echo "send \"AT+CPMS=\\\"ME\\\",\\\"ME\\\",\\\"ME\\\"\"" >> $PPP_3G_CONN_FILE
        echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
        echo "if % = -1 goto timeerror" >>$PPP_3G_CONN_FILE            
        echo "if % = 0 goto next1" >> $PPP_3G_CONN_FILE               
        echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE                
        echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE
		                                                       


        echo ":next1" >> $PPP_3G_CONN_FILE
	echo "send \"AT\^HSMSSS=0,0,1,0\"" >> $PPP_3G_CONN_FILE               
	echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
	echo "if % = -1 goto timeerror" >>$PPP_3G_CONN_FILE            
	echo "if % = 0 goto next2" >> $PPP_3G_CONN_FILE                
	echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE                
	echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE

        echo ":next2" >> $PPP_3G_CONN_FILE
	echo "send \"AT+CNMI=1,1,0,2,0\"" >> $PPP_3G_CONN_FILE               
	echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
	echo "if % = -1 goto timeerror" >>$PPP_3G_CONN_FILE            
	echo "if % = 0 goto next3" >> $PPP_3G_CONN_FILE                
	echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE                
	echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE

        echo ":next3" >> $PPP_3G_CONN_FILE
	echo "send \"AT+CMGF=1\"" >> $PPP_3G_CONN_FILE               
	echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
	echo "if % = -1 goto timeerror" >>$PPP_3G_CONN_FILE            
	echo "if % = 0 goto next4" >> $PPP_3G_CONN_FILE                
	echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE                
	echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE

	echo ":next4" >> $PPP_3G_CONN_FILE                            
	echo "send \"AT\^HCMGS=\\\"$PHONE\\\"\"" >> $PPP_3G_CONN_FILE              
	echo "waitfor 10 \">\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE 
	echo "if % = -1 goto timeerror" >>$PPP_3G_CONN_FILE            
	echo "if % = 0 goto next5" >> $PPP_3G_CONN_FILE                
	echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE                
	echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE  

	echo ":next5" >> $PPP_3G_CONN_FILE                             
	echo "send \"$SMS_TEXT\"" >> $PPP_3G_CONN_FILE                 
	echo "send \"\"" >> $PPP_3G_CONN_FILE                        
	echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
	echo "if % = -1 goto timeerror" >>$PPP_3G_CONN_FILE            
	echo "if % = 0 goto succeed" >> $PPP_3G_CONN_FILE              
	echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE                
	echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE    


	echo ":succeed" >> $PPP_3G_CONN_FILE                           
	echo "print \"Send Succeed\\n\"" >> $PPP_3G_CONN_FILE          
	echo "exit 1" >> $PPP_3G_CONN_FILE                             
		                                                       
	echo ":error" >> $PPP_3G_CONN_FILE                             
	echo "print \"Send Error\\n\"" >> $PPP_3G_CONN_FILE            
	echo "exit 1" >> $PPP_3G_CONN_FILE                             
		                                                       
	echo ":timeerror" >> $PPP_3G_CONN_FILE                         
	echo "print \"Send Timeout\\n\"" >> $PPP_3G_CONN_FILE          
	echo "exit 1" >> $PPP_3G_CONN_FILE                             
		                                                              
	comgt -d $INTERFACE -s $PPP_3G_CONN_FILE                
}

send_handle()
{
	SMS_TEXT=$1                                   
	echo "opengt" > $PPP_3G_CONN_FILE                              
	echo "set com 115200n81" >> $PPP_3G_CONN_FILE                  
	echo "set senddelay 0.05" >> $PPP_3G_CONN_FILE                 
	echo "waitquiet 1 0.2" >> $PPP_3G_CONN_FILE                    


        echo "send \"AT+CPMS=\\\"ME\\\",\\\"ME\\\",\\\"ME\\\"\"" >> $PPP_3G_CONN_FILE
        echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
        echo "if % = -1 goto timeerror" >>$PPP_3G_CONN_FILE            
        echo "if % = 0 goto next1" >> $PPP_3G_CONN_FILE               
        echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE                
        echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE

        echo ":next1" >> $PPP_3G_CONN_FILE
	echo "send \"AT+CMGF=0\"" >> $PPP_3G_CONN_FILE               
	echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
	echo "if % = -1 goto timeerror" >>$PPP_3G_CONN_FILE            
	echo "if % = 0 goto next2" >> $PPP_3G_CONN_FILE                
	echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE                
	echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE

	echo ":next2" >> $PPP_3G_CONN_FILE                            
	echo "send \"AT+CMGS=16\"" >> $PPP_3G_CONN_FILE              
	echo "waitfor 10 \">\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE 
	echo "if % = -1 goto timeerror" >>$PPP_3G_CONN_FILE            
	echo "if % = 0 goto next3" >> $PPP_3G_CONN_FILE                
	echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE                
	echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE  

	echo ":next3" >> $PPP_3G_CONN_FILE                             
	echo "send \"$SMS_TEXT\"" >> $PPP_3G_CONN_FILE                 
	echo "send \"\"" >> $PPP_3G_CONN_FILE                        
	echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
	echo "if % = -1 goto timeerror" >>$PPP_3G_CONN_FILE            
	echo "if % = 0 goto succeed" >> $PPP_3G_CONN_FILE              
	echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE                
	echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE    

	echo ":succeed" >> $PPP_3G_CONN_FILE                           
	echo "send \"AT+CMGF=1\"" >> $PPP_3G_CONN_FILE               
	echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
	echo "print \"Send Succeed\\n\"" >> $PPP_3G_CONN_FILE          
	echo "exit 1" >> $PPP_3G_CONN_FILE                             
		                                                       
	echo ":error" >> $PPP_3G_CONN_FILE                             
	echo "print \"Send Error\\n\"" >> $PPP_3G_CONN_FILE            
	echo "exit 1" >> $PPP_3G_CONN_FILE                             
		                                                       
	echo ":timeerror" >> $PPP_3G_CONN_FILE                         
	echo "print \"Send Timeout\\n\"" >> $PPP_3G_CONN_FILE          
	echo "exit 1" >> $PPP_3G_CONN_FILE                             
		                                                              
	comgt -d $INTERFACE -s $PPP_3G_CONN_FILE                
}



read_index_em660()
{
	echo "opengt" > $PPP_3G_CONN_FILE                              
	echo "set com 115200n81" >> $PPP_3G_CONN_FILE                  
	echo "set senddelay 0.05" >> $PPP_3G_CONN_FILE                 
	echo "waitquiet 1 0.2" >> $PPP_3G_CONN_FILE           

        echo "goto next3" >> $PPP_3G_CONN_FILE
        echo "send \"ATE0\"" >> $PPP_3G_CONN_FILE
        echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
        echo "if % = -1 goto timeerror" >>$PPP_3G_CONN_FILE            
        echo "if % = 0 goto next11" >> $PPP_3G_CONN_FILE               
        echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE                
        echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE

	echo ":next11" >> $PPP_3G_CONN_FILE
	echo "send \"AT+CMGF=1\"" >> $PPP_3G_CONN_FILE               
	echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
	echo "if % = -1 goto timeerror" >>$PPP_3G_CONN_FILE            
	echo "if % = 0 goto next2" >> $PPP_3G_CONN_FILE                
	echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE                

	echo ":next2" >> $PPP_3G_CONN_FILE
        echo "send \"AT+CPMS=\\\"ME\\\",\\\"ME\\\",\\\"ME\\\"\"" >> $PPP_3G_CONN_FILE
        echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
        echo "if % = -1 goto timeerror" >>$PPP_3G_CONN_FILE            
        echo "if % = 0 goto next3" >> $PPP_3G_CONN_FILE               
        echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE                
        echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE
		                                                       


        echo ":next3" >> $PPP_3G_CONN_FILE

	echo "send \"AT\^HCMGL\"" >> $PPP_3G_CONN_FILE               

	echo "let i=0" >> $PPP_3G_CONN_FILE
	echo "system \"echo \\\"\\\" > /tmp/.sms_content\"" >> $PPP_3G_CONN_FILE
	echo "open file \"/tmp/.sms_content\"" >> $PPP_3G_CONN_FILE
	echo ":get_next" >> $PPP_3G_CONN_FILE
	echo "get 1 \"^m\" \$s" >> $PPP_3G_CONN_FILE    
	echo "fprint \$s" >> $PPP_3G_CONN_FILE   
	echo "let \$a=\$s" >> $PPP_3G_CONN_FILE     
	echo "if len(\$a)>=3 let \$b=\$right(\$a,2)" >> $PPP_3G_CONN_FILE    
	echo "if \$b=\"OK\" goto exit" >> $PPP_3G_CONN_FILE       
	echo "inc i" >> $PPP_3G_CONN_FILE  
	echo "if i<50 goto get_next" >> $PPP_3G_CONN_FILE 
	echo ":exit" >> $PPP_3G_CONN_FILE 
	echo "fprint \"\n\"" >> $PPP_3G_CONN_FILE 
	echo "close file" >> $PPP_3G_CONN_FILE 
	echo "goto succeed" >> $PPP_3G_CONN_FILE 

	echo ":succeed" >> $PPP_3G_CONN_FILE                          
	echo "print \"Read Succeed\\n\"" >> $PPP_3G_CONN_FILE                 
	echo "exit 1" >> $PPP_3G_CONN_FILE                             
		                                                       
	echo ":error" >> $PPP_3G_CONN_FILE                             
	echo "print \"Read Error\\n\"" >> $PPP_3G_CONN_FILE                    
	echo "exit 1" >> $PPP_3G_CONN_FILE                             
		                                                       
	echo ":timeerror" >> $PPP_3G_CONN_FILE                         
	echo "print \"Read Timeout\\n\"" >> $PPP_3G_CONN_FILE                  
	echo "exit 1" >> $PPP_3G_CONN_FILE                             
		                                                              
	ret=`comgt -d $INTERFACE -s $PPP_3G_CONN_FILE`
	echo $ret >> /tmp/.sms_content


}
read_one_em660()
{
	INDEX=$1
	echo "opengt" > $PPP_3G_CONN_FILE                              
	echo "set com 115200n81" >> $PPP_3G_CONN_FILE                  
	echo "set senddelay 0.05" >> $PPP_3G_CONN_FILE                 
	echo "waitquiet 1 0.2" >> $PPP_3G_CONN_FILE           

	echo "send \"AT\^HCMGR=$INDEX\"" >> $PPP_3G_CONN_FILE               

	echo "let i=0" >> $PPP_3G_CONN_FILE
	echo "open file \"/tmp/.sms_content\"" >> $PPP_3G_CONN_FILE
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
	echo "goto succeed" >> $PPP_3G_CONN_FILE 

	echo ":succeed" >> $PPP_3G_CONN_FILE                          
	echo "exit 1" >> $PPP_3G_CONN_FILE                             
		                                                       
	echo ":error" >> $PPP_3G_CONN_FILE                             
	echo "exit 1" >> $PPP_3G_CONN_FILE                             
		                                                       
	echo ":timeerror" >> $PPP_3G_CONN_FILE                         
	echo "exit 1" >> $PPP_3G_CONN_FILE                             
		                                                              
	ret=`comgt -d $INTERFACE -s $PPP_3G_CONN_FILE`
}
read_content_em660()
{
	INDEXS=`cat $1`	
	for index in $INDEXS
	do
		read_one_em660 $index
	done	
}

read_handle_em660()
{
	if [ ! -f /tmp/.em660_init_sign ];then
		if [ -f /tmp/3g/3G_cmd_1 ];then
			/tmp/3g/3G_cmd_1  > /dev/null 2>&1
			echo "" > /tmp/.em660_init_sign
		fi	
	fi
	read_index_em660
	index_res_flag=`cat /tmp/.sms_content|grep "Read Succeed"`	
	if [ "$index_res_flag" != "" ];then
		echo "" > /tmp/.sms_index
		cat /tmp/.sms_content |grep "\^HCMGL:" |sed 's/\^HCMGL://' | sed 's/,.*//' > /tmp/.sms_index	
		echo "" > /tmp/.sms_content	
		read_content_em660 /tmp/.sms_index
		cat /tmp/.sms_content
		echo "Read Succeed" 
	else
		echo "Read Error" 
	fi
}


read_handle()
{

	echo "opengt" > $PPP_3G_CONN_FILE                              
	echo "set com 115200n81" >> $PPP_3G_CONN_FILE                  
	echo "set senddelay 0.05" >> $PPP_3G_CONN_FILE                 
	echo "waitquiet 1 0.2" >> $PPP_3G_CONN_FILE           

	echo "send \"AT\^CURC=0\"" >> $PPP_3G_CONN_FILE               
	echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
	echo "if % = -1 goto timeerror" >>$PPP_3G_CONN_FILE            
	echo "if % = 0 goto next1" >> $PPP_3G_CONN_FILE                
	echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE                
	echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE

	echo ":next1" >> $PPP_3G_CONN_FILE

	echo "send \"AT+CMGF=1\"" >> $PPP_3G_CONN_FILE               
	echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
	echo "if % = -1 goto timeerror" >>$PPP_3G_CONN_FILE            
	echo "if % = 0 goto next2" >> $PPP_3G_CONN_FILE                
	echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE                

	echo ":next2" >> $PPP_3G_CONN_FILE
        echo "send \"AT+CPMS=\\\"ME\\\",\\\"ME\\\",\\\"ME\\\"\"" >> $PPP_3G_CONN_FILE
        echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
        echo "if % = -1 goto timeerror" >>$PPP_3G_CONN_FILE            
        echo "if % = 0 goto next3" >> $PPP_3G_CONN_FILE               
        echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE                
        echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE
		                                                       
        echo ":next3" >> $PPP_3G_CONN_FILE

	echo "send \"AT+CMGL=\\\"ALL\\\"\"" >> $PPP_3G_CONN_FILE               

	echo "let i=0" >> $PPP_3G_CONN_FILE
	echo "system \"echo \\\"\\\" > /tmp/.sms_content\"" >> $PPP_3G_CONN_FILE
	echo "open file \"/tmp/.sms_content\"" >> $PPP_3G_CONN_FILE
	echo ":get_next" >> $PPP_3G_CONN_FILE
	echo "get 1 \"^m\" \$s" >> $PPP_3G_CONN_FILE    
	echo "fprint \$s" >> $PPP_3G_CONN_FILE   
	echo "let \$a=\$s" >> $PPP_3G_CONN_FILE     
	echo "if len(\$a)>=3 let \$b=\$right(\$a,2)" >> $PPP_3G_CONN_FILE    
	echo "if \$b=\"OK\" goto exit" >> $PPP_3G_CONN_FILE       
	echo "inc i" >> $PPP_3G_CONN_FILE  
	echo "if i<100 goto get_next" >> $PPP_3G_CONN_FILE 
	echo ":exit" >> $PPP_3G_CONN_FILE 
	echo "fprint \"\n\"" >> $PPP_3G_CONN_FILE 
	echo "close file" >> $PPP_3G_CONN_FILE 
	echo "system \"cat /tmp/.sms_content\"" >> $PPP_3G_CONN_FILE 
	echo "goto succeed" >> $PPP_3G_CONN_FILE 

	echo ":succeed" >> $PPP_3G_CONN_FILE                          
	echo "print \"Read Succeed\\n\"" >> $PPP_3G_CONN_FILE          
	echo "exit 1" >> $PPP_3G_CONN_FILE                             
		                                                       
	echo ":error" >> $PPP_3G_CONN_FILE                             
	echo "print \"Read Error\\n\"" >> $PPP_3G_CONN_FILE            
	echo "exit 1" >> $PPP_3G_CONN_FILE                             
		                                                       
	echo ":timeerror" >> $PPP_3G_CONN_FILE                         
	echo "print \"Read Timeout\\n\"" >> $PPP_3G_CONN_FILE          
	echo "exit 1" >> $PPP_3G_CONN_FILE                             
		                                                              
	comgt -d $INTERFACE -s $PPP_3G_CONN_FILE


}


del_handle()
{
	SMS_INDEX=$1                                   
	echo "opengt" > $PPP_3G_CONN_FILE                              
	echo "set com 115200n81" >> $PPP_3G_CONN_FILE                  
	echo "set senddelay 0.05" >> $PPP_3G_CONN_FILE                 
	echo "waitquiet 1 0.2" >> $PPP_3G_CONN_FILE   
                 
        echo "send \"AT+CPMS=\\\"ME\\\",\\\"ME\\\",\\\"ME\\\"\"" >> $PPP_3G_CONN_FILE
        echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
        echo "if % = -1 goto timeerror" >>$PPP_3G_CONN_FILE            
        echo "if % = 0 goto next1" >> $PPP_3G_CONN_FILE               
        echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE                
        echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE
		                                                     
        echo ":next1" >> $PPP_3G_CONN_FILE

	echo "send \"AT+CMGD=$SMS_INDEX\"" >> $PPP_3G_CONN_FILE               
	echo "waitfor 10 \"OK\",\"ERR\",\"ERROR\"" >> $PPP_3G_CONN_FILE
	echo "if % = -1 goto timeerror" >>$PPP_3G_CONN_FILE            
	echo "if % = 0 goto succeed" >> $PPP_3G_CONN_FILE                
	echo "if % = 1 goto error" >> $PPP_3G_CONN_FILE                
	echo "if % = 2 goto error" >> $PPP_3G_CONN_FILE

	echo ":succeed" >> $PPP_3G_CONN_FILE                           
	echo "print \"Delete Succeed\\n\"" >> $PPP_3G_CONN_FILE          
	echo "exit 1" >> $PPP_3G_CONN_FILE                             
		                                                       
	echo ":error" >> $PPP_3G_CONN_FILE                             
	echo "print \"Delete Error\\n\"" >> $PPP_3G_CONN_FILE            
	echo "exit 1" >> $PPP_3G_CONN_FILE                             
		                                                       
	echo ":timeerror" >> $PPP_3G_CONN_FILE                         
	echo "print \"Delete Timeout\\n\"" >> $PPP_3G_CONN_FILE          
	echo "exit 1" >> $PPP_3G_CONN_FILE                             
		                                                              
	comgt -d $INTERFACE -s $PPP_3G_CONN_FILE          
	
}
check_send()
{
	
	if [ "$dev" = $MODEM1 ];then
		if [ $# -lt 4 ];then 
			usage_send
			exit 1	
		fi	
	else
		if [ $# -ne 3 ];then 
			usage_send
			exit 1	
		fi
	fi
}
check_read()
{
	if [ $# -ne 2 ];then 
		usage_read
		exit 1	
	fi
}
check_del()
{
	if [ $# -ne 3 ];then 
		usage_del
		exit 1	
	fi
}



if [ "$2" = "send" ];then
	check_send $*
	if [ "$dev" = $MODEM1 ];then
		send_handle_em660 $*
	else
		send_handle $3
	fi
elif [ "$2" = "read" ];then
	check_read $*
	if [ "$dev" = $MODEM1 ];then	
		read_handle_em660
	else
		read_handle
	fi	
	
elif [ "$2" = "del" ];then
	check_del $*
	del_handle $3
else
	usage_send
	usage_read
	usage_del
	exit 1
	
fi
