<html>
<head>
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
<META HTTP-EQUIV="Expires" CONTENT="-1">
<META http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<script type="text/javascript" src="/lang/b28n.js"></script>
<link rel="stylesheet" href="/css/normal_ws.css" type="text/css">
<link rel="stylesheet" href="/css/boxStyle.css" type="text/css">
<title>System Management</title>

<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("admin");

var greenapb = '<% getGAPBuilt(); %>';
var http_request = false;

function style_display_on()
{
	if (window.ActiveXObject)
	{ // IE
		return "block";
	}
	else if (window.XMLHttpRequest)
	{ // Mozilla, Safari,...
		return "table-row";
	}
}

function makeRequest(url, content) {
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
        alert('Giving up :( Cannot create an XMLHTTP instance');
        return false;
    }
    http_request.onreadystatechange = alertContents;
    http_request.open('POST', url, true);
    http_request.send(content);
}

function alertContents() {
    if (http_request.readyState == 4) {
        if (http_request.status == 200) {
			// refresh
			window.location.reload();
        } else {
            alert('There was a problem with the request.');
        }
    }
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

function AdmFormCheck()
{
	if (document.Adm.admuser.value == "") {
		alert("Please specify the administrator account.");
		return false;
	}
	if (document.Adm.admpass.value == "") {
		alert("Please specify the administrator password.");
		return false;
	}
	return true;
}

function NTPFormCheck()
{
	if( document.NTP.NTPServerIP.value != "" && 
		document.NTP.NTPSync.value == ""){
		alert("Please specify a value for the interval of synchroniztion.");
		return false;
	}
	if(isAllNum( document.NTP.NTPSync.value ) == 0){
		alert("Invalid NTP synchronization value.");
		return false;
	}
	if( atoi(document.NTP.NTPSync.value, 1) > 300){
		alert("The synchronization value is too big.(1~300)");
		return false;
	}		
	return true;
}

function DDNSFormCheck()
{
	if(  document.DDNS.DDNSProvider.value != "none" && 
		(document.DDNS.Account.value == "" ||
		 document.DDNS.Password.value == "" ||
		 document.DDNS.DDNS.value == "")){
		alert("Please specify account, password, and DDNS.");
		return false;
	}

	return true;
}

function disableTextField (field)
{
  if(document.all || document.getElementById){
    field.disabled = true;
  }else {
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

function DDNSupdateState()
{
	if(document.DDNS.DDNSProvider.options.selectedIndex != 0){
		enableTextField(document.DDNS.Account);
		enableTextField(document.DDNS.Password);
		enableTextField(document.DDNS.DDNS);
	}else{
		disableTextField(document.DDNS.Account);
		disableTextField(document.DDNS.Password);
		disableTextField(document.DDNS.DDNS);
	}
}

function initTranslation()
{
	var e = document.getElementById("manTitle");
	e.innerHTML = _("man title");
	e = document.getElementById("manIntroduction");
	e.innerHTML = _("man introduction");
	e = document.getElementById("manLangApply");
	e.value = _("admin apply");
	e = document.getElementById("manLangCancel");
	e.value = _("admin cancel");

	e = document.getElementById("manLangSet");
	e.innerHTML = _("man language setting");
	e = document.getElementById("manSelectLang");
	e.innerHTML = _("man select language");
	e = document.getElementById("manAdmSet");
	e.innerHTML = _("man admin setting");
	e = document.getElementById("manAdmAccount");
	e.innerHTML = _("man admin account");
	e = document.getElementById("manAdmPasswd");
	e.innerHTML = _("man admin passwd");
	e = document.getElementById("manAdmApply");
	e.value = _("admin apply");
	e = document.getElementById("manAdmCancel");
	e.value = _("admin cancel");

	e = document.getElementById("manNTPSet");
	e.innerHTML = _("man ntp setting");
	e = document.getElementById("manNTPTimeZone");
	e.innerHTML = _("man ntp timezone");
	e = document.getElementById("manNTPMidIsland");
	e.innerHTML = _("man ntp mid island");
	e = document.getElementById("manNTPHawaii");
	e.innerHTML = _("man ntp hawaii");
	e = document.getElementById("manNTPAlaska");
	e.innerHTML = _("man ntp alaska");
	e = document.getElementById("manNTPPacific");
	e.innerHTML = _("man ntp pacific");
	e = document.getElementById("manNTPMountain");
	e.innerHTML = _("man ntp mountain");
	e = document.getElementById("manNTPArizona");
	e.innerHTML = _("man ntp arizona");
	e = document.getElementById("manNTPCentral");
	e.innerHTML = _("man ntp central");
	e = document.getElementById("manNTPMidUS");
	e.innerHTML = _("man ntp mid us");
	e = document.getElementById("manNTPIndianaEast");
	e.innerHTML = _("man ntp indiana east");
	e = document.getElementById("manNTPEastern");
	e.innerHTML = _("man ntp eastern");
	e = document.getElementById("manNTPAtlantic");
	e.innerHTML = _("man ntp atlantic");
	e = document.getElementById("manNTPBolivia");
	e.innerHTML = _("man ntp bolivia");
	e = document.getElementById("manNTPGuyana");
	e.innerHTML = _("man ntp guyana");
	e = document.getElementById("manNTPBrazilEast");
	e.innerHTML = _("man ntp brazil east");
	e = document.getElementById("manNTPMidAtlantic");
	e.innerHTML = _("man ntp mid atlantic");
	e = document.getElementById("manNTPAzoresIslands");
	e.innerHTML = _("man ntp azores islands");
	e = document.getElementById("manNTPGambia");
	e.innerHTML = _("man ntp gambia");
	e = document.getElementById("manNTPEngland");
	e.innerHTML = _("man ntp england");
	e = document.getElementById("manNTPCzechRepublic");
	e.innerHTML = _("man ntp czech republic");
	e = document.getElementById("manNTPGermany");
	e.innerHTML = _("man ntp germany");
	e = document.getElementById("manNTPTunisia");
	e.innerHTML = _("man ntp tunisia");
	e = document.getElementById("manNTPGreece");
	e.innerHTML = _("man ntp greece");
	e = document.getElementById("manNTPSouthAfrica");
	e.innerHTML = _("man ntp south africa");
	e = document.getElementById("manNTPIraq");
	e.innerHTML = _("man ntp iraq");
	e = document.getElementById("manNTPMoscowWinter");
	e.innerHTML = _("man ntp moscow winter");
	e = document.getElementById("manNTPArmenia");
	e.innerHTML = _("man ntp armenia");
	e = document.getElementById("manNTPPakistan");
	e.innerHTML = _("man ntp pakistan");
	e = document.getElementById("manNTPBangladesh");
	e.innerHTML = _("man ntp bangladesh");
	e = document.getElementById("manNTPThailand");
	e.innerHTML = _("man ntp thailand");
	e = document.getElementById("manNTPChinaCoast");
	e.innerHTML = _("man ntp chinacoast");
	e = document.getElementById("manNTPTaipei");
	e.innerHTML = _("man ntp taipei");
	e = document.getElementById("manNTPSingapore");
	e.innerHTML = _("man ntp singapore");
	e = document.getElementById("manNTPAustraliaWA");
	e.innerHTML = _("man ntp australia wa");
	e = document.getElementById("manNTPJapan");
	e.innerHTML = _("man ntp japan");
	e = document.getElementById("manNTPKorean");
	e.innerHTML = _("man ntp korean");
	e = document.getElementById("manNTPGuam");
	e.innerHTML = _("man ntp guam");
	e = document.getElementById("manNTPAustraliaQLD");
	e.innerHTML = _("man ntp australia qld");
	e = document.getElementById("manNTPSolomonIslands");
	e.innerHTML = _("man ntp solomon islands");
	e = document.getElementById("manNTPFiji");
	e.innerHTML = _("man ntp fiji");
	e = document.getElementById("manNTPNewZealand");
	e.innerHTML = _("man ntp newzealand");
	e = document.getElementById("manNTPServer");
	e.innerHTML = _("man ntp server");
	e = document.getElementById("manNTPSync");
	e.innerHTML = _("man ntp sync");
	e = document.getElementById("manNTPApply");
	e.value = _("admin apply");
	e = document.getElementById("manNTPCancel");
	e.value = _("admin cancel");

	e = document.getElementById("manNTPCurrentTime");
	e.innerHTML = _("man ntp current time");
	e = document.getElementById("manNTPSyncWithHost");
	e.value = _("man ntp sync with host");

	e = document.getElementById("manGAPTitle");
	e.innerHTML = _("man gap title");
	e = document.getElementById("manGAPTime");
	e.innerHTML = _("man gap time");
	e = document.getElementById("manGAPAction");
	e.innerHTML = _("man gap action");
	e = document.getElementById("manGAPActionDisable1");
	e.innerHTML = _("admin disable");
	e = document.getElementById("manGAPActionDisable2");
	e.innerHTML = _("admin disable");
	e = document.getElementById("manGAPActionDisable3");
	e.innerHTML = _("admin disable");
	e = document.getElementById("manGAPActionDisable4");
	e.innerHTML = _("admin disable");

	e = document.getElementById("manGreenAPApply");
	e.value = _("admin apply");
	e = document.getElementById("manGreenAPCancle");
	e.value = _("admin cancel");

	e = document.getElementById("manDdnsSet");
	e.innerHTML = _("man ddns setting");
	e = document.getElementById("DdnsProvider");
	e.innerHTML = _("man ddns provider");
	e = document.getElementById("manDdnsNone");
	e.innerHTML = _("man ddns none");
	e = document.getElementById("manDdnsAccount");
	e.innerHTML = _("man ddns account");
	e = document.getElementById("manDdnsPasswd");
	e.innerHTML = _("man ddns passwd");
	e = document.getElementById("manDdns");
	e.innerHTML = _("man ddns");
	e = document.getElementById("manDdnsApply");
	e.value = _("admin apply");
	e = document.getElementById("manDdnsCancel");
	e.value = _("admin cancel");
	
	
	e = document.getElementById("help_head");
	e.innerHTML = _("help help_head");
	e = document.getElementById("management_direc1");
	e.innerHTML = _("management management_direc1");
	e = document.getElementById("management_direc2");
	e.innerHTML = _("management management_direc2");
	e = document.getElementById("management_direc3");
	e.innerHTML = _("management management_direc3");
	e = document.getElementById("management_direc4");
	e.innerHTML = _("management management_direc4");
	
}

function initValue()
{
	var tz = "<% getCfgGeneral(1, "TZ"); %>";
	var ddns_provider = "<% getCfgGeneral(1, "DDNSProvider"); %>";

	var lang_element = document.getElementById("langSelection");
	var lang_en = "<% getLangBuilt("en"); %>";
	var lang_zhtw = "<% getLangBuilt("zhtw"); %>";
	var lang_zhcn = "<% getLangBuilt("zhcn"); %>";

	var dateb = "<% getDATEBuilt(); %>";
	var ddnsb = "<% getDDNSBuilt(); %>";

	initTranslation();
	lang_element.options.length = 0;
	if (lang_en == "1")
		lang_element.options[lang_element.length] = new Option('English', 'en');
	if (lang_zhtw == "1")
		lang_element.options[lang_element.length] = new Option('Traditional Chinese', 'zhtw');
	if (lang_zhcn == "1")
		lang_element.options[lang_element.length] = new Option('Simple Chinese', 'zhcn');

	if (document.cookie.length > 0) {
		var s = document.cookie.indexOf("language=");
		var e = document.cookie.indexOf(";", s);
		var lang = "en";
		var i;

		if (s != -1) {
			if (e == -1)
				lang = document.cookie.substring(s+9);
			else
				lang = document.cookie.substring(s+9, e);
		}
		for (i=0; i<lang_element.options.length; i++) {
			if (lang == lang_element.options[i].value) {
				lang_element.options.selectedIndex = i;
				break;
			}
		}
	}

	if (dateb == "1")
	{
		document.getElementById("div_date").style.visibility = "visible";
		document.getElementById("div_date").style.display = style_display_on();
		document.NTP.ntpcurrenttime.disabled = false;
	} 
	else
	{
		document.getElementById("div_date").style.visibility = "hidden";
		document.getElementById("div_date").style.display = "none";
		document.NTP.ntpcurrenttime.disabled = true;
	}

	if (tz == "CST_008")
		document.NTP.time_zone.options.selectedIndex = 0;
	else if (tz == "UCT_-10")
		document.NTP.time_zone.options.selectedIndex = 1;
	else if (tz == "NAS_-09")
		document.NTP.time_zone.options.selectedIndex = 2;
	else if (tz == "PST_-08")
		document.NTP.time_zone.options.selectedIndex = 3;
	else if (tz == "MST_-07")
		document.NTP.time_zone.options.selectedIndex = 4;
	else if (tz == "MST_-07")
		document.NTP.time_zone.options.selectedIndex = 5;
	else if (tz == "CST_-06")
		document.NTP.time_zone.options.selectedIndex = 6;
	else if (tz == "UCT_-06")
		document.NTP.time_zone.options.selectedIndex = 7;
	else if (tz == "UCT_-05")
		document.NTP.time_zone.options.selectedIndex = 8;
	else if (tz == "EST_-05")
		document.NTP.time_zone.options.selectedIndex = 9;
	else if (tz == "AST_-04")
		document.NTP.time_zone.options.selectedIndex = 10;
	else if (tz == "UCT_-04")
		document.NTP.time_zone.options.selectedIndex = 11;
	else if (tz == "UCT_-03")
		document.NTP.time_zone.options.selectedIndex = 12;
	else if (tz == "EBS_-03")
		document.NTP.time_zone.options.selectedIndex = 13;
	else if (tz == "NOR_-02")
		document.NTP.time_zone.options.selectedIndex = 14;
	else if (tz == "EUT_-01")
		document.NTP.time_zone.options.selectedIndex = 15;
	else if (tz == "UCT_000")
		document.NTP.time_zone.options.selectedIndex = 16;
	else if (tz == "GMT_000")
		document.NTP.time_zone.options.selectedIndex = 17;
	else if (tz == "MET_001")
		document.NTP.time_zone.options.selectedIndex = 18;
	else if (tz == "MEZ_001")
		document.NTP.time_zone.options.selectedIndex = 19;
	else if (tz == "UCT_001")
		document.NTP.time_zone.options.selectedIndex = 20;
	else if (tz == "EET_002")
		document.NTP.time_zone.options.selectedIndex = 21;
	else if (tz == "SAS_002")
		document.NTP.time_zone.options.selectedIndex = 22;
	else if (tz == "IST_003")
		document.NTP.time_zone.options.selectedIndex = 23;
	else if (tz == "MSK_003")
		document.NTP.time_zone.options.selectedIndex = 24;
	else if (tz == "UCT_004")
		document.NTP.time_zone.options.selectedIndex = 25;
	else if (tz == "UCT_005")
		document.NTP.time_zone.options.selectedIndex = 26;
	else if (tz == "UCT_006")
		document.NTP.time_zone.options.selectedIndex = 27;
	else if (tz == "UCT_007")
		document.NTP.time_zone.options.selectedIndex = 28;
	else if (tz == "UCT_-11")
		document.NTP.time_zone.options.selectedIndex = 29;
	else if (tz == "CCT_008")
		document.NTP.time_zone.options.selectedIndex = 30;
	else if (tz == "SST_008")
		document.NTP.time_zone.options.selectedIndex = 31;
	else if (tz == "AWS_008")
		document.NTP.time_zone.options.selectedIndex = 32;
	else if (tz == "JST_009")
		document.NTP.time_zone.options.selectedIndex = 33;
	else if (tz == "KST_009")
		document.NTP.time_zone.options.selectedIndex = 34;
	else if (tz == "UCT_010")
		document.NTP.time_zone.options.selectedIndex = 35;
	else if (tz == "AES_010")
		document.NTP.time_zone.options.selectedIndex = 36;
	else if (tz == "UCT_011")
		document.NTP.time_zone.options.selectedIndex = 37;
	else if (tz == "UCT_012")
		document.NTP.time_zone.options.selectedIndex = 38;
	else if (tz == "NZS_012")
		document.NTP.time_zone.options.selectedIndex = 39;

	if (greenapb == "1")
	{
		document.getElementById("div_greenap").style.visibility = "visible";
		document.getElementById("div_greenap").style.display = style_display_on();
		document.getElementById("div_greenap_submit").style.visibility = "visible";
		document.getElementById("div_greenap_submit").style.display = style_display_on();
	}
	else
	{
		document.getElementById("div_greenap").style.visibility = "hidden";
		document.getElementById("div_greenap").style.display = "none";
		document.getElementById("div_greenap_submit").style.visibility = "hidden";
		document.getElementById("div_greenap_submit").style.display = "none";
	}
	set_greenap();

	if (ddnsb == "1")
	{
		document.getElementById("div_ddns").style.visibility = "visible";
		document.getElementById("div_ddns").style.display = style_display_on();
		document.getElementById("div_ddns_submit").style.visibility = "visible";
		document.getElementById("div_ddns_submit").style.display = style_display_on();
		document.DDNS.Account.disabled = false;
		document.DDNS.Password.disabled = false;
		document.DDNS.DDNS.disabled = false;
		if (ddns_provider == "none")
			document.DDNS.DDNSProvider.options.selectedIndex = 0;
		else if (ddns_provider == "dyndns.org")
			document.DDNS.DDNSProvider.options.selectedIndex = 1;
		else if (ddns_provider == "freedns.afraid.org")
			document.DDNS.DDNSProvider.options.selectedIndex = 2;
		else if (ddns_provider == "zoneedit.com")
			document.DDNS.DDNSProvider.options.selectedIndex = 3;
		else if (ddns_provider == "no-ip.com")
			document.DDNS.DDNSProvider.options.selectedIndex = 4;
		else if (ddns_provider == "3322.org")
			document.DDNS.DDNSProvider.options.selectedIndex = 5;

		DDNSupdateState();
	} 
	else
	{
		document.getElementById("div_ddns").style.visibility = "hidden";
		document.getElementById("div_ddns").style.display = "none";
		document.getElementById("div_ddns_submit").style.visibility = "hidden";
		document.getElementById("div_ddns_submit").style.display = "none";
		document.DDNS.Account.disabled = true;
		document.DDNS.Password.disabled = true;
		document.DDNS.DDNS.disabled = true;
	}
}

function set_greenap()
{
	var ntp_server = "<% getCfgGeneral(1, "NTPServerIP"); %>";

	for(var j=1;j<=4;j++)
	{
	    var shour_e = eval("document.GreenAP.GAPSHour"+j);
	    var sminute_e = eval("document.GreenAP.GAPSMinute"+j);
	    var ehour_e = eval("document.GreenAP.GAPEHour"+j);
	    var eminute_e = eval("document.GreenAP.GAPEMinute"+j);
	    var action_e = eval("document.GreenAP.GAPAction"+j);

	    shour_e.disabled = true;
	    sminute_e.disabled = true;
	    ehour_e.disabled = true;
	    eminute_e.disabled = true;
	    action_e.disabled = true;
	    if (ntp_server != "" && greenapb == "1")
	    {
		action_e.disabled = false;
		switch(j)
		{
		case 1:
		    var action = "<% getCfgGeneral(1, "GreenAPAction1"); %>";
		    var time = "<% getCfgGeneral(1, "GreenAPStart1"); %>";
		    var stimeArray = time.split(" ");
		    time = "<% getCfgGeneral(1, "GreenAPEnd1"); %>";
		    var etimeArray = time.split(" ");
		    break;
		case 2:
		    var action = "<% getCfgGeneral(1, "GreenAPAction2"); %>";
		    var time = "<% getCfgGeneral(1, "GreenAPStart2"); %>";
		    var stimeArray = time.split(" ");
		    time = "<% getCfgGeneral(1, "GreenAPEnd2"); %>";
		    var etimeArray = time.split(" ");
		    break;
		case 3:
		    var action = "<% getCfgGeneral(1, "GreenAPAction3"); %>";
		    var time = "<% getCfgGeneral(1, "GreenAPStart3"); %>";
		    var stimeArray = time.split(" ");
		    time = "<% getCfgGeneral(1, "GreenAPEnd3"); %>";
		    var etimeArray = time.split(" ");
		    break;
		case 4:
		    var action = "<% getCfgGeneral(1, "GreenAPAction4"); %>";
		    var time = "<% getCfgGeneral(1, "GreenAPStart4"); %>";
		    var stimeArray = time.split(" ");
		    time = "<% getCfgGeneral(1, "GreenAPEnd4"); %>";
		    var etimeArray = time.split(" ");
		    break;
		}
		if (action == "Disable")
		    action_e.options.selectedIndex = 0;
		else if (action == "WiFiOFF")
		    action_e.options.selectedIndex = 1;
		else if (action == "TX25")
		    action_e.options.selectedIndex = 2;
		else if (action == "TX50")
		    action_e.options.selectedIndex = 3;
		else if (action == "TX75")
		    action_e.options.selectedIndex = 4;
		greenap_action_switch(j);
		if (action != "" && action != "Disable")
		{
		    shour_e.options.selectedIndex = stimeArray[1];
		    sminute_e.options.selectedIndex = stimeArray[0];
		    ehour_e.options.selectedIndex = etimeArray[1];
		    eminute_e.options.selectedIndex = etimeArray[0];
		}
	    }
	}
}
/*****************************************ÓïÑÔÇÐ»»Ê±£¬Ë¢ÐÂ×ó±ß²Ëµ¥MenuFrameÀ¸º¯Êý***************************************************/
 function refreshMenuFrame(){
    parent.menuFrame.location.reload(true); 		
 }

function setLanguage()
{
	refreshMenuFrame();
	var expiresDate = new Date();
	expiresDate.setDate(expiresDate.getDate()+1000);
	document.cookie="language=" + document.Lang.langSelection.value + ";expires="+expiresDate.toGMTString()+";path=/";
	parent.mainFrame.location.reload(true);

	return true;
	
	
}

function syncWithHost()
{
	var currentTime = new Date();

	var seconds = currentTime.getSeconds();
	var minutes = currentTime.getMinutes();
	var hours = currentTime.getHours();
	var month = currentTime.getMonth() + 1;
	var day = currentTime.getDate();
	var year = currentTime.getFullYear();

	var seconds_str = " ";
	var minutes_str = " ";
	var hours_str = " ";
	var month_str = " ";
	var day_str = " ";
	var year_str = " ";

	if(seconds < 10)
		seconds_str = "0" + seconds;
	else
		seconds_str = ""+seconds;

	if(minutes < 10)
		minutes_str = "0" + minutes;
	else
		minutes_str = ""+minutes;

	if(hours < 10)
		hours_str = "0" + hours;
	else
		hours_str = ""+hours;

	if(month < 10)
		month_str = "0" + month;
	else
		month_str = ""+month;

	if(day < 10)
		day_str = "0" + day;
	else
		day_str = day;

	var tmp = month_str + day_str + hours_str + minutes_str + year + " ";
	makeRequest("/goform/NTPSyncWithHost", tmp);
}

function greenap_action_switch(index)
{
	var shour_e = eval("document.GreenAP.GAPSHour"+index);
	var sminute_e = eval("document.GreenAP.GAPSMinute"+index);
	var ehour_e = eval("document.GreenAP.GAPEHour"+index);
	var eminute_e = eval("document.GreenAP.GAPEMinute"+index);
	var action_e = eval("document.GreenAP.GAPAction"+index);

	shour_e.disabled = true;
	sminute_e.disabled = true;
	ehour_e.disabled = true;
	eminute_e.disabled = true;

	if (action_e.options.selectedIndex != 0)
	{
		shour_e.disabled = false;
		sminute_e.disabled = false;
		ehour_e.disabled = false;
		eminute_e.disabled = false;
	}
}
</script>

</head>
<body onload="initValue()">
<center id="boxes">
	<div id="box">
	<div id="head"></div>	<!--end of head-->	
	<div id="content">

	<table id="layout_table" border="0"><!--start of layout_table-->
<tr><!--start of layout tr-->
<td class="tdwidth1" id="td1"><!--start of td1-->
<div id="left"><!--start of left-->


<table class="body"><tr><td>
<h1 id="manTitle">System Management</h1>
<p style="display:none" id="manIntroduction">You may configure administrator account and password, NTP settings, and Dynamic DNS settings here.</p>
<hr/>
<br/>
<!-- ================= Langauge Settings ================= -->
<form method="post" name="Lang" action="/goform/setSysLang">
<table width="540" border="0" cellspacing="1" cellpadding="2">
  <tr>
    <td class="title" colspan="2" id="manLangSet">Language Settings</td>
  </tr>
  <tr>
    <td class="head" id="manSelectLang">Select Language</td>
   
    <td>
      <select name="langSelection" id="langSelection">
        <!-- added by initValue -->
      </select>
    </td>
  </tr>
</table>
<table width="540" border="0" cellpadding="2" cellspacing="1">
  <tr align="center">
    <td>
      <input type=button  value="Apply" id="manLangApply" onClick="document.Lang.submit();setLanguage()"> &nbsp; &nbsp;
      <input type=reset   value="Cancel" id="manLangCancel" onClick="window.location.reload()">
    </td>
  </tr>
</table>
</form>

<!-- ================= Adm Settings ================= -->
<form method="post" name="Adm" action="/goform/setSysAdm">
<table width="540" border="0" cellspacing="1" cellpadding="2">
  <tr>
    <td class="title" colspan="2" id="manAdmSet">Adminstrator Settings</td>
  </tr>
  <tr>
    <td class="head" id="manAdmAccount">Account</td>
    <td><input type="text" name="admuser" size="16" maxlength="16" value="<% getCfgGeneral(1, "Login"); %>"></td>
  </tr>
  <tr>
    <td class="head" id="manAdmPasswd">Password</td>
    <td><input type="password" name="admpass" size="16" maxlength="32" value="<% getCfgGeneral(1, "Password"); %>"></td>
  </tr>
</table>
<table width="540" border="0" cellpadding="2" cellspacing="1">
  <tr align="center">
    <td>
      <input type=submit  value="Apply" id="manAdmApply" onClick="return AdmFormCheck()"> &nbsp; &nbsp;
      <input type=reset   value="Cancel" id="manAdmCancel" onClick="window.location.reload()">
    </td>
  </tr>
</table>
</form>

<!-- ================= NTP Settings ================= -->
<form method="post" name="NTP" action="/goform/NTP">
<table width="540" border="0" cellspacing="1" cellpadding="2">
<tbody><tr>
  <td class="title" colspan="2" id="manNTPSet">NTP Settings</td>
</tr>
<tr id="div_date">
	<td class="head"  id="manNTPCurrentTime">Current Time</td>
	<td>
		<input size="24" name="ntpcurrenttime" value="<% getCurrentTimeASP(); %>" type="text" readonly="1">
		<input type="button" value="Sync with host" id="manNTPSyncWithHost" name="manNTPSyncWithHost" onClick="syncWithHost()">
	</td>
</tr>
<tr>
  <td class="head" id="manNTPTimeZone">Time Zone:</td>
  <td>
    <select name="time_zone">
      <option value="CST_008" id="manNTPChinaCoast">(GMT+08:00) China Coast,Hong Kong</option>
      <option value="UCT_-10" id="manNTPHawaii">(GMT-10:00) Hawaii</option>
      <option value="NAS_-09" id="manNTPAlaska">(GMT-09:00) Alaska</option>
      <option value="PST_-08" id="manNTPPacific">(GMT-08:00) Pacific Time</option>
      <option value="MST_-07" id="manNTPMountain">(GMT-07:00) Mountain Time</option>
      <option value="MST_-07" id="manNTPArizona">(GMT-07:00) Arizona</option>
      <option value="CST_-06" id="manNTPCentral">(GMT-06:00) Central Time</option>
      <option value="UCT_-06" id="manNTPMidUS">(GMT-06:00) Middle America</option>
      <option value="UCT_-05" id="manNTPIndianaEast">(GMT-05:00) Indiana East, Colombia</option>
      <option value="EST_-05" id="manNTPEastern">(GMT-05:00) Eastern Time</option>
      <option value="AST_-04" id="manNTPAtlantic">(GMT-04:00) Atlantic Time, Brazil West</option>
      <option value="UCT_-04" id="manNTPBolivia">(GMT-04:00) Bolivia, Venezuela</option>
      <option value="UCT_-03" id="manNTPGuyana">(GMT-03:00) Guyana</option>
      <option value="EBS_-03" id="manNTPBrazilEast">(GMT-03:00) Brazil East, Greenland</option>
      <option value="NOR_-02" id="manNTPMidAtlantic">(GMT-02:00) Mid-Atlantic</option>
      <option value="EUT_-01" id="manNTPAzoresIslands">(GMT-01:00) Azores Islands</option>
      <option value="UCT_000" id="manNTPGambia">(GMT) Gambia, Liberia, Morocco</option>
      <option value="GMT_000" id="manNTPEngland">(GMT) England</option>
      <option value="MET_001" id="manNTPCzechRepublic">(GMT+01:00) Czech Republic, N</option>
      <option value="MEZ_001" id="manNTPGermany">(GMT+01:00) Germany</option>
      <option value="UCT_001" id="manNTPTunisia">(GMT+01:00) Tunisia</option>
      <option value="EET_002" id="manNTPGreece">(GMT+02:00) Greece, Ukraine, Turkey</option>
      <option value="SAS_002" id="manNTPSouthAfrica">(GMT+02:00) South Africa</option>
      <option value="IST_003" id="manNTPIraq">(GMT+03:00) Iraq, Jordan, Kuwait</option>
      <option value="MSK_003" id="manNTPMoscowWinter">(GMT+03:00) Moscow Winter Time</option>
      <option value="UCT_004" id="manNTPArmenia">(GMT+04:00) Armenia</option>
      <option value="UCT_005" id="manNTPPakistan">(GMT+05:00) Pakistan, Russia</option>
      <option value="UCT_006" id="manNTPBangladesh">(GMT+06:00) Bangladesh, Russia</option>
      <option value="UCT_007" id="manNTPThailand">(GMT+07:00) Thailand, Russia</option>
      <option value="UCT_-11" id="manNTPMidIsland">(GMT-11:00) Midway Island,Samoa</option>
      <option value="CCT_008" id="manNTPTaipei">(GMT+08:00) Taipei</option>
      <option value="SST_008" id="manNTPSingapore">(GMT+08:00) Singapore</option>
      <option value="AWS_008" id="manNTPAustraliaWA">(GMT+08:00) Australia (WA)</option>
      <option value="JST_009" id="manNTPJapan">(GMT+09:00) Japan, Korea</option>
      <option value="KST_009" id="manNTPKorean">(GMT+09:00) Korean</option>
      <option value="UCT_010" id="manNTPGuam">(GMT+10:00) Guam, Russia</option>
      <option value="AES_010" id="manNTPAustraliaQLD">(GMT+10:00) Australia (QLD, TAS,NSW,ACT,VIC)</option>
      <option value="UCT_011" id="manNTPSolomonIslands">(GMT+11:00) Solomon Islands</option>
      <option value="UCT_012" id="manNTPFiji">(GMT+12:00) Fiji</option>
      <option value="NZS_012" id="manNTPNewZealand">(GMT+12:00) New Zealand</option>
    </select>
  </td>
</tr>
<tr>
  <td class="head" id="manNTPServer">NTP Server</td>
  <td><input size="32" maxlength="64" name="NTPServerIP" value="<% getCfgGeneral(1, "NTPServerIP"); %>" type="text">
	<br>&nbsp;&nbsp;<font color="#808080">ex:&nbsp;time.nist.gov</font>
	<br>&nbsp;&nbsp;<font color="#808080">&nbsp;&nbsp;&nbsp;&nbsp;ntp0.broad.mit.edu</font>
	<br>&nbsp;&nbsp;<font color="#808080">&nbsp;&nbsp;&nbsp;&nbsp;time.stdtime.gov.tw</font>
  </td>
</tr>
<tr>
  <td class="head" id="manNTPSync">NTP synchronization</td>
  <td><input size="4" name="NTPSync" value="<% getCfgGeneral(1, "NTPSync"); %>" type="text"> </td>
</tr>
</tbody></table>

<table width="540" border="0" cellpadding="2" cellspacing="1">
  <tr align="center">
    <td>
      <input type=submit  value="Apply" id="manNTPApply" onClick="return NTPFormCheck()"> &nbsp; &nbsp;
      <input type=reset   value="Cancel"id="manNTPCancel" onClick="window.location.reload()">
    </td>
  </tr>
</table>
</form>

<!-- ================= GreenAP ================= -->
<form method="post" name="GreenAP" action="/goform/GreenAP">
<table id="div_greenap" width="540" border="0" cellspacing="1" cellpadding="2">
  <tr>
    <td class="title" colspan="3" id="manGAPTitle">Green AP</td>
  </tr>
  <tr align="center">
    <td class="head" id="manGAPTime">Time</td>
    <td class="head" id="manGAPAction">Action</td>
  </tr>
  <script language="JavaScript" type="text/javascript">
  for(var j=1;j<=4;j++)
  {
	  var item = "<tr align=\"center\"><td><select name=\"GAPSHour"+j+"\">";
	  for(var i=0;i<24;i++)
	  {
		  if (i < 10)
			  item += "<option value=\""+i+"\">0"+i+"</option>";
		  else
			  item += "<option value=\""+i+"\">"+i+"</option>";
	  }
	  item += "</select>&nbsp;:&nbsp;";
	  document.write(item);

	  var item = "<select name=\"GAPSMinute"+j+"\">";
	  for(var i=0;i<60;i++)
	  {
		  if (i < 10)
			  item += "<option value=\""+i+"\">0"+i+"</option>";
		  else
			  item += "<option value=\""+i+"\">"+i+"</option>";
	  }
	  item += "</select>&nbsp;~&nbsp;";
	  document.write(item);

	  var item = "<select name=\"GAPEHour"+j+"\">";
	  for(var i=0;i<24;i++)
	  {
		  if (i < 10)
			  item += "<option value=\""+i+"\">0"+i+"</option>";
		  else
			  item += "<option value=\""+i+"\">"+i+"</option>";
	  }
	  item += "</select>&nbsp;:&nbsp;";
	  document.write(item);

	  var item = "<select name=\"GAPEMinute"+j+"\">";
	  for(var i=0;i<60;i++)
	  {
		  if (i < 10)
			  item += "<option value=\""+i+"\">0"+i+"</option>";
		  else
			  item += "<option value=\""+i+"\">"+i+"</option>";
	  }
	  item += "</select></td>";
	  item += "<td><select name=\"GAPAction"+j+"\" onClick=\"greenap_action_switch('"+j+"')\">";
	  item += "<option value=\"Disable\" id=\"manGAPActionDisable"+j+"\">Disable</option>";
	  item += "<option value=\"WiFiOFF\">WiFi TxPower OFF</option>";
	  item += "<option value=\"TX25\">WiFi TxPower 25%</option>";
	  item += "<option value=\"TX50\">WiFi TxPower 50%</option>";
	  item += "<option value=\"TX75\">WiFi TxPower 75%</option";
	  item += "</select></td></tr>";
	  document.write(item);
  }
  </script> 
</table>
<table id="div_greenap_submit" width="540" border="0" cellpadding="2" cellspacing="1">
  <tr align="center">
    <td>
      <input type="submit"  value="Apply" id="manGreenAPApply">&nbsp;&nbsp;
      <input type="reset"  value="Cancle" id="manGreenAPCancle" onClick="window.location.reload()">
    </td>
  </tr>
</table>
</form>

<!-- ================= DDNS  ================= -->
<form method="post" name="DDNS" action="/goform/DDNS">
<table id="div_ddns" width="540" border="0" cellspacing="1" cellpadding="2">
<tbody><tr>
  <td class="title" colspan="2" id="manDdnsSet">DDNS Settings</td>
</tr>
<tr>
  <td class="head" id="DdnsProvider">Dynamic DNS Provider</td>
  <td>
    <select onChange="DDNSupdateState()" name="DDNSProvider">
      <option value="none" id="manDdnsNone"> None </option>
      <option value="dyndns.org"> Dyndns.org </option>
      <option value="freedns.afraid.org"> freedns.afraid.org </option>
      <option value="zoneedit.com"> www.zoneedit.com </option>
      <option value="no-ip.com"> www.no-ip.com </option>
      <option value="3322.org"> www.3322.org </option> 
    </select>
  </td>
</tr>
<tr>
  <td class="head" id="manDdnsAccount">Account</td>
  <td><input size="16" name="Account" value="<% getCfgGeneral(1, "DDNSAccount"); %>" type="text"> </td>
</tr>
<tr>
  <td class="head" id="manDdnsPasswd">Password</td>
  <td><input size="16" name="Password" value="<% getCfgGeneral(1, "DDNSPassword"); %>" type="password"> </td>
</tr>
<tr>
  <td class="head" id="manDdns">DDNS</td>
  <td><input size="32" name="DDNS" value="<% getCfgGeneral(1, "DDNS"); %>" type="text"> </td>
</tr>
</tbody></table>

<table id="div_ddns_submit" width="540" border="0" cellpadding="2" cellspacing="1">
  <tr align="center">
    <td>
      <input type=submit  value="Apply" id="manDdnsApply" onClick="return DDNSFormCheck()"> &nbsp; &nbsp;
      <input type=reset   value="Cancel" id="manDdnsCancel" onClick="window.location.reload()">
    </td>
  </tr>
</table>
</form>


</td></tr></table>


</div><!--end of left-->
</td><!--end of td1-->

<td class="tdwidth2" id="td2"><!--start of td2-->
	<div id="right"><!--start of right-->
		<h2 id="help_head">Heeelp..</h2>
		
		<p id="help_content">
			<span id="management_direc1"></span><br/><br/>
			<span id="management_direc2"></span><br/><br/>
			<span id="management_direc3"></span><br/><br/>
			<span id="management_direc4"></span><br/><br/>
		</p>
	</div><!--end of right-->
</td><!--end of td2-->
</tr><!--end of layout tr-->
</table><!--end of layout_table-->


</div><!--end of content-->
	</div>
	</center>
</body></html>
