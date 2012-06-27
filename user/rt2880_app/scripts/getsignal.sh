
if [ $# -ne 1 ];then
	echo "useage: getsignal.sh device"
fi


PPP_3G_CONN_FILE=/tmp/.getsignal.scr
CSQ_FILE=/tmp/.signal_content
echo "" > $CSQ_FILE
        echo "opengt" > $PPP_3G_CONN_FILE
        echo "set com 115200n81" >> $PPP_3G_CONN_FILE
        echo "set senddelay 0.05" >> $PPP_3G_CONN_FILE
        echo "waitquiet 1 0.2" >> $PPP_3G_CONN_FILE


        echo "send \"AT+CSQ^m\"" >> $PPP_3G_CONN_FILE
	
	echo "let i=0" >> $PPP_3G_CONN_FILE
        echo "open file \"$CSQ_FILE\"" >> $PPP_3G_CONN_FILE
        echo ":get_next" >> $PPP_3G_CONN_FILE
        echo "get 1 \"^m\" \$s" >> $PPP_3G_CONN_FILE
        echo "fprint \$s" >> $PPP_3G_CONN_FILE
        echo "let \$a=\$s" >> $PPP_3G_CONN_FILE
        echo "if len(\$a)>=3 let \$b=\$right(\$a,2)" >> $PPP_3G_CONN_FILE
        echo "if \$b=\"OK\" goto exit" >> $PPP_3G_CONN_FILE
        echo "inc i" >> $PPP_3G_CONN_FILE
        echo "if i<20 goto get_next" >> $PPP_3G_CONN_FILE
        echo ":exit" >> $PPP_3G_CONN_FILE
        echo "fprint \"\n\"" >> $PPP_3G_CONN_FILE
        echo "close file" >> $PPP_3G_CONN_FILE




comgt -d $1 -s $PPP_3G_CONN_FILE


CSQ_STR=`cat $CSQ_FILE |grep +CSQ:`

if [ "$CSQ_STR" != "" ];then
	CSQ_STR=`echo $CSQ_STR | sed "s/,.*//"`
	CSQ_STR=`echo $CSQ_STR | sed "s/^ *+CSQ: */signal:/"`
fi

echo "$CSQ_STR"

