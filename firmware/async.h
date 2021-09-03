#ifndef _ASYNC_H
#define _ASYNC_H
 
bool FilterHost (AsyncWebServerRequest *request){ if(request->url()==MyIP) return true; return false;} 
bool FilterStandalone (AsyncWebServerRequest *request){ if(STANDALONE) return true; return false;} 
bool FilterMaster (AsyncWebServerRequest *request)    { if(!STANDALONE) return true; return false;} 
bool check_Auth(AsyncWebServerRequest *request){ //if(!check_Auth(request)) request->send(404);  return request->requestAuthentication(); 
 // PRINTLN(F("check_Auth"));
  if(ACCESS || ADMIN) return true;
 for(byte i=0;i<5;i++) if(request->authenticate(USERS[i].c_str(), PASS[i].c_str())){ 
																			LoginUser=USERS[i];
																			//TelegramPass=PASS[i]; 
																			PRINTLN(F("find user"));
																			ACCESS=true; 
																			if(IsAdmin[i])ADMIN=true; 
																			return true;
																			}
 
  return false; 
}
void onBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
  //Handle body
}
void onUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
  //Handle upload
}
void onEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
  //Handle WebSocket event
}
String getContentType(String filename, AsyncWebServerRequest *request) {
       if (request->hasArg("download")) return "application/octet-stream";
     
  else if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".json")) return "application/json";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}
   //handleFileRead("/Sindex.html", request)
bool handleFileRead(String path, AsyncWebServerRequest *request) {
 PRINT(F("handleFileRead: ")); PRINTLN(path);
  if (path.endsWith("/"))   path += "index.htm";
  String contentType = getContentType(path, request);  //return "text/html";
  String pathWithGz = path + ".gz";   //pathWithGz=Sindex.html.gz
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path))
  {
  //  if (SPIFFS.exists(pathWithGz))    path += ".gz";  //"application/javascript"
    AsyncWebServerResponse *response = request->beginResponse(SPIFFS, path, contentType);
   // if (path.endsWith(".gz"))      response->addHeader("Content-Encoding", "gzip");
    request->send(response);
    return true;
  }
  PRINTF("Cannot find %s\n", path.c_str()); 
  return false;
} 
 void ScanNetwork(AsyncWebServerRequest *request){  //this id of listbox defined here
  String records="<select name=\"Combobox2\" size=\"10\" id=\"_netList\"  onChange=\"selectssid_pass();\"   >";
    records+="<option value=\"100\"> Network Name  ,    Network Strength     </option>";
   int n = WiFi.scanComplete();
  if (n == WIFI_SCAN_FAILED)   WiFi.scanNetworks(true);
  else if (n > 0) 
     {
      for (int i = 0; i < n; ++i)records+="<option value=\"" + String(n)+"\">" +WiFi.SSID(i) +" , " + WiFi.RSSI(i)+"</option>";      
    }
  records+="</select>";
  WiFi.scanDelete();
    request->send(200, "text/html", records); 
} 

void prinScanResult(int networksFound){
  PRINTF("%d network(s) found\n", networksFound);
  for (int i = 0; i < networksFound; i++)
  {
    PRINTF("%d: %s, Ch:%d (%ddBm) %s\n", i + 1, WiFi.SSID(i).c_str(), WiFi.channel(i), WiFi.RSSI(i), WiFi.encryptionType(i) == ENC_TYPE_NONE ? "open" : "");
  }
}

File fsUploadFile;
void handleFileUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
  if (!index) 
  { 
    PRINTF("handleFileUpload Name: %s\n", filename.c_str());
    if (!filename.startsWith("/")) filename = "/" + filename;
    fsUploadFile = SPIFFS.open(filename, "w");
  }
  if (fsUploadFile)
  {
    if (fsUploadFile.write(data, len) != len) PRINTLN(F("Write error during upload"));
  }
  
  if (final) 
  { 
    if (fsUploadFile)   fsUploadFile.close();     
    PRINTF("handleFileUpload Size: %u\n", len);
  }
}
// in setup() WiFi.scanNetworksAsync(prinScanResult);
void show_user_list(AsyncWebServerRequest *request,String path,String _ID,String parseFunction){ 
                     //"/data/user.dat","ListUserListBox","parse_line_and_put_inform"
  String records="<select name=\"Combobox2\" size=\"5\" id=\""+_ID+"\"  onChange=\""+parseFunction+"();\"   >";
  File f = SPIFFS.open(path, "a+");   ///a+ if not exist then create it 
  if (!f) return request->send(200, "text/plain", records); 
  byte line_num_count=0;
  String line="";
   while(f.available())
   { 
 char c=f.read();              
  if(c=='\n' || c=='\r' )          
                      {  if(line.length() >2)
                                            {
                                              line_num_count++; 
                                               records+="<option value=\"" + String(line_num_count)+"\">" +line +"</option>";
                                              line="";  
                                            }        
                      }               
  else              line+=c;  
 } 
   f.close(); 
  
 records+="</select>";
  request->send(200, "text/plain", records); 
}
void AddUser(AsyncWebServerRequest *request,String user,String pass,String scop){
  request->send(404);
  String line=user+ "," + pass + "," + scop; 
   File f = SPIFFS.open("/data/user.dat", "a+");  
  if (!f) return ; 
  f.println(line);
  f.close();
}
void RemoveUser(AsyncWebServerRequest *request,byte num){
  request->send(404);
  String temp="";
  File f = SPIFFS.open("/data/user.dat", "r");
  byte line_num_count=0;
    String line="";
  while(f.available())
  {//Lets read line by line from the file      
  char c=f.read();              
  if(c=='\n' || c=='\r' )          
                      {if(line.length() >2)
                                            {
                                             if(num!=line_num_count)temp=temp+line+"\n\r";              
                                              line_num_count++;
                                             line="";     
                                            }  
                       }
 else              line+=c;                         
} 
  
  f.close();
  f = SPIFFS.open("/data/user.dat", "w+");
  f.println(temp);
   f.close();
}
void Createfile(String path, String filename,AsyncWebServerRequest *request){
  if(SPIFFS.exists(path+filename))  return request->send(200, "text/plain", "FILE EXISTS"); 
  File file = SPIFFS.open(path+filename, "w");
    if(file) file.close();   
  else                         return request->send(200, "text/plain", "CREATE FAILED");
}
void removefile(String path, String filename,AsyncWebServerRequest *request){
  if(!SPIFFS.exists(path+filename))    request->send(404, "text/plain", "FileNotFound");  
      SPIFFS.remove(path+filename);    request->send(200, "text/plain", "removed");
}
void RenameFile(String patch,String oldfilename,String newfilename,AsyncWebServerRequest *request){
 String fold=patch+oldfilename;
 String fnew=patch+newfilename;
if (SPIFFS.exists(fold))
          {
            SPIFFS.rename(fold,fnew);
            request->send(200, "text/plain", "Renamed");
          }
  
}
void show_listfile(AsyncWebServerRequest *request){ ///replace all item of list box
   String records="<select name=\"Combobox1\" width=\"400\" size=\"20\" id=\"listfile\"  onChange=\"parse_line();\"   >";
   byte i=0;
   Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {    
              String fileName = dir.fileName();
              size_t fileSize = dir.fileSize();
              records+="<option value=\"" + String(i)+"\">" +fileName +"</option>";
              i++;
            }
  records+="</select>";
 request->send(200, "text/plain", records);   
}
void show_file_content(String patch, String filename,AsyncWebServerRequest *request){ 
request->send(SPIFFS,patch+filename, String());
}
/*void check_client_update(void){
 	//AsyncClient  x;
	 client=  new AsyncClient();
	  client->onError(onError);
  client->onAck(onAck);
  client->onDisconnect(onDisconnect);
  client->onTimeout(onTimeout);
  client->onData(onData);
  client->onPoll(onPoll);

client->connect("www.google.com",80);
}*/
void data(AsyncWebServerRequest *request){ // from server spect of view ******************************************************************************************
  /*
    if(request->arg(0)=="SV" && Device_info[SensorMAC]==request->arg(3))
    {
       
       remoteip= request->client()->remoteIP().toString();//SV<---26.00$43.00
      PRINT("Got data from sensor @"+remoteip);   // sv temp  name mac
      Temp=request->arg(1).toInt();          
      ws.text(wsNumber,"SCS$1"); delay(1);// green the conection
       delay(1);
      WIFI_SENSOR=true;      
      if(EEPROM.read(Automatic_p)=='S'){ AUTOMATIC=true; delay(10); 
	  
	  ws.text(wsNumber,"AUT$1");}  
 
      delay(10);
     ws.text(wsNumber,"SWN$"+String(Temp)+"$"+remoteip+"$"+request->arg(2));
     
      last_reading=millis();
    }
  */  
}
void handleConfig(AsyncWebServerRequest *request){
 if(!request->authenticate(preSetUser.c_str(), preSetPass.c_str())) return request->requestAuthentication(); 
 ACCESS=ADMIN=true;
 request->send_P(200, "text/html", config_html);
}
 
/*
void Register_telegram(String user, String pass,AsyncWebServerRequest *request){
  
 if(TelegramRegistered) request->send(200, "text/plain", "Reg"); 
 else                 {  request->send(200, "text/plain", "Not"); 
    Device_info[TelegramID]=user;
  //  Device_info[telegrampass]=pass;
   DoRegisterTel=true;
   }
}

int check_bot_command(){
 
http.begin(UpdateBOT);
http.setTimeout(8000);
http.setReuse(true);
 
http.addHeader("ESP-STATUS",compose_status());
http.addHeader(F("Serial"),SerialPart);
//const char * headerkeys[] = {"BOT_COMMAND","x-MD5"};
 //size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
//http.collectHeaders(headerkeys,headerkeyssize);
int httpCode = http.GET();
yield();
if(httpCode > 0)
{
 	if(httpCode == HTTP_CODE_OK)
 	{
		 BOT_COMMAND=http.getString();	
    
     NewTelegramCommand=true;  
     
		//											  int len =http.getSize();  
	//  
		// if(BOT_COMMAND=="ESPjob") {  code=2;}
		//else                  { NewTelegramCommand=true;                    code=3; } 
    
 	}
	// else {code=1;	 }
}	
 

http.end(); 
 
//WS.text(MainWSclinet,"HTP$"+http.errorToString(httpCode));//typecontrol 
return httpCode;
}

String HttpResult;

int RegisterBotID(void){

http.begin(RegiterBOTLink);
http.setTimeout(2000);
             
http.addHeader(F("Serial"),SerialPart);
http.addHeader("Bot-ID",Device_info[TelegramID]);
//http.addHeader("Bot-Pass",Device_info[telegrampass]);
delay(10);
int httpCode = http.POST("Dummy");
if(httpCode > 0)
{
	if(httpCode == HTTP_CODE_OK)
	{
	  HttpResult=http.getString(); 	 
	  RegisterationDone=true; 
	}
	 
}	
 
http.end();  
 
//WS.text(MainWSclinet,"REG$"+http.errorToString(httpCode));//typecontrol 
return httpCode;
}
*/
 #ifdef EnableUpdate
 bool RunUpdate(Stream& in, uint32_t size, String md5, int command){

    StreamString error;
int _lastError;
  HTTPUpdateResult ret = HTTP_UPDATE_FAILED;

    if(!Update.begin(size, command))
  {
        _lastError = Update.getError();
        Update.printError(error);
        error.trim(); // remove line ending
        PRINT(F("[httpUpdate] Update.begin failed!")); PRINTLN(error);
        return false;
    }

    if(md5.length())
  {
        if(!Update.setMD5(md5.c_str())) 
    {
            _lastError = HTTP_UE_SERVER_FAULTY_MD5;
            PRINT(F("[httpUpdate] Update.setMD5 failed!")); PRINTLN(md5);
            return false;
        }
    }

    if(Update.writeStream(in) != size) 
  {///------------------------------------------------------------>>>>>>
        _lastError = Update.getError();
        Update.printError(error);
        error.trim(); // remove line ending       
	   PRINT(F("[httpUpdate] Update.writeStream failed!")); PRINTLN(error);
        return false;
    }

    if(!Update.end()) 
  {
        _lastError = Update.getError();
        Update.printError(error);
        error.trim(); // remove line ending
		PRINT(F("[httpUpdate] Update.end failed!")); PRINTLN(error);        
        return false;
    }

    return true;
}
 #endif 

void handleProcess(AsyncWebServerRequest *request){
 PRINTLN("[HTML]"+request->arg(0));
  if(!ACCESS) return request->send(404); 
       if(request->arg(0)=="CRF") Createfile(request->arg(1),request->arg(2),request) ; //http://192.168.4.1/process?code=RCF&patch=patch&file=file   [-CONFIG-]
  else if(request->arg(0)=="RRF") removefile(request->arg(1),request->arg(2),request) ; //http://192.168.4.1/process?code=RRF&patch=patch&file=file [-CONFIG-]
  else if(request->arg(0)=="RNF") RenameFile(request->arg(1),request->arg(2),request->arg(3),request);//http://192.168.4.1/process?code=RRN&patch=patch& [-CONFIG-]
  else if(request->arg(0)=="DIR") show_listfile(request) ;//http://192.168.4.1/process?code=RDH  CONFIG
  else if(request->arg(0)=="SFC") show_file_content(request->arg(1),request->arg(2),request) ;

  else if(request->arg(0)=="NET") ScanNetwork(request); //[-Standalone-]
  else if(request->arg(0)=="RST") {request->send(200, "text/plain", ""); delay(500); ESP.restart();} //[-Standalone-]
  else if(request->arg(0)=="USL") show_user_list(request,"/data/user.dat","ListUserListBox","parse_line_and_put_inform");//http://192.168.4.1/process?code=RUL [-Standalone-]
  else if(request->arg(0)=="AUS") AddUser(request,request->arg(1),request->arg(2),request->arg(3));//http://192.168.4.1/process?code=RAU&user=user&pass=pass&scop=scop [-Standalone-]
  else if(request->arg(0)=="RUS") RemoveUser(request,request->arg(1)[0]-'0');//http://192.168.4.1/process?code=RRU&index=index  [-Standalone-]

  }

  
  
#endif
