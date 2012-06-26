<html><head><title>Upload Firmware</title>

<link rel="stylesheet" href="/css/normal_ws.css" type="text/css">
<link rel="stylesheet" href="/css/boxStyle.css" type="text/css">
<meta http-equiv="content-type" content="text/html; charset=iso-8859-1">
<script type="text/javascript" src="/js/b28n.js"></script>
<style type="text/css">
<!--
#loading {
       width: 250px;
       height: 200px;
       background-color: #3399ff;
       position: absolute;
       left: 50%;
       top: 50%;
       margin-top: -150px;
       margin-left: -250px;
       text-align: center;
}
-->
</style>

<script language="JavaScript" type="text/javascript">
document.write('<div id="loading" style="display: none;"><br><br><br>Uploading firmware <br><br> Please be patient and don\'t remove usb device if it presented...</div>');
Butterlate.setTextDomain("admin");

var storageb = '<% getStorageBuilt(); %>';
var isStorageBuilt = <% getStorageBuilt(); %>;

var firmware_path = "";

var secs
var timerID = null
var timerRunning = false
var timeout = 3
var delay = 1000

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

function InitializeTimer(){
	if(!isStorageBuilt)
		return;
    // Set the length of the timer, in seconds
    secs = timeout
    StopTheClock()
    StartTheTimer()
}

function StopTheClock(){
    if(timerRunning)
        clearTimeout(timerID)
    timerRunning = false
}

function StartTheTimer(){
	if(!isStorageBuilt)
		return;

    if (secs==0){
        StopTheClock()

        timerHandler();

        secs = timeout
        StartTheTimer()
    }else{
        self.status = secs
        secs = secs - 1
        timerRunning = true
        timerID = self.setTimeout("StartTheTimer()", delay)
    }
}

function timerHandler(){
	if(!isStorageBuilt)
		return;

	makeRequest("/goform/storageGetFirmwarePath", "n/a");
}

var http_request = false;
function makeRequest(url, content) {
	if(!isStorageBuilt)
		return;

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
    http_request.open('GET', url, true);
    http_request.send(content);
}

function alertContents() {
	if(!isStorageBuilt)
		return;

	if (http_request.readyState == 4) {
		if (http_request.status == 200) {
			updateStorageStatus( http_request.responseText);
		} else {
			//alert('There was a problem with the request.');
		}
	}
}

function stripPath(str)
{

}

function updateStorageStatus(str)
{
	if(!isStorageBuilt)
		return;

	if(firmware_path == str)
		return;

	firmware_path = str;
	var paths = new Array();
	paths = firmware_path.split("\n");

	if(paths.length){
		document.UploadFirmwareUSB.firmware_path.length = 0;
		for(var i=0; i<paths.length; i++){
			var j = document.UploadFirmwareUSB.firmware_path.options.length;
			document.UploadFirmwareUSB.firmware_path.options[j] = new Option(paths[i].substring(12), paths[i], false, false);
		}
	}
}

var _singleton = 0;
function uploadFirmwareCheck()
{
	if(_singleton)
		return false;
	if(document.UploadFirmware.filename.value == ""){
		alert("Firmware Upgrade: Please specify a file.");
		return false;
	}

	StopTheClock();

	//document.UploadFirmware.UploadFirmwareSubmit.disabled = true;
	//document.UploadFirmware.filename.disabled = true;
	document.UploadBootloader.UploadBootloaderSubmit.disabled = true;
	document.ScanUSBFirmware.UploadFirmwareUSBScan.disabled = true;
	document.UploadFirmwareUSB.firmware_path.disabled = true;
	document.UploadFirmwareUSB.UploadFirmwareUSBSubmit.disabled = true;
	document.ForceMemUpgrade.ForceMemUpgradeSelect.disabled = true;
	document.ForceMemUpgrade.ForceMemUpgradeSubmit.disabled = true;

    document.getElementById("loading").style.display="block";
	parent.menu.setUnderFirmwareUpload(1);
	_singleton = 1;
	return true;
}

function uploadFirmwareUSBCheck()
{
	if(_singleton)
		return false;
	if(!firmware_path.length){
		alert("No firmware is selected.");
		return false;
	}
	StopTheClock();

	document.UploadFirmware.UploadFirmwareSubmit.disabled = true;
	document.UploadBootloader.UploadBootloaderSubmit.disabled = true;
	//document.UploadFirmwareUSB.UploadFirmwareUSBSubmit.disabled = true;
	document.ForceMemUpgrade.ForceMemUpgradeSelect.disabled = true;
	document.ForceMemUpgrade.ForceMemUpgradeSubmit.disabled = true;

    document.getElementById("loading").style.display="block";
	parent.menu.setUnderFirmwareUpload(1);
	_singleton = 1;
	return true;
}

function formBootloaderCheck()
{
	ret = confirm("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\nThis is for engineer only. Are u sure?\n\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	if(!ret)
		return false;

	if(_singleton)
		return false;
	if(document.UploadBootloader.filename.value == ""){
		alert("Bootloader Upgrade: Please specify a file.");
		return false;
	}

	StopTheClock();

	document.UploadFirmware.filename.disabled = true;
	document.UploadFirmware.UploadFirmwareSubmit.disabled = true;
	//document.UploadBootloader.UploadBootloaderSubmit.disabled = true;
	document.ScanUSBFirmware.UploadFirmwareUSBScan.disabled = true;
	document.UploadFirmwareUSB.firmware_path.disabled = true;
	document.UploadFirmwareUSB.UploadFirmwareUSBSubmit.disabled = true;
	document.ForceMemUpgrade.ForceMemUpgradeSelect.disabled = true;
	document.ForceMemUpgrade.ForceMemUpgradeSubmit.disabled = true;


	document.getElementById("loading").style.display="block";
	parent.menu.setUnderFirmwareUpload(1);
	_singleton = 1;
	return true;
}

function initTranslation()
{
	var e = document.getElementById("uploadTitle");
	e.innerHTML = _("upload title");
	e = document.getElementById("uploadIntroduction1");
	e.innerHTML = _("upload introduction1");
	e = document.getElementById("uploadIntroduction2");
	e.innerHTML = _("upload introduction2");

	e = document.getElementById("uploadFW");
	e.innerHTML = _("upload firmware");
	e = document.getElementById("uploadFWLocation");
	e.innerHTML = _("upload firmware location");
	e = document.getElementById("uploadFWApply");
	e.value = _("admin apply");

	e = document.getElementById("uploadFWFromUSB");
	e.innerHTML = _("upload firmware from usb");
	e = document.getElementById("uploadFWUSBLocation");
	e.innerHTML = _("upload firmware location");
	e = document.getElementById("uploadFWUSBScan");
	e.value = _("admin scan");
	e = document.getElementById("uploadFWUSBApply");
	e.value = _("admin apply");

	e = document.getElementById("uploadBoot");
	e.innerHTML = _("upload bootloader");
	e = document.getElementById("uploadBootLocation");
	e.innerHTML = _("upload bootloader location");
	e = document.getElementById("uploadBootApply");
	e.value = _("admin apply");

	e = document.getElementById("ForceUpgradeViaMem");
	e.innerHTML = _("upload firmware force upgrade via mem");
	e = document.getElementById("Force");
	e.innerHTML = _("upload firmware force");
	e = document.getElementById("ForceMemUpgradeSubmit");
	e.value = _("admin apply");
}

function pageInit(){
	initTranslation();

	if(isStorageBuilt){		
		makeRequest("/goform/storageGetFirmwarePath", "n/a");
		InitializeTimer();
	}

	if(<% getCfgZero(1, "Force_mem_upgrade"); %>)
		document.ForceMemUpgrade.ForceMemUpgradeSelect.options.selectedIndex = 1;
	else
		document.ForceMemUpgrade.ForceMemUpgradeSelect.options.selectedIndex = 0;

    document.UploadFirmware.UploadFirmwareSubmit.disabled = false;
    if (storageb == "1")
    {
	    document.getElementById("uploadFWUSBTable").style.visibility = "visible";
	    document.getElementById("uploadFWUSBTable").style.display = style_display_on();
	    document.getElementById("uploadFWUSBSubmit").style.visibility = "visible";
	    document.getElementById("uploadFWUSBSubmit").style.display = style_display_on();
	    document.getElementById("div_memupgrade").style.visibility = "visible";
	    document.getElementById("div_memupgrade").style.display = style_display_on();
	    document.getElementById("ForceMemUpgradeSubmit").style.visibility = "visible";
	    document.getElementById("ForceMemUpgradeSubmit").style.display = style_display_on();
	    document.ForceMemUpgrade.ForceMemUpgradeSubmit.disabled = false;
    }
    else
    {
	    document.getElementById("uploadFWUSBTable").style.visibility = "hidden";
	    document.getElementById("uploadFWUSBTable").style.display = "none";
	    document.getElementById("uploadFWUSBSubmit").style.visibility = "hidden";
	    document.getElementById("uploadFWUSBSubmit").style.display = "none";
	    document.getElementById("div_memupgrade").style.visibility = "hidden";
	    document.getElementById("div_memupgrade").style.display = "none";
	    document.getElementById("ForceMemUpgradeSubmit").style.visibility = "hidden";
	    document.getElementById("ForceMemUpgradeSubmit").style.display = "none";
	    document.ForceMemUpgrade.ForceMemUpgradeSubmit.disabled = true;
    }
    document.UploadBootloader.UploadBootloaderSubmit.disabled = false;

//	document.UploadFirmware.filename.disabled = false;
	document.getElementById("loading").style.display="none";


}
</script></head><body onLoad="pageInit()">
<center id="boxes">
	<div id="box">
	<div id="head"></div>	<!--end of head-->	
	<div id="content">


	<table id="layout_table" border="0"><!--start of layout_table-->
<tr><!--start of layout tr-->
<td class="tdwidth1" id="td1"><!--start of td1-->
<div id="left"><!--start of left-->



<table class="body"><tbody><tr><td>
<h1 id="uploadTitle">Upgrade Firmware</h1>
<p>
	<font id="uploadIntroduction1" style="display: none">Upgrade the Ralink SoC firmware to obtain new functionality. </font>
	<font id="uploadIntroduction2" color="#ff0000"><br/>><br/>It takes about 1 minute to upload &amp; upgrade flash and be patient please. Caution! A corrupted image will hang up the system.</font> 
</p>
<hr/><br/>
<!-- ----------------- Upload firmware Settings ----------------- -->
<form method="post" name="UploadFirmware" action="/cgi-bin/upload.cgi" enctype="multipart/form-data">
<table border="0" cellpadding="2" cellspacing="1" width="540">
<tbody><tr>
  <td class="title" colspan="2" id="uploadFW">Update Firmware</td>
</tr>
<tr>
  <td class="head" id="uploadFWLocation">Location:</td>
	<td> <input name="filename" size="20" maxlength="256" type="file"> </td>
</tr>
</tbody></table>
<p align="center">
<input value="Apply" id="uploadFWApply" name="UploadFirmwareSubmit" type="submit" onClick="return uploadFirmwareCheck();"> &nbsp;&nbsp;
</p>
</form>

<form method="get" name="UploadFirmwareUSB" action="/cgi-bin/usb_upgrade.cgi">
<table id="uploadFWUSBTable" name="uploadFWUSBTable" border="0" cellpadding="2" cellspacing="1" width="540">
<tbody><tr>
  <td class="title" colspan="2" id="uploadFWFromUSB">Update Firmware From USB Disk</td>
</tr>
<tr>
  <td class="head" id="uploadFWUSBLocation">Location:</td>
  <td>
	<select name="firmware_path" size="1">
		<!-- ....Javascript will update these options.... -->
	</select>                        
  </td>
</tr>
</tbody></table>
<table border="0" id="uploadFWUSBSubmit" name="uploadFWUSBSubmit">
  <tr>
    <td >
      <input value="Apply" id="uploadFWUSBApply" name="UploadFirmwareUSBSubmit" type="submit" onClick="return uploadFirmwareUSBCheck()">
    </td>
</form>
<form method="get" name="ScanUSBFirmware" action="/goform/ScanUSBFirmware">
    <td>
  <input value="Scan" id="uploadFWUSBScan" name="UploadFirmwareUSBScan" type="submit"> &nbsp;&nbsp;
    </td>
</form>
  </tr>
</table>



							<!------------Update Bootloader----------------->
<form style="display: none" method="post" name="UploadBootloader" action="/cgi-bin/upload_bootloader.cgi" enctype="multipart/form-data">
<table border="0" cellpadding="2" cellspacing="1" width="540">
<tbody><tr>
  <td class="title" colspan="2" id="uploadBoot">Update Bootloader</td>
</tr>
<tr>
  <td class="head" id="uploadBootLocation">Location:</td>
	<td> <input name="filename" size="20" maxlength="256" type="file"> </td>
</tr>
</tbody></table>
<p align="center">
<input value="Apply" id="uploadBootApply" name="UploadBootloaderSubmit" onclick="return formBootloaderCheck();" type="submit"> &nbsp;&nbsp;
</p>
</form>

<form method="post" name="ForceMemUpgrade" action="/goform/forceMemUpgrade">
<table border="0" id="div_memupgrade" cellpadding="2" cellspacing="1" width="95%">
<tbody><tr>
	<td class="title" colspan="2" id="ForceUpgradeViaMem">Force upgrade via memory</td>
</tr>
<tr>
	<td class="head" id="Force">Force:</td>
	<td>
		<select name="ForceMemUpgradeSelect" size="1">
			<option value="0">No</option>
			<option value="1">Yes</option>
		</select>                        
	</td>
</tr>
</tbody></table>
<p align="center">
<input value="Apply" id="ForceMemUpgradeSubmit" name="ForceMemUpgradeSubmit" type="submit"> &nbsp;&nbsp;
</p>
</form>

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
