var MyURL="";
var MyIP='192.168.0.0';
var _Connection;
var link;
var line1; 
var line2;
var lines=0;
var _millisPerPixel=500;
var _Index=[];
var _status=[]; 
var phone=[];
var date=[];
var TEXT=[];
var total; 

//////////////////////////////////
function createXHR() 
{ 
		try   {
			                 return new XMLHttpRequest();
			  } 
	catch (e) {
						try {
							 return new ActiveXObject("Microsoft.XMLHTTP");
							} 
				catch (e)  {
							 return new ActiveXObject("Msxml2.XMLHTTP");
						  }
              }
}

function Request_IP()
 { 
 var _AJX=createXHR();    
  _AJX.onreadystatechange = function(){   
										  if(_AJX.readyState == 4)
										  {
											MyIP =_AJX.responseText; 
											//alert(MyIP); 
                                              link="ws://"+	MyIP + ":81"; 
											// link="ws://192.168.1.24:81"; 
                                            _Connection = new WebSocket( link, ['arduino']); 
                                            //alert("status="+_Connection.readyState);
											_Connection.onerror = function (error) {	
																					document.getElementById('_link').style.backgroundColor ="#FFA500";
																					alert('Conection Error\n'+link, error);
																					}
											_Connection.close = function (error) {	document.getElementById('_link').style.backgroundColor ="#FFE4E1";} //gray
											_Connection.onopen = function () { 
																				var newdate=new Date();
																				var C_date=pad2(newdate.getFullYear())+pad2(newdate.getDay()) +pad2(newdate.getDate()) ;
	                                                                            var C_sec=newdate.getSeconds()+ newdate.getMinutes()*60 + newdate.getHours()*3600; 
																				_Connection.send('SGT=' + C_date + C_sec); //15
																				document.getElementById('_link').style.backgroundColor ="#7FFF00"; // grenn
																				document.getElementById('current_data').value=newdate;
																				}
											_Connection.onmessage = function(INCOME){parsing(INCOME.data); }
										  }
									   }
  var command  = "?code=MIP";   
     _AJX.open("GET", "/process" + command, true); //http://192.168.4.1/process?code=RGM
  //  _AJX.open("GET", "http://192.168.1.24/process" + command, true); //http://192.168.4.1/process?code=RGM
   _AJX.send(null); 
 }
 
  function request_status(){ var command  = "RST"; _Connection.send(command);}
  function parsing(_income)
 {
	
	/*alert(_income)*/
	var param_array = _income.split("$"); 
	 len=param_array.length;
	// alert(len)
		 if(param_array[0]=='RST')
								{    // alert( param_array ); Station,MyWell,192.168.1.24,192.168.4.1,keyno,Smart Well,KEASI Ver 0.7,Keyno,ggggg,09127163464,plantation,,2,System
									//document.getElementById("hostID"       ).value=param_array[1];
									document.getElementById("SHN"     ).value=param_array[2];
									document.getElementById("_ssid"      ).value=param_array[5];
									document.getElementById("APN"     ).value=param_array[6];
									document.getElementById("sotware"        ).value=param_array[7];
									document.getElementById("licence" ).value=param_array[8];
									document.getElementById("owner" ).value=param_array[9]; 
									document.getElementById("phone" ).value=param_array[10];
									document.getElementById("station_name" ).value=param_array[11];
									document.getElementById("lastcommand" ).value=param_array[12];
									document.getElementById("scenario" ).value=param_array[13];
									document.getElementById("account" ).value=param_array[14];
								}
	 else if(param_array[0]=='SLS')	// recived pin monitor result SLS$AOAWMEL%1,
								 {
									var ides = param_array[1].split(","); 
									var number_of_idval=ides.length;
									for(i =0; i < number_of_idval; i++) 
																{
																var id_val=ides[i].split("%");
																var id=id_val[0];
																var val=id_val[1];
																//  alert("ID="+id+"  value="+val)																
																document.getElementById(id).value=val; //(val=='1' )?"Turn On":"Turn Off";
																document.getElementById(id).style.backgroundColor =(val=='1' )?"#000cdf":"#FFFF00";																	
																}
								 }	 
  else if(param_array[0]=='HTM') document.getElementById('_CMS').innerHTML += param_array[1];
  
  else if(param_array[0]=='BST')	// recived pin monitor result BST$TS1=0
								 {
									
									var id_val = param_array[1].split("="); 
									var  id=id_val[0];
									var val=id_val[1];
									//alert( id + "-----" + val )
									document.getElementById(id).value=(val=='1' )?"Motion":"Idle";
									document.getElementById(id).style.backgroundColor =(val=='1' )?"#F0F0F0":"#FFFF00";	
									line1.append(new Date().getTime(),  parseInt(val) );
								 }	
 else if(param_array[0]=='SIM')		{
	 
	                                document.getElementById("sim_tb").value+=param_array[1]+"\n"; 
									if(lines>10){document.getElementById("sim_tb").value ="";lines=0;}
									lines++;
                                      }	
 else if(param_array[0]=='SMS')		{
	                                 var smss=param_array[1].split("+CMGL:");
									 total=(smss.length);
	                                 for(i=1;i<smss.length;i++)
										 {//1,"REC READ","+989127163464","","16/07/07,19:46:33+14"__@@admin.admin,GRP-val14____
											var p=smss[i].split(","); //
											 
											//if(p.length<7) alert(p)		//5,"REC READ","7575","","16/07									
												    _Index[i]=p[0];
												 _status[i]=p[1];
												  phone[i]=p[2];
												  sum=p[4]+" "+p[5]+p[6];
												  tx=sum.split("_");
												 // if(p.length<7) TEXT[i]="Binery";
												 // else      
													  TEXT[i]=tx[2]; 
												  date[i]=tx[0];/*			
											     alert("_Index[i]="+_Index[i]+
													 "\nstatus[i]="+_status[i]+
													 "\nphone[i]="+phone[i]+
													
													 "\ntext[i]="+TEXT[i]+
													 "\ndate[i]="+date[i])	*/									
										 }
										// alert(total) 
									    insert_in_table() ;
                                      }		
									  
 } 

 function Off_image(_id)  { document.getElementById(_id).style.filter = "grayscale(100%)"; }
 function  On_image(_id)  { document.getElementById(_id).style.filter = "grayscale(0%)"; }
 function all_off()	 {	 
						document.getElementById('ASS').style.filter = "grayscale(100%)";
						document.getElementById('SAT').style.filter = "grayscale(100%)";
						document.getElementById('IE7').style.filter = "grayscale(100%)";
					 }
 //////////////hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh////////////////////
 function pad2(number) { return (number < 10 ? '0' : '') + number }
function Insert_data()
{
	line1.append(new Date().getTime(), Math.random()*3 );

}

var seriesOptions = [
  { strokeStyle: 'rgba(255, 0, 0, 1)', fillStyle: 'rgba(255, 0, 0, 0)', lineWidth: 3 },
  { strokeStyle: 'rgba(0, 255, 0, 1)', fillStyle: 'rgba(0, 255, 0, 0)', lineWidth: 3 },
  { strokeStyle: 'rgba(10, 20, 100, 1)', fillStyle: 'rgba(10, 20, 100, 0)', lineWidth: 3 },
  { strokeStyle: 'rgba(100, 50, 0, 1)', fillStyle: 'rgba(100, 50, 0, 0)', lineWidth: 3 },
  { strokeStyle: 'rgba(0, 0, 255, 1)', fillStyle: 'rgba(0, 0, 255, 0)', lineWidth: 3 },
  { strokeStyle: 'rgba(255, 255, 0, 1)', fillStyle: 'rgba(255, 255, 0, 0)', lineWidth: 3 }
];

function lunch_chart()
{
	 
  var cpuDataSets = [new TimeSeries(), new TimeSeries(), new TimeSeries(), new TimeSeries(), new TimeSeries(), new TimeSeries()];
  
  var now = new Date().getTime();
  for (var t = now - 1000 * 50; t <= now; t += 1000) {
    addRandomValueToDataSets(t, cpuDataSets);
  }
  
   setInterval(function() {
    addRandomValueToDataSets(new Date().getTime(), cpuDataSets);
  }, 1000);

  var chart = new SmoothieChart( 
								  { 
								  maxValue:500,
								  minValue: 0,
								  grid: { 
										  strokeStyle: 'rgb(100, 100, 255)', 
										  fillStyle:'#0000ff',
										  lineWidth: 1, 
										  millisPerLine: 10000, 
										  verticalSections: 7 										  
										}
								  /*
								  millisPerPixel:_millisPerPixel ,
								  
								  
								  scrollBackwards:true,
								  timestampFormatter:SmoothieChart.timeFormatter, //SmoothieChart.timeFormatter
								  ,														
								  horizontalLines:[
												   {color:'#ffffff',lineWidth:1,value:1}																	  
												   ]	,
									labels:
											{
											disabled: false, // enables/disables labels showing the min/max values
										    fillStyle: '#ffffff', // colour for text of labels,
											fontSize: 10,
											fontFamily: 'sans-serif',
											precision: 0
											}
									*/
								   }													  
								);
 for (var i = 0; i < cpuDataSets.length; i++) 
 {
    chart.addTimeSeries(cpuDataSets[i], seriesOptions[i]);
  }
  chart.streamTo(document.getElementById("mycanvas"), 10);
  
}

function addRandomValueToDataSets(time, dataSets) {
	 var v=[];
  for (var i = 0; i < dataSets.length; i++)
  {
    v[i]=Math.abs(Math.random()*100+i*50+100);
	dataSets[i].append(time, v[i]);
  }
	document.getElementById("VA_0").value=v[0];
	document.getElementById("VA_1").value=v[1];
    document.getElementById("VA_2").value=v[2];
	document.getElementById("VA_3").value=v[3];
	document.getElementById("VA_4").value=v[4];
	//document.getElementById("VA_5").value=v[5];
	document.getElementById("efficency").value=(Math.random()*10+90)+"%";
	document.getElementById("powerfactor").value=(Math.random()*10+90)+"%";
	document.getElementById("power").value=(Math.random()*10+90)+"%";
}

function lunch_powerchart()
{
  //alert("gdfghdfgdf")
  line2 = new TimeSeries();
  var smoothie2 = new SmoothieChart( 
								  { 
								  interpolation: 'step',
								  millisPerPixel:500 ,
								  maxValue:2,
								  minValue: 0,
								  scrollBackwards:true,
								  timestampFormatter:SmoothieChart.timeFormatter, //SmoothieChart.timeFormatter
								  grid: { 
										  strokeStyle: 'rgb(0, 0, 255)', 
										  fillStyle:'#0000ff',
										  lineWidth: 1, 
										  millisPerLine: 10000, 
										  verticalSections: 2 
										  
										  },														
								  horizontalLines:[
												   {color:'#ffffff',lineWidth:1,value:1}																	  
												   ]	,

									labels:
											{
											  disabled: false, // enables/disables labels showing the min/max values
										   fillStyle: '#ffffff', // colour for text of labels,
											fontSize: 10,
											fontFamily: 'sans-serif',
											precision: 0
											}														
								   }													  
								   );
  smoothie2.addTimeSeries(line2, { 
								strokeStyle: 'rgb(0, 255, 0)',
								fillStyle: 'rgba(0, 255, 0, 0.4)',
								lineWidth: 3 
								});

  smoothie2.streamTo(document.getElementById("mycanvas2"), 10);
}
 ////////////////////////////////////////////////////////////////////////
 function set_relay(_id)
 {
	var v=document.getElementById(_id).value;
	var w=_id.substr(0,1);
	var com;
  if(v=="Close" || v=="Stop")	val=w+"1";
  else                          val=w+"0";
   var _AJX=createXHR();    
   _AJX.onreadystatechange = function(){   
										  if(_AJX.readyState == 4)
										  { 
											var r=_AJX.responseText;
											if(r=="O") 
											{
											       if(v=="Close") document.getElementById(_id).value="Open";
                                              else if(v=="Open") document.getElementById(_id).value="Close";
											  else if(v=="Stop") document.getElementById(_id).value="Run";
											  else               document.getElementById(_id).value="Stop";
											}
										  }
									   }
  var command  = "?code=SRS&val="+val;                                     
   _AJX.open("GET",  "/process" + command, true);
   _AJX.send(null); 
 }
 
 function Set_scenario()
 {
	var e = document.getElementById("set_sce");
var sen = e.options[e.selectedIndex].value;
	_Connection.send( "SCN="+sen); 
 }
function insert_in_table()
 {
	remove_table('SMS_table');
var table = document.getElementById('SMS_table');   
	for(i=1;i<total;i++)
	{
	//alert(_Index[i])		 
	var row = table.insertRow(i); 
	var cell4 = row.insertCell(0); cell4.style.textAlign = "center"; /* device type */
	var cell3 = row.insertCell(1); cell3.style.textAlign = "center"; /* Locatoins */
	var cell2 = row.insertCell(2); cell2.style.textAlign = "center"; /* device name  */
	var cell1 = row.insertCell(3); cell1.style.textAlign = "center";  /* id */
	var cell0 = row.insertCell(4); cell0.style.textAlign = "center"; /* select */
	
	 tb = document.createElement("div");   								  
	 tb.innerHTML = "<input type = 'radio' id='"+_Index[i]+"' name='my' onchange='select_id(this.id);return false;' >";    
	cell0.appendChild(tb); /* select */
	 cell1.innerHTML=phone[i];/* id */
	 cell2.innerHTML =date[i]; /* device name  */
	 cell3.innerHTML=TEXT[i]; /* Locatoins */
	 cell4.innerHTML=_status[i];	 
	} 
 }
function remove_sms()
{ 
try {// remove row
	var table = document.getElementById('SMS_table');
	var rowCount = table.rows.length;
	for(var i=1; i<rowCount; i++) {
									var row = table.rows[i];
									var R = row.cells[4].childNodes[0].firstChild;
									// alert(R.id  )
									if(null != R && true == R.checked) 
																		{       
																		//var id=row.cells[4].innerHTML;				
																		var command  = "RSM=" +R.id
																		//alert(command)																		
																		table.deleteRow(i);
																		rowCount--;
																		i--;
																		 _Connection.send(command);
																		}
								}
  }catch(e) { alert(e);	}
}
 
function Update_Wifi(_ID) {  var command  =_ID.id +'='+_ID.value ; //http://192.168.4.1/process?code=RSW&ID=Station&loc=A  
    _Connection.send(command);
}

 function SetNetwork(){
	var     ssid=document.getElementById('_ssid').value;
	var netpass=document.getElementById('_netpass').value;	
  var command  = "SNW="+ ssid + "&" +netpass ;   //http://192.168.4.1/process?code=RNI&ssid=ssid+pass&poz=poz
   _Connection.send(command);	
}

function Restart(){ var command  = "RRE"; _Connection.send(command);}
function RestartHTML()
{
var _AJX=createXHR();    
				   _AJX.onreadystatechange = function(){   
														  if(_AJX.readyState == 4)
														  {  
														  }
													   }
				  var command  = "?code=RST&"
				   _AJX.open("GET", "/process" + command, true);
				   _AJX.send(null); 	
	
}
function update_file_content()
{
	var patch= document.getElementById('_path').value ;
    var file=	document.getElementById('_file').value;
	var content=(document.getElementById('_content').value);
	 
  var command  = "RUF=" + patch+ file+ "&" + content;  
    _Connection.send(command);
	document.getElementById('_content').value="";
	
}

function updatePath()
{
	var path=document.getElementById("_path").value;	
  var command  = "RUP=" + path;  
   _Connection.send(command);
}

///-------------------------------------------------------------------------------------
function show_user_list()
{
		var _AJX=createXHR();    
   _AJX.onreadystatechange = function(){   
										  if(_AJX.readyState == 4)
										  {  
											var div = document.getElementById('userlist');
                                                div.innerHTML = _AJX.responseText;			// id of list box in code = 							
										  }
									   }
  var command  = "?code=USL";                                        //http://192.168.4.1/process?code=RUL
   _AJX.open("GET",  "/process" + command, true);
   _AJX.send(null);
	
}

function AddUser()
{
	var a= document.getElementById("_username").value;
	var b= document.getElementById("_password").value;
	var e = document.getElementById("_userType");
    var c = e.options[e.selectedIndex].text;
	//var line=a+','+b +',' +c;
	 var count=document.getElementById("ListUserListBox").length;
	 if(count<6){
					var _AJX=createXHR();    
				   _AJX.onreadystatechange = function(){   
														  if(_AJX.readyState == 4)
														  {  
															show_user_list();//var recieved =_AJX.responseText;  alert(recieved);							
														  }
													   }
				  var command  = "?code=AUS&user=" + a + '&pass='+b + '&scop='+c; //http://192.168.4.1/process?code=RAU&user=user&pass=pass&scop=scop
				   _AJX.open("GET", "/process" + command, true);
				   _AJX.send(null); 
				}
	else alert("Exceed max user numbers")
}
function RemoveUser()
{
	var e = document.getElementById("ListUserListBox");
	var count=document.getElementById("ListUserListBox").length;
	if(count>1){	
				var index=e.selectedIndex;
				var _AJX=createXHR();    
			   _AJX.onreadystatechange = function(){   
													  if(_AJX.readyState == 4)
													  {  
														show_user_list();//var recieved =_AJX.responseText;  alert(recieved);							
													  }
												   }
			  var command  = "?code=RUS&index=" + index; //http://192.168.4.1/process?code=RRU&index=index
			   _AJX.open("GET","/process" + command, true);
			   _AJX.send(null); 
			}
	else alert("Atleast need one user")
}
function ScanNetwork()
{
	
	 var _AJX=createXHR();    
   _AJX.onreadystatechange = function(){   
										  if(_AJX.readyState == 4)
										  {  
											var div = document.getElementById('netScanResult');
                                                div.innerHTML = _AJX.responseText;			 							
										  }
									   }
  var command  = "?code=NET";                                        //http://192.168.4.1/process?code=RSN
   _AJX.open("GET","/process" + command, true);
   _AJX.send(null);
	
}

function show_file_content()
{
	var patch= document.getElementById('_path').value ;
    var file=	document.getElementById('_file').value;
	 
	var _AJX=createXHR();    
   _AJX.onreadystatechange = function(){   
										  if(_AJX.readyState == 4)
										  { 
											var recieved =_AJX.responseText;  //alert(recieved);											
											document.getElementById('_content').value=recieved;											
										  }
									   }
  var command  = "?code=CON&patch=" + patch+ '&file='+file;    //http://192.168.4.1/process?code=RGP&patch=patch&file=file
   _AJX.open("GET","/process" + command, true);
   _AJX.send(null); 
	
}
function removefile()
{
	var path=document.getElementById("_path").value;
	var file=document.getElementById("_file").value;
	
	var _AJX=createXHR();    
   _AJX.onreadystatechange = function(){   
										  if(_AJX.readyState == 4)
										  {  
											var recieved =_AJX.responseText; show_list_file();  // alert(recieved);							
										  }
									   }
  var command  = "?code=RRF&patch=" + path+ '&file='+file; //http://192.168.4.1/process?code=RRF&patch=patch&file=file 
   _AJX.open("GET","/process" + command, true);
   _AJX.send(null); 
	
}

function CreateFile()
{
	var path=document.getElementById("_path").value;
	var file=document.getElementById("_file").value;
	
	var _AJX=createXHR();    
   _AJX.onreadystatechange = function(){   
										  if(_AJX.readyState == 4)
										  {  
											var recieved =_AJX.responseText;  show_list_file();// alert(recieved);							
										  }
									   }
  var command  = "?code=RCF&patch=" + path+ '&file='+file; //http://192.168.4.1/process?code=RRF&patch=patch&file=file 
   _AJX.open("GET","/process" + command, true);
   _AJX.send(null); 
	
}

function renamefile()
{
	var path=document.getElementById("_path").value;
	var oldfile=document.getElementById("_file").value;
	var newfile=document.getElementById("_newfile").value;
	
	var _AJX=createXHR();    
   _AJX.onreadystatechange = function(){   
										  if(_AJX.readyState == 4)
										  {  
											var recieved =_AJX.responseText;  show_list_file();// alert(recieved);							
										  }
									   }
  var command  = "?code=RRN&patch=" + path+ '&oldfile='+oldfile + '&newfile='+newfile; //http://192.168.4.1/process?code=RRN&patch=patch&oldfile=oldfile&newfile=newfile 
   _AJX.open("GET","/process" + command, true);
   _AJX.send(null); 
	
}

function show_list_file() 
{
	//alert("mmmmmm")
	var _AJX=createXHR();    
   _AJX.onreadystatechange = function(){   
										  if(_AJX.readyState == 4)
										  {  
											var div = document.getElementById('showfiles');
                                                div.innerHTML = _AJX.responseText;			// id of list box =listfile	
                                               var _listBox=document.getElementById('listfile');
                                                _listBox.selectedIndex=_listBox.options.length-1;											   
										  }
									   }
  var command  = "?code=DIR";                                        //http://192.168.4.1/process?code=RGH
   _AJX.open("GET","/process" + command, true);
   _AJX.send(null); 
	
	
}

//??????????????????????????????????????????????????????????????
function selectssid_pass()
{
    var e = document.getElementById("_netList");
	if(e.selectedIndex>0)
	{	
	var strUser = e.options[e.selectedIndex].text;
	var S=strUser.split(",")
	var ssid=S[0].trim()
	document.getElementById('_ssid2').value=ssid;
	}
}

 function CheckIP(_Obj)
  {
    ValidIP = false; 
	var IPText=_Obj.value;
	
    ipParts = IPText.split(".");
    if(ipParts.length==4){
						  for(i=0;i<4;i++){							 
											TheNum = parseInt(ipParts[i]);
											if(TheNum >= 0 && TheNum <= 255){}
											else{break;}											 
										  }
						  if(i==4)ValidIP=true; 
						}
	if(!ValidIP)	alert('Wrong Format')				
  }

function parse_line()
{
	var e = document.getElementById("listfile");
    var line = e.options[e.selectedIndex].text;
	var myArray=line.split("/");
    var num=myArray.length; 
if(num>2){ 
         document.getElementById("_path").value='/'+myArray[1]; 
		 document.getElementById("_file").value='/'+myArray[2]; 
		 }
else     {
		 document.getElementById("_path").value='/'; 
		 document.getElementById("_file").value=myArray[1];
         }
}

function parse_line_and_put_inform()
{
	var e = document.getElementById("ListUserListBox");
    var line = e.options[e.selectedIndex].text;
	var myArray=line.split(",");
    var num=myArray.length; 
     // alert (myArray + '\n'+num)options[i].selected = true   selectedIndex                  
	 document.getElementById("_username").value=myArray[0]; 
	 document.getElementById("_password").value=myArray[1]; 		

}

function showselecteditem()
{
var e = document.getElementById("Comboboxtest");
var strUser = e.options[e.selectedIndex].text;
var num=strUser.split("/").length;
if(num>2){ patch=strUser.split("/")[1]; file=strUser.split("/")[2];
             alert (patch+'      '+file  )}
else  {file=strUser.split("/")[1]; alert ('only file '+file  )}
}

 

function all_off()
{

document.getElementById('_link').style.filter = "grayscale(100%)";
document.getElementById('SAT').style.filter = "grayscale(100%)";
document.getElementById('IE7').style.filter = "grayscale(100%)";
}
function movebar(id,n)
{
var progress='p'+id;
var tbox='T'+id;
var h=parseInt(document.getElementById(progress).style.height); // alert(h);       
if(n && h>5){     h=h-5;   document.getElementById(progress).style.height=h + '%'; document.getElementById(tbox).value=45-h/5;   }
else if(h<95) {     h=h+5;  document.getElementById(progress).style.height=h + '%'; document.getElementById(tbox).value=-4+h/5;}	

}

function animatebar(_id,num){
 
var progress='pbar'+_id;
var tbox='Tbar'+_id;
var current_h=parseInt(document.getElementById(progress).style.height);	//58
var target_h=(4*(50-num)/5)+20;
var current_value=parseInt(document.getElementById(tbox).value);
var stop=setInterval(frame, 10);
function frame() { 
				if (current_h< target_h || current_h >target_h) clearInterval(stop); 
				else {
					  current_h=current_h-2;
					   document.getElementById(progress).style.height=current_h + 'px';
					   document.getElementById(tbox).value=45-current_h/5;
					}
			  }
}
function motor(Status)
{
if(Status=="on") document.getElementById('engine_run').style.display = 'block';
else             document.getElementById('engine_run').style.display ='none';
} 

function Wave(_image,_canvas,_amp,_speed)
{

var h = new Image;      		
h.onload = function(){
						var flag = document.getElementById(_canvas);
						var amp =_amp;
						flag.width  = h.width;
						flag.height = h.height + amp*2;
						flag.getContext('2d').drawImage(h,0,amp,h.width,h.height);
						flag.style.marginLeft = -(flag.width/2)+'px';
						flag.style.marginTop  = -(flag.height/2)+'px';
						var timer = waveFlag( flag, h.width/10, amp ,_speed);
					};
h.src =_image ;
function waveFlag( canvas, wavelength, amplitude, period, shading, squeeze )
{
			if (!squeeze)    squeeze    = 0;
			if (!shading)    shading    = 100;
			if (!period)     period     = 400;
			if (!amplitude)  amplitude  = 10;
			if (!wavelength) wavelength = canvas.width/10;

			var fps = 30;
			var ctx = canvas.getContext('2d');
			var   w = canvas.width, h = canvas.height;
			var  od = ctx.getImageData(0,0,w,h).data;
			// var ct = 0, st=new Date;
			return setInterval(function(){
											var id = ctx.getImageData(0,0,w,h);
											var  d = id.data;
											var now = (new Date)/period;
											for (var y=0;y<h;++y){
												var lastO=0,shade=0;
												var sq = (y-h/2)*squeeze;
												for (var x=0;x<w;++x){
													var px  = (y*w + x)*4;
													var pct = x/w;
													var o   = Math.sin(x/wavelength-now)*amplitude*pct;
													var y2  = y + (o+sq*pct)<<0;
													var opx = (y2*w + x)*4;
													shade = (o-lastO)*shading;
													d[px  ] = od[opx  ]+shade;
													d[px+1] = od[opx+1]+shade;
													d[px+2] = od[opx+2]+shade;
													d[px+3] = od[opx+3];
													lastO = o;
												}
											}
											ctx.putImageData(id,0,0);		
											// if ((++ct)%100 == 0) console.log( 1000 * ct / (new Date - st));
										},1000/fps);
}	
} 

function remove_table(_ID)
{ 
try {
	var table = document.getElementById(_ID);
	var rowCount = table.rows.length;
	for(var i=1; i<rowCount; i++) {            										
									table.deleteRow(i);
									rowCount--;
									i--;
									}
	
   }catch(e) { alert(e);	}
}









