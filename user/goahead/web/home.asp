<HTML>
<!-- Copyright (c) Go Ahead Software Inc., 1994-2000. All Rights Reserved. -->
<HEAD>
<TITLE>3G-WIFI Router</TITLE>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html">
<script language="JavaScript" type="text/javascript">
function initLanguage()
{
	var lang = "<% getCfgGeneral(1, "Language"); %>";
	var cook = "en";
	var lang_en = "<% getLangBuilt("en"); %>";
	var lang_zhtw = "<% getLangBuilt("zhtw"); %>";
	var lang_zhcn = "<% getLangBuilt("zhcn"); %>";

	if (document.cookie.length > 0) {
		var s = document.cookie.indexOf("language=");
		var e = document.cookie.indexOf(";", s);
		if (s != -1) {
			if (e == -1)
				cook = document.cookie.substring(s+9);
			else
				cook = document.cookie.substring(s+9, e);
		}
	}

	if (lang == "en") {
		document.cookie="language=en; path=/";
		if (cook != lang)
			window.location.reload();
		if (lang_en != "1") {
			if (lang_zhtw == "1") {
				document.cookie="language=zhtw; path=/";
				window.location.reload();
			}
			else if (lang_zhcn == "1") {
				document.cookie="language=zhcn; path=/";
				window.location.reload();
			}
		}
	}
	else if (lang == "zhtw") {
		document.cookie="language=zhtw; path=/";
		if (cook != lang)
			window.location.reload();
		if (lang_zhtw != "1") {
			if (lang_en == "1") {
				document.cookie="language=en; path=/";
				window.location.reload();
			}
			else if (lang_zhcn == "1") {
				document.cookie="language=zhcn; path=/";
				window.location.reload();
			}
		}
	}
	else if (lang == "zhcn") {
		document.cookie="language=zhcn; path=/";
		if (cook != lang)
			window.location.reload();
		if (lang_zhcn != "1") {
			if (lang_en == "1") {
				document.cookie="language=en; path=/";
				window.location.reload();
			}
			else if (lang_zhtw == "1") {
				document.cookie="language=zhtw; path=/";
				window.location.reload();
			}
		}
	}
}

function onInit()
{
	initLanguage();
	//<% FirmwareUpgradePostASP(); %>
}

</script>
</HEAD>

<FRAMESET ROWS="70,1*" COLS="*" BORDER="0" FRAMESPACING="0" FRAMEBORDER="NO"  onLoad="onInit()"> 

  	<FRAME SRC="title.asp" NAME="title" FRAMEBORDER="NO" SCROLLING="NO" MARGINWIDTH="0" MARGINHEIGHT="0">

  	<FRAMESET COLS="180,1*"> 
    	<FRAME SRC="treeapp.asp" NAME="menu" MARGINWIDTH="0" MARGINHEIGHT="0" scrolling="AUTO" FRAMEBORDER="NO" >

    <FRAME SRC="adm/status.asp" NAME="view" SCROLLING="AUTO" MARGINWIDTH="0" TOPMARGIN="0" MARGINHEIGHT="0" FRAMEBORDER="NO">
	</FRAMESET>	
</FRAMESET>
<NOFRAMES>
<BODY BGCOLOR="#FFFFFF">

</BODY></NOFRAMES>
</HTML>
