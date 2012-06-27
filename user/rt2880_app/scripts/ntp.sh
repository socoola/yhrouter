#!/bin/sh

srv=`nvram_get 2860 NTPServerIP`
sync=`nvram_get 2860 NTPSync`
tz=`nvram_get 2860 TZ`


killall -q ntpclient

if [ "$srv" = "" ]; then
	exit 0
fi


sync=`expr $sync \* 3600`

if [ "$tz" = "" ]; then
	tz="UCT_000"
fi


echo $tz > /etc/tmpTZ
sed -e 's#.*_\(-*\)0*\(.*\)#GMT-\1\2#' /etc/tmpTZ > /etc/tmpTZ2
sed -e 's#\(.*\)--\(.*\)#\1\2#' /etc/tmpTZ2 > /etc/TZ
rm -rf /etc/tmpTZ
rm -rf /etc/tmpTZ2
ntpclient -d -s -c 0 -h $srv -i $sync &

