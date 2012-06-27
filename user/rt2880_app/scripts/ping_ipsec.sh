
#cat /var/racoon.conf |grep "sainfo" |sed 's/sainfo.*address */ping -I br0 /' | sed 's/\/.*/ >\/dev\/null\&/' > /tmp/.ping_ipsec
cat /var/setkey.conf |grep "spdadd.*-P *in.*" | sed 's/spdadd *//' |sed 's/\/.*/ >\/dev\/null\&/' |sed 's/^/ping -I br0 /' >/tmp/.ping_ipsec
chmod 751 /tmp/.ping_ipsec
./tmp/.ping_ipsec
sleep 1

ps |grep -v grep | grep ping |grep br0 |sed 's/ *//' | sed 's/ .*//' |sed 's/^/kill /' > /tmp/.ping_ipsec
./tmp/.ping_ipsec
