<html>
<head>
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
<META HTTP-EQUIV="Expires" CONTENT="-1">
<META http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<script type="text/javascript" src="/lang/b28n.js"></script>
<title>Settings Management</title>
<link rel="stylesheet" href="/css/normal_ws.css" type="text/css">
<link rel="stylesheet" href="/css/boxStyle.css" type="text/css">

<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("admin");

function initTranslation()
{
	var e = document.getElementById("setmanTitle");
	e.innerHTML = _("setman title");
	e = document.getElementById("setmanIntroduction");
	e.innerHTML = _("setman introduction");

	e = document.getElementById("setmanExpSet");
	e.innerHTML = _("setman export setting");
	e = document.getElementById("setmanExpSetButton");
	e.innerHTML = _("setman export setting button");
	e = document.getElementById("setmanExpSetExport");
	e.value = _("setman export setting export");

	e = document.getElementById("setmanImpSet");
	e.innerHTML = _("setman import setting");
	e = document.getElementById("setmanImpSetFileLocation");
	e.innerHTML = _("setman import setting file location");
	e = document.getElementById("setmanImpSetImport");
	e.value = _("setman import setting import");
	e = document.getElementById("setmanImpSetCancel");
	e.value = _("admin cancel");

	e = document.getElementById("setmanLoadFactDefault");
	e.innerHTML = _("setman load factory default");
	e = document.getElementById("setmanLoadFactDefaultButton");
	e.innerHTML = _("setman load factory default button");
	e = document.getElementById("setmanLoadDefault");
	e.value = _("setman load default");
}

function PageInit()
{
	initTranslation();
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
<h1 id="setmanTitle">Settings Management</h1>
<p style="display:none" id="setmanIntroduction">You might save system settings by exporting them to a configuration file, restore them by importing the file, or reset them to factory default.</p>
<hr />

<!-- ================= Export ================= -->
<br />
<form method="post" name="ExportSettings" action="/cgi-bin/ExportSettings.sh">
<table width="540" border="0" cellspacing="1" cellpadding="3" bordercolor="#9BABBD">
  <tr>
    <td class="title" colspan="2" id="setmanExpSet">Export Settings</td>
  </tr>
  <tr>
    <td class="head" id="setmanExpSetButton">Export Button</td>
    <td><input value="Export" id="setmanExpSetExport" name="Export"  type="submit"></td>
  </tr>
</table>
</form>
<br />

<!-- ================= Import ================= -->
<form method="post" name="ImportSettings" action="/cgi-bin/upload_settings.cgi" enctype="multipart/form-data">
<table width="540" border="0" cellspacing="1" cellpadding="3" bordercolor="#9BABBD">
  <tr>
      <td class="title" colspan="2" id="setmanImpSet">Import Settings</td>
    </tr>
    <tr>
      <td class="head" id="setmanImpSetFileLocation">Settings file location</td>
      <td><input type="File" name="filename" size="20" maxlength="256"></td>
    </tr>
  </table>
<table width="540" border="0" cellpadding="2" cellspacing="1">
  <tr align="center">
    <td>
      <input type=submit  value="Import" id="setmanImpSetImport" onClick="return AdmFormCheck()"> &nbsp; &nbsp;
      <input type=reset   value="Cancel" id="setmanImpSetCancel">
    </td>
  </tr>
</table>
</form>
<br />

<!-- ================= Load FactoryDefaults  ================= -->
<form method="post" name="LoadDefaultSettings" action="/goform/LoadDefaultSettings">
<table width="540" border="0" cellspacing="1" cellpadding="3" bordercolor="#9BABBD">
  <tr>
    <td class="title" colspan="2" id="setmanLoadFactDefault">Load Factory Defaults</td>
  </tr>
  <tr>
    <td class="head" id="setmanLoadFactDefaultButton">Load Default Button</td>
    <td><input value="Load Default" id="setmanLoadDefault" name="LoadDefault"  type="submit"></td>
  </tr>
</table>
</form>


<br>
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
