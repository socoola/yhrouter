<html>
<head>
<title>ipsec</title>
<link rel="stylesheet" href="/style/normal_ws.css" type="text/css">
<meta http-equiv="content-type" content="text/html;charset=gb2312" />
<script type="text/javascript" src="/lang/b28n.js"></script>

<script language="JavaScript" type="text/javascript">

Butterlate.setTextDomain("firewall");   



var showAdv = 0;


function display_on()
{
  if(window.XMLHttpRequest){ // Mozilla, Firefox, Safari,...
    return "table-row";
  } else if(window.ActiveXObject){ // IE
    return "block";
  }
}


function showhide(element, flag)
{
        var e = document.getElementById(element);
        if (e) {
                if (0 == flag) {
                        e.style.visibility = "hidden";
                        e.style.display = "none";
                }else{
                        e.style.visibility = "visible";
                //        e.style.display = display_on();
                        e.style.display = "block";
                }
        }
}


function setSelect(item, value)
{
        for (i=0; i<item.options.length; i++) {
        if (item.options[i].value == value) {
                item.selectedIndex = i;
                break;
        }
    }
}

function submitText(item, name)
{
        return '&' + name + '=' + item.value;
}

function getSelect(item)
{
        var idx;
        if (item.options.length > 0) {
            idx = item.selectedIndex;
            return item.options[idx].value;
        }
        else {
                return '';
    }
}

function submitSelect(item, name)
{
        return '&' + name + '=' + getSelect(item);
}


function localIPModeChange() {
   with ( document.forms[0] ) {
      var mode = localIPMode[localIPMode.selectedIndex].value;
      switch ( mode ) {
      case "subnet":
        showhide("IP11", 1);
        showhide("IP12", 1);
        break; 
      case "single":
        showhide("IP11", 1);
        showhide("IP12", 0);
        break;
      case "any":
        showhide("IP11", 0);
        showhide("IP12", 0);
        break;

      default:
    }
  }
}

function remoteIPModeChange() {
   with ( document.forms[0] ) {
      var mode = remoteIPMode[remoteIPMode.selectedIndex].value;
      switch ( mode ) {
      case "subnet":
        showhide("IP21", 1);
        showhide("IP22", 1);
	showhide("div_policies_remote_lan", 0);
        break; 
      case "single":
        showhide("IP21", 1);
        showhide("IP22", 0);
	showhide("div_policies_remote_lan", 0);
        break;
      case "any":
        showhide("IP21", 0);
        showhide("IP22", 0);
	showhide("div_policies_remote_lan", 1);
        break;
      default:
    }
  }
}


function keyauthChange() {
   with ( document.forms[0] ) {
      var keymode = keyExM[keyExM.selectedIndex].value;
      var authmode = authM[authM.selectedIndex].value;
      switch (keymode) {
      case "auto":
        showhide("AutoKeyEx", 1);
        showhide("ManualKey", 0);
        switch (authmode) {
        case "pre_shared_key":
          showhide("PSK", 1);
          showhide("Certificate", 0);
          break;
        case "certificate":
          showhide("PSK", 0);
          showhide("Certificate", 1);
          break;        
        default:       
        }
        break; 
      case "manual":
        showhide("AutoKeyEx", 0);
        showhide("ManualKey", 1);
        showhide("PSK", 0);
        showhide("Certificate", 0);
                showAdv = 0;
                showhideAdv(showAdv);
        break;
      default:
      }
  }
}


function service_mode_change() {
   with ( document.forms[0] ) {
      var service_mode = serviceselbox[serviceselbox.selectedIndex].value;

      switch (service_mode) {
      case "service":
                  showhide("div_remoteGWaddr", 0);
                  showhide("div_link_mode_sel", 0);
                          
          break;
        case "client":
              showhide("div_remoteGWaddr", 1);
                showhide("div_link_mode_sel", 1);
        break;
      default:
      }
  }
}


function switchAdv() {
   with ( document.forms[0] ) {
     showAdv = !showAdv;
     showhideAdv(showAdv);
   }
}

function showhideAdv(show) {
   with ( document.forms[0] ) {
     showhide("adv", show);
     if (show) {
         advSWButton.value = _("Ipseccfg hide");
     }
     else {
         advSWButton.value = _("Ipseccfg show");
     }
     showAdv = show;
   }
}



function initTranslation()
{
        var e = document.getElementById("Ipseccfgtitle");
        e.innerHTML = _("Ipseccfg title");

        e = document.getElementById("Ipseccfgintroduce");
        e.innerHTML = _("Ipseccfg introduction");

        e = document.getElementById("Ipseccfg_connName");
        e.innerHTML = _("Ipseccfg connName");

        e = document.getElementById("Ipseccfg_remoteGWAddr");
        e.innerHTML = _("Ipseccfg remoteGWAddr");

        e = document.getElementById("Ipseccfg_localIPMode");
        e.innerHTML = _("Ipseccfg localIPMode");

        e = document.getElementById("Ipseccfg_localIP");
        e.innerHTML = _("Ipseccfg localIP");
        e = document.getElementById("Ipseccfg_localMask");
        e.innerHTML = _("Ipseccfg localMask");
        e = document.getElementById("Ipseccfg_remoteIPMode");
        e.innerHTML = _("Ipseccfg remoteIPMode");
        e = document.getElementById("Ipseccfg_remoteIP");
        e.innerHTML = _("Ipseccfg remoteIP");

        e = document.getElementById("Ipseccfg_remoteMask");
        e.innerHTML = _("Ipseccfg remoteMask");
        e = document.getElementById("Ipseccfg_keyExM");
        e.innerHTML = _("Ipseccfg keyExM");
        e = document.getElementById("Ipseccfg_authM");
        e.innerHTML = _("Ipseccfg authM");

        e = document.getElementById("Ipseccfg_psk");
        e.innerHTML = _("Ipseccfg psk");

        e = document.getElementById("Ipseccfg_certificateName");
        e.innerHTML = _("Ipseccfg certificateName");

        e = document.getElementById("Ipseccfg_perfectFSEn");
        e.innerHTML = _("Ipseccfg perfectFSEn");

        e = document.getElementById("Ipseccfg_Disable");
        e.innerHTML = _("firewall disable");

        e = document.getElementById("Ipseccfg_Enable");
        e.innerHTML = _("firewall enable");

        e = document.getElementById("Ipseccfg_advSWButton");
        e.innerHTML = _("Ipseccfg advSWButton");

        e = document.getElementById("Ipseccfg_manualEncryptionAlgo");
        e.innerHTML = _("Ipseccfg manualEncryptionAlgo");

        e = document.getElementById("Ipseccfg_manualEncryptionKey");
        e.innerHTML = _("Ipseccfg manualEncryptionKey");

        e = document.getElementById("Ipseccfg_manualAuthAlgo");
        e.innerHTML = _("Ipseccfg manualAuthAlgo");

        e = document.getElementById("Ipseccfg_manualAuthKey");
        e.innerHTML = _("Ipseccfg manualAuthKey");


        e = document.getElementById("Ipseccfg_ph1Mode");
        e.innerHTML = _("Ipseccfg ph1Mode");

        e = document.getElementById("Ipseccfg_ph1EncryptionAlgo");
        e.innerHTML = _("Ipseccfg ph1EncryptionAlgo");

        e = document.getElementById("Ipseccfg_ph1IntegrityAlgo");
        e.innerHTML = _("Ipseccfg ph1IntegrityAlgo");

        e = document.getElementById("Ipseccfg_ph1DHGroup");
        e.innerHTML = _("Ipseccfg ph1DHGroup");

        e = document.getElementById("Ipseccfg_ph1KeyTime");
        e.innerHTML = _("Ipseccfg ph1KeyTime");



        e = document.getElementById("Ipseccfg_ph2EncryptionAlgo");
        e.innerHTML = _("Ipseccfg ph1EncryptionAlgo");

        e = document.getElementById("Ipseccfg_ph2IntegrityAlgo");
        e.innerHTML = _("Ipseccfg ph1IntegrityAlgo");

        e = document.getElementById("Ipseccfg_ph2DHGroup");
        e.innerHTML = _("Ipseccfg ph1DHGroup");

        e = document.getElementById("Ipseccfg_ph2KeyTime");
        e.innerHTML = _("Ipseccfg ph1KeyTime");


        e = document.getElementById("ipseccfg_apply");
        e.value = _("firewall apply");

        e = document.getElementById("ipseccfg_reset");
        e.value = _("firewall cancel");
	
        e = document.getElementById("Ipseccfg_servicesel");
        e.innerHTML = _("Ipseccfg servicesel");

        e = document.getElementById("option_service");
        e.innerHTML = _("option service");

        e = document.getElementById("option_client");
        e.innerHTML = _("option client");

	e = document.getElementById("vpn_name_help1");
        e.innerHTML = _("vpn name help1");
	e = document.getElementById("vpn_name_help2");
        e.innerHTML = _("vpn name help2");
        e = document.getElementById("id_policies_remote_lan");
        e.innerHTML = _("id policies remote lan");


}



//////////////////////////

function formLoad()
{


    localIPModeChange();
    remoteIPModeChange();
    keyauthChange();
        service_mode_change();
    showhideAdv(showAdv);
        initTranslation();
    
}


function isHexaDigit(digit) {
   var hexVals = new Array("0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
                           "A", "B", "C", "D", "E", "F", "a", "b", "c", "d", "e", "f");
   var len = hexVals.length;
   var i = 0;
   var ret = false;

   for ( i = 0; i < len; i++ )
      if ( digit == hexVals[i] ) break;

   if ( i < len )
      ret = true;

   return ret;
}


function isValidHexKey(val, size) {
   var ret = false;
   if (val.length == size) {
      for ( i = 0; i < val.length; i++ ) {
         if ( isHexaDigit(val.charAt(i)) == false ) {
            break;
         }
      }
      if ( i == val.length ) {
         ret = true;
      }
   }

   return ret;
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

function isDigital(s)
{
    var r,re;
    re = /\d*/i;
    r = s.match(re);
    return (r==s)?1:0;
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
function isCharUnsafe(compareChar) {
   var unsafeString = "\"<>%\\^[]`\+\$\,='#&@.:\t";

   if ( unsafeString.indexOf(compareChar) == -1 && compareChar.charCodeAt(0) >= 32
        && compareChar.charCodeAt(0) < 123 )
      return false; // found no unsafe chars, return false
   else
      return true;
}  
function isValidNameWSpace(name) {
   var i = 0;
   
   for ( i = 0; i < name.length; i++ ) {
      if ( isCharUnsafe(name.charAt(i)) == true )
         return false;
   }

   return true;
}

function isValidVpnName_DeviceID(str)
{

	var id;
	var i;

	if(!(  (str.length>3)&& (str.charAt(0)=='D')&&(str.charAt(1)=='E')&&(str.charAt(2)=='V')   )){
		return true;
	}

	 id="<% getDeviceID(); %>"
	if(str.length>id.length){
		for(i=0;i<id.length;i++){
			if(id.charAt(i)!=str.charAt(i+3)){
				return false;
			}
		}
	}else{
		return false;
	}

	return true;
	 
}

function applyClick() {


                if ( document.ipsec_config.connName.value=="") {

                        alert(_("connection name can not be empty"));
                        document.ipsec_config.connName.focus();
                        return false;
                }
/*

                if ( isValidNameWSpace(document.ipsec_config.connName.value) == false ) {
                        alert(_("invalid connection name"));
                        document.ipsec_config.connName.focus();
                        return false;

                }
*/

                if(getSelect(document.ipsec_config.serviceselbox) == "client"){
                                if ( document.ipsec_config.remoteGWAddr.value=="" ) {
                                        alert(_("input remoteGWAddr"));
                                        document.ipsec_config.remoteGWAddr.focus();
                                        return false;
				}
				/*
                                if ( isValidIpAddress(document.ipsec_config.remoteGWAddr.value) == false ) {
                                //if (isValidSubnetIpAddress(localIP.value) == false ) {
                                        alert(_("invalid remoteGWAddr"));
                                        document.ipsec_config.remoteGWAddr.focus();
                                        return false;
                                }
				*/


				     if(isValidVpnName_DeviceID(document.ipsec_config.connName.value)==false){
                                        alert(_("invalid Vpn Name when use device id!"));
                                       document.ipsec_config.connName.focus();
                                        return false;
					 }

								
                        }
	 if(document.ipsec_config.localIPMode[document.ipsec_config.localIPMode.selectedIndex].value!="any"){
                if ( isValidNetAddress(document.ipsec_config.localIP.value) == false ) {
                //if (isValidSubnetIpAddress(localIP.value) == false ) {
                        alert(_("invalid localIP"));
                        document.ipsec_config.localIP.focus();
                        return false;
                }
                if ( getSelect(document.ipsec_config.localIPMode) == "subnet" && isValidSubnetMask(document.ipsec_config.localMask.value) == false ) {
                        alert(_("invalid localMask"));
                        document.ipsec_config.localMask.focus();
                        return false;
                }
	}
	if(document.ipsec_config.remoteIPMode[document.ipsec_config.remoteIPMode.selectedIndex].value!="any"){ 
                if ( isValidNetAddress(document.ipsec_config.remoteIP.value) == false ) {
                //if ( isValidSubnetIpAddress(remoteIP.value) == false ) {
                        alert(_("invalid remoteIP"));
                        document.ipsec_config.remoteIP.focus();
                        return false;
                }
                if ( getSelect(document.ipsec_config.remoteIPMode) == "subnet" && isValidSubnetMask(document.ipsec_config.remoteMask.value) == false ) {
                        alert(_("invalid remoteMask"));
                        document.ipsec_config.remoteMask.focus();
                        return false;
                }
	}
//	else{                    
 //                       if(document.ipsec_config.policies_remote_lan.value==""){
  //                               alert(_("please input remote lan for policies!"));
   //                             document.ipsec_config.policies_remote_lan.focus();
    //                            return false;
     //                   }   
      // }



                if(document.ipsec_config.localIPMode[document.ipsec_config.localIPMode.selectedIndex].value=="any" &&
                        document.ipsec_config.remoteIPMode[document.ipsec_config.remoteIPMode.selectedIndex].value=="any"){
                        
                                alert(_("Do not select the same: any!"));
                                return false;
                }




                if ( getSelect(document.ipsec_config.keyExM) == "auto" ) {
                var val = parseInt(document.ipsec_config.ph1KeyTime.value);
                if (  isNaN(val) == true || val < 0 ) {
                        alert(_("invalid ph1KeyTime"));
                   showhideAdv(1);
                        document.ipsec_config.ph1KeyTime.focus();
                        return false;
                }
                val = parseInt(document.ipsec_config.ph2KeyTime.value);
                if (  isNaN(val) == true || val < 0 ) {
                        alert(_("invalid ph2KeyTime"));
                   showhideAdv(1);
                        document.ipsec_config.ph2KeyTime.focus();
                        return false;
                }
                if ( getSelect(document.ipsec_config.authM) == "certificate") {
                   if ( document.ipsec_config.certificateName.options.length == 0) {
                      // alert('No certificate avaiable for authentication. Use Certificate menu to add certificates or choose another autentication method.');
                        alert(_("invalid certificateName"));
          
                        return false;
                   }
                }
                }
                if ( getSelect(document.ipsec_config.keyExM) == "manual" ) {
                        var ekeys;
                        if ( getSelect(document.ipsec_config.manualEncryptionAlgo) == "des-cbc") {
                                ekeys = 16;
                        }
                        else if ( getSelect(document.ipsec_config.manualEncryptionAlgo) == "3des-cbc") {
                                ekeys = 48;
                        }
                        else {
                                ekeys = -1;
                        }
                        if ( ekeys != -1 && isValidHexKey(document.ipsec_config.manualEncryptionKey.value, ekeys) == false) {
                                alert(_("invalid manualEncryptionKey"));
                                document.ipsec_config.manualEncryptionKey.focus();
                                return false;
                        }
                        var akeys;
                        if ( getSelect(document.ipsec_config.manualAuthAlgo) == "hmac-md5") {
                                akeys = 32;
                        }
                        else if ( getSelect(document.ipsec_config.manualAuthAlgo) == "hmac-sha1") {
                                akeys = 40;
                        }
                        else {
                                akeys = -1;
                        }
                        if ( akeys != -1 && isValidHexKey(document.ipsec_config.manualAuthKey.value, akeys) == false) {
                                alert(_("invalid manualAuthKey"));
                                document.ipsec_config.manualAuthKey.focus();
                                return false;
                        }
                }

                return true;

}


</script>
  
</head>

<body onload="formLoad()">

<h1 id="Ipseccfgtitle">IPSEC   Settings </h1>
<p id="Ipseccfgintroduce"> IPSEC</p>
<hr />
<form method=post name="ipsec_config" action="/goform/setipsec_config" onSubmit="return applyClick()">
  <div><table border="0" cellpadding="0" cellspacing="2" width="640">
    <tr>
      <td width="40%" id="Ipseccfg_connName"> IPSec 连接名( ID ) </td>
      <td> <input name="connName" size="20" maxlength="60" type="text"> </td>
    </tr>

    <tr>
      <td> &nbsp;</td>
      <td id="vpn_name_help1">you can input DEV+DeviceID+[...] to bind device</td>
     </tr>    
        <tr>
      <td> &nbsp;</td>
      <td id="vpn_name_help2">example:DEV280630562C080435.vpn1.com</td>
     </tr>      
      <tr><td>&nbsp;</td></tr>
      <tr><td>&nbsp;</td></tr>

    <tr>
        <td width="40%" id="Ipseccfg_servicesel">  service mode </td>
        <td>
                <select name="serviceselbox" onChange="service_mode_change();">
                         <option id="option_service" value="service">Service</option>
                         <option id="option_client" value="client">Client</option>
                 </select>
        </td>
        
    </tr>
    
  </table></div>



   <div id="div_link_mode_sel">  
     <table border="0" cellpadding="0" cellspacing="2" width="640">
     <tr>
       <td width="40%" id="Ipseccfg_ph1Mode">模式</td>
       <td>
         <select name="ph1Mode">
           <option value="main">Main</option>
           <option value="aggressive">Aggressive</option>
         </select>
       </td>
       </tr>
       </table>
      </div>


<div id="div_remoteGWaddr">
    <table border="0" cellpadding="0" cellspacing="2" width="640">
            <tr>
                <td width="40%" id="Ipseccfg_remoteGWAddr"> 远程 IPSec 网关地址</td>
                <td><input name="remoteGWAddr" size="20" maxlength="255" type="text"></td>
            </tr>
    </table>
</div>



  <table border="0" cellpadding="0" cellspacing="2" width="640">
   <tr><td>&nbsp;</td></tr>
    <tr>
      <td width="40%" id="Ipseccfg_localIPMode"> 本地IP 地址</td>
      <td >
      <select name="localIPMode" onChange="localIPModeChange();">
        <option value="subnet">Subnet</option>
        <option value="single">Single Address</option>
        <option value="any">Any</option>
      </select>
    </td>
    </tr>
  </table>
  
  <div id = "IP11">
  <table border="0" cellpadding="0" cellspacing="2" width="640">
  
    <tr>
      <td width="40%" id="Ipseccfg_localIP"> VPN IP 地址</td>
      <td> <input name="localIP" type="text"> </td>
     
    </tr>
   </table>
   
   </div>
   <div id = "IP12">
   
   <table border="0" cellpadding="0" cellspacing="2" width="640">
      <tr>
        <td width="40%" id="Ipseccfg_localMask">&nbsp;&nbsp; IP 子网掩码</td>
        <td> <input name="localMask" type="text"> </td> 
      </tr>
   </table>
   
   </div>

   
   <table border="0" cellpadding="0" cellspacing="2" width="640">
      <tr><td></td><td>&nbsp;</td></tr>
      <tr>
        <td width="40%" id="Ipseccfg_remoteIPMode"> 远程IP地址 </td>
        <td >
        <select name="remoteIPMode" onChange="remoteIPModeChange();">
        <option value="subnet" selected>Subnet</option>
        <option value="single">Single Address</option>
        <option value="any">Any</option>
        </select>
        </td>
      </tr>
   </table>
   <div id = "IP21"><table border="0" cellpadding="0" cellspacing="2" width="640">
    <tr>
      <td width="40%" id="Ipseccfg_remoteIP">&nbsp;&nbsp; VPN IP地址</td>
        <td> <input name="remoteIP" type="text"> </td>
    </tr>
   </table></div>
   <div id = "IP22"><table border="0" cellpadding="0" cellspacing="2" width="640">
      <tr>
        <td width="40%" id="Ipseccfg_remoteMask">&nbsp;&nbsp; IP 子网掩码</td>
        <td> <input name="remoteMask" type="text"> </td>
      </tr>
    </table></div>
   <div id = "div_policies_remote_lan">
           <table border="0" cellpadding="0" cellspacing="2" width="640">
              <tr>                                                                                
                <td width="40%" id="id_policies_remote_lan">remote lan for policies</td>
                <td> <input name="policies_remote_lan"  maxlength=190 type="text"> </td>
              </tr>
              <tr>
                <td width="40%" ></td>
                <td id="id_policies_remote_lan_commit">e.g:192.168.1.0/24 10.11.0.0/16</td>
              </tr>
            </table>
    </div>








    
    <table border="0" cellpadding="0" cellspacing="2" width="640">
    <tr><td>&nbsp;</td><td>&nbsp;</td></tr>
     <tr> 
        <td width="40%" id="Ipseccfg_keyExM">Key Exchange 方法 </td>
        <td><select name="keyExM" onChange="keyauthChange();">
            <option value="auto">Auto(IKE)</option>
<!-- 
            <option value="manual">Manual</option>
-->
          </select>
        </td>
     </tr> 
     </table>
    <div id="AutoKeyEx"><table border="0" cellpadding="0" cellspacing="2" width="640">
      <tr>
        <td width="40%" id="Ipseccfg_authM"> 认证 方法 </td>
        <td>
          <select name="authM" onChange="keyauthChange();">
            <option value="pre_shared_key">Pre-Shared Key</option>
<!--
            <option value="certificate">Certificate (X.509)</option>
-->
          </select>
        </td>
      </tr>
    </table> 
    <div id="PSK"><table border="0" cellpadding="0" cellspacing="2" width="640">
       <tr>
        <td width="40%" id="Ipseccfg_psk">Pre-Shared 密钥</td>
        <td> <input type="password" name="psk"></td>
      </tr>
    </table></div>
    <div id="Certificate">
      <table border="0" cellpadding="0" cellspacing="2" width="640">
        <tr>
          <td width="40%" id="Ipseccfg_certificateName"> Certificates </td>
          <td><select name="certificateName">
          </select></td>
        </tr>
      </table></div>


     <table border="0" cellpadding="0" cellspacing="2" width="640">
      <tr>
        <td width="40%" id="Ipseccfg_perfectFSEn">Perfect Forward Secrecy</td>
        <td align="left"> 
          <select name="perfectFSEn">
            <option value="disable" id="Ipseccfg_Disable">Disable</option>
            <option value="enable" id="Ipseccfg_Enable">Enable</option>
          </select>
        </td>
     </tr> 
     <tr><td>&nbsp;</td></tr>

     <tr>                                                                                          
                <td width="40%" id="nat_traversal_str">NAT Traversal</td>                            
                <td>                                                                                   
                <input type="checkbox"  name="nat_traversal">                                          
                </td>                                                                                  
        </tr>                                                                                          
     <tr><td>&nbsp;</td></tr>    






     <tr>
       <td width="40%" id="Ipseccfg_advSWButton">高级 IKE 设置</td>
       <td><input name="advSWButton" type="button" value="高级设置" onClick="switchAdv();" ></td>
     </tr>
    </table></div>

    
    <div id="ManualKey"> <table border="0" cellpadding="0" cellspacing="2" width="640">
       <tr>
        <td width="40%" id="Ipseccfg_manualEncryptionAlgo">加密算法</td>
        <td>
          <select name="manualEncryptionAlgo">
          <option value="des-cbc">DES</option>
          <option value="3des-cbc" selected>3DES</option>
          <option value="aes-cbc">AES(aes-cbc)</option>
        </select>
        </td>
      </tr>
      <tr>
        <td width = "40%" id="Ipseccfg_manualEncryptionKey"> 加密密钥 </td>
        <td> <input name="manualEncryptionKey", size = "40" maxlength = "80">  DES: 16 digit Hex, 3DES: 48 digit Hex <br></td>
      </tr>
      <tr>
       <td width="40%" id="Ipseccfg_manualAuthAlgo">认证算法</td>
       <td>
         <select name="manualAuthAlgo">
           <option value="hmac-md5">MD5</option>
           <option value="hmac-sha1">SHA1</option>
         </select>
       </td>
     </tr>
      <tr>
        <td id="Ipseccfg_manualAuthKey"> 认证密钥 </td>
        <td> <input name="manualAuthKey" size="50" maxlength = "60"> MD5: 32 位16进制, SHA1: 40 位16进制</td>
      </tr>
      <tr>
        <td> SPI </td>
        <td> <input name="spi" size="8" maxlength = "8"> Hex 100-FFFFFFFF</td>
      </tr>      
    </table></div>
    <div id = "adv">
    <table border="0" cellpadding="0" cellspacing="2" width="640">
     <tr>
       <td> Phase 1</td>
     </tr>
   
       <tr>
        <td width="40%" id="Ipseccfg_ph1EncryptionAlgo">加密算法</td>
        <td>
          <select name="ph1EncryptionAlgo">
          <option value="des">DES</option>
          <option value="3des" selected>3DES</option>
          <option value="aes128">AES - 128</option>
          <option value="aes192">AES - 192</option>
          <option value="aes256">AES - 256</option>          
        </select>
        </td>
      </tr>
      <tr>
       <td width="40%" id="Ipseccfg_ph1IntegrityAlgo">完整性算法</td>
       <td>
         <select name="ph1IntegrityAlgo">
           <option value="md5">MD5</option>
           <option value="sha1">SHA1</option>
         </select>
       </td>
       <tr>
       <td width="40%" id="Ipseccfg_ph1DHGroup">选择 Diffie-Hellman 密钥交换</td>
       <td>
         <select name="ph1DHGroup">
           <option value="modp768">768bit</option>
           <option value="modp1024" selected>1024bit</option>
           <option value="modp1536">1536bit</option>
           <option value="modp2048">2048bit</option>
           <option value="modp3072">3072bit</option>
           <option value="modp4096">4096bit</option>
           <option value="modp6144">6144bit</option>
           <option value="modp8192">8192bit</option>
         </select>
       </td>
      </tr>
      <tr>
        <td width="40%" id="Ipseccfg_ph1KeyTime">Key 生命期 </td>
        <td> <input name="ph1KeyTime" value="3600"> &nbsp Seconds </td>
      </tr>
      <tr>
     <tr><td>&nbsp;</td></tr>
     <tr>
       <td> Phase 2</td>
     </tr>
     <tr>
        <td width="40%" id="Ipseccfg_ph2EncryptionAlgo">加密算法</td>
        <td>
          <select name="ph2EncryptionAlgo">
          <option value="des">DES</option>
          <option value="3des" selected >3DES</option>
          <option value="aes128">AES - 128</option>
          <option value="aes192">AES - 192</option>
          <option value="aes256">AES - 256</option>
        </select>
        </td>
      </tr>
      <tr>
       <td width="40%" id="Ipseccfg_ph2IntegrityAlgo">完整性算法</td>
       <td>
         <select name="ph2IntegrityAlgo">
           <option value="hmac_md5">MD5</option>
           <option value="hmac_sha1">SHA1</option>
         </select>
       </td>
      </tr>
      <tr>
       <td width="40%" id="Ipseccfg_ph2DHGroup">Select Diffie-Hellman Group for Key Exchange</td>
       <td>
         <select name="ph2DHGroup">
           <option value="modp768">768bit</option>
           <option value="modp1024" selected>1024bit</option>
           <option value="modp1536">1536bit</option>
           <option value="modp2048">2048bit</option>
           <option value="modp3072">3072bit</option>
           <option value="modp4096">4096bit</option>
           <option value="modp6144">6144bit</option>
           <option value="modp8192">8192bit</option>
         </select>
       </td>
      </tr>
      <tr>
        <td width="40%" id="Ipseccfg_ph2KeyTime">Key 生命期 </td>
        <td> <input name="ph2KeyTime" value="28800"> &nbsp Seconds </td>
      </tr>
   </table></div>

<p>
    <input type="submit" value="apply" id="ipseccfg_apply" name="ipsec apply" >&nbsp;&nbsp;
    <input type="button" value="Reset" id="ipseccfg_reset" onClick="{window.location.replace('./ipsec.asp');}">
</p>

</form>
</body>
</html>
