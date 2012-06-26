<!-- Copyright 2004, Ralink Technology Corporation All Rights Reserved. -->
<html>
<head>
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
<META HTTP-EQUIV="Expires" CONTENT="-1">
<META http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<script type="text/javascript" src="/lang/b28n.js"></script>
<link rel="stylesheet" href="/style/normal_ws.css" type="text/css">
<title>Media Player</title>

<script language="JavaScript" type="text/javascript">
// Butterlate.setTextDomain("media");
var operate = '<% getMediaOpt(); %>';
var mode = '<% getMediaMode(); %>';
var option = '<% getMediaOption(); %>';
var target = '<% getMediaTarget(); %>';
var connected = '<% getMediaConnected(); %>';

function style_display_on()
{
	if (window.ActiveXObject)  // IE
	{
		return "block";
	}
	else if (window.XMLHttpRequest)  // Mozilla, Safari,...
	{
		return "table-row";
	}
}

function initTranslation()
{
	var e = document.getElementById("mediaTitle");
	e.innerHTML = _("media title");
	e = document.getElementById("stalistWirelessNet");
	e.innerHTML = _("stalist wireless network");
	e = document.getElementById("stalistMacAddr");
	e.innerHTML = _("stalist macaddr");
}

function PageInit()
{
	// initTranslation();

	if (connected == "load")
		setTimeout("this.location.reload()", 1000*2);
	document.media_player.service_mode[0].checked = false;
	document.media_player.service_mode[1].checked = false;
	document.media_player.service_mode[2].checked = false;
	document.media_player.inet_radio.disabled = true;
	document.media_player.play_mode[0].disabled = true;
	document.media_player.play_mode[1].disabled = true;
	document.media_player.radio_url.disabled = true;
	document.getElementById("div_track_list").style.visibility = "hidden";
	document.getElementById("div_track_list").style.display = "none";

	if (operate == "play")
	{
		if (connected == "ok")
		{
			document.getElementById("displayImg").src = "images/display01.gif";	
			ImgChg('03a');
		}
		else if (connected == "load")
		{
			document.getElementById("displayImg").src = "images/display02.gif";	
			ImgChg('03a');
		}
		else if (connected == "fail")
		{
			document.getElementById("displayImg").src = "images/display00.gif";	
			ImgChg('03');
		}
	}
	else if (operate == "stop")
	{
		document.getElementById("displayImg").src = "images/display00.gif";
		ImgChg('05a');
	}

	if (mode == "0")
	{
		document.media_player.service_mode[0].checked = true;
		document.media_player.inet_radio.disabled = false;
		if (target.indexOf("hitfmkp") >= 0)
		{
			document.media_player.inet_radio.options.selectedIndex = 0;
		}
		else if (target.indexOf("hitfmtc") >= 0)
		{
			document.media_player.inet_radio.options.selectedIndex = 1;
		}
		else if (target.indexOf("FM917") >= 0)
		{
			document.media_player.inet_radio.options.selectedIndex = 2;
		}
		else if (target.indexOf("fm997") >= 0)
		{
			document.media_player.inet_radio.options.selectedIndex = 3;
		}
		else if (target.indexOf("FM1077") >= 0)
		{
			document.media_player.inet_radio.options.selectedIndex = 4;
		}
		else if (target.indexOf("kmfa.m3u") >= 0)
		{
			document.media_player.inet_radio.options.selectedIndex = 5;
		}
		else if (target.indexOf("rtl-high.m3u") >= 0)
		{
			document.media_player.inet_radio.options.selectedIndex = 6;
		}
		else if (target.indexOf("fm4_live") >= 0)
		{
			document.media_player.inet_radio.options.selectedIndex = 7;
		}
		else if (target.indexOf("oe1-news") >= 0)
		{
			document.media_player.inet_radio.options.selectedIndex = 8;
		}
		else if (target.indexOf("radiowien_live") >= 0)
		{
			document.media_player.inet_radio.options.selectedIndex = 9;
		}
	}
	else if (mode == "1")
	{
		document.media_player.service_mode[1].checked = true;
		document.media_player.play_mode[0].disabled = false;
		document.media_player.play_mode[1].disabled = false;
		if (option == "random")
		{
			document.media_player.play_mode[0].checked = true;
		}
		else if (option == "sequence")
		{
			document.media_player.play_mode[1].checked = true;
		}
		document.getElementById("div_track_list").style.visibility = "visible";
		document.getElementById("div_track_list").style.display = style_display_on();
	}
	else if (mode == "2")
	{
		document.media_player.service_mode[2].checked = true;
		document.media_player.radio_url.disabled = false;
		document.media_player.radio_url.value = target;
	}
}

function checkValue()
{
	if (operate == "play" && connected == "ok")
	{
		alert("Please turn off running media");
		return false;
	}
	if (document.media_player.service_mode[0].checked != true &&
			document.media_player.service_mode[1].checked != true &&
			document.media_player.service_mode[2].checked != true)
	{
		alert("Please Choose one Service Mode");
		return false;
	}

	return true;
}


function submit_apply(opt)
{
	document.media_player.hidden_opt.value = opt;

	if (opt == "play" && !checkValue())
		return false;
	document.media_player.submit();	
	this.blur();
	
	return true;
}

function ImgChg(num)
{
	switch(num)
	{
		case "03":
			document.getElementById("playt").src = "images/media03.gif";
			break;
		case "03a":
			document.getElementById("playt").src = "images/media03a.gif";
			break;
		case "05":
			document.getElementById("stopt").src = "images/media05.gif";
			break;
		case "05a":
			document.getElementById("stopt").src = "images/media05a.gif";
			break;
	}
}

function switch_service()
{
	document.media_player.inet_radio.disabled = true;
	document.media_player.play_mode[0].disabled = true;
	document.media_player.play_mode[1].disabled = true;
	document.media_player.radio_url.disabled = true;
	document.getElementById("div_track_list").style.visibility = "hidden";
	document.getElementById("div_track_list").style.display = "none";

	if (document.media_player.service_mode[0].checked == true)
	{
		document.media_player.inet_radio.disabled = false;
	}
	else if (document.media_player.service_mode[1].checked == true)
	{
		document.media_player.play_mode[0].disabled = false;
		document.media_player.play_mode[1].disabled = false;
		document.getElementById("div_track_list").style.visibility = "visible";
		document.getElementById("div_track_list").style.display = style_display_on();
	}
	else if (document.media_player.service_mode[2].checked == true)
	{
		document.media_player.radio_url.disabled = false;
	}
}

function list_submit(parm)
{
	document.player_list.list_opt.value = parm;
	document.player_list.submit();	

}
</script>
</head>

<body onLoad="PageInit()">
<table class="body"><tr><td>

<h1 id="mediaTitle">Media Player</h1>
<p id="mediaIntroduction"></p>
<hr />

<form method=post name=media_player action="/goform/mediaPlayer">
  <input type=hidden name=hidden_opt value="">
  <input type=hidden name=hidden_vol value="30">
  <table width="540" border="1" cellspacing="1" cellpadding="3" bordercolor="#9BABBD">
    <tr> 
      <td class="title" rowspan="4">
        <img id="displayImg" name="displayImg" src="images/display00.gif"/>
      </td>
      <td class="title" id="">Service Mode</td>
      <td class="title" id="">Option</td>
    </tr>
    <tr>
      <td class="head">
	<input type="radio" name="service_mode" onClick="switch_service()" value="0">Internet Radio
      </td>
      <td>
	<select name="inet_radio">
	  <option value="http://radio.gigigaga.com/hitfmkp">FM 90.1 HitFM(Kaohsiung)
	  <option value="http://radio.gigigaga.com/hitfmtc">FM 91.5 HitFM(Taichung)
	  <option value="http://radio.gigigaga.com/FM917">FM 91.7 HitFM(Taipei)
	  <option value="http://radio.gigigaga.com/fm997">FM 99.7 e-classical
	  <option value="http://radio.gigigaga.com/FM1077">FM 107.7 HitoRadio
	  <option value="http://www.publicbroadcasting.net/kmfa/ppr/kmfa.m3u">KMFA
	  <option value="http://www.89.0rtl.de/webradio/rtl-high.m3u">89.0 RTL
	  <option value="mms://stream1.orf.at/fm4_live">FM4
	  <option value="mms://stream4.orf.at/oe1-news">oe1 Inforadio
	  <option value="mms://stream4.orf.at/radiowien_live">Radio Wien
	</select>
      </td>
    </tr>
    <tr>
      <td class="head">
	<input type="radio" name="service_mode" onClick="switch_service()" value="1">MP3
      </td>
      <td>
	<input type="radio" name="play_mode" value="random">Random
	<input type="radio" name="play_mode" value="sequence" checked>Sequence
      </td>
    </tr>
    <tr>
      <td class="head">
	<input type="radio" name="service_mode" onClick="switch_service()" value="2">URL
      </td>
      <td>
	<input type="text" size="20" maxlength="1024" name="radio_url">
      </td>
    </tr>
  </table>
  <br />
  <table width="240" border="0" cellspacing="0" cellpadding="0">
    <tr>
      <td width="50" nowrap><img src="images/media01.gif" width="50" height="40" /></td>
      <td width="35" nowrap><img alt="Play" src="images/media03.gif" width="35" height="40" name="playt" id="playt" onClick="return submit_apply('play');" /></td>
      <td width="40" nowrap><img src="images/media05.gif" alt="Stop" width="40" height="40" name="stopt" id="stopt" onClick="return submit_apply('stop');" /></td>
      <td width="60" nowrap><img src="images/media07.gif" width="60" height="40" /></td>
      <td width="5" nowrap><img src="images/media08.gif" width="5" height="40" /></td>
      <td width="62" nowrap><img src="images/media11.gif" width="62" height="40" /></td>
    </tr>
  </table>
</form>
<br />
<form method=post name=player_list action="/goform/mediaPlayerList">
  <input type=hidden name=list_opt value="">
  <table id="div_track_list" width="540" border="1" cellspacing="1" cellpadding="3" bordercolor="#9BABBD">
    <tr>
      <td class="title">UnSelected List</td>
      <td class="title"><br /></td>
      <td class="title">Selected List</td>
    </tr>
    <tr>
      <td>
        <select name="unselected_list" size=20 style="width:250px" multiple>
	  <% showUnSelectedList(); %>
	</select>
      </td>
      <td>
        <input type="button" style="{width:30px;}" value=">>" name="add_list" onClick="list_submit('add')"><br />
        <input type="button" style="{width:30px;}" value="<<" name="del_list" onClick="list_submit('del')"><br />
      </td>
      <td>
        <select name="selected_list" size=20 style="width:250px" multiple>
	  <% showSelectedList(); %>
	</select>
      </td>
    </tr>
  </table>
</form>

</td></tr></table>
</body>
</html>
