<html>
<head>
<title></title>
<link rel="stylesheet" href="/css/normal_ws.css" type="text/css">
<link rel="stylesheet" href="/css/boxStyle.css" type="text/css">
<meta http-equiv="content-type" content="text/html;charset=gb2312" />
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("firewall");

function logoutJudge()
{
	var flag= "<% getCfgGeneral(1, "logout_flag"); %>";

	if("1"==flag||""==flag)
		{
			top.location="../login.asp"
		}
	
	else
		{
			//do nothing
		}

}

var TOTAL_ROW = 8;

var MAX_RULES = 20;
var rules_num = <% getIPSECASPRuleNumsASP(); %> ;

function formcheck(){

 if (rules_num>=MAX_RULES)

 	{
	alert(_("invalid rules_num") );
	 return false;
 	}
	<% clean_VpnConfigEditSign();%>
	window.location.replace('./ipsconfig.asp');
 	 return true;

}


function isValidPort(p)
{
	var port = parseInt(p, 10);
   if ( isNaN(port) )
   	return false;

   if ( port < 1 || port > 65535)
       return false;

   return true;
}

function actionClick(act_type)
{
	document.IpsecAction.actionType.value = act_type;
	if(act_type!="edt")
	{
		startProgress(); 
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

function initTranslation()
{
	var e = document.getElementById("Ipsectitle");
	e.innerHTML = _("Ipsec title");
	
	e = document.getElementById("Ipsecintroduce");
	e.innerHTML = _("Ipsec introduction");
	
	e = document.getElementById("Ipseccfg_servicesel");
	e.innerHTML = _("Ipseccfg servicesel");
	
	e = document.getElementById("IPSECurrentrule");
	e.innerHTML = _("Ipsec Current Rule");
	
	e = document.getElementById("IPSECnum");
	e.innerHTML = _("firewall no");

	e = document.getElementById("IPSEC_states");
	e.innerHTML = _("IPSEC states");
	
	e = document.getElementById("IPSEC_name");
	e.innerHTML = _("IPSEC name");
	e = document.getElementById("IPSEC_remotgw");
	e.innerHTML = _("IPSEC remotgw");
	e = document.getElementById("IPSEC_localAdd");
	e.innerHTML = _("IPSEC localAdd");
	e = document.getElementById("IPSEC_remoteAdd");
	e.innerHTML = _("IPSEC remoteAdd");
	
	e = document.getElementById("idActionEnableRules");
	e.value= _("firewall enable");
	e = document.getElementById("idActionDisableRules");
	e.value = _("firewall disable");
	e = document.getElementById("idActionDelRules");
	e.value = _("firewall del");
	
	e = document.getElementById("button_edit");
	e.value= _("button edit");

	e = document.getElementById("idAppendApp");
	e.value = _("Ipsec append");
/*
	e = document.getElementById("vpn_status_enable");
	e.innerHTML = _("vpn_status_enable_display");

	e = document.getElementById("vpn_status_disable");
	e.innerHTML = _("vpn_status_disable_display");

	e = document.getElementById("option_service");
	e.innerHTML = _("option service");
	e = document.getElementById("option_client");
	e.innerHTML = _("option client");

*/




	e = document.getElementById("pptpsettings");
	e.innerHTML = _("id pptp settings");

	e = document.getElementById("pptp_active_str");
	e.innerHTML = _("id pptp active");

	e = document.getElementById("pptp_user_str");
	e.innerHTML = _("id pptp user");
	
	e = document.getElementById("pptp_password_str");
	e.innerHTML = _("id pptp password");

	e = document.getElementById("pptp_server_str");
	e.innerHTML = _("id pptp server");


	e = document.getElementById("pptp_lan_mask_str");
	e.innerHTML = _("id pptp lan mask");


	e = document.getElementById("pptp_option_dhcp");
	e.innerHTML = _("id pptp option dhcp");

		e = document.getElementById("pptp_option_static");
	e.innerHTML = _("id pptp option static");


	e = document.getElementById("local_pptp_ip");
	e.innerHTML = _("id pptp local ip");

	e = document.getElementById("pptp_mppe_encryption_str");
	e.innerHTML = _("id pptp mppe encryption");
	
	e = document.getElementById("pptp_mppe_40_str");
	e.innerHTML = _("id pptp mppe 40");

	e = document.getElementById("pptp_mppe_refuse_stateless_str");
	e.innerHTML = _("id pptp refuse stateless");

	e = document.getElementById("pptp_apply");
	e.value = _("id pptp apply");





	e = document.getElementById("l2tpsettings");
	e.innerHTML = _("id l2tp settings");

	e = document.getElementById("l2tp_active_str");
	e.innerHTML = _("id l2tp active");

	e = document.getElementById("l2tp_user_str");
	e.innerHTML = _("id l2tp user");
	
	e = document.getElementById("l2tp_password_str");
	e.innerHTML = _("id l2tp password");

	e = document.getElementById("l2tp_server_str");
	e.innerHTML = _("id l2tp server");


	e = document.getElementById("l2tp_lan_mask_str");
	e.innerHTML = _("id l2tp lan mask");


	e = document.getElementById("l2tp_option_dhcp");
	e.innerHTML = _("id l2tp option dhcp");

		e = document.getElementById("l2tp_option_static");
	e.innerHTML = _("id l2tp option static");


	e = document.getElementById("local_l2tp_ip");
	e.innerHTML = _("id l2tp local ip");

	e = document.getElementById("l2tp_mppe_encryption_str");
	e.innerHTML = _("id l2tp mppe encryption");
	
	e = document.getElementById("l2tp_mppe_40_str");
	e.innerHTML = _("id l2tp mppe 40");

	e = document.getElementById("l2tp_mppe_refuse_stateless_str");
	e.innerHTML = _("id l2tp refuse stateless");

	e = document.getElementById("l2tp_apply");
	e.value = _("id l2tp apply");
	
	e = document.getElementById("help_head");
	e.innerHTML = _("help help_head");
	e = document.getElementById("vpn_direc");
	e.innerHTML = _("vpn vpn_direc");
	
}








function updateState()
{
	if (rules_num > 0){
		setElementHide("idTblSelectedAction", false);
	}

	//logoutJudge();
	initTranslation();


	load_pptp_para();
	load_l2tp_para();




	pptp_localip_mode_sel_select();
}

//save bar
var counter = 20;

function startProgress(){
	document.getElementById("progressouter").style.display="block";
	savingbar();
}

function savingbar(){
	if (counter < 101){
   	document.getElementById("progressinner").style.width = counter+"%";
     	counter++;

		if(counter == 101)
			counter = 20;
     	setTimeout("savingbar()",200);
   }
}


function setElementHide(form_id, flag)
{
	var e = document.getElementById(form_id);
	if (e) {
		if (true == flag) {
			e.style.visibility = "hidden";
			e.style.display = "none";
		}else{
			e.style.visibility = "visible";
			e.style.display = display_on();
		}
	}
}





function isDigital(s)
{
    var r,re;
    re = /\d*/i;
    r = s.match(re);
    return (r==s)?1:0;
}

function getLeftMostZeroBitPos(num) {
   var i = 0;
   var numArr = [128, 64, 32, 16, 8, 4, 2, 1];

   for ( i = 0; i < numArr.length; i++ )
      if ( (num & numArr[i]) == 0 )
         return i;

   return numArr.length;
}

function getRightMostOneBitPos(num) {
   var i = 0;
   var numArr = [1, 2, 4, 8, 16, 32, 64, 128];

   for ( i = 0; i < numArr.length; i++ )
      if ( ((num & numArr[i]) >> i) == 1 )
         return (numArr.length - i - 1);

   return -1;
}


function isValidSubnetMask(mask) {
   var i = 0, num = 0;
   var zeroBitPos = 0, oneBitPos = 0;
   var zeroBitExisted = false;

//   if ( mask == '0.0.0.0' )
      //return false;

   var maskParts = mask.split('.');
   if ( maskParts.length != 4 ) return false;

   for (i = 0; i < 4; i++) {
      if ( isNaN(maskParts[i]) == true )
         return false;
      num = parseInt(maskParts[i]);
      if ( num < 0 || num > 255 )
         return false;
      if ( zeroBitExisted == true && num != 0 )
         return false;
      zeroBitPos = getLeftMostZeroBitPos(num);
      oneBitPos = getRightMostOneBitPos(num);
      if ( zeroBitPos < oneBitPos )
         return false;
      if ( zeroBitPos < 8 )
         zeroBitExisted = true;
   }

   return true;
}




function isValidNetAddress(address) {
   var i = 0;

 //  if ( address == '0.0.0.0' ||
  //      address == '255.255.255.255' )
     if(   address == '255.255.255.255' )
      return false;
   var addrParts = address.split('.');
   if ( addrParts.length != 4 ) return false;
   for (i = 0; i < 4; i++) {
      if (!isDigital(addrParts[i]))
         return false;
      var num = parseInt(addrParts[i]);
      if ( num < 0 || num >= 255 )
         return false;
      if (i == 0 && num >= 224 && num <= 239)
        return false;
   }
   return true;
}
function isValidIpAddress(address) {
   var i = 0;

   if ( address == '0.0.0.0' ||
        address == '255.255.255.255' )
      return false;
   var addrParts = address.split('.');
   if ( addrParts.length != 4 ) return false;
   for (i = 0; i < 4; i++) {
      if (!isDigital(addrParts[i]))
         return false;
      var num = parseInt(addrParts[i]);
      if ( num < 0 || num >= 255 )
         return false;
      if (i == 0 && num >= 224 && num <= 239)
        return false;
   }
   if(parseInt(addrParts[3]) == 0)
                return false;
   return true;
}








function showhide(element, flag)
{
        var e = document.getElementById(element);
        if (e) {
                if (0 == flag) {
                  //      e.style.visibility = "hidden";
                        e.style.display = "none";
                }else{
                  //      e.style.visibility = "visible";
                        e.style.display = "block";
                }
        }
}



function pptp_localip_mode_sel_select()
{

      var pptp_localip_mode = document.pptp.pptp_localip_mode[document.pptp.pptp_localip_mode.selectedIndex].value;

      switch (pptp_localip_mode) {
      case "dhcp":
                  showhide("id_pptp_local_ip", 0);
          break;
        case "static":
                  showhide("id_pptp_local_ip", 1);  
        break;
      default:;
      }
  }
function l2tp_localip_mode_sel_select()
{

      var l2tp_localip_mode = document.l2tp.l2tp_localip_mode[document.l2tp.l2tp_localip_mode.selectedIndex].value;

      switch (l2tp_localip_mode) {
      case "dhcp":
                  showhide("id_l2tp_local_ip", 0);
          break;
        case "static":
                  showhide("id_l2tp_local_ip", 1);  
        break;
      default:;
      }
  }

	
function load_pptp_para()
{
	var tmp;
	 tmp="<% getCfgGeneral(1,"pptp_active"); %>"
	 if(tmp=="on"){
		document.pptp.pptp_active.checked=true;
	 }else{
		document.pptp.pptp_active.checked=false;
	}

	tmp="<% getCfgGeneral(1,"pptp_user"); %>" 
	document.pptp.pptp_user.value=tmp;
	tmp="<% getCfgGeneral(1,"pptp_password"); %>" 
	document.pptp.pptp_password.value=tmp;
	tmp="<% getCfgGeneral(1,"pptp_server"); %>" 
	document.pptp.pptp_server.value=tmp;


	tmp="<% getCfgGeneral(1,"pptp_remote_lan"); %>" 
	document.pptp.pptp_remote_lan.value=tmp;
	tmp="<% getCfgGeneral(1,"pptp_remote_mask"); %>" 
	document.pptp.pptp_remote_mask.value=tmp;
	tmp="<% getCfgGeneral(1,"pptp_localip_mode"); %>" 
	if(tmp=="static")	document.pptp.pptp_localip_mode.selectedIndex=1;
	else document.pptp.pptp_localip_mode.selectedIndex=0;

	tmp="<% getCfgGeneral(1,"pptp_local_ip"); %>" 
	document.pptp.pptp_local_ip.value=tmp;
	
	tmp="<% getCfgGeneral(1,"pptp_mppe_encryption"); %>" 
	if(tmp=="on")	{
		document.pptp.pptp_mppe_encryption.checked=true;
		document.getElementById('pptp_mppe_advance_sel').style.visibility = "visible";
	}else{
		document.pptp.pptp_mppe_encryption.checked=false;
		document.getElementById('pptp_mppe_advance_sel').style.visibility = "hidden"; 

	}

	
	tmp="<% getCfgGeneral(1,"pptp_mppe_40"); %>" 
	if(tmp=="on")	document.pptp.pptp_mppe_40.checked=true;
	else document.pptp.pptp_mppe_40.checked=false;	
	tmp="<% getCfgGeneral(1,"pptp_mppe_refuse_stateless"); %>" 
	if(tmp=="on")	document.pptp.pptp_mppe_refuse_stateless.checked=true;
	else document.pptp.pptp_mppe_refuse_stateless.checked=false;	
	
}


function load_l2tp_para()
{
	var tmp;
	 tmp="<% getCfgGeneral(1,"l2tp_active"); %>"
	 if(tmp=="on"){
		document.l2tp.l2tp_active.checked=true;
	 }else{
		document.l2tp.l2tp_active.checked=false;
	}

	tmp="<% getCfgGeneral(1,"l2tp_user"); %>" 
	document.l2tp.l2tp_user.value=tmp;
	tmp="<% getCfgGeneral(1,"l2tp_password"); %>" 
	document.l2tp.l2tp_password.value=tmp;
	tmp="<% getCfgGeneral(1,"l2tp_server"); %>" 
	document.l2tp.l2tp_server.value=tmp;


	tmp="<% getCfgGeneral(1,"l2tp_remote_lan"); %>" 
	document.l2tp.l2tp_remote_lan.value=tmp;
	tmp="<% getCfgGeneral(1,"l2tp_remote_mask"); %>" 
	document.l2tp.l2tp_remote_mask.value=tmp;
	tmp="<% getCfgGeneral(1,"l2tp_localip_mode"); %>" 
	if(tmp=="static")	document.l2tp.l2tp_localip_mode.selectedIndex=1;
	else document.l2tp.l2tp_localip_mode.selectedIndex=0;

	tmp="<% getCfgGeneral(1,"l2tp_local_ip"); %>" 
	document.l2tp.l2tp_local_ip.value=tmp;
	
	tmp="<% getCfgGeneral(1,"l2tp_mppe_encryption"); %>" 
	if(tmp=="on")	{
		document.l2tp.l2tp_mppe_encryption.checked=true;
		document.getElementById('l2tp_mppe_advance_sel').style.visibility = "visible";
	}else{
		document.l2tp.l2tp_mppe_encryption.checked=false;
		document.getElementById('l2tp_mppe_advance_sel').style.visibility = "hidden"; 

	}

	
	tmp="<% getCfgGeneral(1,"l2tp_mppe_40"); %>" 
	if(tmp=="on")	document.l2tp.l2tp_mppe_40.checked=true;
	else document.l2tp.l2tp_mppe_40.checked=false;	
	tmp="<% getCfgGeneral(1,"l2tp_mppe_refuse_stateless"); %>" 
	if(tmp=="on")	document.l2tp.l2tp_mppe_refuse_stateless.checked=true;
	else document.l2tp.l2tp_mppe_refuse_stateless.checked=false;	
	
}




function pptp_on_apply()
{
	if(document.pptp.pptp_active.checked==true){
		
		if(document.pptp.pptp_user.value==""){
			alert(_("please input user!"));
			document.pptp.pptp_user.focus();
			return false;
		}
		if(document.pptp.pptp_password.value==""){
			alert(_("please input password!"));
			document.pptp.pptp_password.focus();
			return false;
		}
		if(document.pptp.pptp_server.value==""){
			alert(_("please input server!"));
			document.pptp.pptp_server.focus();
			return false;
		}
		if(document.pptp.pptp_remote_lan.value==""){
			alert(_("please input remote lan!"));
			document.pptp.pptp_remote_lan.focus();
			return false;
		}
		if(isValidNetAddress(document.pptp.pptp_remote_lan.value)==false){
			alert(_("please input correct remote lan!"));
			document.pptp.pptp_remote_lan.focus();
			return false;	
		}

		if(document.pptp.pptp_remote_mask.value==""){
			alert(_("please input remote mask!"));
			document.pptp.pptp_remote_mask.focus();
			return false;
		}
		if(isValidSubnetMask(document.pptp.pptp_remote_mask.value)==false){
			alert(_("please input correct remote mask!"));
			document.pptp.pptp_remote_mask.focus();
			return false;	
		}
		if(document.pptp.pptp_localip_mode[document.pptp.pptp_localip_mode.selectedIndex].value=="static"){
			if(document.pptp.pptp_local_ip.value==""){
				alert(_("please input local ip!"));
				document.pptp.pptp_local_ip.focus();
				return false;
			}
			if(isValidNetAddress(document.pptp.pptp_local_ip.value)==false){
				alert(_("please input correct ip!"));
				document.pptp.pptp_local_ip.focus();
				return false;	
			}
		}		

		
	}
	return true;
}




function l2tp_on_apply()
{
	if(document.l2tp.l2tp_active.checked==true){
		
		if(document.l2tp.l2tp_user.value==""){
			alert(_("please input user!"));
			document.l2tp.l2tp_user.focus();
			return false;
		}
		if(document.l2tp.l2tp_password.value==""){
			alert(_("please input password!"));
			document.l2tp.l2tp_password.focus();
			return false;
		}
		if(document.l2tp.l2tp_server.value==""){
			alert(_("please input server!"));
			document.l2tp.l2tp_server.focus();
			return false;
		}
		if(document.l2tp.l2tp_remote_lan.value==""){
			alert(_("please input remote lan!"));
			document.l2tp.l2tp_remote_lan.focus();
			return false;
		}
		if(isValidNetAddress(document.l2tp.l2tp_remote_lan.value)==false){
			alert(_("please input correct remote lan!"));
			document.l2tp.l2tp_remote_lan.focus();
			return false;	
		}

		if(document.l2tp.l2tp_remote_mask.value==""){
			alert(_("please input remote mask!"));
			document.l2tp.l2tp_remote_mask.focus();
			return false;
		}
		if(isValidSubnetMask(document.l2tp.l2tp_remote_mask.value)==false){
			alert(_("please input correct remote mask!"));
			document.l2tp.l2tp_remote_mask.focus();
			return false;	
		}
		if(document.l2tp.l2tp_localip_mode[document.l2tp.l2tp_localip_mode.selectedIndex].value=="static"){
			if(document.l2tp.l2tp_local_ip.value==""){
				alert(_("please input local ip!"));
				document.l2tp.l2tp_local_ip.focus();
				return false;
			}
			if(isValidNetAddress(document.l2tp.l2tp_local_ip.value)==false){
				alert(_("please input correct ip!"));
				document.l2tp.l2tp_local_ip.focus();
				return false;	
			}
		}		

		
	}
	return true;
}





</script>
</head>


<!--     body      -->
<body onLoad="updateState()">
<center id="boxes">
	<div id="box">
	<div id="head"></div>	<!--end of head-->	
	<div id="content">

	<table id="layout_table" border="0"><!--start of layout_table-->
<tr><!--start of layout tr-->
<td class="tdwidth1" id="td1"><!--start of td1-->
<div id="left"><!--start of left-->


<table class="body">
<tr><td>
<h1 id="Ipsectitle" style="display: none;">IPSEC   Settings </h1>
<h1 align="left">Ipsec</h1>
<% checkIfUnderBridgeModeASP(); %>
<p id="Ipsecintroduce" style="display: none;"> IPSEC</p>
<hr />
<!-- 20090505 Gord Add Saving Bar-->
<br>
<div id="progressouter" style="width: 500px; height: 20px; border: 6px; display:none;">
   <div id="progressinner" style="position: relative; height: 20px; background-color: #2C5EA4;
   width: 20%; font-size:15px; font-weight:bolder; color:#FFFFF0; ">Saving Now...</div>
</div>
<!--  delete rules -->
<!-- do action -->




<!--     IPSEC VPN      -->
<form name="IpsecAction" action="/goform/IpsecAction" method=POST >
  <table width="540" border="1" cellpadding="2" cellspacing="1">
    <tr>
		<td class="title" colspan="10" id="IPSECurrentrule">Ipsec Vpn List</td>
	</tr>

	<tr>
		<td align=center id="IPSECnum"> No.</td>
		<td align=center id="IPSEC_states"> states </td>
		<td align=center id="IPSEC_name"> name </td>
		<td align=center id="Ipseccfg_servicesel"> service mode </td>
		<td align=center id="IPSEC_remotgw"> Remote gateway </td>
		<td align=center id="IPSEC_localAdd"> local address</td>
		<td align=center id="IPSEC_remoteAdd"> remote address</td>
	</tr>
    
    <% showIPSECASP(); %>
    <tr id="idTblSelectedAction">
<!--	<td colspan='2' id="idinfoSelected">
		Selected Rule(s) setting:
      </td>
-->
	<td colspan='7' align=center >
		<input type="submit" value="Enable" id="idActionEnableRules" name="btnAction" onClick="return actionClick('en')">
		&nbsp;&nbsp;
		<input type="submit" value="Disable" id="idActionDisableRules" name="btnAction" onClick="return actionClick('dis')">
		&nbsp;&nbsp;
		&nbsp;&nbsp;
		&nbsp;&nbsp;
		&nbsp;&nbsp;
		&nbsp;&nbsp;
		<input type="submit" value="Delete" id="idActionDelRules" name="btnAction" onClick="return actionClick('del')">
		&nbsp;&nbsp;
		<input type="submit"  value="Edit" id="button_edit" name="btnAction" onClick="return actionClick('edt')">

      </td>
    </tr>
  </table>
  
  <br>

	<p>
	<table width="540" border="0" cellpadding="2" cellspacing="1">

	  <tr align="center">
	    <td>
	         <input type="button" value="Append" id="idAppendApp" name="appendApp" onClick="return formcheck();">
	         <input name="actionType" type="hidden" value="">
	    </td>
	  </tr>
	</table>
	</p>

</form>

<!--     PPTP VPN      -->

<h1 id="pptptitle" style="display: none;">PPTP </h1>
<form name="pptp" action="/goform/pptp_action" method=POST onSubmit="return pptp_on_apply()" style="display: none;">
	  <table width="540" border="0" cellpadding="2" cellspacing="1">
	  	<tr>
	  		<td class="title" colspan="10" id="pptpsettings">PPTP VPN Settings</td>
	  	</tr>
		
	  	<tr>
	  		<td  width="40%" id="pptp_active_str">PPTP VPN Active</td>
	  		<td>
	  			<input type="checkbox"  name="pptp_active">
	  		</td>
	  	</tr>

	  	<tr>
	  		<td width="40%" id="pptp_user_str">PPTP User</td>
	  		<td>
	  			<input type="text" maxlength=50 size=20 name="pptp_user">
	  		</td>
	  	</tr>

	  	<tr>
	  		<td width="40%" id="pptp_password_str">PPTP Password</td>
	  		<td>
	  			<input type="password" maxlength=50 size=20 name="pptp_password">
	  		</td>
	  	</tr>

	  	
	  	<tr>
	  		<td width="40%" id="pptp_server_str">PPTP Server</td>
	  		<td>
	  			<input type="text"  maxlength=50 size=20 name="pptp_server">
	  		</td>
	  	</tr>
	  	<tr>
	  		<td width="40%" id="pptp_lan_mask_str">Remote Lan/Mask</td>
	  		<td>
	  			<input type="text" maxlength=15 size=15 name="pptp_remote_lan">
	  			/
	  			<input type="text"  maxlength=15 size=15 name="pptp_remote_mask">
	  		</td>
	  	</tr>
	</table>
	  <table width="540" border="0" cellpadding="3" cellspacing="1">
	  	<tr>
	  		<td width="40%" id="local_pptp_ip">Local PPTP IP</td>
	  		<td width="20%">
				<select name="pptp_localip_mode" onChange="pptp_localip_mode_sel_select();" >
					<option id="pptp_option_dhcp" value="dhcp">dhcp</option>
					<option id="pptp_option_static" value="static">static</option>
				</select>
			</td>
			<td>
	  			<input type="text" id="id_pptp_local_ip" maxlength=15 size=15 name="pptp_local_ip">
			</td>
	  	</tr>
	</table>

 	 <table width="540" border="0" cellpadding="2" cellspacing="1">
	  	<tr>
	  		<td width="40%" id="pptp_mppe_encryption_str">MPPE Encryption</td>
	  		<td>
	  			<input type="checkbox"  name="pptp_mppe_encryption" onclick="document.getElementById('pptp_mppe_advance_sel').style.visibility = this.checked ? 'visible' : 'hidden'">
	  		</td>
	  	</tr>
	  </table>

	    <table id="pptp_mppe_advance_sel" width="540" border="0" cellpadding="2" cellspacing="1">

	  	<tr>
	  		<td width="40%" id="pptp_mppe_40_str">40 bit Encryption(Default is 128 bit)</td>
	  		<td>
	  			<input type="checkbox"  name="pptp_mppe_40">
	  		</td>
	  	</tr>
	  	<tr>
	  		<td width="40%" id="pptp_mppe_refuse_stateless_str">Refuse Stateless Encryption</td>
	  		<td>
	  			<input type="checkbox"  name="pptp_mppe_refuse_stateless">
	  		</td>
	  	</tr>
	  </table>
	  

	<p>
	<table width="540" border="0" cellpadding="2" cellspacing="1">

	  <tr align="center">
	    <td>
	         <input type="submit" value="apply" id="pptp_apply" name="pptp_apply" >
	    </td>
	  </tr>
	</table>
	</p>

	  
</form>











<!--     L2TP VPN      -->


<h1 id="l2tptitle" style="display: none;">L2TP </h1>
<form name="l2tp" action="/goform/l2tp_action" method=POST onSubmit="return l2tp_on_apply()" style="display: none;">
	  <table width="540" border="0" cellpadding="2" cellspacing="1">
	  	<tr>
	  		<td class="title" colspan="10" id="l2tpsettings">L2TP VPN Settings</td>
	  	</tr>
		
	  	<tr>
	  		<td  width="40%" id="l2tp_active_str">L2TP VPN Active</td>
	  		<td>
	  			<input type="checkbox"  name="l2tp_active">
	  		</td>
	  	</tr>

	  	<tr>
	  		<td width="40%" id="l2tp_user_str">L2TP User</td>
	  		<td>
	  			<input type="text" maxlength=50 size=20 name="l2tp_user">
	  		</td>
	  	</tr>

	  	<tr>
	  		<td width="40%" id="l2tp_password_str">L2TP Password</td>
	  		<td>
	  			<input type="password" maxlength=50 size=20 name="l2tp_password">
	  		</td>
	  	</tr>

	  	
	  	<tr>
	  		<td width="40%" id="l2tp_server_str">L2TP Server</td>
	  		<td>
	  			<input type="text"  maxlength=50 size=20 name="l2tp_server">
	  		</td>
	  	</tr>
	  	<tr>
	  		<td width="40%" id="l2tp_lan_mask_str">Remote Lan/Mask</td>
	  		<td>
	  			<input type="text" maxlength=15 size=15 name="l2tp_remote_lan">
	  			/
	  			<input type="text"  maxlength=15 size=15 name="l2tp_remote_mask">
	  		</td>
	  	</tr>
	</table>
	  <table width="540" border="0" cellpadding="3" cellspacing="1">
	  	<tr>
	  		<td width="40%" id="local_l2tp_ip">Local PPTP IP</td>
	  		<td width="20%">
				<select name="l2tp_localip_mode" onChange="l2tp_localip_mode_sel_select();" >
					<option id="l2tp_option_dhcp" value="dhcp">dhcp</option>
					<option id="l2tp_option_static" value="static">static</option>
				</select>
			</td>
			<td>
	  			<input type="text" id="id_l2tp_local_ip" maxlength=15 size=15 name="l2tp_local_ip">
			</td>
	  	</tr>
	</table>

 	 <table width="540" border="0" cellpadding="2" cellspacing="1">
	  	<tr>
	  		<td width="40%" id="l2tp_mppe_encryption_str">MPPE Encryption</td>
	  		<td>
	  			<input type="checkbox"  name="l2tp_mppe_encryption" onclick="document.getElementById('l2tp_mppe_advance_sel').style.visibility = this.checked ? 'visible' : 'hidden'">
	  		</td>
	  	</tr>
	  </table>

	    <table id="l2tp_mppe_advance_sel" width="540" border="0" cellpadding="2" cellspacing="1">

	  	<tr>
	  		<td width="40%" id="l2tp_mppe_40_str">40 bit Encryption(Default is 128 bit)</td>
	  		<td>
	  			<input type="checkbox"  name="l2tp_mppe_40">
	  		</td>
	  	</tr>
	  	<tr>
	  		<td width="40%" id="l2tp_mppe_refuse_stateless_str">Refuse Stateless Encryption</td>
	  		<td>
	  			<input type="checkbox"  name="l2tp_mppe_refuse_stateless">
	  		</td>
	  	</tr>
	  </table>
	  

	<p>
	<table width="540" border="0" cellpadding="2" cellspacing="1">

	  <tr align="center">
	    <td>
	         <input type="submit" value="apply" id="l2tp_apply" name="l2tp_apply" >
	    </td>
	  </tr>
	</table>
	</p>

	  
</form>

  </td>
</tr></table>

</div><!--end of left-->
</td><!--end of td1-->

<td class="tdwidth2" id="td2"><!--start of td2-->
	<div id="right"><!--start of right-->
		<h2 id="help_head">Heeelp...</h2>
		
		<p id="help_content"><span id="vpn_direc"></span></p>
	</div><!--end of right-->
</td><!--end of td2-->
</tr><!--end of layout tr-->
</table><!--end of layout_table-->


</div><!--end of content-->
	</div>
	</center>
</body>
</html>
