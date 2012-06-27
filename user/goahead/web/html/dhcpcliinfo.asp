<!-- Copyright 2004, Ralink Technology Corporation All Rights Reserved. -->
<html>
<head>
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
<META HTTP-EQUIV="Expires" CONTENT="-1">
<META http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<link rel="stylesheet" href="/css/normal_ws.css" type="text/css">
<link rel="stylesheet" href="/css/boxStyle.css" type="text/css">
<title>DHCP Client List</title>

<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("internet");

function initValue()
{
	var e = document.getElementById("dTitle");
	e.innerHTML = _("dhcp title");
	e = document.getElementById("dIntroduction");
	e.innerHTML = _("dhcp introduction");
	e = document.getElementById("dClients");
	e.innerHTML = _("dhcp clients");
	e = document.getElementById("dHostname");
	e.innerHTML = _("inet hostname");
	e = document.getElementById("dMac");
	e.innerHTML = _("inet mac");
	e = document.getElementById("dIp");
	e.innerHTML = _("inet ip");
	e = document.getElementById("dExpr");
	e.innerHTML = _("dhcp expire");
	
	e = document.getElementById("help_head");
	e.innerHTML = _("help help_head");
	e = document.getElementById("dhcp_direc");
	e.innerHTML = _("dhcp dhcp_direc");
	
}
</script>
</head>


<body onLoad="initValue()">
<center id="boxes">
	<div id="box">
	<div id="head"></div>	<!--end of head-->	
	<div id="content">
	<table id="layout_table" border="0"><!--start of layout_table-->
<tr><!--start of layout tr-->
<td class="tdwidth1" id="td1"><!--start of td1-->
<div id="left"><!--start of left-->


<table class="body"><tr><td>

<h1 id="dTitle"></h1>
<p id="dIntroduction" style="display: none;"></p>
<hr /><br/>

<table width="540" border="0" cellspacing="1" cellpadding="3" bordercolor="#9BABBD">
  <tr> 
    <td class="title" colspan="4" id="dClients">DHCP Clients</td>
  </tr>
  <tr>
    <td  id="dHostname">Hostname</td>
    <td  id="dMac">MAC Address</td>
    <td  id="dIp">IP Address</td>
    <td	 id="dExpr">Expires in</td>
  </tr>
  <% getDhcpCliList(); %>
</table>

</td></tr></table>


</div><!--end of left-->
</td><!--end of td1-->

<td class="tdwidth2" id="td2"><!--start of td2-->
	<div id="right"><!--start of right-->
		<h2 id="help_head"></h2>
		
		<p id="help_content"><span id="dhcp_direc"></span></p>
	</div><!--end of right-->
</td><!--end of td2-->
</tr><!--end of layout tr-->
</table><!--end of layout_table-->
</div><!--end of content-->
	</div>
	</center>
</body>
</html>


