#ifndef _HTML_H
#define _HTML_H

const char config_html[] PROGMEM = R"=====(
<html>
<head>
<meta http-equiv="Content-Language" content="en-us">
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Keyno Guidance & Control</title>
<script>
var MyIP=location.host;
var _Connection;
var link="ws://"+MyIP+"/ws";

window.onload = function (){  setTimeout(ini_ws,1000); } 
function ini_ws(){
_Connection = new WebSocket( link); 
_Connection.onerror = function (error) {  
                    document.getElementById('_link').style.backgroundColor ="#FFA500";
                    alert('Conection Error '+'\n'+link);
                    }
_Connection.close = function (error) {  document.getElementById('_link').style.backgroundColor ="#FFE4E1";} //gray
_Connection.onopen = function (evt) {  
                  var newdate=new Date();
                  var C_date=pad2(newdate.getFullYear())+pad2(newdate.getDay()) +pad2(newdate.getDate()) ;
                  var C_sec=newdate.getSeconds()+ newdate.getMinutes()*60 + newdate.getHours()*3600; 
                  _Connection.send('SGT=' + C_date + C_sec); //15
                  document.getElementById('_link').style.backgroundColor ="#7FFF00"; // grenn
                  document.getElementById('current_data').value=newdate;
                  }
_Connection.onmessage = function(INCOME){parsing(INCOME.data); }     
}
function parsing(_income) {// alert(_income) 
 // document.getElementById('_content').value=_income;
 }
function pad2(number) { return (number < 10 ? '0' : '') + number }
function createXHR(){
try{return new XMLHttpRequest();}catch (e){try {return new ActiveXObject("Microsoft.XMLHTTP");} 
catch (e){return new ActiveXObject("Msxml2.XMLHTTP");}}
}
function show_list_file() {
var _AJX=createXHR();    
_AJX.onreadystatechange = function(){ 
if(_AJX.readyState == 4)
{ var div = document.getElementById('showfiles');
div.innerHTML = _AJX.responseText;      // id of list box =listfile 
var _listBox=document.getElementById('listfile');
_listBox.selectedIndex=_listBox.options.length-1;                        
}}
_AJX.open("GET","/process?code=DIR" , true);
_AJX.send(null); 

}
function show_file_content(){
var patch= document.getElementById('_path').value ;
var file= document.getElementById('_file').value;  
var _AJX=createXHR();    
_AJX.onreadystatechange = function(){   
if(_AJX.readyState == 4){ 
var recieved =_AJX.responseText;
document.getElementById('_content').value=recieved; }}
_AJX.open("GET","/process?code=SFC&patch=" + patch+ '&file='+file, true);
_AJX.send(null); }
function update_file_content(){
var patch= document.getElementById('_path').value ;
var file= document.getElementById('_file').value;
var content=(document.getElementById('_content').value); 
_Connection.send("RUF=" + patch+ file+ "&" + content);
document.getElementById('_content').value="";}
function removefile(){
var path=document.getElementById("_path").value;
var file=document.getElementById("_file").value;
var _AJX=createXHR();    
_AJX.onreadystatechange = function(){   
if(_AJX.readyState == 4){  
var recieved =_AJX.responseText; show_list_file();            
}}
_AJX.open("GET","/process?code=RRF&patch=" + path+ '&file='+file, true);
_AJX.send(null);}
function CreateFile(){
var path=document.getElementById("_path").value;
var file=document.getElementById("_file").value;
var _AJX=createXHR();    
_AJX.onreadystatechange = function(){   
if(_AJX.readyState == 4){  
var recieved =_AJX.responseText;  show_list_file();}}
_AJX.open("GET","/process?code=CRF&patch=" + path+ '&file='+file, true);
_AJX.send(null);}
function renamefile(){
var path=document.getElementById("_path").value;
var oldfile=document.getElementById("_file").value;
var newfile=document.getElementById("_newfile").value;
var _AJX=createXHR();    
_AJX.onreadystatechange = function(){   
if(_AJX.readyState == 4){  
var recieved =_AJX.responseText;  show_list_file();}}
_AJX.open("GET","/process?code=RNF&patch=" + path+ '&oldfile='+oldfile + '&newfile='+newfile, true);
_AJX.send(null); }
function parse_line(){
var e = document.getElementById("listfile");
var line = e.options[e.selectedIndex].text;
var myArray=line.split("/");
var num=myArray.length; 
if(num>2){ 
document.getElementById("_path").value='/'+myArray[1]; 
document.getElementById("_file").value='/'+myArray[2]; }
else {
document.getElementById("_path").value='/'; 
document.getElementById("_file").value=myArray[1];}
}
</script>
<script type="text/javascript">      
//window.onload = function (){setTimeout(Request_IP,1000);} 
</script>
</head>
<body>
<input type="button" id="Bu51on1" onclick="upload_file();return false;" name="" value="Upload file" style="position:absolute;left:809px;top:3px;width:73px;height:30px;z-index:0;">
<script>
function upload_file(){
var file=document.getElementById("FileUpload1");
var path=document.getElementById("_path").value + document.getElementById("path").value;
if(file.files.length === 0)  return;   
var formData = new FormData();

   formData.append("data", file.files[0], path); 
var _AJX = new XMLHttpRequest(); 
_AJX.onreadystatechange = function(){
if (_AJX.readyState == 4){ 
if(_AJX.status != 200)    alert("ERROR["+_AJX.status+"]: "+_AJX.responseText); 
else                     show_list_file(); 
Show_Image_List();}}; 
_AJX.open("POST", "/upload");
_AJX.send(formData); }
</script>
<input type="file" id="FileUpload1" style="position:absolute;left:714px;top:5px;width:73px;height:23px;line-height:23px;z-index:1;" name="FileUpload1" multiple ="false" onchange="check_validity();return false;"> 
<script>
function check_validity() {
 var file=document.getElementById("FileUpload1");
 var path=document.getElementById("path");
  if(file.files.length === 0)     return; 
  var filename = file.files[0].name; 
  var ext = /(?:\.([^.]+))?$/.exec(filename)[1]; 
  var name = /(.*)\.[^.]+$/.exec(filename)[1]; 
  if(typeof name !== undefined){ filename = name; } 
  if(typeof ext !== undefined){ 
       if(ext === "html") ext = "htm"; 
  else if(ext === "jpeg") ext = "jpg"; 
  filename = filename + "." + ext; }
  if(path.value === "/" || path.value.lastIndexOf("/") === 0)  { path.value = "/"+filename; } 
  else { path.value = path.value.substring(0, path.value.lastIndexOf("/")+1)+filename; }
 }  
</script>
<input type="text" id="path" style="position:absolute;left:552px;top:6px;width:145px;height:23px;line-height:23px;z-index:2;" name="Editbox1" value="">
<div id="showfiles" style="position:absolute;left:552px;top:152px;width:110px;height:71px;z-index:3">
</div>
<input type="button" id="Button3" onclick="show_list_file();return false;" name="" value="show file List" style="position:absolute;left:802px;top:118px;width:85px;height:25px;z-index:4;">
<textarea name="TextArea1" id="_content" style="position:absolute;left:4px;top:42px;width:532px;height:387px;z-index:5;" rows="23" cols="86"></textarea>
<input type="button" id="Button5" onclick="update_file_content();return false;" name="" value="Update File content" style="position:absolute;left:132px;top:6px;width:127px;height:25px;z-index:6;">
<input type="button" id="Button2" onclick="show_file_content();return false;" name="" value="Show file content" style="position:absolute;left:4px;top:6px;width:119px;height:25px;z-index:7;">
<input type="text" id="_file" style="position:absolute;left:435px;top:8px;width:92px;height:23px;line-height:23px;z-index:8;" name="Editbox1" value="" text-align="right">
<input type="text" id="_path" style="position:absolute;left:318px;top:8px;width:82px;height:23px;line-height:23px;z-index:9;" name="Editbox1" value="" text-align="right">
<div id="wb_Text3" style="position:absolute;left:268px;top:12px;width:50px;height:18px;z-index:10;text-align:left;">
<span style="color:#000000;font-family:Arial;font-size:16px;">patch</span></div>
<div id="wb_Text4" style="position:absolute;left:405px;top:12px;width:30px;height:18px;z-index:11;text-align:left;">
<span style="color:#000000;font-family:Arial;font-size:16px;">file</span></div>
<input type="button" id="Button1" onclick="removefile();return false;" name="" value="Remove file" style="position:absolute;left:552px;top:80px;width:80px;height:25px;z-index:12;">
<input type="button" id="Button7" onclick="updatePath();return false;" name="" value="Update Path" style="position:absolute;left:552px;top:118px;width:80px;height:25px;z-index:13;">
<input type="button" id="Button4" onclick="renamefile();return false;" name="" value="Rename file" style="position:absolute;left:679px;top:80px;width:84px;height:25px;z-index:14;">
<input type="button" id="Button6" onclick="CreateFile();return false;" name="" value="Create  file" style="position:absolute;left:802px;top:80px;width:80px;height:25px;z-index:15;">
<input type="submit" id="_link" onclick="ini_ws();return false;" name="" value="link" style="position:absolute;left:552px;top:42px;width:330px;height:24px;z-index:17;">
</body>
</html>
)=====";

#endif