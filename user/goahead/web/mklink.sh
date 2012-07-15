cd /flash/web

cd adm
rm -rf *
ln -s ../html/management.asp ./management.asp
ln -s ../html/restart.asp ./restart.asp
ln -s ../html/ICMP.asp ./ICMP.asp
ln -s ../html/settings.asp ./settings.asp
ln -s ../html/statistic.asp ./statistic.asp
ln -s ../html/status.asp ./status.asp
ln -s ../html/syslog.asp ./syslog.asp
ln -s ../html/upload_firmware.asp ./upload_firmware.asp
cd ..

cd firewall
rm -rf *
ln -s ../html/DMZ.asp ./DMZ.asp
ln -s ../html/port_filtering.asp ./port_filtering.asp
ln -s ../html/port_forward.asp ./port_forward.asp
ln -s ../html/system_firewall.asp ./system_firewall.asp
cd ..

cd internet
rm -rf *
ln -s ../html/dhcpcliinfo.asp ./dhcpcliinfo.asp
ln -s ../html/dtu.asp ./dtu.asp
ln -s ../html/ipsecVpn.asp ./ipsec.asp
ln -s ../html/lan.asp ./lan.asp
ln -s ../html/linkbackup.asp ./linkbackup.asp
ln -s ../html/routing.asp ./routing.asp
ln -s ../html/snmp.asp ./snmp.asp
ln -s ../html/wan.asp ./wan.asp
cd ..

cd wireless
rm -rf *
ln -s ../html/advanced.asp ./advanced.asp
ln -s ../html/basic.asp ./basic.asp
ln -s ../html/security.asp ./security.asp
ln -s ../html/stainfo.asp ./stainfo.asp
ln -s ../html/wds.asp ./wds.asp
cd ..

cd lang/en/
rm -f *
ln -s ../admin_en.xml ./admin.xml
ln -s ../firewall_en.xml ./firewall.xml
ln -s ../internet_en.xml ./internet.xml
ln -s ../main_en.xml ./main.xml
ln -s ../usb_en.xml ./usb.xml
ln -s ../wireless_en.xml ./wireless.xml
cd ../../

cd lang/zhcn/
rm -f *
ln -s ../admin_cn.xml ./admin.xml
ln -s ../firewall_cn.xml ./firewall.xml
ln -s ../internet_cn.xml ./internet.xml
ln -s ../main_cn.xml ./main.xml
ln -s ../usb_cn.xml ./usb.xml
ln -s ../wireless_cn.xml ./wireless.xml
cd ../../

cd lang
rm b28n.js
ln -s ../js/b28n.js ./b28n.js
cd ..

rm -f home.asp
ln -s ./RouterProject.html ./home.asp


