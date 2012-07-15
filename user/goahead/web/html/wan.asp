<html>
<head>
<title>Wide Area Network (WAN) Settings</title>
<link rel="stylesheet" href="/css/normal_ws.css" type="text/css">
<link rel="stylesheet" href="/css/boxStyle.css" type="text/css">
<meta http-equiv="content-type" content="text/html; charset=iso-8859-1">
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
var http_request = false;
Butterlate.setTextDomain("internet");

function macCloneMacFillSubmit()
{
    http_request = false;
    if (window.XMLHttpRequest) { // Mozilla, Safari,...
        http_request = new XMLHttpRequest();
        if (http_request.overrideMimeType) {
            http_request.overrideMimeType('text/xml');
        }
    } else if (window.ActiveXObject) { // IE
        try {
            http_request = new ActiveXObject("Msxml2.XMLHTTP");
        } catch (e) {
            try {
            http_request = new ActiveXObject("Microsoft.XMLHTTP");
            } catch (e) {}
        }
    }
    if (!http_request) {
        alert('Cannot create an XMLHTTP instance');
        return false;
    }
    http_request.onreadystatechange = doFillMyMAC;

    http_request.open('POST', '/goform/getMyMAC', true);
    http_request.send('n\a');
}

function doFillMyMAC()
{
    if (http_request.readyState == 4) {
		if (http_request.status == 200) {
			document.getElementById("macCloneMac").value = http_request.responseText;
		} else {
			alert("Can\'t get the mac address.");
		}
	}
}


function macCloneSwitch()
{
	if (document.wanCfg.macCloneEnbl.options.selectedIndex == 1) {
		document.getElementById("macCloneMacRow").style.visibility = "visible";
		document.getElementById("macCloneMacRow").style.display = style_display_on();
	}
	else {
		document.getElementById("macCloneMacRow").style.visibility = "hidden";
		document.getElementById("macCloneMacRow").style.display = "none";
	}
}

function connectionTypeSwitch()
{
	document.getElementById("static").style.visibility = "hidden";
	document.getElementById("static").style.display = "none";
	document.getElementById("dhcp").style.visibility = "hidden";
	document.getElementById("dhcp").style.display = "none";
	document.getElementById("pppoe").style.visibility = "hidden";
	document.getElementById("pppoe").style.display = "none";
	document.getElementById("l2tp").style.visibility = "hidden";
	document.getElementById("l2tp").style.display = "none";
	document.getElementById("pptp").style.visibility = "hidden";
	document.getElementById("pptp").style.display = "none";
	document.getElementById("3G").style.visibility = "hidden";
	document.getElementById("3G").style.display = "none";
	document.getElementById("Operators").style.visibility = "hidden";
	document.getElementById("Operators").style.display = "none";
	document.getElementById("CurOperators").style.visibility = "hidden";
	document.getElementById("CurOperators").style.display = "none";

	if (document.wanCfg.connectionType.options.selectedIndex == 0) {
		document.getElementById("static").style.visibility = "visible";
		document.getElementById("static").style.display = "block";
		showSubmit();
	}
	else if (document.wanCfg.connectionType.options.selectedIndex == 1) {
		document.getElementById("dhcp").style.visibility = "visible";
		document.getElementById("dhcp").style.display = "block";
		showSubmit();
	}
	else if (document.wanCfg.connectionType.options.selectedIndex == 2) {
		document.getElementById("pppoe").style.visibility = "visible";
		document.getElementById("pppoe").style.display = "block";
		showSubmit();
		pppoeOPModeSwitch();
	}
	/*else if (document.wanCfg.connectionType.options.selectedIndex == 3) {
		document.getElementById("l2tp").style.visibility = "visible";
		document.getElementById("l2tp").style.display = "block";
		l2tpOPModeSwitch();
	}
	else if (document.wanCfg.connectionType.options.selectedIndex == 4) {
		document.getElementById("pptp").style.visibility = "visible";
		document.getElementById("pptp").style.display = "block";
		pptpOPModeSwitch();
	}*/
	else if (document.wanCfg.connectionType.options.selectedIndex == 3) {
		document.getElementById("3G").style.visibility = "visible";
		document.getElementById("3G").style.display = "block";
		document.getElementById("Operators").style.visibility = "visible";
		document.getElementById("Operators").style.display = "block";
		document.getElementById("CurOperators").style.visibility = "visible";
		document.getElementById("CurOperators").style.display = "block";
        G3OPModeSwitch();
        hideSubmit();
	}
	else {
		document.getElementById("static").style.visibility = "visible";
		document.getElementById("static").style.display = "block";
		showSubmit();
	}
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

function l2tpModeSwitch()
{
	if (document.wanCfg.l2tpMode.selectedIndex == 0) {
		document.getElementById("l2tpIp").style.visibility = "visible";
		document.getElementById("l2tpIp").style.display = style_display_on();
		document.getElementById("l2tpNetmask").style.visibility = "visible";
		document.getElementById("l2tpNetmask").style.display = style_display_on();
		document.getElementById("l2tpGateway").style.visibility = "visible";
		document.getElementById("l2tpGateway").style.display = style_display_on();
	}
	else {
		document.getElementById("l2tpIp").style.visibility = "hidden";
		document.getElementById("l2tpIp").style.display = "none";
		document.getElementById("l2tpNetmask").style.visibility = "hidden";
		document.getElementById("l2tpNetmask").style.display = "none";
		document.getElementById("l2tpGateway").style.visibility = "hidden";
		document.getElementById("l2tpGateway").style.display = "none";
	}
}

function pptpModeSwitch()
{
	if (document.wanCfg.pptpMode.selectedIndex == 0) {
		document.getElementById("pptpIp").style.visibility = "visible";
		document.getElementById("pptpIp").style.display = style_display_on();
		document.getElementById("pptpNetmask").style.visibility = "visible";
		document.getElementById("pptpNetmask").style.display = style_display_on();
		document.getElementById("pptpGateway").style.visibility = "visible";
		document.getElementById("pptpGateway").style.display = style_display_on();
	}
	else {
		document.getElementById("pptpIp").style.visibility = "hidden";
		document.getElementById("pptpIp").style.display = "none";
		document.getElementById("pptpNetmask").style.visibility = "hidden";
		document.getElementById("pptpNetmask").style.display = "none";
		document.getElementById("pptpGateway").style.visibility = "hidden";
		document.getElementById("pptpGateway").style.display = "none";
	}
}

function pppoeOPModeSwitch()
{
	document.wanCfg.pppoeRedialPeriod.disabled = true;
	document.wanCfg.pppoeIdleTime.disabled = true;
	if (document.wanCfg.pppoeOPMode.options.selectedIndex == 0) 
		document.wanCfg.pppoeRedialPeriod.disabled = false;
	else if (document.wanCfg.pppoeOPMode.options.selectedIndex == 1)
		document.wanCfg.pppoeIdleTime.disabled = false;
}

function l2tpOPModeSwitch()
{
	document.wanCfg.l2tpRedialPeriod.disabled = true;
	// document.wanCfg.l2tpIdleTime.disabled = true;
	if (document.wanCfg.l2tpOPMode.options.selectedIndex == 0) 
		document.wanCfg.l2tpRedialPeriod.disabled = false;
	/*
	else if (document.wanCfg.l2tpOPMode.options.selectedIndex == 1)
		document.wanCfg.l2tpIdleTime.disabled = false;
	*/
}

function pptpOPModeSwitch()
{
	document.wanCfg.pptpRedialPeriod.disabled = true;
	// document.wanCfg.pptpIdleTime.disabled = true;
	if (document.wanCfg.pptpOPMode.options.selectedIndex == 0) 
		document.wanCfg.pptpRedialPeriod.disabled = false;
	/*
	else if (document.wanCfg.pptpOPMode.options.selectedIndex == 1)
		document.wanCfg.pptpIdleTime.disabled = false;
	*/
}

function G3OPModeSwitch()
{
	//alert("G3OPModeSwitch");
    document.getElementById("G3TimeOptions").style.visibility = "hidden";
	document.getElementById("G3TimeOptions").style.display = "none";
    document.getElementById("G3TimePeriodsTable").style.visibility = "hidden";
	document.getElementById("G3TimePeriodsTable").style.display = "none";
	document.getElementById("g3connectbtn").style.visibility = "hidden";
/*	document.wanCfg.G3RedialPeriod.disabled = true;*/
	document.wanCfg.G3IdleTime.disabled = true;
	if (document.wanCfg.G3Mode.options.selectedIndex == 0) 
	{
		/*document.wanCfg.G3RedialPeriod.disabled = false;        */
	}
	else if (document.wanCfg.G3Mode.options.selectedIndex == 1)
	{
		document.wanCfg.G3IdleTime.disabled = false;
        //document.getElementById("G3TimeOptions").style.visibility = "visible";
		//document.getElementById("G3TimeOptions").style.display = style_display_on();
	}
    else if (document.wanCfg.G3Mode.options.selectedIndex == 2)
    {
       // document.getElementById("G3TimePeriodsTable").style.visibility = "visible";
	    //document.getElementById("G3TimePeriodsTable").style.display =style_display_on();;
    }
    else if (document.wanCfg.G3Mode.options.selectedIndex == 3)
    {
	//document.getElementById("g3connectbtn").style.visibility = "visible";
    }
        
}

function MSPListOnSelectStr(field, id)
{
    var str = document.getElementById(id).innerHTML;
    if(str == "&nbsp;")
    {
        field.value = "";
    }
    else
    {
        field.value = str;
    }
}

function MSPListOnSelectSelect(field, id)
{
    field.options.selectedIndex = document.getElementById(id).value;
}

function MSPListOnSelect(num)
{
	var id = "Name" + num;
    MSPListOnSelectStr(document.Operators.G3Comment, id);
	id = "NetType" + num;
    MSPListOnSelectSelect(document.Operators.g3NetworkType, id);
	id = "DialNum" + num;
    MSPListOnSelectStr(document.Operators.DialingStr, id);
	id = "InitCmd" + num;
    MSPListOnSelectStr(document.Operators.InitCmdStr, id);
	id = "User" + num;
    MSPListOnSelectStr(document.Operators.G3UserName, id);
	id = "Password" + num;
    MSPListOnSelectStr(document.Operators.G3Password, id);	
	id = "LocalIp" + num;
    MSPListOnSelectStr(document.Operators.G3LocalIp, id);	
	id = "AuthType" + num;
    MSPListOnSelectSelect(document.Operators.G3AuthProtocol, id);
	id = "Bcomp" + num;
	document.Operators.G3UseCompress.checked = 
	parseInt(document.getElementById(id).innerHTML, 10);
}

function atoi(str, num)
{
	i = 1;
	if (num != 1) {
		while (i != num && str.length != 0) {
			if (str.charAt(0) == '.') {
				i++;
			}
			str = str.substring(1);
		}
		if (i != num)
			return -1;
	}

	for (i=0; i<str.length; i++) {
		if (str.charAt(i) == '.') {
			str = str.substring(0, i);
			break;
		}
	}
	if (str.length == 0)
		return -1;
	return parseInt(str, 10);
}

function checkRange(str, num, min, max)
{
	d = atoi(str, num);
	if (d > max || d < min)
		return false;
	return true;
}

function isAllNum(str)
{
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) >= '0' && str.charAt(i) <= '9') || (str.charAt(i) == '.' ))
			continue;
		return 0;
	}
	return 1;
}

function checkIpAddr(field, ismask)
{
	if (field.value == "") {
		alert("Error. IP address is empty.");
		field.value = field.defaultValue;
		field.focus();
		return false;
	}

	if (isAllNum(field.value) == 0) {
		alert('It should be a [0-9] number.');
		field.value = field.defaultValue;
		field.focus();
		return false;
	}

	if (ismask) {
		if ((!checkRange(field.value, 1, 0, 256)) ||
				(!checkRange(field.value, 2, 0, 256)) ||
				(!checkRange(field.value, 3, 0, 256)) ||
				(!checkRange(field.value, 4, 0, 256)))
		{
			alert('IP adress format error.');
			field.value = field.defaultValue;
			field.focus();
			return false;
		}
	}
	else {
		if ((!checkRange(field.value, 1, 0, 255)) ||
				(!checkRange(field.value, 2, 0, 255)) ||
				(!checkRange(field.value, 3, 0, 255)) ||
				(!checkRange(field.value, 4, 1, 254)))
		{
			alert('IP adress format error.');
			field.value = field.defaultValue;
			field.focus();
			return false;
		}
	}
	return true;
}

function CheckTimeValue(field, min, max)
{
    var time;
    
    if (field.value == "") {
		alert("Error. time is empty.");
		field.focus();
		return false;
	}

	if (isAllNum(field.value) == 0) {
		alert("It should be a [0-9] number.");
		field.focus();
		return false;
	}

    time = parseInt(field.value, 10);
    if((time < min) || (time > max))
    {
        alert("Out of range.");
        return false;
    }

    return true;
}

function CheckTimePairsValue(fieldHour0, fieldMin0, fieldHour1, fieldMin1)
{
    var time0, time1;
    if(!CheckTimeValue(fieldHour0, 0, 23))
    {
        return false;
    }

    if(!CheckTimeValue(fieldMin0, 0, 59))
    {
        return false;
    }

    if(!CheckTimeValue(fieldHour1, 0, 23))
    {
        return false;
    }

    if(!CheckTimeValue(fieldMin1, 0, 59))
    {
        return false;
    }

    time0 = parseInt(fieldHour0.value);
    time1 = parseInt(fieldHour1.value);
    if(time0 > time1)
    {
        alert("start time must less than end time");
        return false;
    }
    else if(time0 == time1)
    {
        time0 = parseInt(fieldMin0.value);
        time1 = parseInt(fieldMin1.value);

        if(time0 >= time1)
        {
            alert("start time must less than end time");
            return false;
        }
    }

    return true;
}

function CheckDotValue()
{
    for(var i = 0; i < document.wanCfg.timeperiod.length; i++)
    {
        if(document.wanCfg.timeperiod[i].checked == true)
        {
            return CheckTimePairsValue(document.getElementById("tpsh"+i),
            document.getElementById("tpsm"+i),
            document.getElementById("tpeh"+i),
            document.getElementById("tpem"+i));
        }
    }

    alert("Please specify a time period");
    return false;
}

function CheckMSPStringValue(field)
{
    var str = field.value;

    for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) != '!') && (str.charAt(i) != '|' ))
			continue;
        alert("Contain invalid character");
        field.value = field.defaultValue;
		field.focus();
		return false;
	}

	return true;
}

function CheckMSPValue()
{
	var numStr = document.CurOperators.totalMSP.value;
	var numint = parseInt(numStr, 10) - 1;

	if (document.Operators.G3Comment.value == "")
	{
		alert("the name of Mobile MSP is empty!");
		return false;
	}

    if(!CheckMSPStringValue(document.Operators.G3Comment))
    {     
        return false;
    }

    if(!CheckMSPStringValue(document.Operators.DialingStr))
    {   
        return false;
    }

    if(!CheckMSPStringValue(document.Operators.InitCmdStr))
    {   
        return false;
    }

    if(!CheckMSPStringValue(document.Operators.G3UserName))
    {   
        return false;
    }

    if(!CheckMSPStringValue(document.Operators.G3Password))
    {   
        return false;
    }
    if(!CheckMSPStringValue(document.Operators.G3LocalIp))
    {   
        return false;
    }

	for (var i=0; i<numint; i++) {
		var curStrId = "Name"+i;

		//if((document.getElementById(curStrId).innerHTML == document.Operators.G3Comment.value))
		//{
		//	alert("a MSP with this name exist!");
		//	return false;
		//}
	}
	
	return true;
}

function CheckValue()
{
	if (document.wanCfg.connectionType.selectedIndex == 0) {      //STATIC
		if (!checkIpAddr(document.wanCfg.staticIp, false))
			return false;
		if (!checkIpAddr(document.wanCfg.staticNetmask, true))
			return false;
		if (document.wanCfg.staticGateway.value != "")
			if (!checkIpAddr(document.wanCfg.staticGateway, false))
				return false;
		if (document.wanCfg.staticPriDns.value != "")
			if (!checkIpAddr(document.wanCfg.staticPriDns, false))
				return false;
		if (document.wanCfg.staticSecDns.value != "")
			if (!checkIpAddr(document.wanCfg.staticSecDns, false))
				return false;
	}
	else if (document.wanCfg.connectionType.selectedIndex == 1) { //DHCP
	}
	else if (document.wanCfg.connectionType.selectedIndex == 2) { //PPPOE
		if (document.wanCfg.pppoePass.value != document.wanCfg.pppoePass2.value) {
			alert("Password mismatched!");
			return false;
		}
		if (document.wanCfg.pppoeOPMode.options.selectedIndex == 0)
		{
			if (document.wanCfg.pppoeRedialPeriod.value == "")
			{
				alert("Please specify Redial Period");
				document.wanCfg.pppoeRedialPeriod.focus();
				document.wanCfg.pppoeRedialPeriod.select();
				return false;
			}
		}
		else if (document.wanCfg.pppoeOPMode.options.selectedIndex == 1);
		{
			if (document.wanCfg.pppoeIdleTime.value == "")
			{
				alert("Please specify Idle Time");
				document.wanCfg.pppoeIdleTime.focus();
				document.wanCfg.pppoeIdleTime.select();
				return false;
			}
		}
		if((document.wanCfg.pppoe_mtu.value != "")&&(!isAllNum(document.wanCfg.pppoe_mtu.value))){
			document.wanCfg.pppoe_mtu.focus();
			return false;	
		}
	}
	else if (document.wanCfg.connectionType.selectedIndex == 3) { //L2TP
		if (document.wanCfg.l2tpOPMode.options.selectedIndex == 0)
		{
			if (document.wanCfg.l2tpRedialPeriod.value == "")
			{
				alert("Please specify Redial Period");
				document.wanCfg.l2tpRedialPeriod.focus();
				document.wanCfg.l2tpRedialPeriod.select();
				return false;
			}
		}
		/*
		else if (document.wanCfg.l2tpOPMode.options.selectedIndex == 1)
		{
			if (document.wanCfg.l2tpIdleTime.value == "")
			{
				alert("Please specify Idle Time");
				document.wanCfg.l2tpIdleTime.focus();
				document.wanCfg.l2tpIdleTime.select();
				return false;
			}
		}
		*/
	}
	else if (document.wanCfg.connectionType.selectedIndex == 4) { //PPTP
		if (document.wanCfg.pptpPass.value != document.wanCfg.pptpPass2.value) {
			alert("Password mismatched!");
			return false;
		}
		if (!checkIpAddr(document.wanCfg.pptpServer, false))
			return false;
		if (document.wanCfg.pptpMode.selectedIndex == 0) {
			if (!checkIpAddr(document.wanCfg.pptpIp, false))
				return false;
			if (!checkIpAddr(document.wanCfg.pptpNetmask, true))
				return false;
			if (!checkIpAddr(document.wanCfg.pptpGateway, false))
				return false;
		}
		if (document.wanCfg.pptpOPMode.options.selectedIndex == 0)
		{
			if (document.wanCfg.pptpRedialPeriod.value == "")
			{
				alert("Please specify Redial Period");
				document.wanCfg.pptpRedialPeriod.focus();
				document.wanCfg.pptpRedialPeriod.select();
				return false;
			}
		}
		/*
		else if(document.wanCfg.pptpOPMode.options.selectedIndex == 1)
		{
			if (document.wanCfg.pptpIdleTime.value == "")
			{
				alert("Please specify Idle Time");
				document.wanCfg.pptpIdleTime.focus();
				document.wanCfg.pptpIdleTime.select();
				return false;
			}
		}
		*/
	}
	else if (document.wanCfg.connectionType.selectedIndex == 5) { //G3
        var opmode = document.wanCfg.G3Mode.options.selectedIndex; 
        if(opmode == 1) 
        {
            if(!CheckTimeValue(document.wanCfg.G3IdleTime.value, 0, 59))
            {
                return false;
            }
        }
        else if(opmode == 2)
        {
    		if(!CheckDotValue())
    		{
                return false;
    		}
        }
	}
	else
		return false;



	
	if((document.wanCfg.cell_mtu.value != "")&&(!isAllNum(document.wanCfg.cell_mtu.value))){
		document.wanCfg.cell_mtu.focus();
		return false;	
	}
	if (document.wanCfg.macCloneEnbl.options.selectedIndex == 1) {
		var re = /[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}/;
		if (document.wanCfg.macCloneMac.value.length == 0) {
			alert("MAC Address should not be empty!");
			document.wanCfg.macCloneMac.focus();
			return false;
		}
		if (!re.test(document.wanCfg.macCloneMac.value)) {
			alert("Please fill the MAC Address in correct format! (XX:XX:XX:XX:XX:XX)");
			document.wanCfg.macCloneMac.focus();
			return false;
		}
	}
	return true;
}

function initTranslation()
{
	var e = document.getElementById("wTitle");
	e.innerHTML = _("wan title");
	e = document.getElementById("wIntroduction");
	e.innerHTML = _("wan introduction");

	e = document.getElementById("wConnectionType");
	e.innerHTML = _("wan connection type");
	e = document.getElementById("wConnTypeStatic");
	e.innerHTML = _("wan connection type static");
	e = document.getElementById("wConnTypeDhcp");
	e.innerHTML = _("wan connection type dhcp");
	e = document.getElementById("wConnTypePppoe");
	e.innerHTML = _("wan connection type pppoe");
	//e = document.getElementById("wConnTypeL2tp");
	//e.innerHTML = _("wan connection type l2tp");
	//e = document.getElementById("wConnTypePptp");
	//e.innerHTML = _("wan connection type pptp");

	e = document.getElementById("wStaticMode");
	e.innerHTML = _("wan static mode");
	e = document.getElementById("wStaticIp");
	e.innerHTML = _("inet ip");
	e = document.getElementById("wStaticNetmask");
	e.innerHTML = _("inet netmask");
	e = document.getElementById("wStaticGateway");
	e.innerHTML = _("inet gateway");
	e = document.getElementById("wStaticPriDns");
	e.innerHTML = _("inet pri dns");
	e = document.getElementById("wStaticSecDns");
	e.innerHTML = _("inet sec dns");

	e = document.getElementById("wDhcpMode");
	e.innerHTML = _("wan dhcp mode");
	e = document.getElementById("wDhcpHost");
	e.innerHTML = _("inet hostname");

	e = document.getElementById("wPppoeMode");
	e.innerHTML = _("wan pppoe mode");
	e = document.getElementById("wPppoeUser");
	e.innerHTML = _("inet user");
	e = document.getElementById("wPppoePassword");
	e.innerHTML = _("inet password");
	e = document.getElementById("wPppoePass2");
	e.innerHTML = _("inet pass2");
	e = document.getElementById("wPppoeOPMode");
	e.innerHTML = _("wan protocol opmode");
	e = document.getElementById("wPppoeKeepAlive");
	e.innerHTML = _("wan protocol opmode keepalive");
	e = document.getElementById("wPppoeOnDemand");
	e.innerHTML = _("wan protocol opmode ondemand");
	e = document.getElementById("wPppoeManual");
	e.innerHTML = _("wan protocol opmode manual");

	e = document.getElementById("wL2tpMode");
	e.innerHTML = _("wan l2tp mode");
	e = document.getElementById("wL2tpServer");
	e.innerHTML = _("inet server");
	e = document.getElementById("wL2tpUser");
	e.innerHTML = _("inet user");
	e = document.getElementById("wL2tpPassword");
	e.innerHTML = _("inet password");
	e = document.getElementById("wL2tpAddrMode");
	e.innerHTML = _("wan address mode");
	e = document.getElementById("wL2tpAddrModeS");
	e.innerHTML = _("wan address mode static");
	e = document.getElementById("wL2tpAddrModeD");
	e.innerHTML = _("wan address mode dynamic");
	e = document.getElementById("wL2tpIp");
	e.innerHTML = _("inet ip");
	e = document.getElementById("wL2tpNetmask");
	e.innerHTML = _("inet netmask");
	e = document.getElementById("wL2tpGateway");
	e.innerHTML = _("inet gateway");
	e = document.getElementById("wL2tpOPMode");
	e.innerHTML = _("wan protocol opmode");
	e = document.getElementById("wL2tpKeepAlive");
	e.innerHTML = _("wan protocol opmode keepalive");
	/*
	e = document.getElementById("wL2tpOnDemand");
	e.innerHTML = _("wan protocol opmode ondemand");
	*/
	e = document.getElementById("wL2tpManual");
	e.innerHTML = _("wan protocol opmode manual");

	e = document.getElementById("wPptpMode");
	e.innerHTML = _("wan pptp mode");
	e = document.getElementById("wPptpServer");
	e.innerHTML = _("inet server");
	e = document.getElementById("wPptpUser");
	e.innerHTML = _("inet user");
	e = document.getElementById("wPptpPassword");
	e.innerHTML = _("inet password");
	e = document.getElementById("wPptpAddrMode");
	e.innerHTML = _("wan address mode");
	e = document.getElementById("wPptpAddrModeS");
	e.innerHTML = _("wan address mode static");
	e = document.getElementById("wPptpAddrModeD");
	e.innerHTML = _("wan address mode dynamic");
	e = document.getElementById("wPptpIp");
	e.innerHTML = _("inet ip");
	e = document.getElementById("wPptpNetmask");
	e.innerHTML = _("inet netmask");
	e = document.getElementById("wPptpGateway");
	e.innerHTML = _("inet gateway");
	e = document.getElementById("wPptpOPMode");
	e.innerHTML = _("wan protocol opmode");
	e = document.getElementById("wPptpKeepAlive");
	e.innerHTML = _("wan protocol opmode keepalive");
	/*
	e = document.getElementById("wPptpOnDemand");
	e.innerHTML = _("wan protocol opmode ondemand");
	*/
	e = document.getElementById("wPptpManual");
	e.innerHTML = _("wan protocol opmode manual");

	e = document.getElementById("w3GMode");
	e.innerHTML = _("wan 3G mode");
	e = document.getElementById("w3GDev");
	e.innerHTML = _("wan 3G modem");
    e = document.getElementById("wG3Pin");
	e.innerHTML = _("wan 3G PIN");
    e = document.getElementById("G3OPMode");
	e.innerHTML = _("wan 3G opmode");
	e = document.getElementById("G3AolMode");
	e.innerHTML = _("wan protocol opmode keepalive");
	e = document.getElementById("G3DodMode");
	e.innerHTML = _("wan protocol opmode ondemand");
	e = document.getElementById("G3DotMode");
	e.innerHTML = _("wan protocol opmode ontime");
	e = document.getElementById("G3ManualMode");
	e.innerHTML = _("wan protocol opmode manual");
    e = document.getElementById("MobileOperators");
	e.innerHTML = _("wan 3G mspparam");
    e = document.getElementById("G3Comment");
	e.innerHTML = _("wan 3G mspname");
    e = document.getElementById("NetworkType");
	e.innerHTML = _("wan 3G nettype");
    e = document.getElementById("DialingString");
	e.innerHTML = _("wan 3G dialnum");
    e = document.getElementById("InitCmdString");
	e.innerHTML = _("wan 3G cmdstr");
    e = document.getElementById("G3UserName");
	e.innerHTML = _("wan 3G username");
    e = document.getElementById("G3Password");
	e.innerHTML = _("wan 3G password");
    e = document.getElementById("G3LocalIp");
	e.innerHTML = _("wan 3G localip");
	e = document.getElementById("CurG3Comment");
	e.innerHTML = _("wan 3G mspname");
    e = document.getElementById("G3CurOperation");
	e.innerHTML = _("wan 3G mspopr");
    e = document.getElementById("CurDialingString");
	e.innerHTML = _("wan 3G dialnum");
    e = document.getElementById("CurInitCmdString");
	e.innerHTML = _("wan 3G cmdstr");
    e = document.getElementById("CurG3UserName");
	e.innerHTML = _("wan 3G username");
    e = document.getElementById("CurG3Password");
	e.innerHTML = _("wan 3G password");
    e = document.getElementById("CurG3LocalIp");
	e.innerHTML = _("wan 3G localip");
    e = document.getElementById("CurOperatorsInSys");
	e.innerHTML = _("wan 3G curMSPs");
	e = document.getElementById("id_G3AuthProtocol");
	e.innerHTML = _("wan 3G AuthType");
    e = document.getElementById("G3OtherOptions");
	e.innerHTML = _("wan 3G OtherOptions");
	e = document.getElementById("G3UseSWComp");
	e.innerHTML = _("wan 3G UseComp");
	e = document.getElementById("AddMSP2List");
	e.value = _("wan 3G AddMSP");
	e = document.getElementById("SelAsDftMSP");
	e.value = _("wan 3G SelDft");
    e = document.getElementById("G3CurOperation");
	e.innerHTML = _("wan 3G mspdel");
    
	e = document.getElementById("wMacClone");
	e.innerHTML = _("wan mac clone");
	e = document.getElementById("wMacCloneD");
	e.innerHTML = _("inet disable");
	e = document.getElementById("wMacCloneE");
	e.innerHTML = _("inet enable");
	e = document.getElementById("wMacCloneAddr");
	e.innerHTML = _("inet mac");
	
	e = document.getElementById("exam");
	e.innerHTML = _("wan exam");
	e = document.getElementById("connNode");
	e.innerHTML = _("wan connNode");	
	e = document.getElementById("firAlive");
	e.innerHTML = _("wan firAlive");	
	e = document.getElementById("secDmd");
	e.innerHTML = _("wan secDmd");	
	e = document.getElementById("thdTime");
	e.innerHTML = _("wan thdTime");
	e = document.getElementById("idT");
	e.innerHTML = _("wan idT");
	e = document.getElementById("idTm");
	e.innerHTML = _("wan idTm");

	e = document.getElementById("wan_direc1");
	e.innerHTML = _("wan wan_direc1");
	e = document.getElementById("wan_direc2");
	e.innerHTML = _("wan wan_direc2");
	e = document.getElementById("help_head");
	e.innerHTML = _("help help_head");

	e = document.getElementById("wApply");
	e.value = _("inet apply");
	e = document.getElementById("wCancel");
	e.value = _("inet cancel");
	
	e = document.getElementById("resetbutton");
	e.value = _("wan resetbutton");
	e = document.getElementById("refreshbutton");
	e.value = _("refresh button");    
	
	e = document.getElementById("ppoe_keep");
	e.innerHTML = _("ppoe ppoe_keep");
	
	e = document.getElementById("ppoe_sec");
	e.innerHTML = _("ppoe ppoe_sec");
	
	e = document.getElementById("ppoe_demand");
	e.innerHTML = _("ppoe ppoe_demand");
	
	e = document.getElementById("ppoe_min");
	e.innerHTML = _("ppoe ppoe_min");
}

function initValue()
{
	var mode = "<% getCfgGeneral(1, "wanConnectionMode"); %>";
	var pptpMode = <% getCfgZero(1, "wan_pptp_mode"); %>;
	var clone = <% getCfgZero(1, "macCloneEnabled"); %>;

	initTranslation();
	if (mode == "STATIC") {
		document.wanCfg.connectionType.options.selectedIndex = 0;
	}
	else if (mode == "DHCP") {
		document.wanCfg.connectionType.options.selectedIndex = 1;
	}
	else if (mode == "PPPOE") {
		var pppoe_opmode = "<% getCfgGeneral(1, "wan_pppoe_opmode"); %>";
		var pppoe_optime = "<% getCfgGeneral(1, "wan_pppoe_optime"); %>";
		var pppoe_mtu_temp = "<% getCfgGeneral(1, "pppoe_mtu"); %>";

		document.wanCfg.connectionType.options.selectedIndex = 2;
		if (pppoe_opmode == "Manual")
		{
			document.wanCfg.pppoeOPMode.options.selectedIndex = 2;
		}
		else if (pppoe_opmode == "OnDemand")
		{
			document.wanCfg.pppoeOPMode.options.selectedIndex = 1;
			if (pppoe_optime != "")
				document.wanCfg.pppoeIdleTime.value = pppoe_optime;
		}
		else if (pppoe_opmode == "KeepAlive")
		{
			document.wanCfg.pppoeOPMode.options.selectedIndex = 0;
			if (pppoe_optime != "")
				document.wanCfg.pppoeRedialPeriod.value = pppoe_optime;
		}

		document.wanCfg.pppoe_mtu.value = pppoe_mtu_temp;
		pppoeOPModeSwitch();
	}
	else if (mode == "L2TP") {
		var l2tp_opmode = "<% getCfgGeneral(1, "wan_l2tp_opmode"); %>";
		var l2tp_optime = "<% getCfgGeneral(1, "wan_l2tp_optime"); %>";
		
		document.wanCfg.connectionType.options.selectedIndex = 3;
		if (l2tp_opmode == "Manual")
		{
			document.wanCfg.l2tpOPMode.options.selectedIndex = 2;
		}
		/*
		else if (l2tp_opmode == "OnDemand")
		{
			document.wanCfg.l2tpOPMode.options.selectedIndex = 1;
			if (l2tp_optime != "")
				document.wanCfg.l2tpIdleTime.value = l2tp_optime;
		}
		*/
		else if (l2tp_opmode == "KeepAlive")
		{
			document.wanCfg.l2tpOPMode.options.selectedIndex = 0;
			if (l2tp_optime != "")
				document.wanCfg.l2tpRedialPeriod.value = l2tp_optime;
		}
		l2tpOPModeSwitch();
	}
	else if (mode == "PPTP") {
		var pptp_opmode = "<% getCfgGeneral(1, "wan_pptp_opmode"); %>";
		var pptp_optime = "<% getCfgGeneral(1, "wan_pptp_optime"); %>";

		document.wanCfg.connectionType.options.selectedIndex = 4;
		document.wanCfg.pptpMode.options.selectedIndex = 1*pptpMode;
		pptpModeSwitch();
		if (pptp_opmode == "Manual")
		{
			document.wanCfg.pptpOPMode.options.selectedIndex = 2;
			if (pptp_optime != "")
				document.wanCfg.pptpIdleTime.value = pptp_optime;
		}
		/*
		else if (pptp_opmode == "OnDemand")
		{
			document.wanCfg.pptpOPMode.options.selectedIndex = 1;
			if (pptp_optime != "")
				document.wanCfg.pptpIdleTime.value = pptp_optime;
		}
		*/
		else if (pptp_opmode == "KeepAlive")
		{
			document.wanCfg.pptpOPMode.options.selectedIndex = 0;
			if (pptp_optime != "")
				document.wanCfg.pptpRedialPeriod.value = pptp_optime;
		}
		pptpOPModeSwitch();
	}
	else if (mode == "3G") {
		var dev_3g = "<% getCfgGeneral(1, "wan_3g_dev"); %>";
		var g3_opmode = "<% getCfgGeneral(1, "wan_3g_opmode"); %>";
		var g3_optime = "<% getCfgGeneral(1, "g3idletime"); %>";
		var g3_network = "<% getCfgGeneral(1, "g3_network_type"); %>";
		var cell_mtu_temp = "<% getCfgGeneral(1, "cell_mtu"); %>";
        var g3_operator = "<% getCfgGeneral(1, "g3_operator_name"); %>"; 
        var g3_dotsel = <% getCfgGeneral(1, "DOT_select_index"); %>;
              
		document.wanCfg.connectionType.options.selectedIndex = 5;

		if (dev_3g == "HUAWEI-EM560")
			document.wanCfg.Dev3G.options.selectedIndex = 0;
		else if (dev_3g == "HUAWEI-EM660")
			document.wanCfg.Dev3G.options.selectedIndex = 1;
		else if (dev_3g == "HUAWEI-EM770")
			document.wanCfg.Dev3G.options.selectedIndex = 2;
		
		else if (dev_3g == "SYNCWISER-801/401")
			document.wanCfg.Dev3G.options.selectedIndex = 3;

		else if (dev_3g == "LONGSUNG-U6300/U5300")
			document.wanCfg.Dev3G.options.selectedIndex = 4;
	
		else if (dev_3g == "MC5728")
			document.wanCfg.Dev3G.options.selectedIndex = 5;
		else if (dev_3g == "F3607gw")
			document.wanCfg.Dev3G.options.selectedIndex = 6;
		else if (dev_3g == "ZTE-MF210V")
			document.wanCfg.Dev3G.options.selectedIndex = 7;	
    else if (dev_3g == "SIERRA-MC8785")
    	document.wanCfg.Dev3G.options.selectedIndex = 8;
    else if (dev_3g == "AD3812")
    	document.wanCfg.Dev3G.options.selectedIndex = 9;
		else
			document.wanCfg.Dev3G.options.selectedIndex = 1;


			
	document.wanCfg.cell_mtu.value = cell_mtu_temp;
        if(g3_opmode == "G3ModeDot")
        {
        	document.wanCfg.G3Mode.options.selectedIndex = 2;    
            
        	if((g3_dotsel >= 0) && (g3_dotsel < 4))
        	{
                for(var i = 0; i < 4; i++)
                {
                    if(i == g3_dotsel)
        		        document.wanCfg.timeperiod[i].checked=true;
                    else
                        document.wanCfg.timeperiod[i].checked=false;
                }
        	}
        }
		else if (g3_opmode == "G3ModeManual")
		{
			document.wanCfg.G3Mode.options.selectedIndex = 3;			
		}
		
		else if (g3_opmode == "G3ModeDod")
		{
			document.wanCfg.G3Mode.options.selectedIndex = 1;
			if (g3_optime != "")
				document.wanCfg.G3IdleTime.value = g3_optime;
		}
		else if (g3_opmode == "G3ModeAol")
		{
			document.wanCfg.G3Mode.options.selectedIndex = 0;			
		}

		G3OPModeSwitch();
		
		if (g3_network == "0")
		{
			document.Operators.g3NetworkType.options.selectedIndex = 0;
		}
		else if (g3_network == "1")
		{
			document.Operators.g3NetworkType.options.selectedIndex = 1;
		}
		else if (g3_network == "2")
		{
			document.Operators.g3NetworkType.options.selectedIndex = 2;
		}
		else
		{
			document.Operators.g3NetworkType.options.selectedIndex = 0;
		}

	}
	else {
		document.wanCfg.connectionType.options.selectedIndex = 0;
	}
	if(mode=="STATIC"){
		document.wanCfg.connectionType.options[0].selected=true;
		connectionTypeSwitch();
		showSubmit();
		}
	else if(mode=="DHCP"){
		document.wanCfg.connectionType.options[1].selected=true;
		connectionTypeSwitch();
		showSubmit();
		}
	else if(mode=="PPPOE"){
		document.wanCfg.connectionType.options[2].selected=true;
		connectionTypeSwitch();
		showSubmit();
		}
	else if(mode=="3G"){
		document.wanCfg.connectionType.options[3].selected=true;
		connectionTypeSwitch();
		hideSubmit();
		}
	else {
		document.wanCfg.connectionType.options[3].selected=true;
		connectionTypeSwitch();
		}

	if (clone == 1)
		document.wanCfg.macCloneEnbl.options.selectedIndex = 1;
	else
		document.wanCfg.macCloneEnbl.options.selectedIndex = 0;
	macCloneSwitch();
	rmebSelOpt();
	rmbCurrMode();
	rmbCurrRadioTime();
	rmbCurrOnDmd();
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

<h1 id="wTitle"></h1>
<p style="display:none" id="wIntroduction"></p>
<hr />

<form method=post name="wanCfg" action="/goform/setWan" onSubmit="return CheckValue()">
<table width="540" cellpadding="2" cellspacing="1">
<tr align="center">
  <td><b id="wConnectionType"></b>&nbsp;&nbsp;&nbsp;&nbsp;</td>
  <td>
    <select name="connectionType" size="1"  onChange="connectionTypeSwitch();">
      <option value="STATIC" id="wConnTypeStatic">Static Mode (fixed IP)</option>
      <option value="DHCP" id="wConnTypeDhcp">DHCP (Auto Config)</option>
      <option value="PPPOE" id="wConnTypePppoe">PPPOE (ADSL)</option>
     <!-- <option value="L2TP" style="display:none" id="wConnTypeL2tp">L2TP</option>
      <option value="PPTP" style="display:none" id="wConnTypePptp">PPTP</option>-->
      <option value="3G" id="wConnType3G">Mobile Modem</option> 
    </select>
  </td>
</tr>
</table>

<!-- ================= STATIC Mode ================= -->
<table id="static" width="540" border="0" cellpadding="2" cellspacing="1">
<tr>
  <td class="title" colspan="2" id="wStaticMode">Static Mode</td>
</tr>
<tr>
  <td class="head" id="wStaticIp">IP Address</td>
  <td><input name="staticIp" maxlength=15 value="<% getWanIp(); %>"></td>
</tr>
<tr>
  <td class="head" id="wStaticNetmask">Subnet Mask</td>
  <td><input name="staticNetmask" maxlength=15 value="<% getWanNetmask(); %>">
  </td>
</tr>
<tr>
  <td class="head" id="wStaticGateway">Default Gateway</td>
  <td><input name="staticGateway" maxlength=15 value="<% getWanGateway(); %>">
  </td>
</tr>
<tr>
  <td class="head" id="wStaticPriDns">Primary DNS Server</td>
  <td><input name="staticPriDns" maxlength=15 value="<% getDns(1); %>"></td>
</tr>
<tr>
  <td class="head" id="wStaticSecDns">Secondary DNS Server</td>
  <td><input name="staticSecDns" maxlength=15 value="<% getDns(2); %>"></td>
</tr>
</table>

<!-- ================= DHCP Mode ================= -->
<table id="dhcp" width="540" border="0" cellpadding="2" cellspacing="1">
<tr>
  <td class="title" colspan="2" id="wDhcpMode">DHCP Mode</td>
</tr>
<tr>
  <td class="head"><div id="wDhcpHost">Host Name</div> (optional)</td>
  <td><input type=text name="hostname" size=28 maxlength=32 value=""></td>
</tr>
</table>

<!-- ================= PPPOE Mode ================= -->
<table id="pppoe" width="540" border="0" cellpadding="2" cellspacing="1">
<tr>
  <td class="title" colspan="2" id="wPppoeMode">PPPoE Mode</td>
</tr>
<tr>
  <td class="head" id="wPppoeUser">User Name</td>
  <td><input name="pppoeUser" maxlength=32 size=32
             value="<% getCfgGeneral(1, "wan_pppoe_user"); %>"></td>
</tr>
<tr>
  <td class="head" id="wPppoePassword">Password</td>
  <td><input type="password" name="pppoePass" maxlength=32 size=32
             value="<% getCfgGeneral(1, "wan_pppoe_pass"); %>"></td>
</tr>
<tr>
  <td class="head" id="wPppoePass2">Verify Password</td>
  <td><input type="password" name="pppoePass2" maxlength=32 size=32
             value="<% getCfgGeneral(1, "wan_pppoe_pass"); %>"></td>
</tr>
<tr>
  <td class="head"  id="id_pppoe_mtu">MTU</td>
  <td>
   <input type="text" name="pppoe_mtu" maxlength="4" size="4">
  </td>
</tr>
<tr>
  <td class="head" rowspan="2" id="wPppoeOPMode">Operation Mode</td>
  <td>
    <select name="pppoeOPMode" size="1" onChange="pppoeOPModeSwitch()">
      <option value="KeepAlive" id="wPppoeKeepAlive">Keep Alive</option>
      <option value="OnDemand" id="wPppoeOnDemand">On Demand</option>
      <option value="Manual" id="wPppoeManual">Manual</option>
    </select>
  </td>
</tr>
<tr>
  <td>
    <span id="ppoe_keep">Keep Alive Mode: Redial Period</span>
    <input type="text" name="pppoeRedialPeriod" maxlength="5" size="3" value="60">
    <span id="ppoe_sec">senconds</span>
    <br />
    <span id="ppoe_demand">On demand Mode:  Idle Time</span>
    <input type="text" name="pppoeIdleTime" maxlength="3" size="2" value="5">
    <span id="ppoe_min">minutes</span>
  </td>
</tr>




</table>

<!-- ================= L2TP Mode ================= -->
<table id="l2tp" width="540" border="0" cellpadding="2" cellspacing="1">
<tr>
  <td class="title" colspan="2" id="wL2tpMode">L2TP Mode</td>
</tr>
<tr>
  <td class="head" id="wL2tpServer">L2TP Server IP Address</td>
  <td><input name="l2tpServer" maxlength="15" size=15 value="<%
       getCfgGeneral(1, "wan_l2tp_server"); %>"></td>
</tr>
<tr>
  <td class="head" id="wL2tpUser">User Name</td>
  <td><input name="l2tpUser" maxlength="20" size=20 value="<%
       getCfgGeneral(1, "wan_l2tp_user"); %>"></td>
</tr>
<tr>
  <td class="head" id="wL2tpPassword">Password</td>
  <td><input type="password" name="l2tpPass" maxlength="32" size=32 value="<%
       getCfgGeneral(1, "wan_l2tp_pass"); %>"></td>
</tr>
<tr>
  <td class="head" id="wL2tpAddrMode">Address Mode</td>
  <td>
    <select name="l2tpMode" size="1" onChange="l2tpModeSwitch()">
      <option value="0" id="wL2tpAddrModeS">Static</option>
      <option value="1" id="wL2tpAddrModeD">Dynamic</option>
    </select>
  </td>
</tr>
<tr id="l2tpIp">
  <td class="head" id="wL2tpIp">IP Address</td>
  <td><input name="l2tpIp" maxlength=15 size=15 value="<% getCfgGeneral(1, "wan_l2tp_ip"); %>"></td>
</tr>
<tr id="l2tpNetmask">
  <td class="head" id="wL2tpNetmask">Subnet Mask</td>
  <td><input name="l2tpNetmask" maxlength=15 size=15 value="<% getCfgGeneral(1, "wan_l2tp_netmask"); %>">
  </td>
</tr>
<tr id="l2tpGateway">
  <td class="head" id="wL2tpGateway">Default Gateway</td>
  <td><input name="l2tpGateway" maxlength=15 size=15 value="<% getCfgGeneral(1, "wan_l2tp_gateway"); %>">
  </td>
</tr>
<tr>
  <td class="head" rowspan="3" id="wL2tpOPMode">Operation Mode</td>
  <td>
    <select name="l2tpOPMode" size="1" onChange="l2tpOPModeSwitch()">
      <option value="KeepAlive" id="wL2tpKeepAlive">Keep Alive</option>
      <!--
      <option value="OnDemand" id="wL2tpOnDemand">On Demand</option>
      -->
      <option value="Manual" id="wL2tpManual">Manual</option>
    </select>
  </td>
</tr>
<tr>
  <td>
    Keep Alive Mode: Redial Period
    <input type="text" name="l2tpRedialPeriod" maxlength="5" size="3" value="60">
    senconds
    <!--
    <br />
    On demand Mode:  Idle Time
    <input type="text" name="l2tpIdleTime" maxlength="3" size="2" value="5">
    minutes
    -->
  </td>
</tr>
</table>

<!-- ================= PPTP Mode ================= -->
<table id="pptp" width="540" border="0" cellpadding="2" cellspacing="1">
<tr>
  <td class="title" colspan="2" id="wPptpMode">PPTP Mode</td>
</tr>
<tr>
  <td class="head" id="wPptpServer">PPTP Server IP Address</td>
  <td><input name="pptpServer" maxlength="15" size=15 value="<%
       getCfgGeneral(1, "wan_pptp_server"); %>"></td>
</tr>
<tr>
  <td class="head" id="wPptpUser">User Name</td>
  <td><input name="pptpUser" maxlength="20" size=20 value="<%
       getCfgGeneral(1, "wan_pptp_user"); %>"></td>
</tr>
<tr>
  <td class="head" id="wPptpPassword">Password</td>
  <td><input type="password" name="pptpPass" maxlength="32" size=32 value="<%
       getCfgGeneral(1, "wan_pptp_pass"); %>"></td>
</tr>
<tr>
  <td class="head" id="wPptpAddrMode">Address Mode</td>
  <td>
    <select name="pptpMode" size="1" onChange="pptpModeSwitch()">
      <option value="0" id="wPptpAddrModeS">Static</option>
      <option value="1" id="wPptpAddrModeD">Dynamic</option>
    </select>
  </td>
</tr>
<tr id="pptpIp">
  <td class="head" id="wPptpIp">IP Address</td>
  <td><input name="pptpIp" maxlength=15 size=15 value="<% getCfgGeneral(1, "wan_pptp_ip"); %>"></td>
</tr>
<tr id="pptpNetmask">
  <td class="head" id="wPptpNetmask">Subnet Mask</td>
  <td><input name="pptpNetmask" maxlength=15 size=15 value="<% getCfgGeneral(1, "wan_pptp_netmask"); %>">
  </td>
</tr>
<tr id="pptpGateway">
  <td class="head" id="wPptpGateway">Default Gateway</td>
  <td><input name="pptpGateway" maxlength=15 size=15 value="<% getCfgGeneral(1, "wan_pptp_gateway"); %>">
  </td>
</tr>
<tr>
  <td class="head" rowspan="3" id="wPptpOPMode">Operation Mode</td>
  <td>
    <select name="pptpOPMode" size="1" onChange="pptpOPModeSwitch()">
      <option value="KeepAlive" id="wPptpKeepAlive">Keep Alive</option>
      <!--
      <option value="OnDemand" id="wPptpOnDemand">On Demand</option>
      -->
      <option value="Manual" id="wPptpManual">Manual</option>
    </select>
  </td>
</tr>
<tr>
  <td>
    Keep Alive Mode: Redial Period
    <input type="text" name="pptpRedialPeriod" maxlength="5" size="3" value="60">
    senconds
    <!--
    <br />
    On demand Mode:  Idle Time
    <input type="text" name="pptpIdleTime" maxlength="3" size="2" value="5">
    minutes
    -->
  </td>
</tr>
</table>

<!-- =========== G3 Modular =========== -->
<table id="3G" width="540" border="0" cellpadding="2" cellspacing="1">
<tr>
  <td class="title"  colspan="2" id="w3GMode">3G Mode</td>
</tr>
<tr >
  <td width="218px" rowspan="1" id="w3GDev">USB 3G modem</td>
  <td width="322px" >
    <select name="Dev3G" size="1" style="width=150">
      <option value="HUAWEI-EM560" id="EM560">HUAWEI-EM560</option>
      <option value="HUAWEI-EM660" id="EM660">HUAWEI-EM660</option>
      <option value="HUAWEI-EM770" id="EM770">HUAWEI-EM770</option>
      <option value="SYNCWISER-801/401" id="A801_A401">SYNCWISER-801/401</option>       
       <option value="LONGSUNG-U6300/U5300" id="U6300_U5300">LONGSUNG-U6300/U5300</option>         
       <option value="MC5728" id="ID_MC5728">SIERRA-MC5728</option>   
       <option value="F3607gw" id="ID_SIMCOM_SIM700">Ericsson F3607gw</option>       
       <option value="ZTE-MF210V" id="id_zte_mf210v">ZTE-MF210</option>   	
       <option value="SIERRA-MC8785" id="id_gtm681w">SIERRA-MC8785/8790</option> 
       <option value="AD3812" id="id_Serria">ZTE AD3812</option>  
    </select>
  </td>
</tr>
<tr>
  <td  id="wG3Pin">PIN</td>
  <td><input type="text" maxlength=32 size=32 name="simPIN" value="<% getCfgGeneral(1, "g3_sim_pin"); %>"></td>  
</tr>
<tr>
  <td   id="id_cell_mtu" >MTU</td>
  <td >
   <input type="text" name="cell_mtu" maxlength="4" size="4">
  </td>
</tr>
<tr style="display:none">
	<td style="vertical-align: top"  id="G3OPMode" rowspan="3">
		Mode
	</td>
	<td>
		<select name="G3Mode" size="1" onChange="G3OPModeSwitch()" style="width=150">
   		<option value="G3ModeAol" id="G3AolMode">always on line</option>
		<option value="G3ModeDod" id="G3DodMode">dial on demand</option>
		<option value="G3ModeDot" id="G3DotMode">dial on time</option>
   		<option style="display:none" value="G3ModeManual" id="G3ManualMode">manual</option>
   		</select>&nbsp;&nbsp;
   		<% showMuanalBtnASP(); %>
	</td>
</tr>
<tr>
  <td id="G3TimeOptions">
    <!--
    Keep Alive Mode: Redial Period
    <input type="text" name="G3RedialPeriod" maxlength="5" size="3" value="60">
    senconds
    <br />
    On demand Mode:  -->Idle Time
    <input type="text" name="G3IdleTime" maxlength="3" size="2" value="5">
    minutes
  </td>
  <td id="G3TimePeriodsTable" name="G3TimePeriods">
  	example:15:50---22:30 ,set NTP Server in management page before used.
	<br />
    <br />
    <input type="radio" name="timeperiod" value="0">&nbsp;&nbsp;
    <input type="text" name="timeperiod0StartHour" id="tpsh0" maxlength="2" size="1" value="<% getCfgGeneral(1, "tp_sh_0"); %>">:
    <input type="text" name="timeperiod0StartMin" id="tpsm0" maxlength="2" size="1" value="<% getCfgGeneral(1, "tp_sm_0"); %>">---
    <input type="text" name="timeperiod0EndHour" id="tpeh0" maxlength="2" size="1" value="<% getCfgGeneral(1, "tp_eh_0"); %>">:
    <input type="text" name="timeperiod0EndMin" id="tpem0" maxlength="2" size="1" value="<% getCfgGeneral(1, "tp_em_0"); %>">
    <br />
    <input type="radio" name="timeperiod" value="1">&nbsp;&nbsp;
    <input type="text" name="timeperiod1StartHour" id="tpsh1" maxlength="2" size="1" value="<% getCfgGeneral(1, "tp_sh_1"); %>">:
    <input type="text" name="timeperiod1StartMin" id="tpsm1" maxlength="2" size="1" value="<% getCfgGeneral(1, "tp_sm_1"); %>">---
    <input type="text" name="timeperiod1EndHour" id="tpeh1" maxlength="2" size="1" value="<% getCfgGeneral(1, "tp_eh_1"); %>">:
    <input type="text" name="timeperiod1EndMin" id="tpem1" maxlength="2" size="1" value="<% getCfgGeneral(1, "tp_em_1"); %>">
    <br />
    <input type="radio" name="timeperiod" value="2">&nbsp;&nbsp;
    <input type="text" name="timeperiod2StartHour" id="tpsh2" maxlength="2" size="1" value="<% getCfgGeneral(1, "tp_sh_2"); %>">:
    <input type="text" name="timeperiod2StartMin" id="tpsm2" maxlength="2" size="1" value="<% getCfgGeneral(1, "tp_sm_2"); %>">---
    <input type="text" name="timeperiod2EndHour" id="tpeh2" maxlength="2" size="1" value="<% getCfgGeneral(1, "tp_eh_2"); %>">:
    <input type="text" name="timeperiod2EndMin" id="tpem2" maxlength="2" size="1" value="<% getCfgGeneral(1, "tp_em_2"); %>">
    <br />
    <input type="radio" name="timeperiod" value="3">&nbsp;&nbsp;
    <input type="text" name="timeperiod3StartHour" id="tpsh3" maxlength="2" size="1" value="<% getCfgGeneral(1, "tp_sh_3"); %>">:
    <input type="text" name="timeperiod3StartMin" id="tpsm3" maxlength="2" size="1" value="<% getCfgGeneral(1, "tp_sm_3"); %>">---
    <input type="text" name="timeperiod3EndHour" id="tpeh3" maxlength="2" size="1" value="<% getCfgGeneral(1, "tp_eh_3"); %>">:
    <input type="text" name="timeperiod3EndMin" id="tpem3" maxlength="2" size="1" value="<% getCfgGeneral(1, "tp_em_3"); %>">
  </td>
</tr>
</table>

<!-- =========== MAC Clone =========== -->
<table width="540" border="0" cellpadding="2" cellspacing="1">
<tr style="display:none">
  <td class="title" colspan="2" id="wMacClone">MAC Address Clone</td>
</tr>
<tr style="display:none">
  <td class="head">Enabled</td>
  <td>
    <select name="macCloneEnbl" size="1" onChange="macCloneSwitch()" style="width=150">
      <option value="0" id="wMacCloneD">Disable</option>
      <option value="1" id="wMacCloneE">Enable</option>
    </select>
  </td>
</tr>
<tr id="macCloneMacRow">
  <td class="head" id="wMacCloneAddr">MAC Address</td>
  <td>
	<input name="macCloneMac" id="macCloneMac" maxlength=17 value="<% getCfgGeneral(1, "macCloneMac"); %>">
	<input type="button" name="macCloneMacFill" id="macCloneMacFill" value="Fill my MAC" onclick="macCloneMacFillSubmit();" >
 </td>
</tr>
</table>

<script type="text/javascript">
	function showSubmit(){
			document.getElementById("showHide").style.display = "block";	
	}
	function hideSubmit(){
			document.getElementById("showHide").style.display = "none";	
	}
</script>
<table id="showHide"  width="540" cellpadding="2" cellspacing="1">
<tr  align="center">
  <td>
    <input type=submit  value="Apply" id="wApply">&nbsp;&nbsp; 
    <input type=reset value="Cancel" id="wCancel" onClick="window.location.reload()">
  </td>
</tr>
</table>
</form>



<script type="text/javascript">
			function SelectMspofTable(name){
				//alert(name);
				var objs = document.getElementsByName("RuleNo");    
				for(var i=0;i<objs.length;i++)   
				{
					//alert(objs[i].value);
					if(objs[i].value == name)
						document.CurOperators.RuleNo[i].click();
				}

			}

			function changeMspName(){
				
				if(document.getElementById("mspName").options[0].selected){
					/*document.getElementById("DialingStrValue").value="DialingStrValue1";
					document.getElementById("InitCmdStrValue").value="InitCmdStrValue1";
					document.getElementById("G3UserNameValue").value="G3UserNameValue1";
					document.getElementById("G3PasswordValue").value="G3PasswordValue1";
					document.getElementById("G3LocalIpValue").value="G3LocalIpValue1";*/
					//document.CurOperators.RuleNo[0].click();
					SelectMspofTable(document.getElementById("mspName").options[0].text);
				}
				else if(document.getElementById("mspName").options[1].selected){
					/*document.getElementById("DialingStrValue").value="DialingStrValue2";
					document.getElementById("InitCmdStrValue").value="InitCmdStrValue2";
					document.getElementById("G3UserNameValue").value="G3UserNameValue2";
					document.getElementById("G3PasswordValue").value="G3PasswordValue2";
					document.getElementById("G3LocalIpValue").value="G3LocalIpValue2";*/
					//document.CurOperators.RuleNo[1].click();
					SelectMspofTable(document.getElementById("mspName").options[1].text);
				}
				else if(document.getElementById("mspName").options[2].selected){
					/*document.getElementById("DialingStrValue").value="DialingStrValue3";
					document.getElementById("InitCmdStrValue").value="InitCmdStrValue3";
					document.getElementById("G3UserNameValue").value="G3UserNameValue3";
					document.getElementById("G3PasswordValue").value="G3PasswordValue3";
					document.getElementById("G3LocalIpValue").value="G3LocalIpValue3";*/
					//document.CurOperators.RuleNo[2].click();
					SelectMspofTable(document.getElementById("mspName").options[2].text);
				}
				else if(document.getElementById("mspName").options[3].selected){
					/*document.getElementById("DialingStrValue").value="DialingStrValue4";
					document.getElementById("InitCmdStrValue").value="InitCmdStrValue4";
					document.getElementById("G3UserNameValue").value="G3UserNameValue4";
					document.getElementById("G3PasswordValue").value="G3PasswordValue4";
					document.getElementById("G3LocalIpValue").value="G3LocalIpValue4";*/
					//document.CurOperators.RuleNo[3].click();
					SelectMspofTable(document.getElementById("mspName").options[3].text);
				}
				else if(document.getElementById("mspName").options[4].selected){
					/*document.getElementById("DialingStrValue").value="DialingStrValue5";
					document.getElementById("InitCmdStrValue").value="InitCmdStrValue5";
					document.getElementById("G3UserNameValue").value="G3UserNameValue5";
					document.getElementById("G3PasswordValue").value="G3PasswordValue5";
					document.getElementById("G3LocalIpValue").value="G3LocalIpValue5";*/
					//document.CurOperators.RuleNo[4].click();
					SelectMspofTable(document.getElementById("mspName").options[4].text);
				}		
			}
			
			function rmebSelOpt(){
				var RuleNo = document.getElementsByName("RuleNo");
				var mspNameSel = document.getElementById("mspName")
				var index = document.getElementById("mspName").selectedIndex;
				for(var i=0;i<RuleNo.length;i++){
					if(RuleNo[i].checked){
							var value = mspNameSel.value = RuleNo[i].value;
							if(value=="WCDMA") index = 0
							else if(value=="EVDO") index=1
							else if(value=="TDSCDMA") index=2
							else if(value=="GPRS") index=3
							else if(value=="CDMA") index=4
							
							mspNameSel.options[index].selected = true;
							//alert("index=="+index);
							break;
					}
				}
			}
			
			function refreshMsp(){
	
				var objs = document.getElementsByName("RuleNo");    
				var comment = document.getElementsByName("G3Comment")[0].value;
				var dialingstr = document.getElementsByName("DialingStr")[0].value;
				var apn = document.getElementsByName("InitCmdStr")[0].value;
				var user = document.getElementsByName("G3UserName")[0].value;
				var pwd = document.getElementsByName("G3Password")[0].value;
				
				
				var onDmdValue = document.getElementById('onDmd').value;
				
				var currsh0 = document.getElementById("currsh0").value;
				var currsm0 = document.getElementById("currsm0").value;
				var curreh0 = document.getElementById("curreh0").value;
				var currem0 = document.getElementById("currem0").value;
				
				var currsh1 = document.getElementById("currsh1").value;
				var currsm1 = document.getElementById("currsm1").value;
				var curreh1 = document.getElementById("curreh1").value;
				var currem1 = document.getElementById("currem1").value;
				
				var currsh2 = document.getElementById("currsh2").value;
				var currsm2 = document.getElementById("currsm2").value;
				var curreh2 = document.getElementById("curreh2").value;
				var currem2 = document.getElementById("currem2").value;
				
				var currsh3 = document.getElementById("currsh3").value;
				var currsm3 = document.getElementById("currsm3").value;
				var curreh3 = document.getElementById("curreh3").value;
				var currem3 = document.getElementById("currem3").value;
				
					document.getElementsByName('DelRule')[0].click();
					
					//document.getElementById("AddMSP2List").click();	
					
					//setTimeout('alert("add new msp")',8000);
					
					var	txb = document.getElementsByName("DialingStr");
						txb[0].value = dialingstr;
						txb = document.getElementsByName("InitCmdStr");
						txb[0].value = apn;
						txb = document.getElementsByName("G3UserName");
						txb[0].value = user;
						txb = document.getElementsByName("G3Password");
						txb[0].value = pwd;
						
						
					document.wanCfg.G3IdleTime.value = onDmdValue;
					
					document.getElementById('tpsh0').value = currsh0;
					document.getElementById('tpsm0').value = currsm0;
					document.getElementById('tpeh0').value = curreh0;
					document.getElementById('tpem0').value = currem0;
					
					document.getElementById('tpsh1').value = currsh1;
					document.getElementById('tpsm1').value = currsm1;
					document.getElementById('tpeh1').value = curreh1;
					document.getElementById('tpem1').value = currem1;
					
					document.getElementById('tpsh2').value = currsh2;
					document.getElementById('tpsm2').value = currsm2;
					document.getElementById('tpeh2').value = curreh2;
					document.getElementById('tpem2').value = currem2;
					
					document.getElementById('tpsh3').value = currsh3;
					document.getElementById('tpsm3').value = currsm3;
					document.getElementById('tpeh3').value = curreh3;
					document.getElementById('tpem3').value = currem3;
					
					setTimeout('document.getElementById("AddMSP2List").click()',2000);
				//	setTimeout('document.getElementById("SelAsDftMSP").click()',3000);				
					setTimeout('document.getElementById("wApply").click()',3000);
				}
			
			function resetPage(){
				var form0 = document.forms[0];
				var form1 = document.forms[1];
				var form2 = document.forms[2];
				//var form3 = document.forms[3];
				
				for(var i=1;i<form0.length;i++){
					var formType = form0.elements[i].type;
					if(formType=="text"||formType=="password"||formType=="select-one")
						form0.elements[i].value="";
					}
				/*for(var i=1;i<form3.length;i++){
					var formType = form3.elements[i].type;
				if(formType=="text"||formType=="password"||formType == "select-one")
						form3.elements[i].value="";
					}*/
				for(var i=0;i<form1.length;i++){
					var formType = form1.elements[i].type;
					if(formType=="text"||formType=="password"||formType=="select-one")
						form1.elements[i].value="";
					}
				
				for(var i=0;i<form2.length;i++){
					var formType = form2.elements[i].type;
					if(formType=="text"||formType=="password"||formType=="select-one")
						form2.elements[i].value="";
					}
				}
</script>
</script>


<form method=post name="Operators" action=/goform/Operators id="Operators">
<table width="540" border="0" cellpadding="2" cellspacing="1">

<tr style="display:none">
  <td class="title" colspan="2" id="MobileOperators">mobile MSP parameter</td>
</tr>

<tr >
	<td class="head" id="G3Comment">
		MSP Name
	</td>
	<td style="display:none">
		<input type="text" name="G3Comment" size="32" maxlength="32" value="<% getCfgGeneral(1, "g3_operator_name"); %>">
	</td>
	
	<td>
		<select id="mspName" name="mspName" onchange="changeMspName()">
			<option>WCDMA</option>
			<option>EVDO</option>
			<option>TDSCDMA</option>
			<option>GPRS</option>
			<option>CDMA</option>
		</select>
	</td>
	
</tr>

<tr style="display:none">
	<td class="head" id="NetworkType">
		Network Type
	</td>
	<td>
	<select onChange="updateState()" name="g3NetworkType" size="1" style="width=150">	
		<%showNetworkTypeASP();%>
    </select>
    </td>
</tr>

<tr>
	<td class="head" id="DialingString">
		Dialing number
	</td>
	<td>
  		<input type="text" maxlength=32 size=32 name="DialingStr" value="<% getCfgGeneral(1, "g3_dial_num"); %>">
	</td>
</tr>

<tr>
	<td class="head" id="InitCmdString">
		Initial Command String
	</td>
	<td>
  		<input type="text" maxlength=100 size=32 name="InitCmdStr" value="<% getCfgGeneral(1, "g3_initial_cmd"); %>">
  		<!--<input type="text" maxlength=100 size=32 name="InitCmdStr">-->
	</td>
</tr>

<tr>
	<td class="head" id="G3UserName">
		User name
	</td>
	<td>
  		<input type="text" maxlength=100 size=32 name="G3UserName" value="<% getCfgGeneral(1, "G3UserName"); %>">
	</td>
</tr>

<tr>
	<td class="head" id="G3Password">
		Password
	</td>
	<td>
  		<input type="password" maxlength=100 size=32 name="G3Password" value="<% getCfgGeneral(1, "G3Password"); %>">
	</td>
</tr>

<tr style="display:none">
	<td class="head" id="G3LocalIp">
		Local Ip
	</td>
	<td>
  		<input type="text" maxlength=30 size=30 name="G3LocalIp" value="<% getCfgGeneral(1, "G3LocalIp"); %>">
	</td>
</tr>
<tr style="display:none">
	<td class="head" id="id_G3AuthProtocol">
		Auth protocol
	</td>
	<td>
		<select name="G3AuthProtocol" value="<% getCfgGeneral(1, "G3auth_type"); %>" style="width=150">
   		<option value=0>AUTO</option>
		<option value=1>CHAP</option>
   		<option value=2>PAP</option>
   		</select>&nbsp;&nbsp;
	</td>
</tr>

<tr style="display:none">
	<td class="head" id="G3OtherOptions">
		Other Options
	</td>
	<td>
		<input type="checkbox" name="G3UseCompress" value="compress">&nbsp;<font id="G3UseSWComp">Use Software Compress</font>&nbsp;
	</td>
</tr>
</table>

<table style="display:none" width="540" cellpadding="2" cellspacing="1">
<tr align="center">
<td>
	<input type="submit" value="Add to List" id="AddMSP2List" onClick="return CheckMSPValue()" style="width:90;height:25"> &nbsp;&nbsp;
</td>
</tr>
</table>
</form>


<script type="text/javascript">
			/*function showMode(){
				var currMode = document.getElementById('modeSelect');

				if(currMode.options[0].selected){
					document.getElementById('onDmdTd').style.display = "none";
					document.getElementById('onTimeTd').style.display = "none";
				}
				else if(currMode.options[1].selected){
					document.getElementById('onDmdTd').style.display = "block";
					document.getElementById('onTimeTd').style.display = "none";
				}
				else if(currMode.options[2].selected){
					document.getElementById('onDmdTd').style.display = "none";
					document.getElementById('onTimeTd').style.display = "block";
				}
			}*/
			
			function modeSelects(){
				document.getElementById('onDmdTd').style.display = "none";
				document.getElementById('onTimeTd').style.display = "none";
				var currMode = document.getElementById('modeSelect');
				var realMode = document.wanCfg.G3Mode;
				if(currMode.options.selectedIndex==0){
					realMode.options.selectedIndex=0;
				}
				else if(currMode.options.selectedIndex==1){
					document.getElementById('onDmdTd').style.display = "block";
					realMode.options.selectedIndex=1;
				}
				else if(currMode.options.selectedIndex==2){
					document.getElementById('onTimeTd').style.display = "block";
					realMode.options.selectedIndex=2;
				}
				G3OPModeSwitch();
			}
			
			
			function rmbCurrMode(){
				
				document.getElementById('onDmdTd').style.display = "none";
				document.getElementById('onTimeTd').style.display = "none";
					
				var currMode = document.getElementById('modeSelect');
				var realMode = document.wanCfg.G3Mode;
				if(realMode.options.selectedIndex==0){
					currMode.options.selectedIndex=0;
					currMode.options[0].selected=true;
				}	
				else if(realMode.options.selectedIndex==1){
					realMode.options.selectedIndex=1;
					currMode.options[1].selected=true;
					document.getElementById('onDmdTd').style.display = "block";
				}
				else if(realMode.options.selectedIndex==2){
					realMode.options.selectedIndex=2;
					currMode.options[2].selected=true;
					document.getElementById('onTimeTd').style.display = "block";
				}
			}
			function radioTimeCheck(){
				if(document.getElementsByName('radioTime')[0].checked){
					document.getElementsByName('timeperiod')[0].checked = true;
				}
				else if(document.getElementsByName('radioTime')[1].checked){
					document.getElementsByName('timeperiod')[1].checked = true;
				}
				else if(document.getElementsByName('radioTime')[2].checked){
					document.getElementsByName('timeperiod')[2].checked = true;
				}
				else if(document.getElementsByName('radioTime')[3].checked){
					document.getElementsByName('timeperiod')[3].checked = true;
				}
			}
			
			function rmbCurrRadioTime(){
				if(document.getElementsByName('timeperiod')[0].checked)	{
					document.getElementsByName('radioTime')[0].checked = true;
				}
				else if(document.getElementsByName('timeperiod')[1].checked)	{
					document.getElementsByName('radioTime')[1].checked = true;
				}
				else if(document.getElementsByName('timeperiod')[2].checked)	{
					document.getElementsByName('radioTime')[2].checked = true;
				}
				else if(document.getElementsByName('timeperiod')[3].checked)	{
					document.getElementsByName('radioTime')[3].checked = true;
				}
			}
			function rmbCurrOnDmd(){
				document.getElementById('onDmd').value = document.wanCfg.G3IdleTime.value;
			}
			
			function hello(){
					alert("hello");
				}
</script>

	


<form action=/goform/OperatorDelete method=POST name="CurOperators" id="CurOperators" >



<table width="540" id="connModeNew">
				<tr>
					<td style="vertical-align: top;width:40%" id="connNode">connect mode</td>
					<td>
						<select id="modeSelect" onchange="modeSelects()">
							<option id="firAlive">Keep Alive</option>
							<option id="secDmd">On Demand</option>
							<option id="thdTime">On Time</option>
						</select>
						<br/>
						      
						<br />
						<table>
							<tr>
								<td  id="onDmdTd">
									<span id="idT">Idle Time</span>
									<input type="text" size="3" id="onDmd"/>
									<span id="idTm">minutes</span>
								</td>
							</tr>
							<tr>
								<td id="onTimeTd">								
									<table>
										<tr>
											<td><span id="exam">example:15:50---22:30 ,set NTP Server in management page before used. </span></td>
										</tr>
										<tr>
											
											<td>
												<input type="radio" name="radioTime" onclick="radioTimeCheck()"/>
												
												<input type="text" size="2" id="currsh0" value="<% getCfgGeneral(1, "tp_sh_0"); %>"/>:<input type="text" size="2" id="currsm0" value="<% getCfgGeneral(1, "tp_sm_0"); %>"/>
												--- <input type="text" size="2" id="curreh0" value="<% getCfgGeneral(1, "tp_eh_0"); %>"/>:<input type="text" size="2" id="currem0" value="<% getCfgGeneral(1, "tp_em_0"); %>"/>
											</td>
										</tr>
										<tr>
											<td>
												<input type="radio" name="radioTime" onclick="radioTimeCheck()"/>
												
												<input type="text" size="2" id="currsh1" value="<% getCfgGeneral(1, "tp_sh_1"); %>"/>:<input type="text" size="2" id="currsm1" value="<% getCfgGeneral(1, "tp_sm_1"); %>"/>
												--- <input type="text" size="2" id="curreh1" value="<% getCfgGeneral(1, "tp_eh_1"); %>"/>:<input type="text" size="2" id="currem1" value="<% getCfgGeneral(1, "tp_em_1"); %>"/>
											</td>
										</tr>
										<tr>
											<td>
												<input type="radio" name="radioTime" onclick="radioTimeCheck()"/>
												
												<input type="text" size="2" id="currsh2" value="<% getCfgGeneral(1, "tp_sh_2"); %>"/>:<input type="text" size="2" id="currsm2" value="<% getCfgGeneral(1, "tp_sm_2"); %>"/>
												--- <input type="text" size="2" id="curreh2" value="<% getCfgGeneral(1, "tp_eh_2"); %>"/>:<input type="text" size="2" id="currem2" value="<% getCfgGeneral(1, "tp_em_2"); %>"/>
											</td>
										</tr>
										<tr>
											<td>
												<input type="radio" name="radioTime" onclick="radioTimeCheck()"/>
												
												<input type="text" size="2" id="currsh3" value="<% getCfgGeneral(1, "tp_sh_3"); %>"/>:<input type="text" size="2" id="currsm3" value="<% getCfgGeneral(1, "tp_sm_3"); %>"/>
												--- <input type="text" size="2" id="curreh3" value="<% getCfgGeneral(1, "tp_eh_3"); %>"/>:<input type="text" size="2" id="currem3" value="<% getCfgGeneral(1, "tp_em_3"); %>"/>
											</td>
										</tr>
									</table>
								</td>
							</tr>
						</table>
					</td>
				</tr>			
	</table>
<table style="display:none" width="540" border="1" cellpadding="2" cellspacing="1">	
	<tr>
		<td class="title" colspan="8" id="CurOperatorsInSys">MSP List</td>
	</tr>

	<tr>
		<td id="G3OperatorNumber"> No.</td>
		<td align=center id="CurG3Comment"> MSP Name</td>
		<!--<td align=center id="CurNetworkType"> Network Type</td>-->
		<td align=center id="CurDialingString"> Dialing Number</td>
		<td align=center id="CurInitCmdString"> Initial Command</td>
		<td align=center id="CurG3UserName"> User name</td>
		<td align=center id="CurG3Password"> Password</td>
		<td align=center id="CurG3LocalIp"> Local IP</td>
		<td align=center id="G3CurOperation"> operation</td>
	</tr>

	<% showOperatorsASP(); %>
</table>
<br>

<table width="540" cellpadding="2" cellspacing="1">
<tr align="center">
<td>
	<input id="refreshbutton" type="button" value="Refresh" onclick="refreshMsp();" style="width:auto;height:25"/>
	<input id="resetbutton" type="button" value="Reset" onclick="resetPage()" style="width:auto;height:25"/>
	
<input style="display:none" type="submit" value="Select" id="SelAsDftMSP" name="setDefaultMSP" onClick="return deleteClick()" style="width:auto;height:25">&nbsp;&nbsp; 
</td>
</tr>
</table>
</form>

</td></tr></table>


</div><!--end of left-->
</td><!--end of td1-->

<td class="tdwidth2" id="td2"><!--start of td2-->
	<div id="right"><!--start of right-->
		<h2 id="help_head">Heeelp...<a href="#">more</a></h2>
		
		<p id="help_content"><span id="wan_direc1"></span><br/><br/><span id="wan_direc2"></span></p>
	</div><!--end of right-->
</td><!--end of td2-->
</tr><!--end of layout tr-->
</table><!--end of layout_table-->

</div><!--end of content-->
	</div>
	</center>
</body>
</html>

