<html><head><title>System Log</title>

<link rel="stylesheet" href="/css/normal_ws.css" type="text/css">
<link rel="stylesheet" href="/css/boxStyle.css" type="text/css">
<meta http-equiv="content-type" content="text/html; charset=iso-8859-1">
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("admin");


var http_request = false;
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
			uploadLogField(http_request.responseText);
        } else {
            alert('There was a problem with the request.');
        }
    }
}

function uploadLogField(str)
{
	if(str == "-1"){
		document.getElementById("syslog").value =
		"Not support.\n(Busybox->\n  System Logging Utilitie ->\n    syslogd\n    Circular Buffer\n    logread"
	}else
		document.getElementById("syslog").value = str;
}

function updateLog()
{
	makeRequest("/goform/syslog", "n/a", false);
}

function initTranslation()
{
	var e = document.getElementById("syslogTitle");
	e.innerHTML = _("syslog title");
	//e = document.getElementById("syslogIntroduction");
	//e.innerHTML = _("syslog introduction");

	e = document.getElementById("syslogSysLog");
	e.innerHTML = _("syslog system log");
	//e = document.getElementById("syslogSysLogClear");
	//e.value = _("syslog clear");
	//e = document.getElementById("syslogSysLogRefresh");
	//e.value = _("syslog refresh");
    e = document.getElementById("syslogSysLogClearDown");
	e.value = _("syslog clear");
	e = document.getElementById("syslogSysLogRefreshDown");
	e.value = _("syslog refresh");
	
	e = document.getElementById("help_head");
	e.innerHTML = _("help help_head");
	e = document.getElementById("syslog_direc");
	e.innerHTML = _("syslog syslog_direc");
}

function pageInit()
{
	initTranslation();
	updateLog();
}

function clearlogclick()
{
	document.getElementById("syslog").value = "";
	return ture;
}

function refreshlogclick()
{
	updateLog();
	return true;
}

</script>

</head>
<body onload="pageInit()"><center id="boxes">
	<div id="box">
	<div id="head"></div>	<!--end of head-->	
	<div id="content">


<table id="layout_table" border="0"><!--start of layout_table-->
<tr><!--start of layout tr-->
<td class="tdwidth1" id="td1"><!--start of td1-->
<div id="left"><!--start of left-->



<table class="body"><tr><td>
<h1 id="syslogTitle">System Log</h1>
<hr/><br/>
<!--<p id="syslogIntroduction"> Syslog: </p>-->

<!--<form method="post" name ="SubmitClearLog" action="/goform/clearlog">
<p align="center">
	<input type="button" value="Refresh" id="syslogSysLogRefresh" name="refreshlog" onclick="refreshlogclick();">
	<input type="submit" value="Clear" id="syslogSysLogClear" name="clearlog" onclick="clearlogclick();">
</p>
</form>
-->

<!-- ================= System log ================= -->
<table border="1" cellpadding="2" cellspacing="1" width="540">
<tr>
	<td class="title"colspan="2" id="syslogSysLog">System Log: </td>
</tr>
<tr><td>
		<textarea style=font-size:9pt name="syslog" id="syslog" cols="73" rows="30" wrap="off" readonly="1">
		</textarea>
	</td>
</tr>
</table>
<br>
<form method="post" name ="SubmitClearLog" action="/goform/clearlog">
<p align="center">
	<input type="button" value="Refresh" id="syslogSysLogRefreshDown" name="refreshlog" onclick="refreshlogclick();" style="width:55;height:25">
	<input type="submit" value="Clear" id="syslogSysLogClearDown" name="clearlog" onclick="clearlogclick();" style="width:55;height:25">
</p>
</form>
<br>
</td></tr></table>


</div><!--end of left-->
</td><!--end of td1-->

<td class="tdwidth2" id="td2"><!--start of td2-->
	<div id="right"><!--start of right-->
		<h2 id="help_head">Heeelp..</h2>
		
		<p id="help_content"><span id="syslog_direc"></span></p>
	</div><!--end of right-->
</td><!--end of td2-->
</tr><!--end of layout tr-->
</table><!--end of layout_table-->


</div><!--end of content-->
	</div>
	</center>
</body></html>
