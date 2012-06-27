<html>
<head>
<title>SNMP</title>
<link rel="stylesheet" href="/css/normal_ws.css" type="text/css">
<link rel="stylesheet" href="/css/boxStyle.css" type="text/css">
<meta http-equiv="content-type" content="text/html;charset=gb2312" />
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("internet");






function selectRadio(name,value)
{              
	 var radioObject = document.getElementsByName(name);              

    if(value == "")              
   {              
       radioObject[0].checked = true;              
       return;              
   }              
    for (var i = 0; i < radioObject.length; i++)               
    {              
        if(radioObject[i].value == value)              
        {              
            radioObject[i].checked = true;              
           break;              
        }                  
   }              
}              



function getRadioValueByName(name) 
{ 
  var rvs = document.getElementsByName(name); 
  var strETD = ""; 
  for(var i=0; i < rvs.length; i++) 
  { 
    if(rvs[i].checked == true) 
    { 
      strETD = rvs[i].value; 
      break; 
    } 
  } 
  return strETD; 
} 
function isCheckBoxSelected(name) 
{ 
  var t = false; 
  var cbx = document.getElementsByName(name); 
  for(var i=0; i < cbx.length; i++) 
  { 
    if(cbx[i].checked == true) 
    { 
      t = true; 
      break; 
    } 
  } 
  return t; 
} 


function isDigital(s)
{
    var r,re;
    re = /\d*/i;
    r = s.match(re);
    return (r==s)?1:0;
}



function initTranslation()
{
	var e;
	 e = document.getElementById("id_snmp_settings");
        e.innerHTML = _("id snmp settings");
        e = document.getElementById("id_snmp_active");
        e.innerHTML = _("id snmp active");
        e = document.getElementById("id_snmp_contact_info");
        e.innerHTML = _("id snmp contact info");
        e = document.getElementById("id_snmp_location");
        e.innerHTML = _("id snmp location");
        e = document.getElementById("id_snmp_v2_settings");
        e.innerHTML = _("id snmp v2 settings");
        e = document.getElementById("id_snmp_v2_user");
        e.innerHTML = _("id snmp v2 user");
        e = document.getElementById("id_snmp_v2_host_lan");
        e.innerHTML = _("id snmp v2 host lan");
        e = document.getElementById("id_snmp_v2_writable");
        e.innerHTML = _("id snmp v2 writable");
        e = document.getElementById("id_snmp_v3_settings");
        e.innerHTML = _("id snmp v3 settings");
        e = document.getElementById("id_snmp_v3_user");
        e.innerHTML = _("id snmp v3 user");
        e = document.getElementById("id_snmp_v3_host_lan");
        e.innerHTML = _("id snmp v3 host lan");
        e = document.getElementById("id_snmp_v3_writable");
        e.innerHTML = _("id snmp v3 writable");
        e = document.getElementById("id_snmp_v3_security_mode");
        e.innerHTML = _("id snmp v3 security mode");
        e = document.getElementById("id_snmp_v3_authentication_str");
        e.innerHTML = _("id snmp v3 authentication str");
  			e = document.getElementById("id_snmp_v3_encryption_str");
        e.innerHTML = _("id snmp v3 encryption str");
        e = document.getElementById("id_snmp_v3_authentication_password");
        e.innerHTML = _("id snmp v3 authentication password");
        
        e = document.getElementById("help_head");
				e.innerHTML = _("help help_head");
	
        e = document.getElementById("snmp_direc1");
        e.innerHTML = _("snmp snmp_direc1");
        e = document.getElementById("snmp_direc2");
        e.innerHTML = _("snmp snmp_direc2");
        e = document.getElementById("snmp_direc3");
        e.innerHTML = _("snmp snmp_direc3");
        e = document.getElementById("snmp_direc4");
        e.innerHTML = _("snmp snmp_direc4");
        e = document.getElementById("snmp_direc5");
        e.innerHTML = _("snmp snmp_direc5");
        e = document.getElementById("snmp_direc6");
        e.innerHTML = _("snmp snmp_direc6");







        e = document.getElementById("id_snmp_apply");
        e.value = _("id snmp apply");
	
}



function load_cur_para()
{
	var tmp;
	tmp="<% getCfgGeneral(1,"snmp_active");%>"
	if(tmp=="on"){
		document.snmp_config.snmp_active.checked=true;
	}else{
		document.snmp_config.snmp_active.checked=false;
	}
	 tmp="<% getCfgGeneral(1,"snmp_contact_info"); %>"
	 document.snmp_config.snmp_contact_info.value=tmp;
	 tmp="<% getCfgGeneral(1,"snmp_location"); %>"
	 document.snmp_config.snmp_location.value=tmp;


	 tmp="<% getCfgGeneral(1,"snmp_v2_user"); %>"
	 if(tmp=="")
	 	 document.snmp_config.snmp_v2_user.value="public";
	 else
		 document.snmp_config.snmp_v2_user.value=tmp;
	 tmp="<% getCfgGeneral(1,"snmp_v2_host_lan"); %>"
	 if(tmp=="")
	 	document.snmp_config.snmp_v2_host_lan.value="0.0.0.0/0";
	 else
	 	document.snmp_config.snmp_v2_host_lan.value=tmp;
	tmp="<% getCfgGeneral(1,"snmp_v2_writable");%>"
	if(tmp=="on"){
		document.snmp_config.snmp_v2_writable.checked=true;
	}else{
		document.snmp_config.snmp_v2_writable.checked=false;
	}

	 tmp="<% getCfgGeneral(1,"snmp_v3_user"); %>"
	 document.snmp_config.snmp_v3_user.value=tmp;
	 tmp="<% getCfgGeneral(1,"snmp_v3_host_lan"); %>"
	 if(tmp=="")
	 	 document.snmp_config.snmp_v3_host_lan.value="0.0.0.0/0";
	 else
		 document.snmp_config.snmp_v3_host_lan.value=tmp;
	tmp="<% getCfgGeneral(1,"snmp_v3_writable");%>"
	if(tmp=="on"){
		document.snmp_config.snmp_v3_writable.checked=true;
	}else{
		document.snmp_config.snmp_v3_writable.checked=false;
	}



	 tmp="<% getCfgGeneral(1,"snmp_v3_security_mode");%>"
	if(tmp=="noauth"){
		selectRadio("snmp_v3_security_mode","noauth");
	}else if(tmp=="auth"){
		selectRadio("snmp_v3_security_mode","auth");
	}else{
		selectRadio("snmp_v3_security_mode","priv");
	}
	
	tmp="<% getCfgGeneral(1,"snmp_v3_authentication");%>"
	if(tmp=="SHA"){
		selectRadio("snmp_v3_authentication","SHA");
	}else{
		selectRadio("snmp_v3_authentication","MD5");
	}
	tmp="<% getCfgGeneral(1,"snmp_v3_encryption");%>"
	if(tmp=="AES"){
		selectRadio("snmp_v3_encryption","AES");
	}else{
		selectRadio("snmp_v3_encryption","DES");
	}

	 tmp="<% getCfgGeneral(1,"snmp_v3_authentication_password"); %>"
	 document.snmp_config.snmp_v3_authentication_password.value=tmp;
	 tmp="<% getCfgGeneral(1,"snmp_v3_encryption_password"); %>"
	 document.snmp_config.snmp_v3_encryption_password.value=tmp;

	  
}












function formload()
{
	
       load_cur_para();
	initTranslation();
}


function applyClick()
{
	if(document.snmp_config.snmp_active.checked==true){
		if(document.snmp_config.snmp_v2_user.value==""){
				  alert(_("please input snmp v2 user!"));
				  document.snmp_config.snmp_v2_user.focus();
				  return false; 
		}
		if(document.snmp_config.snmp_v2_host_lan.value==""){
				  alert(_("please input snmp v2 host/lan!"));
				  document.snmp_config.snmp_v2_host_lan.focus();
				  return false; 
		}
		if(document.snmp_config.snmp_v3_user.value==""){
				  alert(_("please input snmp v3 user!"));
				  document.snmp_config.snmp_v3_user.focus();
				  return false; 
		}
		if(document.snmp_config.snmp_v3_host_lan.value==""){
				  alert(_("please input snmp v3 host/lan!"));
				  document.snmp_config.snmp_v3_host_lan.focus();
				  return false; 
		}

		if(document.snmp_config.snmp_v3_authentication_password.value==""){
				  alert(_("please input snmp v3 authentication password!"));
				  document.snmp_config.snmp_v3_authentication_password.focus();
				  return false; 
				  
		}
		var str=document.snmp_config.snmp_v3_authentication_password.value;
		if(str.length<8){
			  alert(_("length >=  8 "));
			  document.snmp_config.snmp_v3_authentication_password.focus();
			  return false; 	
		}
		
		if(document.snmp_config.snmp_v3_encryption_password.value==""){
				  alert(_("please input snmp v3 encryption password!"));
				  document.snmp_config.snmp_v3_encryption_password.focus();
				  return false; 
		}
		str=document.snmp_config.snmp_v3_encryption_password.value;
		if(str.length<8){
			  alert(_("length >=  8 "));
			  document.snmp_config.snmp_v3_encryption_password.focus();
			  return false; 	
		}
		
	}
}


</script>
</head>


<body onload="formload()">
<center id="boxes">
	<div id="box">
	<div id="head"> </div>	<!--end of head-->	
	<div id="content">


	<table id="layout_table" border="0"><!--start of layout_table-->
<tr><!--start of layout tr-->
<td class="tdwidth1" id="td1"><!--start of td1-->
<div id="left"><!--start of left-->


<h1 align="left"> SNMP </h1>
<hr />



<form name="snmp_config" method=post action="/goform/snmp_action" onSubmit="return applyClick()">

	<table border="0" cellpadding="2" cellspacing="1" width="540" bordercolor="#9BABBD">
		<tr>
			<td class="title" colspan="10" id="id_snmp_settings">SNMP Settings</td>
		</tr>
		<tr>
			<td  width="40%" id="id_snmp_active">SNMP Active</td>
			<td>
				<input type="checkbox" name="snmp_active">
			</td>
		</tr>


		<tr>
			<td width="40%" id="id_snmp_contact_info">Contact Info</td>
			<td> 
				<input name="snmp_contact_info" size=20 type="text"> 
			</td>
		</tr>

		<tr>
			<td width="40%" id="id_snmp_location">Location</td>
			<td> 
				<input name="snmp_location" size=20 type="text"> 
			</td>
		</tr>


	<!--
		<tr>
			<td  width="40%" id="id_snmp_send_to">SNMP Send to</td>
			<td>
				<input type="radio" name="snmp_sendto_inf" value="serial"><font id="id_message_to_serial">Serial</font>
				<input type="radio" name="snmp_sendto_inf" value="net" checked="true" >TCP/IP
			</td>
		</tr>
	-->
	</table>
	
	<table border="0" cellpadding="2" cellspacing="1" width="540" bordercolor="#9BABBD">
		<tr>
			<td class="title" colspan="10" id="id_snmp_v2_settings">SNMP V1 and V2c Settings</td>
		</tr>


		<tr>
			<td width="40%" id="id_snmp_v2_user">user</td>
			<td> 
				<input name="snmp_v2_user" size=20 type="text"> 
			</td>
		</tr>
		<tr>
			<td width="40%" id="id_snmp_v2_host_lan">host/lan</td>
			<td> 
				<input name="snmp_v2_host_lan" size=20 type="text" value="0.0.0.0/0"> 
			</td>
		</tr>
		<tr>
			<td  width="40%" id="id_snmp_v2_writable">writable</td>
			<td>
				<input type="checkbox" name="snmp_v2_writable">
			</td>
		</tr>

	</table>

	
	<table border="0" cellpadding="2" cellspacing="1" width="540" bordercolor="#9BABBD">
		<tr>
			<td class="title" colspan="10" id="id_snmp_v3_settings">SNMP V3 Settings</td>
		</tr>
		<tr>
			<td width="40%" id="id_snmp_v3_user">user</td>
			<td> 
				<input name="snmp_v3_user" size=20 type="text"> 
			</td>
		</tr>
		<tr style="display:none">
			<td width="40%" id="id_snmp_v3_host_lan">host/lan</td>
			<td> 
				<input name="snmp_v3_host_lan" size=20 type="text"> 
			</td>
		</tr>
		<tr>
			<td  width="40%" id="id_snmp_v3_writable">writable</td>
			<td>
				<input type="checkbox" name="snmp_v3_writable">
			</td>
		</tr>

		<tr>
			<td  width="40%" id="id_snmp_v3_security_mode">Security mode</td>
			<td>
				<input type="radio" name="snmp_v3_security_mode" value="noauth" >None
				<input type="radio" name="snmp_v3_security_mode" value="auth"  >Authorized
				<input type="radio" name="snmp_v3_security_mode" value="priv" checked="true" >Private
			</td>
		</tr>

		<tr>
			<td  width="40%" id="id_snmp_v3_authentication_str">Authentication</td>
			<td>
				<input type="radio" name="snmp_v3_authentication" value="MD5" checked="true">MD5
				<input type="radio" name="snmp_v3_authentication" value="SHA"  >SHA
			</td>
		</tr>
		<tr>
			<td  width="40%" id="id_snmp_v3_encryption_str">Encryption</td>
			<td>
				<input type="radio" name="snmp_v3_encryption" value="DES" checked="true">DES
				<input type="radio" name="snmp_v3_encryption" value="AES"  >AES
			</td>
		</tr>

		<tr>
			<td width="40%" id="id_snmp_v3_authentication_password">Authentication Password</td>
			<td> 
				<input name="snmp_v3_authentication_password" size=20 type="password"> 
			</td>
		</tr>
		<tr>
			<td width="40%" id="id_snmp_v3_encryption_password">Encryption Password</td>
			<td> 
				<input name="snmp_v3_encryption_password" size=20 type="password"> 
			</td>
		</tr>
		
	</table>


	<p>
	<table width="540" border="0" cellpadding="2" cellspacing="1">
		  <tr align="center">
		    <td>
		         <input type="submit" value="apply" id="id_snmp_apply" name="snmp_apply" >
		    </td>
		  </tr>
	</table>
	</p>


</form>


</div><!--end of left-->
</td><!--end of td1-->

<td class="tdwidth2" id="td2"><!--start of td2-->
	<div id="right"><!--start of right-->
		<h2 id="help_head">Heeelp...</h2>
		
		<p id="help_content">
			<span id="snmp_direc1"></span><br/><br/>
			<span id="snmp_direc2"></span><br/><br/>
			<span id="snmp_direc3"></span><br/><br/>
			<span id="snmp_direc4"></span><br/><br/>
			<span id="snmp_direc5"></span><br/><br/>
			<span id="snmp_direc6"></span><br/><br/>
		</p>
	</div><!--end of right-->
</td><!--end of td2-->
</tr><!--end of layout tr-->
</table><!--end of layout_table-->


</div><!--end of content-->
	</div>
	</center>

</body>

</html>

