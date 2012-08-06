<html>
<head>
<title>Operation Mode</title>
<link rel="stylesheet" href="/css/normal_ws.css" type="text/css">
<link rel="stylesheet" href="/css/boxStyle.css" type="text/css">
<meta http-equiv="content-type" content="text/html; charset=iso-8859-1">
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("main");

var opmode;
var old_mode;

function changeMode()
{
	var nat_en = "<% getCfgZero(1, "natEnabled"); %>";
	var dpbsta = "<% getDpbSta(); %>";
	var ec_en = "<% getCfgZero(1, "ethConvert"); %>";
	var mii_built = "<% getMiiInicBuilt(); %>"
	var mii_en = "<% getCfgZero(1, "InicMiiEnable"); %>";

	document.getElementById("eth_conv").style.visibility = "hidden";
	document.getElementById("eth_conv").style.display = "none";
	document.getElementById("eth_conv").style.disable = true;
	document.getElementById("nat").style.visibility = "hidden";
	document.getElementById("nat").style.display = "none";
	document.getElementById("nat").style.disable = true;
	document.getElementById("miiInic").style.visibility = "hidden";
	document.getElementById("miiInic").style.display = "none";
	document.getElementById("miiInic").style.disable = true;

	if (document.opmode.opMode[0].checked) {
		document.getElementById('selectMode').options[0].selected = true;
		opmode = 0;
		if (dpbsta == "1") {
			document.getElementById("eth_conv").style.visibility = "visible";
			document.getElementById("eth_conv").style.display = "block";
			document.getElementById("eth_conv").style.disable = false;
			if (ec_en == "1") {
				document.opmode.ethConv.options.selectedIndex = 1;
			}
		}
		if (mii_built == "1") {
			document.getElementById("miiInic").style.visibility = "visible";
			document.getElementById("miiInic").style.display = "block";
			document.getElementById("miiInic").style.disable = false;
			if (mii_en == "1") {
				document.opmode.miiMode.options.selectedIndex = 1;
			}
		}
	}
	else if (document.opmode.opMode[1].checked || document.opmode.opMode[3].checked) {
		if(document.opmode.opMode[1].checked)
		document.getElementById('selectMode').options[1].selected = true;
		else if(document.opmode.opMode[3].checked)
		document.getElementById('selectMode').options[2].selected = true;
		opmode = 1;
		document.getElementById("nat").style.visibility = "visible";
		document.getElementById("nat").style.display = "block";
		document.getElementById("nat").style.disable = false;
		if (nat_en == "1") {
			document.opmode.natEnbl.options.selectedIndex = 1;
		}
	}
	else if (document.opmode.opMode[2].checked) {
		document.getElementById('selectMode').options[1].selected = true;
		opmode = 2;
		//keep the nat table hidden, and always enable NAT
		document.getElementById("nat").style.disable = false;
		document.opmode.natEnbl.options.selectedIndex = 1;
	}
}

function initTranslation()
{
	var e = document.getElementById("oTitle");
	e.innerHTML = _("opmode title");
	e = document.getElementById("oIntroduction");
	e.innerHTML = _("opmode introduction");

	e = document.getElementById("oModeB");
	e.innerHTML = _("opmode mode b");
	e = document.getElementById("oModeBIntro");
	e.innerHTML = _("opmode mode b intro");
	e = document.getElementById("oModeG");
	e.innerHTML = _("opmode mode g");
	e = document.getElementById("oModeGIntro");
	e.innerHTML = _("opmode mode g intro");
	e = document.getElementById("oModeE");
	e.innerHTML = _("opmode mode e");
	e = document.getElementById("stadd");
	e.innerHTML = _("opmode mode e intro");
	e = document.getElementById("oModeA");
	e.innerHTML = _("opmode mode a");
	e = document.getElementById("apclidd");
	e.innerHTML = _("opmode mode a intro");

	e = document.getElementById("oNat");
	e.innerHTML = _("opmode nat");
	e = document.getElementById("oNatD");
	e.innerHTML = _("main disable");
	e = document.getElementById("oNatE");
	e.innerHTML = _("main enable");

	e = document.getElementById("oEthConv");
	e.innerHTML = _("opmode eth conv");
	e = document.getElementById("oEthConvD");
	e.innerHTML = _("main disable");
	e = document.getElementById("oEthConvE");
	e.innerHTML = _("main enable");
	
	e = document.getElementById("help_head");
	e.innerHTML = _("help help_head");
	e = document.getElementById("opmode_direc");
	e.innerHTML = _("opmode opmode_direc");

	e = document.getElementById("oApply");
	e.value = _("main apply");
	e = document.getElementById("oCancel");
	e.value = _("main cancel");
	
}

function initValue()
{
	opmode = "<% getCfgZero(1, "OperationMode"); %>";
	old_mode = opmode;

	var gwb = "<% getGWBuilt(); %>";
	var apcli = "<% getWlanApcliBuilt(); %>";
	var sta = "<% getStationBuilt(); %>";

	initTranslation();

	if (gwb == "0") {
		document.getElementById("gwdt").style.visibility = "hidden";
		document.getElementById("gwdt").style.display = "none";
		document.getElementById("oModeGIntro").style.visibility = "hidden";
		document.getElementById("oModeGIntro").style.display = "none";
	}
	if (apcli == "0") {
		document.getElementById("apclidt").style.visibility = "hidden";
		document.getElementById("apclidt").style.display = "none";
		document.getElementById("apclidd").style.visibility = "hidden";
		document.getElementById("apclidd").style.display = "none";
	}
	if (sta == "0") {
		document.getElementById("stadt").style.visibility = "hidden";
		document.getElementById("stadt").style.display = "none";
		document.getElementById("stadd").style.visibility = "hidden";
		document.getElementById("stadd").style.display = "none";
	}

	if (opmode == "1")
		document.opmode.opMode[1].checked = true;
	else if (opmode == "2")
		document.opmode.opMode[2].checked = true;
	else if (opmode == "3")
		document.opmode.opMode[3].checked = true;
	else
		document.opmode.opMode[0].checked = true;
	changeMode();
}

function msg()
{
	if(document.opmode.opMode[1].checked == true && <% isOnePortOnly(); %> ){
		alert("In order to access web page please \nchange or alias your IP address to 172.32.1.1");
	}
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



<div id="div">
<table class="body" border="0"><tr><td>

<h1 id="oTitle"></h1>
<p id="oIntroduction" style="display:none"></p>
<hr><br/>

<form method="post" name="opmode" action="/goform/setOpMode">
<dl style="display:none">
  <dt><input type="radio" name="opMode" id="opMode" value="0" onClick="changeMode()"><b id="oModeB">Bre:</b></dt>
  <dd id="oModeBIntro"></dd>
  <dt id="gwdt"><input type="radio" name="opMode" id="opMode" value="1" onClick="changeMode()"><b id="oModeG">Gateway:</b></dt>
  <dd id="oModeGIntro"></dd>
  <dt id="stadt"><input type="radio" name="opMode" id="opMode" value="2" onClick="changeMode()"><b id="oModeE">Ethernet Converter:</b></dt>
  <dd id="stadd"></dd>
  <dt id="apclidt"><input type="radio" name="opMode" id="opMode" value="3" onClick="changeMode()"><b id="oModeA">AP Client:</b></dt>
  <dd id="apclidd"></dd>
</dl>

<script type="text/javascript">
		function select(){
			if(document.getElementById('selectMode').options[0].selected){
				document.opmode.opMode[0].click();
			}
			else if(document.getElementById('selectMode').options[1].selected){
				document.opmode.opMode[1].click();
			}
			if(document.getElementById('selectMode').options[2].selected){
				document.opmode.opMode[3].click();
			}
		}
</script>
<select id="selectMode" onchange="select()">
			<option>Bridge</option>
			<option>Gateway</option>
			<option>AP Client</option>
</select>


<table id="nat" border="0" cellpadding="2" cellspacing="1">
<tr>
  <td id="oNat">NAT Enabled:<td>
  <td>
    <select id="natEnbl" name="natEnbl" size="1">
      <option value="0" id="oNatD">Disable</option>
      <option value="1" id="oNatE">Enable</option>
    </select>
  </td>
</tr>
</table>
<table id="eth_conv" border="0" cellpadding="2" cellspacing="1">
<tr>
  <td id="oEthConv">Ethernet Converter Enabled:<td>
  <td>
    <select id="ethConv" name="ethConv" size="1">
      <option value="0" id="oEthConvD">Disable</option>
      <option value="1" id="oEthConvE">Enable</option>
    </select>
  </td>
</tr>
</table>
<table id="miiInic" border="0" cellpadding="2" cellspacing="1">
<tr>
  <td id="oMiiMode">INIC Mii Mode:<td>
  <td>
    <select id="miiMode" name="miiMode" size="1">
      <option value="0" id="oMiiModeD">Disable</option>
      <option value="1" id="oMiiModeE">Enable</option>
    </select>
  </td>
</tr>
</table>
<p />
<center>
<input type="button" value="Apply" id="oApply" onClick="msg();document.opmode.submit();<!--parent.mainFrame.location.reload();-->">&nbsp;&nbsp;
<input type="reset"  value="Reset" id="oCancel" onClick="window.location.reload()">
</center>
</form>

</td></tr></table>
</div>



</div><!--end of left-->
</td><!--end of td1-->

<td class="tdwidth2" id="td2"><!--start of td2-->
	<div id="right"><!--start of right-->
		<h2 id="help_head">Heeelp...</h2>
		
		<p id="help_content"><span id="opmode_direc"></span></p>
	</div><!--end of right-->
</td><!--end of td2-->
</tr><!--end of layout tr-->
</table><!--end of layout_table-->
</div><!--end of content-->
	</div>
	</center>
</body>
</html>
