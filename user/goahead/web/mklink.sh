cd /flash/web

cd adm
rm -rf *
ln -s ../html/management.asp ./management.asp
ln -s ../html/reboot.asp ./reboot.asp
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
ln -s ../html/ipsec.asp ./ipsec.asp
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
ln -s ../admin_en.asp ./admin.asp
ln -s ../firewall_en.asp ./firewall.asp
ln -s ../internet_en.asp ./internet.asp
ln -s ../main_en.asp ./main.asp
ln -s ../usb_en.asp ./usb.asp
ln -s ../wireless_en.asp ./wireless.asp
cd ../../

cd lang/zhcn/
rm -f *
ln -s ../admin_cn.asp ./admin.asp
ln -s ../firewall_cn.asp ./firewall.asp
ln -s ../internet_cn.asp ./internet.asp
ln -s ../main_cn.asp ./main.asp
ln -s ../usb_cn.asp ./usb.asp
ln -s ../wireless_cn.asp ./wireless.asp
cd ../../

cd lang
rm b28n.js
ln -s ../js/b28n.js ./b28n.js
cd ..

ln -s ./RouterProject.html ./home.asp


