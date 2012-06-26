<html>
<head>
<title>Router Fail Over</title>
<link rel="stylesheet" href="/css/normal_ws.css" type="text/css">
<link rel="stylesheet" href="/css/boxStyle.css" type="text/css">
<meta http-equiv="content-type" content="text/html;charset=gb2312" />
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
 Butterlate.setTextDomain("internet");
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
      if ( num < 0 || num > 255 )
         return false;
      if (i == 0 && num >= 224 && num <= 239)
        return false;
   }
   if(parseInt(addrParts[3]) == 0)
                return false;
   return true;
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
		
	e= document.getElementById("id_linkbackup");
	        e.innerHTML = _("lbk_id_linkbackup");
	e= document.getElementById("id_linkbackup_operation_mode");
	        e.innerHTML = _("lbk_id_linkbackup_operation_mode");
	e= document.getElementById("id_link_backup_status");
	        e.innerHTML = _("lbk_id_link_backup_status");

	e= document.getElementById("id_back_to_main");
	        e.innerHTML = _("lbk_id_back_to_main");
	e= document.getElementById("id_linkbackup_router_priority_setting");
	        e.innerHTML = _("lbk_id_linkbackup_router_priority_setting");
			
		e= document.getElementById("cell_high_prio");
	        e.innerHTML = _("lbk_high_prio");
		e= document.getElementById("cell_low_prio");
	        e.innerHTML = _("lbk_low_prio");
			
		e= document.getElementById("wan_high_prio");
	        e.innerHTML = _("lbk_high_prio");
		e= document.getElementById("wan_low_prio");
	        e.innerHTML = _("lbk_low_prio");

	        	e= document.getElementById("id_wan_mode_cellular");
	        e.innerHTML = _("lbk_id_wan_mode_cellular");
			
		e= document.getElementById("id_linkbackup_connectivity_check");
	        e.innerHTML = _("lbk_id_linkbackup_connectivity_check");
			
		e= document.getElementById("id_linkbackup_check_count");
	        e.innerHTML = _("lbk_id_linkbackup_check_count");

		e= document.getElementById("id_linkbackup_wan1_check_method");
	        e.innerHTML = _("lbk_id_linkbackup_wan1_check_method");

		e= document.getElementById("id_link_backup_apply");
	        e.value = _("lbk_id_link_backup_apply");


 
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



function wan1_ping_sel_select()
{
   with ( document.forms[0] ) {
      var wan1_ping = wan1_ping_sel[wan1_ping_sel.selectedIndex].value;

 

      switch (wan1_ping) {
      case "ping_gateway":
                  showhide("id_link_backup_wan1_host", 0);
          break;
        case "ping_ip":
                  showhide("id_link_backup_wan1_host", 1);  
        break;
      default:
      }
  }


	
}

function wan2_ping_sel_select()
{
   with ( document.forms[0] ) {
      var wan2_ping = wan2_ping_sel[wan2_ping_sel.selectedIndex].value;

 

      switch (wan2_ping) {
      case "ping_gateway":
                  showhide("id_link_backup_wan2_host", 0);
          break;
        case "ping_ip":
                  showhide("id_link_backup_wan2_host", 1);  
        break;
      default:
      }
  }


	
}



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





 function applyClick()
 {

	

	if(document.form_linkbackup.link_backup_status.checked==true){

		

			if(isCheckBoxSelected("linkbackup_cellural_priority_sel")==false){
				alert(_("please choose cellural priority!"));
				return false;
			}

			if(isCheckBoxSelected("linkbackup_wan_priority_sel")==false){
				alert(_("please choose wan priority!"));
				return false;
			}

			
			if (getRadioValueByName("linkbackup_cellural_priority_sel") == getRadioValueByName("linkbackup_wan_priority_sel") ){
				alert(_("please choose different priority!"));
				return false;
			}

			if(document.form_linkbackup.link_backup_checkcount.value==""){
		           alert(_("please input a number!"));
			       document.form_linkbackup.link_backup_checkcount.focus();
			      return false; 
			}
			
			  if (!isDigital(document.form_linkbackup.link_backup_checkcount.value)){
		           alert(_("please input number!"));
			       document.form_linkbackup.link_backup_checkcount.focus();
			      return false; 
		       }

			  
			  if((document.form_linkbackup.link_backup_checkcount.value< 1) || (document.form_linkbackup.link_backup_checkcount.value>50)){
			  	
				alert(_("please input number (1-50) !"));
			       document.form_linkbackup.link_backup_checkcount.focus();
			      return false; 
			  }


			if(document.form_linkbackup.wan1_ping_sel[document.form_linkbackup.wan1_ping_sel.selectedIndex].value=="ping_ip"){

				  if(document.form_linkbackup.link_backup_wan1_host.value==""){
			      		    alert(_("please input  IP address!"));
				     	 document.form_linkbackup.link_backup_wan1_host.focus();
				    	  return false; 
				  }
			    if(!isValidIpAddress(document.form_linkbackup.link_backup_wan1_host.value)){
			      	    alert(_("please input a true IP address!"));
				      document.form_linkbackup.link_backup_wan1_host.focus();
				      return false; 
			       }

			}
/*
			if(document.form_linkbackup.wan2_ping_sel[document.form_linkbackup.wan2_ping_sel.selectedIndex].value=="ping_ip"){
				  if(document.form_linkbackup.link_backup_wan2_host.value==""){
			      		    alert(_("please input  IP address!"));
				     	 document.form_linkbackup.link_backup_wan2_host.focus();
				    	  return false; 
				}
			    if(!isValidIpAddress(document.form_linkbackup.link_backup_wan2_host.value)){
			          alert(_("please input a true IP address!"));
				      document.form_linkbackup.link_backup_wan2_host.focus();
				      return false; 
			       }

			}
*/	
	}


	


  
}


	
 function disp_para()
{
	var tmp;

	
    tmp="<% getCfgGeneral(1,"link_backup_status"); %>"
	if(tmp=="on"){
                document.form_linkbackup.link_backup_status.checked=true;
        }else{
      		    document.form_linkbackup.link_backup_status.checked=false;
    	}
    tmp="<% getCfgGeneral(1,"link_backup_backtomain"); %>"
	if(tmp=="on"){
                document.form_linkbackup.link_backup_backtomain.checked=true;
        }else{
      		    document.form_linkbackup.link_backup_backtomain.checked=false;
    	}

      tmp="<% getCfgGeneral(1,"link_backup_first"); %>"
	  if(tmp=="3G") {
			selectRadio("linkbackup_cellural_priority_sel","0");
	}else if(tmp=="DHCP"){
		 document.form_linkbackup.linkbackup_wan_sel.selectedIndex=0;
		 selectRadio("linkbackup_wan_priority_sel","0");
	}else if(tmp=="STATIC"){
		 document.form_linkbackup.linkbackup_wan_sel.selectedIndex=1;
		 selectRadio("linkbackup_wan_priority_sel","0");		
	}else if(tmp=="PPPOE"){
		 document.form_linkbackup.linkbackup_wan_sel.selectedIndex=2;
		 selectRadio("linkbackup_wan_priority_sel","0");		
	}
	
      tmp="<% getCfgGeneral(1,"link_backup_second"); %>"
	  if(tmp=="3G") {
			selectRadio("linkbackup_cellural_priority_sel","1");
	}else if(tmp=="DHCP"){
		 document.form_linkbackup.linkbackup_wan_sel.selectedIndex=0;
		 selectRadio("linkbackup_wan_priority_sel","1");
	}else if(tmp=="STATIC"){
		 document.form_linkbackup.linkbackup_wan_sel.selectedIndex=1;
		 selectRadio("linkbackup_wan_priority_sel","1");		
	}else if(tmp=="PPPOE"){
		 document.form_linkbackup.linkbackup_wan_sel.selectedIndex=2;
		 selectRadio("linkbackup_wan_priority_sel","1");		
	}


		
		
	tmp="<% getCfgGeneral(1,"link_backup_checkcount"); %>"
        document.form_linkbackup.link_backup_checkcount.value=tmp;
		  
		  
		  
	tmp="<% getCfgGeneral(1,"link_backup_ping"); %>"
	if(tmp=="ping_gateway"){
     		   	document.form_linkbackup.wan1_ping_sel.selectedIndex=1;
	}else {
     		   	document.form_linkbackup.wan1_ping_sel.selectedIndex=0;			
	}

	 tmp="<% getCfgGeneral(1,"link_backup_host"); %>"
	document.form_linkbackup.link_backup_wan1_host.value=tmp;


	tmp="<% getCfgGeneral(1,"link_backup_wan2_ping"); %>"
	if(tmp=="ping_gateway"){
     		   	document.form_linkbackup.wan2_ping_sel.selectedIndex=1;
	}else {
     		   	document.form_linkbackup.wan2_ping_sel.selectedIndex=0;			
	}

	 tmp="<% getCfgGeneral(1,"link_backup_wan2_host"); %>"
	document.form_linkbackup.link_backup_wan2_host.value=tmp;


			

 }
   
 function load_cur_para()	
{
       		disp_para();
}



function formLoad()
{
        load_cur_para();
	wan1_ping_sel_select();
	wan2_ping_sel_select();
	 initTranslation();
}

 
</script>

</head>





<body onLoad="formLoad()">
<center id="boxes">
	<div id="box">
	<div id="head"></div>	<!--end of head-->	
	<div id="content">

	<table id="layout_table" border="0"><!--start of layout_table-->
<tr><!--start of layout tr-->
<td class="tdwidth1" id="td1"><!--start of td1-->
<div id="left"><!--start of left-->


<h1 class="STYLE14" id="id_linkbackup" align="left">Route Fail Over </h1>

<hr /> <br/>

<form method=post name="form_linkbackup" action="/goform/linkbackup_action" onSubmit="return applyClick()">

<table width="588" border="0" cellpadding="2" cellspacing="1"  >
	<tr>
		<td width="578"		 class="title"  id="id_linkbackup_connectivity_check" colspan="2">
		<span class="STYLE14"><font color=#FFFFFF>Connectivity Check</font></span>
		</td>
	</tr>
	
	<tr>
		<td width="40%" id="id_linkbackup_check_count">Check Count</td>
		<td>
			<input name="link_backup_checkcount" maxlength=2  size=2  type="text" >  &nbsp;(1-50)
		</td>
	</tr>

 </table>


<table width="588" border="0" cellpadding="3" cellspacing="1"  >
	<tr>
		<td  width="40%" id="id_linkbackup_wan1_check_method">Check method</td>
		<td width="20%" >
			<select name="wan1_ping_sel"  onChange="wan1_ping_sel_select();">

				<option id="id_option_ping_ip" value="ping_ip">ping ip</option>
<!--			<option id="id_option_ping_gateway" value="ping_gateway">ping gateway</option>   -->
			</select>
		</td>
		<td>
			<input name="link_backup_wan1_host" id="id_link_backup_wan1_host" maxlength=15  size=15  type="text" >
			
		</td>
	</tr>


	
	<tr style="display:none">
		<td  width="40%" id="id_linkbackup_wan2_check_method">WAN2 Check method</td>
		<td width="20%">
			<select name="wan2_ping_sel" onChange="wan2_ping_sel_select();" >
				<option id="id_option_ping_ip" value="ping_ip">ping ip</option>
				<option id="id_option_ping_gateway" value="ping_gateway">ping gateway</option>
			</select>
		</td>
		<td>
			<input name="link_backup_wan2_host"  id="id_link_backup_wan2_host"   maxlength=15  size=15 type="text" >
			
		</td>
	</tr>
 </table>



 <table width="588" cellpadding="2" cellspacing="1"  border="0" >
    <tr>
	<td class="title"  id="id_linkbackup_operation_mode" colspan="2">
		 <span class="STYLE14"><font color=#FFFFFF>Operation Mode </font></span>
	</td>
   </tr>
   
	<tr>
		<td width="40%" id="id_link_backup_status" >Active/Passive</td>
		<td>
			<input type="checkbox"  name="link_backup_status">
		</td>

	</tr>
	
	<tr>
		<td width="40%" id="id_back_to_main" >Back To Primary WAN When Possible</td>
		<td>
			<input type="checkbox"  name="link_backup_backtomain">
		</td>
	</tr>

    
 </table>




 
 <table width="588" cellpadding="2" cellspacing="1"  border="0" >
    <tr>
	<td class="title"  id="id_linkbackup_router_priority_setting" colspan="2">
	<span class="STYLE14"><font color=#FFFFFF>Router Priority</font></span>
	</td> 
   </tr>

   
   <tr>
	<td id="id_wan_mode_cellular" width="40%">Cellular</td>
	<td>
		<input type="radio"  name="linkbackup_cellural_priority_sel" value="0" ><font id="cell_high_prio">High Priority</font>
		<input type="radio"  name="linkbackup_cellural_priority_sel" value="1" ><font id="cell_low_prio">Low Priority</font>
	</td>
   </tr>

   
	<tr>
		<td width="40%">
			<select  name="linkbackup_wan_sel">
				<option id="id_option_dhcp" value="DHCP">DHCP</option>
				<option id="id_option_static" value="STATIC">STATIC</option>
				<option id="id_option_pppoe"  value="PPPOE">PPPOE</option>    
			</select> 
		</td>
		<td>
			<input type="radio"  name="linkbackup_wan_priority_sel" value="0" ><font id="wan_high_prio" >High Priority</font>
			<input type="radio"  name="linkbackup_wan_priority_sel" value="1" ><font  id="wan_low_prio">Low Priority</font>
		</td>
	</tr>
 </table>







 






 <p>
<table width="540" border="0" cellpadding="2" cellspacing="1">
	<tr align="center">
		<td>
			<input name="link_backup_apply" type="submit"  id="id_link_backup_apply" value="Apply" >
		</td>  
	</tr>
</table>
</p>

  
</form>  



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

</html>
