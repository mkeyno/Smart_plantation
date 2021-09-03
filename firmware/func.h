#ifndef _FUNC_H
#define _FUNC_H
/*
 //typedef std::function<void(void*, AsyncClient*) > AcConnectHandler;
void             onPoll(void *obj, AsyncClient* c){  PRINTF("onPoll:%lu\n", millis());}//AcConnectHandler
void            onError(void *obj, AsyncClient* c, int8_t error) {  PRINTF("onError:%d\n", error);}
void              onAck(void *obj, AsyncClient* c, size_t len, uint32_t time){  PRINTF("onAck:%u:%u\n", len, time);}
void       onDisconnect(void *obj, AsyncClient* c) {  PRINTF("onDisconnect\n");  c->free();  delete c;  client = NULL;}//AcConnectHandler
void          onTimeout(void *obj, AsyncClient* c, uint32_t time) {  PRINTF("onTimeout:%u\n", time);  client->close();}
void             onData(void *obj, AsyncClient* c, void *buf, size_t len){
 PRINTF("onData:%u\n", len);
  size_t i;
  for(i=0; i<len; i++){
    while(((U0S >> USTXC) & 0x7F) != 0);
      U0F = ((uint8_t*)buf)[i];
  }
}
void onClient(void *obj, AsyncClient* c){//AcConnectHandler
  if(client){    PRINTF("r\n");    if(client->connected())     client->close();  }
  PRINTF("c\n");
  client = c;
  client->onError(onError);
  client->onAck(onAck);
  client->onDisconnect(onDisconnect);
  client->onTimeout(onTimeout);
  client->onData(onData);
  client->onPoll(onPoll);
}
*/
String formatBytes(size_t bytes){
       if (bytes < 1024)                 return String(bytes)+"B"; 
  else if(bytes < (1024 * 1024))         return String(bytes/1024.0)+"KB";
  else if(bytes < (1024 * 1024 * 1024))  return String(bytes/1024.0/1024.0)+"MB";
  else                                   return String(bytes/1024.0/1024.0/1024.0)+"GB";
}
const char *FlashMode[] = {"QIO", "QOUT", "DIO", "DOUT"};
void print_chip_info(void){ 
PRINT(F("Slected FlashChipSize: "));PRINTLN(formatBytes(ESP.getFlashChipSize()));
PRINT(F("FlashChipRealSize: "));PRINTLN(formatBytes(ESP.getFlashChipRealSize()));
PRINT(F("SketchSize:"));PRINTLN(formatBytes(ESP.getSketchSize()));
PRINT(F("FreeSketchSpace:"));PRINTLN(formatBytes(ESP.getFreeSketchSpace()));
PRINT(F("FreeHeap:"));PRINTLN(formatBytes(ESP.getFreeHeap()));

PRINT(F("\nChipId: "));PRINTLN(ESP.getChipId());
PRINT(F("CpuFreqMHz: "));PRINTLN(ESP.getCpuFreqMHz());  
PRINT(F("SdkVersion: "));PRINTLN(ESP.getSdkVersion());
PRINT(F("FlashChipSpeed: "));PRINTLN(ESP.getFlashChipSpeed());
PRINT(F("FlashChipMode: "));PRINTLN(FlashMode[ESP.getFlashChipMode()]);	
 
}
void print_device_info(void){
   PRINTLN("\n\nDevice ID=" + preSetPass +"(" + preSetUser + ")");/*
  PRINT(F("Mode="));      PRINTLN(Device_info[MyMode]);
  PRINT(F("Hostname="));      PRINTLN(Device_info[myHostname]);
  PRINT(F("SSID="));           PRINTLN(Device_info[_SSID]);
  PRINT(F("Password ="));      PRINTLN(Device_info[SSIDPASS]);
  PRINT(F("AP SSID="));        PRINTLN(Device_info[AP_SSID]);
  PRINT(F("AP Password ="));   PRINTLN(Device_info[AP_SSIDPASS]);
  PRINT(F("reset number ="));  PRINTLN(reset_number);
  PRINT(F("Server IP Address ="));  PRINTLN(Device_info[AP_Server_IP]);
 // PRINT(F("remote Actuator  IP ="));  PRINTLN(Device_info[Remote_Sensor_IP]);
  

  PRINT(F("static IP ="));  PRINTLN(Device_info[Static_IP]);  
 // PRINT(F("Sensor Des ="));  PRINTLN(Device_info[Sensor_DES]);
 // PRINT(F("Sensor ID ="));  PRINTLN(Device_info[Remote_Sensor_IP]);
  PRINT(F("Ego Seconds ="));  PRINTLN(Ego_Seconds);
 
  
  PRINT(F("Store sensor data ="));  PRINTLN(STORE);
 PRINT(F("Sending Mode ="));  PRINTLN(SendingMode);
 PRINT(F("static mode ="));  PRINTLN(STATIC);
 PRINT(F("Mode="));          PRINTLN((Device_info[MyMode][0]=='A')?"Access Point":"Station");
 PRINT(F("Network Mode="));  PRINTLN((STANDALONE)?"Slave Mode":"Server Mode");
 PRINT(F("System Mode="));  PRINTLN((COOLING)?"Cooling":"Heating");
 PRINT(F("Control Mode="));  PRINTLN((AUTOMATIC)?"Automatic":"Manual");
 PRINT(F("last store Mac="));  PRINTLN(Device_info[SensorMAC]);
*/
}

#define TIME_ZONE    +3
extern "C" void     system_set_os_print(uint8 onoff);

 bool initTime(){
  time_t rawtime;
  configTime(TIME_ZONE * 3600, 0, "pool.ntp.org", "time.nist.gov");
  uint8_t i = 0;
  system_set_os_print(0);
  while(time(&rawtime) == 0 && i++ < 100) delay(10);
  system_set_os_print(1);
  if(i==100)  return false;
  return true;
}
String getIP(){
  IPAddress ip = (WiFi.getMode() & WIFI_STA) ? WiFi.localIP() : WiFi.softAPIP();
  return ip.toString();
}
 
IPAddress str2ip(String sip){
   IPAddress result;
   String s="";
   byte j=0;
   for (byte i = 0; i < sip.length() ; i++)
   {
    if(sip[i]!='.') s+=sip[i];
    else     {result[j]=s.toInt(); j++; s="";}
   } 
  result[3]=s.toInt(); 
return result;  
}
void UpdateContent(String path, String income){
          File f = SPIFFS.open(path, "w");   
  if (!f) {
	
    PRINTLN("[F-w-"+path+"]");// now write two lines in key/value style with  end-of-line characters  
    return;
      }   
  else {   f.println(income); f.close();  }        
}
void restor_user_pass(void){
  File f = SPIFFS.open(UserData, "r");
 if(!f) 
	   {
			 f = SPIFFS.open(UserData, "w+"); f.close();
			  PRINTLN(F("\t\t[Open User Pass Failed]"));
			  return;
	  } 
    int numLine=1;
 String line="";
   while(f.available())
   { 
 char c=f.read();              
  if(c=='\n' || c=='\r' )          
                      {  if(line.length() >4)
                                            {
                                              byte commaIndex = line.indexOf(',');
                                              byte secondCommaIndex = line.indexOf(',', commaIndex+1);
                                              USERS[numLine]= line.substring(0, commaIndex); USERS[numLine].trim();
                                              PASS[numLine] = line.substring(  commaIndex+1, secondCommaIndex); PASS[numLine].trim();
                                              String S=line.substring(secondCommaIndex+1);
                                              if(S[0]=='A') IsAdmin[numLine]=1; else IsAdmin[numLine]=0; 
                                               PRINTLN(USERS[numLine] +":"+PASS[numLine]+":"+String(IsAdmin[numLine]));              
                                              numLine++;  
                                            }        
                      }               
  else              line+=c;  
 } 
   f.close(); 
}

void DIR(void){
  uint32_t total=0;
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {    
              String fileName = dir.fileName();
              size_t fileSize = dir.fileSize();
                    total+=fileSize;
              PRINTF("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
            }
    PRINT("\t\t\tTotal:");PRINTLN(total);
  
}
void Append_Data(String data){ // general file   store data in file
 
 File f = SPIFFS.open(STOR_DATA, "a+");
 if (!f) {   
   PRINTLN("[F-a+/store/data.bin]");//
 return ;}
 f.println(data);
  f.close();
}

IPAddress apIP(192, 168, 4, 1);

void wifi_startup(void){
	
  if(WifiMode=='A' || reset_number>4 || Device_info[ST_ssid].length()<3 ){
                               PRINTLN(F("\t\t\t Entering the Access Mode"));                               
                               STANDALONE=true; 
							   
                               EEPROM.write(STANDALONE_p,'E');EEPROM.commit();                             
                               WiFi.mode(WIFI_AP);
                               WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
                               String temp_name="SmartTouch_" +String(ESP.getChipId(), HEX);                               
                               if(Device_info[AP_ssid].length()<3) {WiFi.softAP(temp_name.c_str(),"12345678"); Device_info[AP_ssid]=temp_name; }
                               else                                 WiFi.softAP(Device_info[AP_ssid].c_str(),Device_info[AP_pass].c_str());                                
                               delay(500); // Without delay I've seen the IP address blank
                               EEPROM.write(0, 0);EEPROM.commit();  
                               MyIP=WiFi.softAPIP().toString();
                               MyMAC=WiFi.softAPmacAddress();
                               PRINT("AP IP address: ");PRINTLN(MyIP);
                               PRINT("My MAC: ");PRINTLN(MyMAC);
                              // dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
                              // dnsServer.start(DNS_PORT, "*", apIP); 
                             // digitalWrite(Wifi_Mode,HIGH);
                            }
else if(reset_number<5){
						PRINTLN(F("\t\t\t Entering the Station Mode"));
						WiFi.mode(WIFI_STA);
						if(STATIC)   WiFi.config(str2ip(Device_info[Static_IP]),str2ip(Device_info[AP_Server_IP]), IPAddress(255,255,255,0));   
						WiFi.begin(Device_info[ST_ssid].c_str(), Device_info[ST_pass].c_str());             
						unsigned long startTime = millis();
						while (WiFi.status() != WL_CONNECTED && millis() - startTime < 7000) { byte j=0;
																								Serial.write('%');delay(500);
																								} 
						if(WiFi.status() != WL_CONNECTED) {
															  reset_number++;                                               
														   EEPROM.write(0, reset_number);EEPROM.commit();                                               
														  ESP.restart();
														  }            
						 MyIP=WiFi.localIP().toString();
						 MyMAC=WiFi.macAddress();
						 PRINT("AP IP address: ");PRINTLN(MyIP);
						 PRINT("My MAC: ");PRINTLN(MyMAC);
						if (!MDNS.begin(Device_info[Hostname].c_str())) {
															PRINTLN(F("Error setting up MDNS responder!"));
															while(1) {  delay(1000); }
														  }
						PRINTLN(F("mDNS responder started"));
            #ifdef EnableMQTT
						  if(_MQTT) Mqtt_setting();
            #endif
					  }	
}
void ConfigureOTA(void) {
  // ArduinoOTA.setPort(8266);
  ArduinoOTA.setHostname(("OTA_"+preSetUser).c_str());
  ArduinoOTA.setPassword(preSetPass.c_str());  
  ArduinoOTA.onStart([]() {SPIFFS.end(); /* Disable client connections */WS.enable(false);  WS.textAll("OTA Update Started");WS.closeAll(); PRINTLN(F("StartOTA \n"));  });
  ArduinoOTA.onEnd([]()   {SPIFFS.end(); PRINTLN("\nEnd OTA\n");  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) { PRINTF("OTA Progress: %u%%\n", (progress / (total / 100))); });
  ArduinoOTA.onError([](ota_error_t error) {
                                            PRINTF("Error[%u]: ", error);
                                                 if (error == OTA_AUTH_ERROR)    PRINTLN(F ("Auth Failed"));
                                            else if (error == OTA_BEGIN_ERROR)   PRINTLN(F ("Begin Failed"));
                                            else if (error == OTA_CONNECT_ERROR) PRINTLN(F ("Connect Failed"));
                                            else if (error == OTA_RECEIVE_ERROR) PRINTLN(F ("Receive Failed"));
                                            else if (error == OTA_END_ERROR)     PRINTLN(F ("End Failed"));
                                          });
  PRINTLN(F("\nOTA Ready"));
  ArduinoOTA.begin();
}
/*
  bool send_multicast_data(String ips,String val){//2,7,
String _MSG="MUS@"+ips+"&"+val; 
 int L=_MSG.length();
 byte buffer[L+1];
  _MSG.getBytes(buffer,sizeof(buffer));
  _UDP.beginPacketMulticast(multicast_channel, multicast_PORT,WiFi.localIP()); //NTP requests are to port 123
 byte n= _UDP.write( buffer,L);
  // PRINT("number of write=");PRINT(n);
  byte r= _UDP.endPacket();  
  PRINT(F(">UDP send:"));PRINTLN(r);
   return r;     
}
 bool send_data_to_UDP(String des,String com, String val){  
 String _MSG=com+"&"+val;
  int L=_MSG.length();
//  PRINT("length=");PRINT(L);
  byte buffer[L+1];
  _MSG.getBytes(buffer,sizeof(buffer));
   //PRINT("message=");PRINT(_MSG); 
 //  PRINT("   buffer2send=");Serial.write((const char *)buffer);
   _UDP.beginPacket(des.c_str(), UDP_PORT); //NTP requests are to port 123
 byte n= _UDP.write( buffer,L);
  // PRINT("number of write=");PRINT(n);
  byte r= _UDP.endPacket();  
  PRINT(F(">UDP send:"));PRINTLN(r);
   return r;      
}
 
 bool send_data_to_TCP(String des,String com, String val){ //-->>
//HTTPClient http;
bool rez=false;
String s="http://"+des+"/data?com="+com+"&val="+val;
PRINTLN(s);
http.begin(s); 
int httpCode = http.GET(); 
if(httpCode > 0)
        {
          if(httpCode == HTTP_CODE_OK) {
                        String payload = http.getString();
                         PRINTLN(payload);
                      rez=true;
                      }
        }
http.end(); 
return rez;                          
}
 
void PerformCommand(){
	PRINT(F("Perform the Command="));PRINTLN(BOT_COMMAND);	
	//=D0011 =A  =M
 
     if(BOT_COMMAND[0]=='A')  {Automatic=1; EEPROM.write(Automatic_p, Automatic);EEPROM.commit();}
else if(BOT_COMMAND[0]=='M')  {Automatic=0; EEPROM.write(Automatic_p, Automatic);EEPROM.commit();}
else if(BOT_COMMAND[0]=='N')  {EnableTelegram=0; EEPROM.write(EnableTelegram_p, EnableTelegram);EEPROM.commit();}
else{		
	for(byte i=0;i<sizeof(PINS);i++) //D0111
	{                              byte bit=(BOT_COMMAND[1+i]-'0');
	                               if(bit>0) bit=1;
		                           if(bit!=PinLastStatus[i])
								   {
									PinLastStatus[i]=bit;
									digitalWrite(PINS[i],bit);
									if(onResetMode==2){EEPROM.write(Key_B_Staus_p+i, bit);EEPROM.commit();}   
								   }
	}
	}
	//compose_status();
	NewTelegramCommand=false;
}


void read_pins(void){	for(byte i=0;i<sizeof(PINS);i++) PinLastStatus[i]= digitalRead(PINS[i]);	 }
void capture_pin_staus(){
	read_pins();
	EEPROM.write(Key_B_Staus_p ,PinLastStatus[0]);
	EEPROM.write(Key_A_Staus_p ,PinLastStatus[1]);
	EEPROM.write(Key_C_Staus_p ,PinLastStatus[2]);
	EEPROM.commit();
}

void Send_to_server(String val,byte protcol){
switch(protcol)
			{
				case 0 : send_data_to_TCP(Device_info[Master_IP],"SV",  val); break;
				case 1 : send_data_to_UDP(Device_info[Master_IP],"SV",  val); break;
				case 2 : mqttClient.publish(Device_info[MQTT_Topic].c_str(), QOS, Retain, val.c_str(), val.length()); break;
				case 3 : send_multicast_data("244.1.1.10", val); break;
			}
  
}
*/ 
String compose_system(void){
//     0            1              2             3         4            5           6           7        
//WifiMode  Function_Modde Scenario_index  onResetMode  CONTROL   STANDALONE       ADMIN  EnableTelegram   
//      A          A             0            0            M            S            A         N         
 //AA00MSAND0000
String s=String(WifiMode); 
if(STANDALONE)     s+="S";
else               s+="M";
if(ADMIN)          s+="A";
else               s+="L";

/*
if(COOLINGsys)     s+="B";
else               s+="R";
*/

return s;
}


String inputString;
uint32_t lastWS_send=0;
#define WS_Delay 50
 void parseSerial(){   
 
//Serial1.println(inputString.length());
Serial1.print(inputString);
inputString="SRO@"+inputString;
//AsyncWebSocketMessageBuffer * buffer = WS.makeBuffer(inputString.length());
 
// if( (millis()-lastWS_send)>WS_Delay ){
                                        WS.text(MainWSclinet, inputString);
                                    //    Serial1.print(inputString.length());Serial1.print(inputString);
                                        inputString = "";  
                                        
                                        // lastWS_send=millis();
                                     //    Serial1.println("-->>>");  
                               //        }

 
 }

 
 void SerialEvent(){

if(Serial.available())
                      {
                      // while(Serial.available()){ }
                    //  size_t len = Serial.available();
                     // uint8_t * sbuf = (uint8_t *)malloc(len);
                     // Serial.readBytes(sbuf, len);
                      inputString= Serial.readString();                      
                     // free(sbuf);
                     // parseSerial();  inputString.length())+
                     Serial1.println("[" + inputString + "]");
                     WS.text(MainWSclinet,"SRO@"+inputString);
                      }

/*
if (Serial.available())
                       {                         
                         // char inChar = (char)Serial.read();
                        //  inputString += inChar; 
                          // New line char ?
                          inputString=Serial.readStringUntil('\r');
                         // if (inChar == '\r'  ) 
                          parseSerial();                          
                        }
  */                       
 }
 
#ifdef EnableSwSerial      
String inputSwString;

void parseSwSerial(void){
	
  byte data[inputSwString.length()];
  inputSwString.getBytes(data, inputSwString.length());
  FfbOnUsbData( data, inputSwString.length())	
}	 
void swSerialEvent(){ 
 if (swSerial.available())
                       {
                          char inChar = (char)swSerial.read();
                          inputSwString += inChar; 
                          // New line char ?
                          if (inChar == '\r'  ) parseSwSerial();
                        }
 }  
#endif

 
 #endif
