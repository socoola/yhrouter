<html>
<head>
<title>System Settings</title>
<link rel="stylesheet" href="/css/normal_ws.css" type="text/css">
<link rel="stylesheet" href="/css/boxStyle.css" type="text/css">
<meta http-equiv="content-type" content="text/html; charset=iso-8859-1">
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("firewall");

function initTranslation()
{
	var e = document.getElementById("sysfwTitle");
	e.innerHTML = _("sysfw title");
	e = document.getElementById("sysfwIntroduction");
	e.innerHTML = _("sysfw introduction");
	e = document.getElementById("sysfwRemoteManagementTitle");
	e.innerHTML = _("sysfw remote management title");
	e = document.getElementById("sysfwRemoteManagementHead");
	e.innerHTML = _("sysfw remote management head");
	e = document.getElementById("sysfwRemoteManagementEnable");
	e.innerHTML = _("sysfw allow");
	e = document.getElementById("sysfwRemoteManagementDisable");
	e.innerHTML = _("sysfw deny");
	e = document.getElementById("sysfwPingFrmWANFilterTitle");
	e.innerHTML = _("sysfw wanping title");
	e = document.getElementById("sysfwPingFrmWANFilterHead");
	e.innerHTML = _("sysfw wanping head");
	e = document.getElementById("sysfwPingFrmWANFilterEnable");
	e.innerHTML = _("firewall enable");
	e = document.getElementById("sysfwPingFrmWANFilterDisable");
	e.innerHTML = _("firewall disable");
	e = document.getElementById("sysfwSPIFWTitle");
	e.innerHTML = _("sysfw spi title");
	e = document.getElementById("sysfwSPIFWHead");
	e.innerHTML = _("sysfw spi head");
	e = document.getElementById("sysfwSPIFWEnable");
	e.innerHTML = _("firewall enable");
	e = document.getElementById("sysfwSPIFWDisable");
	e.innerHTML = _("firewall disable");

	e = document.getElementById("sysfwApply");
	e.value = _("sysfw apply");
	e = document.getElementById("sysfwReset");
	e.value = _("sysfw reset");
	
	e = document.getElementById("help_head");
	e.innerHTML = _("help help_head");
	e = document.getElementById("system_firewall_direc");
	e.innerHTML = _("system_firewall system_firewall_direc");
}

function updateState()
{
	initTranslation();

	var rm = "<% getCfgGeneral(1, "RemoteManagement"); %>";
	var wpf = "<% getCfgGeneral(1, "WANPingFilter"); %>";
	var spi = "<% getCfgGeneral(1, "SPIFWEnabled"); %>";
	if(rm == "1")
		document.websSysFirewall.remoteManagementEnabled.options.selectedIndex = 1;
	else
		document.websSysFirewall.remoteManagementEnabled.options.selectedIndex = 0;
	if(wpf == "1")
		document.websSysFirewall.pingFrmWANFilterEnabled.options.selectedIndex = 1;
	else
		document.websSysFirewall.pingFrmWANFilterEnabled.options.selectedIndex = 0;
	if(spi == "1")
		document.websSysFirewall.spiFWEnabled.options.selectedIndex = 1;
	else
		document.websSysFirewall.spiFWEnabled.options.selectedIndex = 0;
}
</script>
</head>


<!--     body      -->
<body onload="updateState()">
<center id="boxes">
	<div id="box">
	<div id="head"></div>	<!--end of head-->	
	<div id="content">


	<table id="layout_table" border="0"><!--start of layout_table-->
<tr><!--start of layout tr-->
<td class="tdwidth1" id="td1"><!--start of td1-->
<div id="left"><!--start of left-->



<table class="body"><tr><td>
<h1 id="sysfwTitle"> System Firewall Settings </h1>
<% checkIfUnderBridgeModeASP(); %>
<p style="display:none" id="sysfwIntroduction"> You may configure the system firewall to protect itself from attacking.</p>
<hr /><br/>

<form method=post name="websSysFirewall" action=/goform/websSysFirewall>
<table width="540" border="0" cellpadding="2" cellspacing="1">
<tr>
	<td class="title" colspan="2" id="sysfwRemoteManagementTitle">Remote management</td>
</tr>
<tr>
	<td class="head" id="sysfwRemoteManagementHead">
		Remote management (via WAN)
	</td>
	<td>
	<select name="remoteManagementEnabled" size="1">
	<option value=0 id="sysfwRemoteManagementDisable">Disable</option>
	<option value=1 id="sysfwRemoteManagementEnable">Enable</option>
	</select>
	</td>
</tr>
</table>

<br />
<table width="540" border="0" cellpadding="2" cellspacing="1">
<tr>
	<td class="title" colspan="2" id="sysfwPingFrmWANFilterTitle">Ping form WAN Filter</td>
</tr>
<tr>
	<td class="head" id="sysfwPingFrmWANFilterHead">
	Ping form WAN Filter
	</td>
	<td>
	<select name="pingFrmWANFilterEnabled" size="1">
	<option value=0 id="sysfwPingFrmWANFilterDisable">Disable</option>
	<option value=1 id="sysfwPingFrmWANFilterEnable">Enable</option>
	</select>
	</td>
</tr>
</table>

<br />
<table width="540" border="0" cellpadding="2" cellspacing="1">
<tr>
	<td class="title" colspan="2" id="sysfwSPIFWTitle">Stateful Packet Inspection (SPI) Firewall</td>
</tr>
<tr>
	<td class="head" id="sysfwSPIFWHead">
	SPI Firewall
	</td>
	<td>
	<select name="spiFWEnabled" size="1">
	<option value=0 id="sysfwSPIFWDisable">Disable</option>
	<option value=1 id="sysfwSPIFWEnable" selected>Enable</option>
	</select>
	</td>
</tr>
</table>

<p align="center">
	<input type="submit" value="Apply" id="sysfwApply" name="sysfwApply" > &nbsp;&nbsp;
	<input type="reset" value="Reset" id="sysfwReset" name="sysfwReset">
</p>
</form>

<br>

</tr></td></table>


</div><!--end of left-->
</td><!--end of td1-->

<td class="tdwidth2" id="td2"><!--start of td2-->
	<div id="right"><!--start of right-->
		<h2 id="help_head">Heeelp...</h2>
		
		<p id="help_content"><span id="system_firewall_direc"></span></p>
	</div><!--end of right-->
</td><!--end of td2-->
</tr><!--end of layout tr-->
</table><!--end of layout_table-->


</div><!--end of content-->
	</div>
	</center>
</body>
</html>
