<html><head><title>Static Routing Settings</title>

<link rel="stylesheet" href="/css/normal_ws.css" type="text/css">
<link rel="stylesheet" href="/css/boxStyle.css" type="text/css">
<meta http-equiv="content-type" content="text/html; charset=iso-8859-1">
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("internet");

var opmode = "<% getCfgZero(1, "OperationMode"); %>";

var destination = new Array();
var gateway = new Array();
var netmask = new Array();
var flags = new Array();
var metric = new Array();
var ref = new Array();
var use = new Array();
var true_interface = new Array();
var category = new Array();
var interface = new Array();
var idle = new Array();
var comment = new Array();

function deleteClick()
{
	return true;
}

function checkRange(str, num, min, max)
{
    d = atoi(str,num);
    if(d > max || d < min)
        return false;
    return true;
}

function checkIpAddr(field)
{
    if(field.value == ""){
        field.focus();
        return false;
    }

    if ( isAllNum(field.value) == 0) {
        field.focus();
        return false;
    }

    if( (!checkRange(field.value,1,0,255)) ||
        (!checkRange(field.value,2,0,255)) ||
        (!checkRange(field.value,3,0,255)) ||
        (!checkRange(field.value,4,0,255)) ){
        field.focus();
        return false;
    }

   return true;
}


function atoi(str, num)
{
	i=1;
	if(num != 1 ){
		while (i != num && str.length != 0){
			if(str.charAt(0) == '.'){
				i++;
			}
			str = str.substring(1);
		}
	  	if(i != num )
			return -1;
	}
	
	for(i=0; i<str.length; i++){
		if(str.charAt(i) == '.'){
			str = str.substring(0, i);
			break;
		}
	}
	if(str.length == 0)
		return -1;
	return parseInt(str, 10);
}

function isAllNum(str)
{
	for (var i=0; i<str.length; i++){
	    if((str.charAt(i) >= '0' && str.charAt(i) <= '9') || (str.charAt(i) == '.' ))
			continue;
		return 0;
	}
	return 1;
}

function formCheck()
{
	if( document.addrouting.dest.value != "" && !checkIpAddr(document.addrouting.dest )){
		alert("The destination has wrong format.");
		return false;
	}
	if( document.addrouting.netmask.value != "" && !checkIpAddr(document.addrouting.netmask )){
		alert("The netmask has wrong format.");
		return false;
	}
	if( document.addrouting.gateway.value != "" && !checkIpAddr(document.addrouting.gateway)){
		alert("The gateway has wrong format.");
		return false;
	}

	if(	document.addrouting.dest.value == ""){
		alert("please input the destination.");
		return false;
	}

    if( document.addrouting.hostnet.selectedIndex == 1 &&
		document.addrouting.netmask.value == ""){
		alert("please input the netmask.");
        return false;
    }

	if(document.addrouting.interface.value == "Custom" &&
		document.addrouting.custom_interface.value == ""){
		alert("please input custom interface name.");
		return false;
	}


	return true;
}


function display_on()
{
  if(window.XMLHttpRequest){ // Mozilla, Firefox, Safari,...
    return "table-row";
  } else if(window.ActiveXObject){ // IE
    return "block";
  }
}

function disableTextField (field)
{
  if(document.all || document.getElementById)
    field.disabled = true;
  else {
    field.oldOnFocus = field.onfocus;
    field.onfocus = skip;
  }
}

function enableTextField (field)
{
  if(document.all || document.getElementById)
    field.disabled = false;
  else {
    field.onfocus = field.oldOnFocus;
  }
}

function initTranslation()
{
	var e;
	e = document.getElementById("routingTitle");
	e.innerHTML = _("routing title");
	e = document.getElementById("routingIntroduction");
	e.innerHTML = _("routing Introduction");
	e = document.getElementById("routingAddRule");
	e.innerHTML = _("routing add rule");
	e = document.getElementById("routingDest");
	e.innerHTML = _("routing routing dest");
	e = document.getElementById("routingRange");
	e.innerHTML = _("routing range");
	//e = document.getElementById("routingNetmask");
	//e.innerHTML = _("routing netmask");
	e = document.getElementById("routingGateway");
	e.innerHTML = _("routing gateway");
	e = document.getElementById("routingInterface");
	e.innerHTML = _("routing interface");
	e = document.getElementById("routingCustom");
	e.innerHTML = _("routing custom");
	e = document.getElementById("routingComment");
	e.innerHTML = _("routing comment");
	e = document.getElementById("routingSubmit");
	e.value = _("routing submit");
	e = document.getElementById("routingReset");
	e.value = _("routing reset");
	e = document.getElementById("routingCurrentRoutingTableRules");
	e.innerHTML = _("routing del title");
	e = document.getElementById("routingNo");
	e.innerHTML = _("routing Number");
	e = document.getElementById("routingDelDest");
	e.innerHTML = _("routing del dest");
	e = document.getElementById("routingDelNetmask");
	e.innerHTML = _("routing del netmask");
	e = document.getElementById("routingDelGateway");
	e.innerHTML = _("routing del gateway");
	e = document.getElementById("routingDelFlags");
	e.innerHTML = _("routing del flags");
	e = document.getElementById("routingDelMetric");
	e.innerHTML = _("routing del metric");
	e = document.getElementById("routingDelRef");
	e.innerHTML = _("routing del ref");
	e = document.getElementById("routingDelUse");
	e.innerHTML = _("routing del use");
	e = document.getElementById("routingDelInterface");
	e.innerHTML = _("routing del interface");
	e = document.getElementById("routingDelComment");
	e.innerHTML = _("routing del comment");
	e = document.getElementById("routingDel");
	e.value = _("routing del");
	e = document.getElementById("routingDelReset");
	e.value = _("routing del reset");
	e = document.getElementById("routing host");
	e.innerHTML = _("routing host");
	e = document.getElementById("routing net");
	e.innerHTML = _("routing net");
	e = document.getElementById("routing LAN");
	e.innerHTML = _("routing LAN");
	if(document.getElementById("routing WAN")){
		e = document.getElementById("routing WAN");
		e.innerHTML = _("routing WAN");
	}
	e = document.getElementById("dynamicRoutingTitle");
	e.innerHTML = _("routing dynamic Title");
	e = document.getElementById("dynamicRoutingTitle2");
	e.innerHTML = _("routing dynamic Title2");
	e = document.getElementById("RIPDisable");
	e.innerHTML = _("routing dynamic rip disable");
	e = document.getElementById("RIPEnable");
	e.innerHTML = _("routing dynamic rip enable");
	e = document.getElementById("dynamicRoutingApply");
	e.value = _("routing dynamic rip apply");
	e = document.getElementById("dynamicRoutingReset");
	e.value = _("routing dynamic rip reset");
}

function onInit()
{
	initTranslation();

	document.addrouting.hostnet.selectedIndex = 0;

	document.addrouting.netmask.readOnly = true;
	document.getElementById("routingNetmaskRow").style.visibility = "hidden";
	document.getElementById("routingNetmaskRow").style.display = "none";

	document.addrouting.interface.selectedIndex = 0;
	document.addrouting.custom_interface.value = "";
	document.addrouting.custom_interface.readOnly = true;

	document.dynamicRouting.RIPSelect.selectedIndex = <% getCfgZero(1, "RIPEnable"); %>;

	mydiv = document.getElementById("dynamicRoutingDiv");
	if(! <% getDynamicRoutingBuilt(); %>){
		mydiv.style.display = "none";
		mydiv.style.visibility = "hidden";
	}

}

function wrapDel(str, idle)
{
	if(idle == 1){
		document.write("<del>" + str + "</del>");
	}else
		document.write(str);
}

function style_display_on()
{
	if (window.ActiveXObject) { // IE
		return "block";
	}
	else if (window.XMLHttpRequest) { // Mozilla, Safari,...
		return "table-row";
	}
}
function hostnetChange()
{
	if(document.addrouting.hostnet.selectedIndex == 1){
		document.getElementById("routingNetmaskRow").style.visibility = "visible";
		document.getElementById("routingNetmaskRow").style.display = style_display_on();
		document.addrouting.netmask.readOnly = false;
		document.addrouting.netmask.focus();

	}else{
		document.addrouting.netmask.value = "";
		document.addrouting.netmask.readOnly = true;
		document.getElementById("routingNetmaskRow").style.visibility = "hidden";
		document.getElementById("routingNetmaskRow").style.display = "none";
	}
}
function interfaceChange()
{
	if(document.addrouting.interface.selectedIndex == 2){
		document.addrouting.custom_interface.readOnly = false;
		document.addrouting.custom_interface.focus();
	}else{
		document.addrouting.custom_interface.value = "";
		document.addrouting.custom_interface.readOnly = true;
	}
}

</script><!--     body      --></head>
<body onload="onInit()">
<center id="boxes">
	<div id="box">
	<div id="head"></div>	<!--end of head-->	
	<div id="content">


	<table id="layout_table" border="0"><!--start of layout_table-->
<tr><!--start of layout tr-->
<td class="tdwidth1" id="td1"><!--start of td1-->
<div id="left"><!--start of left-->



<table class="body"><tbody><tr><td>
<h1 id="routingTitle">Static Routing  Settings </h1>
<p id="routingIntroduction" style="display: none;"> You may add or remote Internet routing rules here.</p>
<hr><br/>

<form method="post" name="addrouting" action="/goform/addRouting">
<table border="0" cellpadding="2" cellspacing="1" width="540">
<tbody><tr>
  <td class="title" colspan="2" id="routingAddRule">Add a routing rule</td>
</tr>

<tr>
	<td class="head" id="routingDest">
		Destination
	</td>
	<td>
  		<input size="16" name="dest" type="text">
	</td>
</tr>

<tr>
	<td class="head" id="routingRange">
		Host/Net
	</td>
	<td>
		<select name="hostnet" onChange="hostnetChange()">
		<option select="" value="host" id="routing host">Host</option>
		<option value="net"  id="routing net">Net</option>
		</select>
	</td>
</tr>

<tr id="routingNetmaskRow">
	<td class="head" id="routingNetmask">
		Sub Netmask
	</td>
	<td>
  		<input size="16" name="netmask" type="text">
	</td>
</tr>

<tr>
	<td class="head" id="routingGateway">
		Gateway
	</td>
	<td>
  		<input size="16" name="gateway" type="text">
	</td>
</tr>

<tr>
	<td class="head" id="routingInterface">
		Interface
	</td>
	<td>
		<select name="interface" onChange="interfaceChange()">
		<option select="" value="LAN" id="routing LAN">LAN</option>

		<script language="JavaScript" type="text/javascript">
			if(opmode == "1")
				document.write("<option value=\"WAN\" id=\"routing WAN\">WAN</option>");
		</script>

		<option value="Custom" id="routingCustom">Custom</option>
		</select>
		<input alias="right" size="16" name="custom_interface" type="text">
	</td>
</tr>

<tr style="display: none;">
	<td class="head" id="routingComment">
		Comment
	</td>
	<td>
		<input name="comment" size="16" maxlength="32" type="text">
	</td>
</tr>
</tbody></table>

<p align="center">
	<input value="Apply"  id="routingSubmit" name="addFilterPort" onclick="return formCheck()" type="submit"> &nbsp;&nbsp;
	<input value="Reset"  id="routingReset" name="reset" type="reset">
</p>
</form>

<br>
<!--  delete rules -->
<form action="/goform/delRouting" method="post" name="delRouting">

<table border="1" cellpadding="2" cellspacing="1" width="540">	
	<tbody><tr>
		<td class="title" colspan="10" id="routingCurrentRoutingTableRules">Current Routing table in the system: </td>
	</tr>

	<tr>
		<td id="routingNo"> No.</td>
		<td id="routingDelDest" align="center"> Destination </td>
		<td id="routingDelNetmask" align="center"> Netmask</td>
		<td id="routingDelGateway" align="center"> Gateway</td>
		<td id="routingDelFlags" align="center"> Flags</td>
		<td id="routingDelMetric" align="center"> Metric</td>
		<td id="routingDelRef" align="center"> Ref</td>
		<td id="routingDelUse" align="center"> Use</td>
		<td id="routingDelInterface" align="center"> Interface</td>
		<td id="routingDelComment" align="center"> Comment</td>
	</tr>

	<script language="JavaScript" type="text/javascript">
	var i;
	var entries = new Array();
	var all_str = <% getRoutingTable(); %>;

	entries = all_str.split(";");
	for(i=0; i<entries.length; i++){
		var one_entry = entries[i].split(",");


		true_interface[i] = one_entry[0];
		destination[i] = one_entry[1];
		gateway[i] = one_entry[2];
		netmask[i] = one_entry[3];
		flags[i] = one_entry[4];
		ref[i] = one_entry[5];
		use[i] = one_entry[6];
		metric[i] = one_entry[7];
		category[i] = parseInt(one_entry[8]);
		interface[i] = one_entry[9];
		idle[i] = parseInt(one_entry[10]);
		comment[i] = one_entry[11];
		if(comment[i] == " " || comment[i] == "")
			comment[i] = "&nbsp";
	}

	for(i=0; i<entries.length; i++){
		if(category[i] > -1){
			document.write("<tr bgcolor=#F1F1FF>");
			document.write("<td>");
			document.write(i+1);
			document.write("<input type=checkbox name=DR"+ category[i] + 
				" value=\""+ destination[i] + " " + netmask[i] + " " + true_interface[i] +"\">");
			document.write("</td>");
		}else{
			document.write("<tr>");
			document.write("<td>"); 	document.write(i+1);			 	document.write("</td>");
		}

		document.write("<td>"); 	wrapDel(destination[i], idle[i]); 	document.write("</td>");
		document.write("<td>"); 	wrapDel(netmask[i], idle[i]);		document.write("</td>");
		document.write("<td>"); 	wrapDel(gateway[i], idle[i]); 		document.write("</td>");
		document.write("<td>"); 	wrapDel(flags[i], idle[i]);			document.write("</td>");
		document.write("<td>"); 	wrapDel(metric[i], idle[i]);		document.write("</td>");
		document.write("<td>"); 	wrapDel(ref[i], idle[i]);			document.write("</td>");
		document.write("<td>"); 	wrapDel(use[i], idle[i]);			document.write("</td>");

		if(interface[i] == "LAN")
			interface[i] = _("routing LAN");
		else if(interface[i] == "WAN")
			interface[i] = _("routing WAN");
		else if(interface[i] == "Custom")
			interface[i] = _("routing custom");

		document.write("<td>"); 	wrapDel(interface[i] + "(" +true_interface[i] + ")", idle[i]);		document.write("</td>");
		document.write("<td>"); 	wrapDel(comment[i], idle[i]);		document.write("</td>");
		document.write("</tr>\n");
	}
	</script>

</tbody></table>
<br>
<p align="center">
<input value="Delete Selected"  id="routingDel" name="deleteSelPortForward" onclick="return deleteClick()" type="submit">&nbsp;&nbsp;
<input value="Reset"  id="routingDelReset" name="reset" type="reset">
</p>
</form>

<div id=dynamicRoutingDiv>
<h1 id="dynamicRoutingTitle">Dynamic Routing Settings </h1>
<form method=post name="dynamicRouting" action=/goform/dynamicRouting>
<table width="540" border="0" cellpadding="2" cellspacing="1">
<tr>
	<td class="title" colspan="2" id="dynamicRoutingTitle2">Dynamic routing</td>
</tr>
<tr>
	<td class="head" id="RIP">
		RIP
	</td>

	<td>
	<select name="RIPSelect" size="1">
	<option value=0 id="RIPDisable">Disable</option>
	<option value=1 id="RIPEnable">Enable</option>
	</select>
	</td>
</tr>
</table>

<p align="center">
	<input type="submit" value="Apply" id="dynamicRoutingApply" name="dynamicRoutingApply" > &nbsp;&nbsp;
	<input type="reset" value="Reset" id="dynamicRoutingReset" name="dynamicRoutingReset">
</p>
</form>
</div>


</td></tr></tbody></table>


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
