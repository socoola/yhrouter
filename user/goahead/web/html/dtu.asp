<html>
<head>
<title>ipsec</title>
<link rel="stylesheet" href="/css/normal_ws.css" type="text/css">
<link rel="stylesheet" href="/css/boxStyle.css" type="text/css">
<meta http-equiv="content-type" content="text/html;charset=gb2312" />
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("internet");   


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




	e= document.getElementById("dtu_settings");
	        e.innerHTML = _("dtu setting");
	e= document.getElementById("dtu_status_table");
	        e.innerHTML = _("dtu status table");
	e= document.getElementById("dtu_status");
	        e.innerHTML = _("dtu status");
	e= document.getElementById("option_off");
	        e.innerHTML = _("option off");
	e= document.getElementById("option_on");
	        e.innerHTML = _("option on");
	e= document.getElementById("dtu_serial_table");
	        e.innerHTML = _("id dtu serial table");
			
	e= document.getElementById("dtu_bandrate");
	        e.innerHTML = _("dtu bandrate");
	e= document.getElementById("dtu_parity");
	        e.innerHTML = _("dtu parity");
	e= document.getElementById("option_none");
	        e.innerHTML = _("option none");
	e= document.getElementById("option_odd");
	        e.innerHTML = _("option odd");
	e= document.getElementById("option_even");
	        e.innerHTML = _("option even");
	e= document.getElementById("dtu_databits");
	        e.innerHTML = _("dtu databits");
	e= document.getElementById("dtu_stopbits");
	        e.innerHTML = _("dtu stopbits");
			
	e= document.getElementById("dtu_flowcontrol");
	        e.innerHTML = _("dtu flowcontrol");
	e= document.getElementById("option_n");
	        e.innerHTML = _("option n");
	e= document.getElementById("option_h");
	        e.innerHTML = _("option h");
	e= document.getElementById("option_s");
	        e.innerHTML = _("option s");
	e= document.getElementById("dtu_config_table");
	        e.innerHTML = _("dtu config");
			
	e= document.getElementById("dtu_linktype");
	        e.innerHTML = _("dtu linktype");
	e= document.getElementById("option_server");
	      e.innerHTML = _("option server");                                                              
	e= document.getElementById("option_client");
	       e.innerHTML = _("option client");
	e= document.getElementById("dtu_socktype");
	       e.innerHTML = _("dtu socktype");
		   
	e= document.getElementById("dtu_server_port");
	       e.innerHTML = _("dtu server port");
	e= document.getElementById("dtu_center_1");
	       e.innerHTML = _("dtu center 1");
	e= document.getElementById("dtu_center_2");
	       e.innerHTML = _("dtu center 2");
	e= document.getElementById("dtu_center_3");
	       e.innerHTML = _("dtu center 3");
	e= document.getElementById("dtu_center_4");
	       e.innerHTML = _("dtu center 4");
	e= document.getElementById("dtu_heart_time");
	       e.innerHTML = _("dtu heart time");
	e= document.getElementById("dtu_heart_info");
	       e.innerHTML = _("dtu heart info");
	e= document.getElementById("dtu_senddata_time");
	      e.innerHTML = _("dtu send data time");
	e= document.getElementById("dtu_off_heart_after_nodata_select");
	      e.innerHTML = _("dtu off heart after nodata select");
	e= document.getElementById("dtu_off_heart_delay_time");
	      e.innerHTML = _("dtu off heart delay time");
	      
	
	e= document.getElementById("dtu_setting_new");
	      e.innerHTML = _("dtu settings");
	e= document.getElementById("data_center_config_new");
	      e.innerHTML = _("data center configuration");
	e= document.getElementById("heartbeat_settings");
	      e.innerHTML = _("heartbeat settings");
	e= document.getElementById("uart_settings");
	      e.innerHTML = _("uart settings");
	
	e= document.getElementById("sever_name");
	      e.innerHTML = _("Server Name");
	e= document.getElementById("ip_address");
	      e.innerHTML = _("Ip Address");
	e= document.getElementById("server_port");
	      e.innerHTML = _("Port");
	e= document.getElementById("heartbeatData");
	      e.innerHTML = _("dtu heartbeatData");
	      
	e= document.getElementById("heartBeatService");
	      e.innerHTML = _("dtu heartBeatService");
	
	e= document.getElementById("hbSta1");
	      e.innerHTML = _("dtu hbSta1");
	
	e= document.getElementById("hbSta2");
	      e.innerHTML = _("dtu hbSta2");
	         
	e= document.getElementById("dtu_apply");
	       e.value = _("dtu apply");
	
	e = document.getElementById("help_head");
	e.innerHTML = _("help help_head");
	
	e= document.getElementById("dtu_direc1");
	e.innerHTML = _("dtu dtu_direc1");
	e= document.getElementById("dtu_direc2");
	e.innerHTML = _("dtu dtu_direc2");
	e= document.getElementById("dtu_direc3");
	e.innerHTML = _("dtu dtu_direc3");
	


		
//	e= document.getElementById("dtu_settings");
    //    e.innerHTML = _("dtu setting");


		
/*
        var e;
		
	e= document.getElementById("Ipseccfgtitle");
        e.innerHTML = _("Ipseccfg title");


        e = document.getElementById("ipseccfg_apply");
        e.value = _("firewall apply");
*/
}



//////////////////////////////////////////////////////////////////////////////////////////////


function dtu_linktype_select() {
   with ( document.forms[0] ) {
      var dtu_status = dtu_linktype_sel[dtu_linktype_sel.selectedIndex].value;

 

      switch (dtu_status) {
      case "server":
                  showhide("server_block", 1);
                  showhide("client_block", 0); 
          break;
        case "client":
                  showhide("server_block", 0);
                  showhide("client_block", 1);       
        break;
      default:
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


function isValidHex(s)
{
 var  len,i;
    len = s.length;
     if(len % 2 == 0){
            for(  i = 0 ;i<len ; i++ ) {
                   if(!((s.charCodeAt(i)>=48&&s.charCodeAt(i)<=57)||(s.charCodeAt(i)>=65&&s.charCodeAt(i)<=70)||(s.charCodeAt(i)>=97&&s.charCodeAt(i)<=101)))
                        
                                {return false;
                                  break;                           }            
                                
                }       
        }

        
       else{ 
               return false;
        }

	return true;
}

function applyClick() {


		if(getSelect(document.dtu_config.dtu_linktype_sel) == "server" ){  //server
			if(document.dtu_config.dtu_serverport.value==""){
						alert(_("please input port for server!"));
						document.dtu_config.dtu_serverport.focus();
		                        	return false;	
			}
			if(!isDigital(document.dtu_config.dtu_serverport.value)){
				alert(_("invalid port of server!"));
				document.dtu_config.dtu_serverport.focus();
		              return false;		
			}
			
		}else{
		

				if((document.dtu_config.center_1_check.checked==false)&&
					(document.dtu_config.center_2_check.checked==false)&&
					(document.dtu_config.center_3_check.checked==false)&&
					(document.dtu_config.center_4_check.checked==false)
				){
					 alert(_("please select some center!"));
		                        return false;
				}

				if(document.dtu_config.center_1_check.checked==true){
					if(document.dtu_config.dtu_center_1_ip.value==""){
						alert(_("please input ip of center 1 !"));
						document.dtu_config.dtu_center_1_ip.focus();
		                        	return false;	
					}else if(document.dtu_config.dtu_center_1_port.value==""){
						alert(_("please input port of center 1 !"));
						document.dtu_config.dtu_center_1_port.focus();
						return false;	
					}
					/*
					 if ( isValidIpAddress(document.dtu_config.dtu_center_1_ip.value) == false ) {
		                                        alert(_("invalid ip of center 1"));
		                                        document.dtu_config.dtu_center_1_ip.focus();
		                                        return false;
		                       }
					*/
					if(!isDigital(document.dtu_config.dtu_center_1_port.value)){
						alert(_("invalid port of center 1!"));
						document.dtu_config.dtu_center_1_port.focus();
				              return false;		
					}


					
				}
					
				if(document.dtu_config.center_2_check.checked==true){
					if(document.dtu_config.dtu_center_2_ip.value==""){
						alert(_("please input ip of center 2 !"));
						document.dtu_config.center_2_check.focus();
		                        	return false;	
					}else if(document.dtu_config.dtu_center_2_port.value==""){
						alert(_("please input port of center 2 !"));
						document.dtu_config.dtu_center_2_port.focus();
						return false;	
					}
				/*	
					if ( isValidIpAddress(document.dtu_config.dtu_center_2_ip.value) == false ) {
		                                        alert(_("invalid ip of center 2"));
		                                        document.dtu_config.dtu_center_2_ip.focus();
		                                        return false;
		                       }
				*/
					if(!isDigital(document.dtu_config.dtu_center_2_port.value)){
						alert(_("invalid port of center 2!"));
						document.dtu_config.dtu_center_2_port.focus();
				              return false;		
					}
				}

				if(document.dtu_config.center_3_check.checked==true){
					if(document.dtu_config.dtu_center_3_ip.value==""){
						alert(_("please input ip of center 3 !"));
						document.dtu_config.dtu_center_3_ip.focus();
		                        	return false;	
					}else if(document.dtu_config.dtu_center_3_port.value==""){
						alert(_("please input port of center 3 !"));
						document.dtu_config.dtu_center_3_port.focus();
						return false;	
					}
					/*
					 if ( isValidIpAddress(document.dtu_config.dtu_center_3_ip.value) == false ) {
		                                        alert(_("invalid ip of center 3"));
		                                        document.dtu_config.dtu_center_3_ip.focus();
		                                        return false;
		                       }
					*/
					if(!isDigital(document.dtu_config.dtu_center_3_port.value)){
						alert(_("invalid port of center 3!"));
						document.dtu_config.dtu_center_3_port.focus();
				              return false;		
					}
				}

				if(document.dtu_config.center_4_check.checked==true){
					if(document.dtu_config.dtu_center_4_ip.value==""){
						alert(_("please input ip of center 4 !"));
						document.dtu_config.dtu_center_4_ip.focus();
		                        	return false;	
					}else if(document.dtu_config.dtu_center_4_port.value==""){
						alert(_("please input port of center 4 !"));
						document.dtu_config.dtu_center_4_port.focus();
						return false;	
					}
					/*
					 if ( isValidIpAddress(document.dtu_config.dtu_center_4_ip.value) == false ) {
		                                        alert(_("invalid ip of center 4"));
		                                        document.dtu_config.dtu_center_4_ip.focus();
		                                        return false;
		                       }
					*/
					 if(!isDigital(document.dtu_config.dtu_center_4_port.value)){
						alert(_("invalid port of center 4!"));
						document.dtu_config.dtu_center_4_port.focus();
				              return false;		
					}
				}


		                if ( document.dtu_config.dtu_h_time.value=="") {
		                        alert(_("please input heart beat interval time!"));
						document.dtu_config.dtu_h_time.focus();
		                        return false;
		                }
				if(!isDigital(document.dtu_config.dtu_h_time.value)){
					alert(_("invalid heart beat time!"));
					document.dtu_config.dtu_h_time.focus();
				             return false;		
				}

				
		                if ((document.dtu_config.dtu_h_info_type.checked==true)){
						 if(isValidHex(document.dtu_config.dtu_h_info.value)==false){
				                        alert(_("please input hex!"));
								document.dtu_config.dtu_h_info.focus();
				                        return false;
						 }
		                }
						
				if(!isDigital(document.dtu_config.dtu_send_data_time.value)){
					alert(_("invalid send data time!"));
					document.dtu_config.dtu_send_data_time.focus();
				             return false;		
				}	

		                if ((document.dtu_config.dtu_off_heart_after_nodata_recv.checked==true)){

				                if ( document.dtu_config.dtu_off_heart_after_nodata_time.value=="") {
			                        alert(_("please input off heart beat delay time!"));
							document.dtu_config.dtu_off_heart_after_nodata_time.focus();
			                        return false;		
			               	 }						
						 if(!isDigital(document.dtu_config.dtu_off_heart_after_nodata_time.value)){
				                        alert(_("invalid heart beat delay time!"));
								document.dtu_config.dtu_off_heart_after_nodata_time.focus();
				                        return false;
						 }
		                }




			}



                return true;

}




function disp_para()
{
	var tmp;
	var linktype;
	//dtu status
        tmp="<% getCfgGeneral(1,"dtu_status"); %>"
        if(tmp=="on"){
                document.dtu_config.dtu_status_sel.selectedIndex=1;
        }else{
                document.dtu_config.dtu_status_sel.selectedIndex=0;
        }

	//dtu bandrate
        tmp="<% getCfgGeneral(1,"dtu_bandrate"); %>"
        if(tmp=="115200"){
                document.dtu_config.dtu_bandrate_sel.selectedIndex=5;
        }else if(tmp=="57600"){
                document.dtu_config.dtu_bandrate_sel.selectedIndex=4;
	}else if(tmp=="38400"){
                document.dtu_config.dtu_bandrate_sel.selectedIndex=3;
	 }else if(tmp=="19200"){
                document.dtu_config.dtu_bandrate_sel.selectedIndex=2;
	}else if(tmp=="9600"){
                document.dtu_config.dtu_bandrate_sel.selectedIndex=1;
	 }else if(tmp=="4800"){
                document.dtu_config.dtu_bandrate_sel.selectedIndex=0;
        }else{
        	  document.dtu_config.dtu_bandrate_sel.selectedIndex=5;
        }

	//dtu parity
        tmp="<% getCfgGeneral(1,"dtu_parity"); %>"
        if(tmp=="e"){
                document.dtu_config.dtu_parity_sel.selectedIndex=2;
        }else if(tmp=="o"){
      		    document.dtu_config.dtu_parity_sel.selectedIndex=1;
    	}else{
    	 	   document.dtu_config.dtu_parity_sel.selectedIndex=0;
    	}
	//dtu databits
        tmp="<% getCfgGeneral(1,"dtu_databits"); %>"
        if(tmp=="7"){
                document.dtu_config.dtu_databits_sel.selectedIndex=0;
        }else{
      		    document.dtu_config.dtu_databits_sel.selectedIndex=1;
    	}
	//dtu stopbits
        tmp="<% getCfgGeneral(1,"dtu_stopbits"); %>"
        if(tmp=="2"){
                document.dtu_config.dtu_stopbits_sel.selectedIndex=1;
        }else{
      		    document.dtu_config.dtu_stopbits_sel.selectedIndex=0;
    	}



	//dtu flowcontrol
        tmp="<% getCfgGeneral(1,"dtu_flowcontrol"); %>"
        if(tmp=="s"){
                document.dtu_config.dtu_flowcontrol_sel.selectedIndex=2;
        }else if(tmp=="h"){
      		    document.dtu_config.dtu_flowcontrol_sel.selectedIndex=1;
    	}else{
    	 	   document.dtu_config.dtu_flowcontrol_sel.selectedIndex=0;
    	}

	//dtu socktype
        tmp="<% getCfgGeneral(1,"dtu_socktype"); %>"
        if(tmp=="udp"){
                document.dtu_config.dtu_socktype_sel.selectedIndex=1;
        }else{
      		    document.dtu_config.dtu_socktype_sel.selectedIndex=0;
    	}
		
	//dtu linktype
        tmp="<% getCfgGeneral(1,"dtu_linktype"); %>"
        if(tmp=="server"){
                document.dtu_config.dtu_linktype_sel.selectedIndex=0;
			linktype=0;
        }else{
      		    document.dtu_config.dtu_linktype_sel.selectedIndex=1;
			linktype=1;
    	}



		
	        //dtu center 1 ip
	        tmp="<% getCfgGeneral(1,"dtu_serverport"); %>"
	        document.dtu_config.dtu_serverport.value=tmp;


	

	

//center 1
		//dtu center 1 en/dis
        tmp="<% getCfgGeneral(1,"dtu_center1_status"); %>"
        if(tmp=="en"){
                document.dtu_config.center_1_check.checked=true;
        }else{
      		    document.dtu_config.center_1_check.checked=false;
    	}

        //dtu center 1 ip
        tmp="<% getCfgGeneral(1,"dtu_center1_ip"); %>"
        document.dtu_config.dtu_center_1_ip.value=tmp;



       //dtu center 1 ip
        tmp="<% getCfgGeneral(1,"dtu_center1_port"); %>"
        document.dtu_config.dtu_center_1_port.value=tmp;


//center 2
		//dtu center 2 en/dis
        tmp="<% getCfgGeneral(1,"dtu_center2_status"); %>"
        if(tmp=="en"){
                document.dtu_config.center_2_check.checked=true;
        }else{
      		    document.dtu_config.center_2_check.checked=false;
    	}

        //dtu center 2 ip
        tmp="<% getCfgGeneral(1,"dtu_center2_ip"); %>"
        document.dtu_config.dtu_center_2_ip.value=tmp;

       //dtu center 2 ip
        tmp="<% getCfgGeneral(1,"dtu_center2_port"); %>"
        document.dtu_config.dtu_center_2_port.value=tmp;

//center 3
		//dtu center 2 en/dis
        tmp="<% getCfgGeneral(1,"dtu_center3_status"); %>"
        if(tmp=="en"){
                document.dtu_config.center_3_check.checked=true;
        }else{
      		    document.dtu_config.center_3_check.checked=false;
    	}

        //dtu center 3 ip
        tmp="<% getCfgGeneral(1,"dtu_center3_ip"); %>"
        document.dtu_config.dtu_center_3_ip.value=tmp;

       //dtu center 3 port
        tmp="<% getCfgGeneral(1,"dtu_center3_port"); %>"
        document.dtu_config.dtu_center_3_port.value=tmp;

//center 4
		//dtu center 4 en/dis
        tmp="<% getCfgGeneral(1,"dtu_center4_status"); %>"
        if(tmp=="en"){
                document.dtu_config.center_4_check.checked=true;
        }else{
      		    document.dtu_config.center_4_check.checked=false;
    	}

        //dtu center 3 ip
        tmp="<% getCfgGeneral(1,"dtu_center4_ip"); %>"
        document.dtu_config.dtu_center_4_ip.value=tmp;

       //dtu center 3 port
        tmp="<% getCfgGeneral(1,"dtu_center4_port"); %>"
        document.dtu_config.dtu_center_4_port.value=tmp;


        //heart beat time
        tmp="<% getCfgGeneral(1,"dtu_h_time"); %>"
        document.dtu_config.dtu_h_time.value=tmp;

		//dtu heart_info-type
        tmp="<% getCfgGeneral(1,"dtu_h_info_type"); %>"
        if(tmp=="str"){
                document.dtu_config.dtu_h_info_type.checked=false;
        }else{
      		    document.dtu_config.dtu_h_info_type.checked=true;
    	}


        //heart beat info
        tmp="<% getCfgGeneral(1,"dtu_h_info"); %>"
        document.dtu_config.dtu_h_info.value=tmp;
		
        //send data time
        tmp="<% getCfgGeneral(1,"dtu_send_data_time"); %>"
       // if((tmp>999)||(tmp<0)) tmp=100;
        document.dtu_config.dtu_send_data_time.value=tmp;
		


        //off heart beat when no data heart beat 
        tmp="<% getCfgGeneral(1,"dtu_off_heart_after_nodata_recv"); %>"
         if(tmp=="en"){
                document.dtu_config.dtu_off_heart_after_nodata_recv.checked=true;
        }else{
      		    document.dtu_config.dtu_off_heart_after_nodata_recv.checked=false;
    	}

        //off heart beat delay time
        tmp="<% getCfgGeneral(1,"dtu_off_heart_after_nodata_time"); %>"
        document.dtu_config.dtu_off_heart_after_nodata_time.value=tmp;
	
	
	
}


function load_cur_para()	
{
       		disp_para();
}



function formLoad()
{
        load_cur_para();
	dtu_linktype_select();
	initTranslation();
	navigatorType();
	rmbHtStatusSelect();
	htStatus();
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
<div id="left" style="width:540;"><!--start of left-->


<h1 id="dtu_settings" align="left">DTU Settings</h1>

<hr /> <br/>
<form method=post name="dtu_config" action="/goform/dtu_action" onSubmit="return applyClick()">
	
	<fieldset>
		<style type="text/css">
				.hidden{
					display:none
				}
				.width1{width:255px}
				.width2{width:250px}
		</style>
		<legend id="dtu_setting_new" style="text-align:left;color:#3A587A;font-weight:bold">DTU Setting</legend>	
		<table border="0" cellpadding="2" cellspacing="1" width="540"   bordercolor="#9BABBD">
    <tr class="hidden">
    		<td class="title" colspan="2" id="dtu_status_table">DTU Status</td>
     </tr>
    <tr>
      <td width="50%" id="dtu_status">dtu status</td>
	    <td>
          <select name="dtu_status_sel">
            <option id="option_off" value="off">off</option>
            <option id="option_on" value="on">on</option>
           </select>
      </td>
    </tr>
    
    <tr>
				<td id="dtu_linktype">linktype</td>
				<td> 
			                <select name="dtu_linktype_sel" onChange="dtu_linktype_select();">
			                         <option id="option_server" value="server">server</option>
			                         <option id="option_client" value="client">client</option>
			                 </select>
				</td>
		</tr>
		
		<tr>
			<td  id="dtu_socktype">socktype</td>
			<td> 
		                <select name="dtu_socktype_sel">
		                         <option id="option_tcp" value="tcp">tcp</option>
		                         <option id="option_udp" value="udp">udp</option>
		                 </select>
			</td>
		</tr>
		<tr style="display:none">
					<td>Port</td>
					<td> <input type="text" size="5" /></td>
		</tr>
		<tr class="hidden">
					<td>Received Packet Max Length</td>
					<td> <input type="text" size="5" value="1024"/>Bytes</td>
		</tr>
		<tr class="hidden">
					<td>Received Timeout</td>
					<td> <input type="text" size="5" value="500"/>Milliseconds</td>
		</tr>
		<tr class="hidden">
					<td>UART Data Timeout</td>
					<td> <input type="text" size="5" value="500"/>Milliseconds</td>
		</tr>
  </table>
  
  
  
  
  		<fieldset>
  			<legend id="data_center_config_new" style="text-align:left;color:#3A587A;font-weight:bold">Data Center Configure</legend>
  			<table border="1" style="text-align:center;vertical-align:bottom;border:1px solid #000000">
  				<tr>
  					<td id="sever_name" width="25%" style="background:gray; color:#FFFFFF;border:none">Name</td>
  					<td id="ip_address" width="25%" style="background:gray; color:#FFFFFF;border:none">Ip Address</td>
  					<td id="server_port" width="10%" style="background:gray; color:#FFFFFF;border:none">Port</td>
  					<!--<td width="20%"></td>
  					<td width="20%"></td>-->
  				</tr>
  				<tr>
  					<td  id="dtu_center_1">center 1</td>
  					<td><input name="dtu_center_1_ip"  maxlength=100  size=15  type="text"> </td>
						<td> 
							<!--	<input type="checkbox" checked=\"checked\" name="center_1_check">  -->
							<input type="checkbox"  name="center_1_check" class="hidden">
		
							<input name="dtu_center_1_port"  maxlength=5 size=5   type="text"> 
						</td>
  				</tr>
  				<tr>
  					<td  id="dtu_center_2">center 2</td>
  					<td><input name="dtu_center_2_ip" maxlength=100  size=15 type="text"> </td>
  					
						<td> 
							<!--	<input type="checkbox" checked=\"checked\" name="center_1_check">  -->
							<input type="checkbox"  name="center_2_check" class="hidden">
							
							<input name="dtu_center_2_port" maxlength=5  size=5  type="text"> 
						</td>
  				</tr>
  				<tr>
  					<td  id="dtu_center_3">center 3</td>
  					<td> <input name="dtu_center_3_ip" maxlength=100  size=15 type="text"></td>
						<td> 
							<!--	<input type="checkbox" checked=\"checked\" name="center_1_check">  -->
							<input type="checkbox"  name="center_3_check" class="hidden">
							 
							<input name="dtu_center_3_port" maxlength=5  size=5 type="text"> 
						</td>
  				</tr>
  				<tr>
  					<td id="dtu_center_4">center 4</td>
  					<td><input name="dtu_center_4_ip" maxlength=100  size=15 type="text"> </td>
						<td> 
							<!--	<input type="checkbox" checked=\"checked\" name="center_1_check">  -->
							<input type="checkbox"  name="center_4_check" class="hidden">
							
							<input name="dtu_center_4_port"  maxlength=5  size=5 type="text"> 
						</td>
  				</tr>
  			</table>
  		</fieldset>
  		
  		<script type="text/javascript">
  			
  			function htStatus(){
  					var dtu_h_time_value = document.dtu_config.dtu_h_time.value;
  					if(document.getElementById("hbStatus").options[0].selected){
  							/*
  							document.getElementById("heartbeatData").style.display = "block";
  							document.getElementById("hbData").style.display = "block";
  							
  							document.getElementById("dtu_senddata_time").style.display = "block";
  							document.getElementById("sdTime").style.display = "block";
  							
  							document.getElementById("dtu_heart_time").style.display = "block";
  							document.getElementById("hbTime").style.display = "block";
  							
  							document.getElementById("dtu_off_heart_delay_time").style.display = "block";
  							document.getElementById("offTime").style.display = "block";*/
								
								document.getElementById("tableSetting").style.display = "block";
  							document.dtu_config.dtu_h_time.value = '<% getCfgGeneral(1,"dtu_h_time"); %>';
  						}
  					else if(document.getElementById("hbStatus").options[1].selected){
  							
  						/*	document.getElementById("heartbeatData").style.display = "none";
  							document.getElementById("hbData").style.display = "none";
  							
  							document.getElementById("dtu_senddata_time").style.display = "none";
  							document.getElementById("sdTime").style.display = "none";
  							
  							document.getElementById("dtu_heart_time").style.display = "none";
  							document.getElementById("hbTime").style.display = "none";
  							
  							document.getElementById("dtu_off_heart_delay_time").style.display = "none";
  							document.getElementById("offTime").style.display = "none";*/
  							
  							document.getElementById("tableSetting").style.display = "none";
  							document.dtu_config.dtu_h_time.value = 0;
  						}
  				}
  			
  			function rmbHtStatusSelect(){
  					if(document.dtu_config.dtu_h_time.value > 0){
  							document.getElementById("hbStatus").options[0].selected = true;
  						}
  					else
  						document.getElementById("hbStatus").options[1].selected = true;
  				}
  				
  			function navigatorType(){
  					if(navigator.userAgent.indexOf("MSIE")>0){
					   document.getElementById("heartBeatService").style.width = "247";
					   document.getElementById("heartbeatData").style.width = "255";
					  }else if(navigator.userAgent.indexOf("Firefox")>0){
					   document.getElementById("heartBeatService").style.width = "255";
					   document.getElementById("heartbeatData").style.width = "273";
					  }else if(navigator.userAgent.indexOf("Chrome")>0){
					   document.getElementById("heartBeatService").style.width = "255";
					   document.getElementById("heartbeatData").style.width = "276";
					  }else{
					   document.getElementById("heartBeatService").style.width = "255";
					   document.getElementById("heartbeatData").style.width = "255";
   					}
  				}
  			
  		</script>
  		<fieldset>
  			<legend id="heartbeat_settings" style="text-align:left;color:#3A587A;font-weight:bold">Heartbeat Settings</legend>
  			
  			<table width="500" >
					<tr>
						<td id="heartBeatService">Heartbeat Service</td>
						<td >
							<select id="hbStatus" onchange="htStatus()">
								<option id="hbSta1">Enable</option>
								<option id="hbSta2">Disable</option>
							</select>
						</td>		
					</tr>
				</table>
				<table width="500" id="tableSetting">
					<tr>
						<td  id="heartbeatData">Heartbeat Data</td>
						<td  id="hbData"><input name="dtu_h_info" maxlength=10  size=10 type="text"></td>
					</tr>
					<tr>
						<td id="dtu_heart_time">heart beat time</td>
						<td id="hbTime"> 
							<input name="dtu_h_time" maxlength=5 size=5  type="text">
							s ( 0 means disable ) 
						</td>
					</tr>
					
					
					<tr> 
							<td  id="dtu_off_heart_delay_time">off heart beat delay time</td>
							<td id="offTime" class="width2">
								<input name="dtu_off_heart_after_nodata_time" maxlength=5  size=5 type="text">
								s
							</td>
					</tr>
					
					<tr>
						<td " id="dtu_senddata_time"> send data time</td>
						<td id="sdTime" > 
							<input name="dtu_send_data_time"  maxlength=3 size=3   type="text" >
							ms ( 0~999 ) 
						</td>
					</tr>				
				</table>
  		</fieldset>
  		
  		
  		<fieldset>
  			<legend id="uart_settings" style="text-align:left;color:#3A587A;font-weight:bold">UART Setting</legend>
  			<table >
  				<tr>
						<td class="width1"  id="dtu_bandrate">serial baudrate</td>
						<td class="width2" > 
					                <select name="dtu_bandrate_sel" >
					                         <option id="option_4800" value="4800">4800</option>
					                         <option id="option_9600" value="9600">9600</option>
					                         <option id="option_19200" value="19200">19200</option>
					                         <option id="option_38400" value="38400">38400</option>
					                         <option id="option_57600" value="57600">57600</option>
					                         <option id="option_115200" value="115200" selected="selected">115200</option>
					                 </select>
					                 bps
						</td>
						
					</tr>
					<tr>
						<td  id="dtu_parity">serial parity</td>
						<td> 
					                <select name="dtu_parity_sel">
					                         <option id="option_none" value="n">none</option>
					                         <option id="option_odd" value="o">odd</option>
					                         <option id="option_even" value="e">even</option>
					                 </select>
						</td>
				  </tr>
				  <tr>
						<td  id="dtu_databits">serial databits</td>
						<td> 
					                <select name="dtu_databits_sel" >
					                         <option id="option_7" value="7">7</option>
					                         <option id="option_8" value="8">8</option>
					                 </select>
					                 bits
						</td>
						
				  </tr>
				  <tr>
						<td  id="dtu_stopbits">serial stopbits</td>
						<td> 
					                <select name="dtu_stopbits_sel">
					                         <option id="option_1" value="1">1</option>
					                         <option id="option_2" value="2">2</option>
					                 </select>
					                 bits
						</td>
					</tr>
  				<tr>
						<td  id="dtu_flowcontrol">serial flow control</td>
						<td> 
					                <select name="dtu_flowcontrol_sel" >
					                         <option id="option_n" value="n">none</option>
					                         <option id="option_h" value="h">hardware</option>
					                         <option id="option_s" value="s">software</option>
					                 </select>
						</td>
				  </tr>
  			</table>
  		</fieldset>
	</fieldset>
  


  <table class="hidden" border="0" cellpadding="2" cellspacing="1" width="500"   bordercolor="#9BABBD">

    	<tr>
    		<td class="title" colspan="2" id="dtu_serial_table">DTU Serial setting</td>
     	</tr>

   

   	


   	

   	


   	

  </table>





  <table class="hidden" border="0" cellpadding="2" cellspacing="1" width="500"   bordercolor="#9BABBD">

    	<tr>
    		<td class="title" colspan="2" id="dtu_config_table">DTU config</td>
     	</tr>

	    


    
	</table>

	
	<div id="server_block">
		<table border="0" cellpadding="2" cellspacing="1" width="500"  bordercolor="#9BABBD">
			<tr>
				<td width="40%" id="dtu_server_port">server port</td>
				<td> 
					<input name="dtu_serverport" maxlength=5 size=5 type="text"> 

				</td>
			</tr>
		</table>
	</div>


	<div id="client_block">
	<table class="hidden" border="0" cellpadding="2" cellspacing="1" width="500" bordercolor="#9BABBD" >
     	<tr>
		
	</tr>
     	<tr>
		
	</tr>
     	<tr>
		
	</tr>
	<tr>
		
	</tr>

	
	<tr > 
			<td width="40%" id="dtu_heart_info">heart beat information</td>
			<td >
			hex
			<input type="checkbox"  name="dtu_h_info_type">
			<!--<input name="dtu_h_info" maxlength=240  size=32 type="text">-->
			</td>
	</tr>

	<tr > 
			<td width="40%" id="dtu_off_heart_after_nodata_select">off heart beat when no serial data</td>
			<td >
				<input type="checkbox"  name="dtu_off_heart_after_nodata_recv">
			</td>
	</tr>
	


	
</table>
</div>

 <table class="hidden" border="0" cellpadding="2" cellspacing="1" width="500"   bordercolor="#9BABBD">
	
</table>


<p>
<table width="500" border="0" cellpadding="2" cellspacing="1">

  <tr align="center">
    <td>
         <input type="submit" value="apply" id="dtu_apply" name="dtu_apply" >
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
			<span id="dtu_direc1"></span><br/><br/>
			<span id="dtu_direc2"></span><br/><br/>
			<span id="dtu_direc3"></span>
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

