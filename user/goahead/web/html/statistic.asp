<html><head><title>Statistic</title>

<link rel="stylesheet" href="/css/normal_ws.css" type="text/css">
<link rel="stylesheet" href="/css/boxStyle.css" type="text/css">
<meta http-equiv="content-type" content="text/html; charset=iso-8859-1">
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("admin");

function initTranslation()
{
	var e = document.getElementById("statisticTitle");
	e.innerHTML = _("statistic title");
	e = document.getElementById("statisticIntroduction");
	e.innerHTML = _("statistic introduction");

//	e = document.getElementById("statisticMM");
	//e.innerHTML = _("statistic memory");
	//e = document.getElementById("statisticMMTotal");
	//e.innerHTML = _("statistic memory total");
	//e = document.getElementById("statisticMMLeft");
	//e.innerHTML = _("statistic memory left");

	e = document.getElementById("statisticWANLAN");
	e.innerHTML = _("statistic wanlan");
	e = document.getElementById("statisticWANRxPkt");
	e.innerHTML = _("statistic wan rx pkt");
	e = document.getElementById("statisticWANRxBytes");
	e.innerHTML = _("statistic wan rx bytes");
	e = document.getElementById("statisticWANTxPkt");
	e.innerHTML = _("statistic wan tx pkt");
	e = document.getElementById("statisticWANTxBytes");
	e.innerHTML = _("statistic wan tx bytes");
	e = document.getElementById("statisticLANRxPkt");
	e.innerHTML = _("statistic lan rx pkt");
	e = document.getElementById("statisticLANRxBytes");
	e.innerHTML = _("statistic lan rx bytes");
	e = document.getElementById("statisticLANTxPkt");
	e.innerHTML = _("statistic lan tx pkt");
	e = document.getElementById("statisticLANTxBytes");
	e.innerHTML = _("statistic lan tx bytes");

//	e = document.getElementById("statisticAllIF");
	//e.innerHTML = _("statistic all interface");
}

function PageInit()
{
	initTranslation();
}

function formCheck()
{
	if( document.SystemCommand.command.value == ""){
		alert("Please specify a command.");
		return false;
	}

	return true;
}

</script>

</head>
<body onload="PageInit()">
<center id="boxes">
	<div id="box">
	<div id="head"></div>	<!--end of head-->	
	<div id="content">


	<table id="layout_table" border="0"><!--start of layout_table-->
<tr><!--start of layout tr-->
<td class="tdwidth1" id="td1"><!--start of td1-->
<div id="left"><!--start of left-->



<table class="body"><tr><td>

<h1 id="statisticTitle">Statistic</h1>
<p style="display:none" id="statisticIntroduction"> Take a look at the Ralink SoC statistics </p>
<hr/><br/>

<table border="0" cellpadding="2" cellspacing="1" width="540">
<tbody>

<!-- =================  MEMORY  ================= -->
<!--
<tr>
  <td class="title" colspan="2" id="statisticMM">Memory</td>
</tr>
<tr>
  <td class="head" id="statisticMMTotal">Memory total: </td>
  <td> <% getMemTotalASP(); %></td>
</tr>
<tr>
  <td class="head" id="statisticMMLeft">Memory left: </td>
  <td> <% getMemLeftASP(); %></td>
</tr>
-->

<!-- =================  WAN/LAN  ================== -->
<tr>
  <td class="title" colspan="2" id="statisticWANLAN">WAN/LAN</td>
</tr>
<tr>
  <td class="head" id="statisticWANRxPkt">WAN Rx packets: </td>
  <td> <% getWANRxPacketASP(); %></td>
</tr>
<tr>
  <td class="head" id="statisticWANRxBytes">WAN Rx bytes: </td>
  <td> <% getWANRxByteASP(); %></td>
</tr>
<tr>
  <td class="head" id="statisticWANTxPkt">WAN Tx packets: </td>
  <td> <% getWANTxPacketASP(); %></td>
</tr>
<tr>
  <td class="head" id="statisticWANTxBytes">WAN Tx bytes: </td>
  <td> <% getWANTxByteASP(); %></td>
</tr>
<tr>
  <td class="head" id="statisticLANRxPkt">LAN Rx packets: &nbsp; &nbsp; &nbsp; &nbsp;</td>
  <td> <% getLANRxPacketASP(); %></td>
</tr>
<tr>
  <td class="head" id="statisticLANRxBytes">LAN Rx bytes: </td>
  <td> <% getLANRxByteASP(); %></td>
</tr>
<tr>
  <td class="head" id="statisticLANTxPkt">LAN Tx packets: </td>
  <td> <% getLANTxPacketASP(); %></td>
</tr>
<tr>
  <td class="head" id="statisticLANTxBytes">LAN Tx bytes: </td>
  <td> <% getLANTxByteASP(); %></td>
</tr>

<!-- =================  ALL  ================= -->
<!--
<tr>
  <td class="title" colspan="2" id="statisticAllIF">All interfaces</td>
<tr>

<script type="text/javascript">
var i;
var a = new Array();
a = [<% getAllNICStatisticASP(); %>];
for(i=0; i<a.length; i+=5){
	// name
	document.write("<tr> <td class=head> Name </td><td class=head>");
	document.write(a[i]);
	document.write("</td></tr>");

	// Order is important! rxpacket->rxbyte->txpacket->txbyte
	// rxpacket
	document.write("<tr> <td class=head> Rx Packet </td><td>");
	document.write(a[i+1]);
	document.write("</td></tr>");

	// rxbyte
	document.write("<tr> <td class=head> Rx Byte </td><td>");
	document.write(a[i+2]);
	document.write("</td></tr>");

	// txpacket
	document.write("<tr> <td class=head> Tx Packet </td><td>");
	document.write(a[i+3]);
	document.write("</td></tr>");

	// txbyte
	document.write("<tr> <td class=head> Tx Byte </td><td>");
	document.write(a[i+4]);
	document.write("</td></tr>");
}
</script>
-->
</tbody>
</table>

</td></tr></table>



</div><!--end of left-->
</td><!--end of td1-->

<td class="tdwidth2" id="td2"><!--start of td2-->
	<div id="right"><!--start of right-->
		<h2 id="help_head">Heeelp...<a href="#">more</a></h2>
		
		<p id="help_content">Something provide help........</p>
	</div><!--end of right-->
</td><!--end of td2-->
</tr><!--end of layout tr-->
</table><!--end of layout_table-->



</div><!--end of content-->
	</div>
	</center>
</body></html>

