<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01//EN"
"http://www.w3.org/TR/html4/strict.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" lang="en">
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
		<title>menuFrame</title>
		<meta name="author" content="Administrator" />
		<!-- Date: 2012-04-23 -->
		<link rel="stylesheet" type="text/css" href="../css/menuFrame.css" />
		<script type="text/javascript" src="../js/jquery-1.7.2.min.js"></script>
		<script type="text/javascript" src="../js/menuFrame.js"></script>
		
		<script type="text/javascript" src="/lang/b28n.js"></script>
		<script language="JavaScript" type="text/javascript">
			Butterlate.setTextDomain("admin");
			function initTranslation()
			{
				var support_dtu = "<% DtuIsSupport(); %>";
				var support_linkbackup = "<% LinkbackupIsSupport(); %>";
				var support_wifi = "<% WifiIsSupport(); %>";
				var support_yinghua = "<% YinghuaIsSupport(); %>";
				var e = document.getElementById("status");
				e.innerHTML = _("menuFrame status");
						
				e = document.getElementById("opmode");
				e.innerHTML = _("menuFrame opmode");
				
				
				e = document.getElementById("internet");
				e.innerHTML = _("menuFrame internet");
				e = document.getElementById("wan");
				e.innerHTML = _("menuFrame wan");
				e = document.getElementById("lan");
				e.innerHTML = _("menuFrame lan");
				e = document.getElementById("dhcpcliinfo");
				e.innerHTML = _("menuFrame dhcpcliinfo");
				e = document.getElementById("routing");
				e.innerHTML = _("menuFrame routing");
				e = document.getElementById("snmp");
				e.innerHTML = _("menuFrame snmp");
				
				e = document.getElementById("VPN");
				e.innerHTML = _("menuFrame VPN");
				e = document.getElementById("ipsecVpn");
				e.innerHTML = _("menuFrame ipsecVpn");
				e = document.getElementById("pptp");
				e.innerHTML = _("menuFrame pptp");
				e = document.getElementById("l2tp");
				e.innerHTML = _("menuFrame l2tp");
				if(support_dtu == "y")
				{
					e = document.getElementById("dtu");
					e.innerHTML = _("menuFrame dtu");
				}
				
				if(support_linkbackup == "y")
				{
					e = document.getElementById("linkbackup");
					e.innerHTML = _("menuFrame linkbackup");
				}
				
				
				if(support_wifi == "y")
				{
					e = document.getElementById("wire_less");
					e.innerHTML = _("menuFrame wire_less");
					e = document.getElementById("basic");
					e.innerHTML = _("menuFrame basic");
					e = document.getElementById("security");
					e.innerHTML = _("menuFrame security");
					e = document.getElementById("advanced");
					e.innerHTML = _("menuFrame advanced");
					e = document.getElementById("wds");
					e.innerHTML = _("menuFrame wds");
					e = document.getElementById("stainfo");
					e.innerHTML = _("menuFrame stainfo");
				}
				
				
				e = document.getElementById("firewall");
				e.innerHTML = _("menuFrame firewall");
				e = document.getElementById("port_filtering");
				e.innerHTML = _("menuFrame port_filtering");
				e = document.getElementById("port_forward");
				
				e.innerHTML = _("menuFrame port_forward");
				e = document.getElementById("DMZ");
				e.innerHTML = _("menuFrame DMZ");
				e = document.getElementById("system_firewall");
				e.innerHTML = _("menuFrame system_firewall");
				
				e = document.getElementById("admin");
				e.innerHTML = _("menuFrame admin");
				e = document.getElementById("management");
				e.innerHTML = _("menuFrame management");
				e = document.getElementById("upload_firmware");
				e.innerHTML = _("menuFrame upload_firmware");
				e = document.getElementById("settings");
				e.innerHTML = _("menuFrame settings");
				e = document.getElementById("reboot");
				e.innerHTML = _("menuFrame reboot");
				e = document.getElementById("ICMP");
				e.innerHTML = _("menuFrame ICMP");
				e = document.getElementById("restart");
				e.innerHTML = _("menuFrame restart");
				e = document.getElementById("statistic");
				e.innerHTML = _("menuFrame statistic");
				e = document.getElementById("syslog");
				e.innerHTML = _("menuFrame syslog");
				if(support_yinghua == "y")
				{
					e = document.getElementById("yinghua");
					e.innerHTML = _("menuFrame yinghua");
				}
				
			}
		</script>
	</head>
	<body onload="initTranslation()">
		<div id="menu">
			<a id="status" class="dot" href="status.asp" target="mainFrame">运行状态</a>
			
			<a id="opmode" class="dot" href="opmode.asp" target="mainFrame">运作模式</a>
			
			<a id="internet" class="plus" href="WAN_CONFIG.html" target="mainFrame">网络配置</a>
			<div class="subMenu">
				<a id="wan" href="wan.asp" target="mainFrame">WAN口配置</a>
				<a id="lan" href="lan.asp" target="mainFrame">LAN口配置</a>
				<a id="dhcpcliinfo" href="dhcpcliinfo.asp" target="mainFrame">DHCP客户端列表</a>
				<a id="routing" href="routing.asp" target="mainFrame">高级路由配置</a>
				<a id="snmp" href="snmp.asp" target="mainFrame">SNMP</a>
			</div><!--end of submenu-->
			
			<a id="VPN" class="plus" href="VPN.html" target="mainFrame">VPN</a>
			<div class=" subMenu">
				<a id="ipsecVpn" href="ipsecVpn.asp" target="mainFrame">Ipsec</a>
				<a id="pptp" href="pptp.asp" target="mainFrame">PPTP</a>
				<a id="l2tp" href="l2tp.asp" target="mainFrame">L2TP</a>
			</div>
			
			<script type="text/javascript">if("<% DtuIsSupport(); %>" != "y")document.write("<!--");</script>			
			<a id="dtu" class="dot" href="dtu.asp" target="mainFrame">DTU</a>
			<script type="text/javascript">if("<% DtuIsSupport(); %>" != "y")document.write("-->");</script>
			<script type="text/javascript">if("<% LinkbackupIsSupport(); %>" != "y")document.write("<!--");</script>				
			<a id="linkbackup" class="dot" href="linkbackup.asp" target="mainFrame">路由备份</a>
			<script type="text/javascript">if("<% LinkbackupIsSupport(); %>" != "y")document.write("-->");</script>
			<script type="text/javascript">if("<% WifiIsSupport(); %>" != "y")document.write("<!--");</script>				
			<a id="wire_less" class="plus" href="BASIC_CONFIG.html" target="mainFrame">无线设置</a>	
			<div class="subMenu">
				<a id="basic" href="basic.asp" target="mainFrame">基本设置</a>
				<a id="security" href="security.asp" target="mainFrame">无线安全设置</a>
				<a id="advanced" href="advanced.asp" target="mainFrame">无线高级设置</a>
				<a id="wds" href="wds.asp" target="mainFrame">WDS</a>
				<a id="stainfo" href="stainfo.asp" target="mainFrame">主机状态</a>
			</div>
			<script type="text/javascript">if("<% WifiIsSupport(); %>" != "y")document.write("-->");</script>
			<!--end of submenu-->	
			
			<a id="firewall" class="plus" href="FILTER.html" target="mainFrame">防火墙</a>	
			<div class="subMenu">
				<a id="port_filtering" href="port_filtering.asp" target="mainFrame">过滤</a>
				<a id="port_forward" href="port_forward.asp" target="mainFrame">虚拟服务器</a>
				<a id="DMZ" href="DMZ.asp" target="mainFrame">DMZ主机</a>
				<a id="system_firewall" href="system_firewall.asp" target="mainFrame">系统安全设置</a>
			</div><!--end of submenu-->	
		
			
			<a id="admin" class="plus" href="#" target="mainFrame">系统配置</a>	
			<div class="subMenu">
				<a id="management" href="management.asp" target="mainFrame">管理</a>
				<a id="upload_firmware" href="upload_firmware.asp" target="mainFrame">软件升级</a>
				<a id="settings" href="settings.asp" target="mainFrame">参数管理</a>

				<a id="reboot"	style="padding-left:13px;
					background: url('../image/plus.gif') no-repeat left;"
				 	id="reboot" class="plus" href="RESTART_ROUTER.html" 
				 	target="mainFrame">重启路由器</a>
				<div class="subMenu" style="padding-left: 10px">
				<a id="ICMP" href="ICMP.asp" target="mainFrame">ICMP重启</a>
				<a id="restart" href="restart.asp" target="mainFrame">定时和立即重启</a>
				</div><!--end of reboot-->	

                <a id="statistic" href="statistic.asp" target="mainFrame">流量统计</a>
                <a id="syslog" href="syslog.asp" target="mainFrame">系统日志</a>
				<script type="text/javascript">if("<% YinghuaIsSupport(); %>" != "y")document.write("<!--");</script>				
                <a id="yinghua" href="http://www.yinghuatn.com" target="_blank">盈华科技其他产品</a>
				<script type="text/javascript">if("<% YinghuaIsSupport(); %>" != "y")document.write("-->");</script>				
			</div><!--end of submenu-->	
		</div><!--end of menu-->
	</body>
</html>
