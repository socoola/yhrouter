<html>
<head>
<title>ipsec</title>

<link rel="stylesheet" href="/css/normal_ws.css" type="text/css">
<link rel="stylesheet" href="/css/boxStyle.css" type="text/css">
<meta http-equiv="content-type" content="text/html;charset=gb2312" />
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("admin");   


function showhide(element, flag)
{
        var e = document.getElementById(element);
        if (e) {
                if (0 == flag) {
                        e.style.visibility = "hidden";
                        e.style.display = "none";
                }else{
                        e.style.visibility = "visible";
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




function initTranslation()
{
        var e;

	e= document.getElementById("rb_settings");
	        e.innerHTML = _("ICMP Reboot");
	e= document.getElementById("rb_timer_settings");
	        e.innerHTML = _("id_rb_timer_settings");	
	e= document.getElementById("rb_when_timeout");
	        e.innerHTML = _("id_rb_when_timeout");
	e= document.getElementById("rb_timer_str");
	        e.innerHTML = _("id_rb_timer_str");
			
	e= document.getElementById("rb_timer_apply");
	        e.value = _("id_rb_timer_apply");
			
	e= document.getElementById("rb_reboot_system");
	        e.innerHTML = _("id_rb_reboot_system");
	e= document.getElementById("rb_reboot_now");
	        e.innerHTML = _("id_rb_reboot_now");

	e= document.getElementById("rb_reboot");
	        e.value = _("id_rb_reboot");			
		


	e= document.getElementById("rb_network_settings");
	        e.innerHTML = _("id_rb_network_settings");
	e= document.getElementById("rb_network_error");
	        e.innerHTML = _("id_rb_network_error");
	e= document.getElementById("rb_network_check_method");
	        e.innerHTML = _("id_rb_network_check_method");
//	e= document.getElementById("rb_network_option_ping_ip");
//	        e.innerHTML = _("id_rb_network_option_ping_ip");
	e= document.getElementById("rb_network_check_interval_time_str");
	        e.innerHTML = _("id_rb_network_check_interval_time_str");
	e= document.getElementById("rb_network_check_count");
	        e.innerHTML = _("id_rb_network_check_count");
	e= document.getElementById("rb_network_comment");
	        e.innerHTML = _("id_rb_network_comment");
	e= document.getElementById("rb_network_apply");
	        e.value = _("id_rb_network_apply");		
	e= document.getElementById("rb_check_host_ip");
	        e.value = _("id_rb_check_host_ip");	
	e= document.getElementById("rb_check_host_ip_2");
	        e.value = _("id_rb_check_host_ip");	



	e= document.getElementById("rb_network_sleep_count");
	        e.innerHTML = _("id_rb_network_sleep_count");
	e= document.getElementById("rb_network_sleep_time");
	        e.innerHTML = _("id_rb_network_sleep_time");
	        
	e = document.getElementById("help_head");
	e.innerHTML = _("help help_head");
	e = document.getElementById("icmp_direc");
	e.innerHTML = _("icmp icmp_direc");

}



//////////////////////////////////////////////////////////////////////////////////////////////



function isDigital(s)
{
    var r,re;
    re = /\d*/i;
    r = s.match(re);
    return (r==s)?1:0;
}


function rb_timer_applyClick() {


	if(document.rb_timer_form.rb_when_timeout.checked==true){

	

		if((document.rb_timer_form.rb_timer.value<1)||(document.rb_timer_form.rb_timer.value>9999999))
		{
				alert(_("please input the timer  !"));
				 document.rb_timer_form.rb_timer.focus();
				 return false;
		}else{
			if(!isDigital(document.rb_timer_form.rb_timer.value))
			{
				alert(_("please input digital for timer !"));
				 document.rb_timer_form.rb_timer.focus();
				 return false;
			}
		}
	}
                return true;

}


function rb_network_applyClick() {


	if(document.rb_network_form.rb_when_network_error.checked==true){
		if(getSelect(document.rb_network_form.rb_network_ping_sel)=="ping_ip"){
			if(!isDigital(document.rb_network_form.rb_network_check_interval_time.value))
			{
				alert(_("please input digital!"));
				 document.rb_network_form.rb_network_check_interval_time.focus();
				 return false;
			}
			if((document.rb_network_form.rb_network_check_interval_time.value<60)||(document.rb_network_form.rb_network_check_interval_time.value>86400)){
				alert(_("please input digital (60--86400)!"));
				 document.rb_network_form.rb_network_check_interval_time.focus();
				 return false;	
			}

			 if(document.rb_network_form.rb_network_check_host.value=="")
                        {
                                alert(_("please input host!"));
                                 document.rb_network_form.rb_network_check_host.focus();
                                 return false;
                        }

			 if(document.rb_network_form.rb_network_check_host_2.value=="")
                        {
                                alert(_("please input host!"));
                                 document.rb_network_form.rb_network_check_host_2.focus();
                                 return false;
                        }

			if(!isDigital(document.rb_network_form.rb_network_checkcount.value))
			{
				alert(_("please input digital!"));
				 document.rb_network_form.rb_network_checkcount.focus();
				 return false;
			}
			if((document.rb_network_form.rb_network_checkcount.value<3)||(document.rb_network_form.rb_network_checkcount.value>1000)){
				alert(_("please input digital (3--1000)!"));
				 document.rb_network_form.rb_network_checkcount.focus();
				 return false;	
			}

			if(!isDigital(document.rb_network_form.rb_network_sleep_count.value))
			{
				alert(_("please input digital!"));
				 document.rb_network_form.rb_network_sleep_count.focus();
				 return false;
			}
			if((document.rb_network_form.rb_network_sleep_count.value<2)||(document.rb_network_form.rb_network_sleep_count.value>50)){
				alert(_("please input digital (2--50)!"));
				 document.rb_network_form.rb_network_sleep_count.focus();
				 return false;	
			}
			if(!isDigital(document.rb_network_form.rb_network_sleep_time.value))
			{
				alert(_("please input digital!"));
				 document.rb_network_form.rb_network_sleep_time.focus();
				 return false;
			}
			if((document.rb_network_form.rb_network_sleep_time.value<10)||(document.rb_network_form.rb_network_sleep_time.value>43200)){
				alert(_("please input digital (10--43200)!"));
				 document.rb_network_form.rb_network_sleep_time.focus();
				 return false;	
			}
			
			
		}else{ //ping_gateway not used

		}


	}
        return true;

}



function disp_para()
{
	var tmp;


// rb_timer_form
        tmp="<% getCfgGeneral(1,"rb_when_timeout"); %>"
   if(tmp=="on"){
               document.rb_timer_form.rb_when_timeout.checked=true;
        }else{
      		  document.rb_timer_form.rb_when_timeout.checked=false;
    	}

	
        tmp="<% getCfgGeneral(1,"rb_timer"); %>"
        document.rb_timer_form.rb_timer.value=tmp;	






// rb_network_form
        tmp="<% getCfgGeneral(1,"rb_when_network_error"); %>"
 	  if(tmp=="on"){
               document.rb_network_form.rb_when_network_error.checked=true;
        }else{
      		  document.rb_network_form.rb_when_network_error.checked=false;
    	}


	        tmp="<% getCfgGeneral(1,"rb_network_check_method"); %>"
 	  if(tmp=="ping_gateway"){
               document.rb_network_form.rb_network_ping_sel.index=1;
        }else{
      		  document.rb_network_form.rb_network_ping_sel.index=0;
    	}
        tmp="<% getCfgGeneral(1,"rb_network_check_host"); %>"
        document.rb_network_form.rb_network_check_host.value=tmp;	

        tmp="<% getCfgGeneral(1,"rb_network_check_host_2"); %>"
        document.rb_network_form.rb_network_check_host_2.value=tmp;	

	   tmp="<% getCfgGeneral(1,"rb_network_check_interval_time"); %>"
        document.rb_network_form.rb_network_check_interval_time.value=tmp;	

		   tmp="<% getCfgGeneral(1,"rb_network_checkcount"); %>"
        document.rb_network_form.rb_network_checkcount.value=tmp;	

	tmp="<% getCfgGeneral(1,"rb_network_sleep_count"); %>"
	document.rb_network_form.rb_network_sleep_count.value=tmp;
	tmp="<% getCfgGeneral(1,"rb_network_sleep_time"); %>"
	document.rb_network_form.rb_network_sleep_time.value=tmp;

	
}



function rb_network_ping_sel_select()
{
   with ( document.forms[0] ) {
      var rb_networkpingsel= document.rb_network_form.rb_network_ping_sel[document.rb_network_form.rb_network_ping_sel.selectedIndex].value;

 

      switch (rb_networkpingsel) {
      case "ping_gateway":
                  showhide("rb_network_check_host", 0);
          break;
        case "ping_ip":
                  showhide("rb_network_check_host", 1);  
        break;
      default:
      }
  }


	
}



function load_cur_para()	
{
       		disp_para();
}



function formLoad()
{
        load_cur_para();
	rb_network_ping_sel_select();
	initTranslation();
}

function button_click(act_type)
{
	document.rb_network_form.rb_button_sign.value=act_type;
	return true;
}

/*
function check_hostip_fun()
{
	var tmp;
	if(tmp=="OK"){
	       alert(_("Host/IP Ping OK !"));
               document.rb_network_form.rb_network_check_host.focus();
               return false;	
	}else{
	       alert(_("Host/IP Ping Error !"));
               document.rb_network_form.rb_network_check_host.focus();
               return false;	
		
	}
}
*/




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




<h1 id="rb_settings" align="left">Reboot Settings </h1>

<hr /> <br/>







<form method=post name="rb_network_form" action="/goform/rb_network_action" onSubmit="return rb_network_applyClick()">

  <table border="0" cellpadding="2" cellspacing="1" width="540"   bordercolor="#9BABBD">
    <tr>
    		<td class="title" colspan="2" id="rb_network_settings">ICMP check and Reboot Settings</td>
     </tr>

    <tr>
      <td width="40%" id="rb_network_error">Reboot When Network Error</td>
	<td>
             <input type="checkbox"  name="rb_when_network_error">
	</td>
    </tr>

   </table>
    
      <table border="0" cellpadding="3" cellspacing="1" width="540" >
	<tr>
		<td  width="40%" rowspan="2" id="rb_network_check_method">Check method</td>


		<td width="25%" style="display:none">
			<select name="rb_network_ping_sel"  onChange="rb_network_ping_sel_select();">

				<option id="rb_network_option_ping_ip" value="ping_ip">ping host/IP</option>
				<option id="id_rb_network_option_ping_gateway" value="ping_gateway">ping gateway</option>   
			</select>
		</td>


		<td>
			<input name="rb_network_check_host" maxlength=100  size=15  type="text" >
			
		</td>
		<td>
	        	<input type="submit" value="Host/IP check" id="rb_check_host_ip" name="rb_check_host_ip_button" onClick="return button_click('check_1')">
		</td>
	</tr>

	<tr>
		<td>
			<input name="rb_network_check_host_2" maxlength=100  size=15  type="text" >
			
		</td>
		<td>
	        	<input type="submit" value="Host/IP check" id="rb_check_host_ip_2" name="rb_check_host_ip_button_2" onClick="return button_click('check_2')">
		</td>	
	</tr>

	
	</table>

	<table border="0" cellpadding="2" cellspacing="1" width="540" >
		<tr>
			<td width="40%" id="rb_network_check_interval_time_str">Check interval time (sec)</td>
			<td>
				<input name="rb_network_check_interval_time" maxlength=5  size=5  type="text" >  &nbsp;(60-86400)
			</td>
		</tr>
		
		<tr>
			<td width="40%" id="rb_network_check_count">Check Count</td>
			<td>
				<input name="rb_network_checkcount" maxlength=4  size=4  type="text" >  &nbsp;(3-1000)
			</td>
		</tr>




		<tr>
			<td width="40%" id="rb_network_sleep_count">Reboot Count Before Sleep</td>
			<td>
				<input name="rb_network_sleep_count" maxlength=2  size=2  type="text" >  &nbsp;(2-50)
			</td>
		</tr>


		<tr>
			<td width="40%" id="rb_network_sleep_time">Sleep Time (min)</td>
			<td>
				<input name="rb_network_sleep_time" maxlength=5  size=5  type="text" >  &nbsp;(10-43200)
			</td>
		</tr>
	
	  </table>
	  
	<table width="540" border="0" cellpadding="1" cellspacing="1">
	  <tr>
		<td id="rb_network_comment">Comment: It is only used for 3G keep_alive and on_time mode! </td>
	  </tr>
	</table>
	
	<table width="540" border="0" cellpadding="2" cellspacing="1">
 
	  <tr align="center">
	    <td>
	         <input type="submit" value="Apply" id="rb_network_apply" name="rb_network_apply_button" onClick="return button_click('apply')" >
		<input name="rb_button_sign" type="hidden" value="" > 
	        <!-- <input type="button" value="Host/IP check" id="rb_check_host_ip" name="rb_check_host_ip_button" onClick="check_hostip_fun()" >
	        <input type="submit" value="Host/IP check" id="rb_check_host_ip" name="rb_check_host_ip_button" >
		-->
	    </td>
	  </tr>
	</table>

</form>











<form style="display:none" method=post name="rb_timer_form" action="/goform/rb_timer_action" onSubmit="return rb_timer_applyClick()">

  <table border="0" cellpadding="2" cellspacing="1" width="540"   bordercolor="#9BABBD">
    <tr>
    		<td class="title" colspan="2" id="rb_timer_settings">Reboot Timer Settings</td>
     </tr>
    <tr>
    
      <td width="40%" id="rb_when_timeout">Reboot When Timeout</td>
	<td>
             <input type="checkbox"  name="rb_when_timeout">
	</td>
    </tr>

	<tr>
		 <td width="40%" id="rb_timer_str">Timer</td>
   		 <td><input name="rb_timer" maxlength=7 size=7 type="text"></td>
   	</tr>

  </table>
	<table width="540" border="0" cellpadding="2" cellspacing="1">
 
	  <tr align="center">
	    <td>
	         <input type="submit" value="Apply" id="rb_timer_apply" name="rb_timer_apply_button" >
	    </td>
	  </tr>
	</table>



</form>










<form style="display:none" method=post name="rb_form" action="/goform/rb_action">

<table border="0" cellpadding="2" cellspacing="1" width="540"   bordercolor="#9BABBD">


<tr>
    		<td class="title" colspan="2" id="rb_reboot_system">Reboot System</td>
 </tr>

    	 
  <tr >
  <td width="40%" id="rb_reboot_now">Reboot Now</td>
    <td>
    	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
         <input type="submit" value="Reboot" id="rb_reboot" name="rb_reboot_button" >
    </td>
  </tr>
  
  
</table>


</form>

</div><!--end of left-->
</td><!--end of td1-->

<td class="tdwidth2" id="td2"><!--start of td2-->
	<div id="right"><!--start of right-->
		<h2 id="help_head">Heeelp...<a href="#">more</a></h2>
		
		<p id="help_content"><span id="icmp_direc"></span></p>
	</div><!--end of right-->
</td><!--end of td2-->
</tr><!--end of layout tr-->
</table><!--end of layout_table-->



</div><!--end of content-->
	</div>
	</center>

</body>
</html>

