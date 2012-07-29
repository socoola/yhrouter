ps>/var/ping.log
echo `cat /var/ping.log|grep ping_any`>/var/pingResult.log
str=`cat /var/pingResult.log`
echo ${str%% *}>/var/str.log
PID=`cat var/str.log`
kill $PID
