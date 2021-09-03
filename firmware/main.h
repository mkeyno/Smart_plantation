#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <time.h>
#include <FS.h>
#include <Hash.h>
#include "ffb.h"
//#include <ESPAsyncTCP.h>
#ifdef EnableUDP
	 #include "ESPAsyncUDP.h"
	 //#include <WiFiUdp.h>
	 #define UDP_PORT       747 
	 #define multicast_PORT 787
	 WiFiUDP _UDP;
	 IPAddress multicast_channel(224,1,1,10);
#endif


#include <ESPAsyncWebServer.h>
		 AsyncWebServer server(80);
		 AsyncWebSocket WS("/ws");
//		  AsyncClient  *client;
//		 AsyncServer Aserver(80);
	//	 AsyncClient  Aclient;		 
//       AsyncEventSource events("/events");
#include <EEPROM.h>
//#define EnableMQTT
 #ifdef EnableMQTT
     #include <AsyncMqttClient.h>
     #include <mqttset.h>
 #endif
 //#define EnableUpdate
#ifdef EnableUpdate
    #include <ESP8266HTTPClient.h>
                     HTTPClient http;				 
    #include <StreamString.h>
    #include <ESP8266httpUpdate.h>
    
     #include <ESP8266HTTPUpdateServer.h>
               ESP8266HTTPUpdateServer httpUpdater;
 #endif
//#include <WiFiClientSecure.h>	
 //         WiFiClientSecure SeClient;	 

 
void setup ( void );
void loop ( void );
#include "htm.h"
#include "param.h"

#include "func.h"
#include "awsocket.h"
#include "async.h"


#include <WiFiClient.h>

 
void setup ( void ) {
 
	 
  Serial.begin(115200);//, SERIAL_8N1,SERIAL_TX_ONLY);
  delay(200);  
  Serial.setTimeout(50);
  //Serial.setDebugOutput(true);
  Serial.print(F("\n\n\n Keyno Guidance & Control\nwww.Keyno-GC.com\nVersion="));  Serial.println(SerialPart); 
  SIM.begin(115200);
  delay(500);
  Wire.begin(SDA_pin,SCL_pin);
 ///////////////////////
 /////////////////////////////////////////////////////////////
 #ifdef DEBUG_Serial1
 Serial1.begin(115200, SERIAL_8N1,SERIAL_TX_ONLY);
 delay(200);
 Serial1.println(F("\n\n\n Keyno Guidance & Control\nwww.Keyno-GC.com\n this is from Serial1"));
 #endif
  
  SPIFFS.begin();
 EEPROM.begin(128);
 delay(100);
   restore_setting();
  // compose_status();
  DIR(); 
  print_chip_info();
  print_device_info();
   wifi_startup();

  if(EnabaleOTA) ConfigureOTA();

   server.on("/exit",             [](AsyncWebServerRequest *request){ ACCESS=ADMIN=false; request->requestAuthentication(); request->send(401, "text/plain", "Thanks\n Exit");  });
server.on("/process", HTTP_GET,[](AsyncWebServerRequest *request){ /*if(Access) */handleProcess(request); });  
server.on("/config",           [](AsyncWebServerRequest *request){ handleConfig(request);  });
server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request){
              if(!check_Auth(request))   request->requestAuthentication(); 
              request->send(200, "text/html", "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>");
              });
              
            
server.on("/update", HTTP_POST, [](AsyncWebServerRequest *request){                                                 
                      shouldReboot = !Update.hasError();
                        PRINTF("Update was %s\n",shouldReboot?"OK":"FAIL");
                      AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", shouldReboot?"OK":"FAIL");
                                              response->addHeader("Connection", "close");
                                request->send(response);
  },[](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)  {
    if(!index){
        PRINTF("Update Start: %s\n", filename.c_str());
        Update.runAsync(true);
        if(!Update.begin((ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000)) Update.printError(Serial);
       }
    if(!Update.hasError()){
              if(Update.write(data, len) != len)      Update.printError(Serial);
            }
    if(final){
        if(Update.end(true))  PRINTF("Update Success: %uB\n", index+len);
        else               Update.printError(Serial);
      }
  }); 
server.on("/upload", HTTP_POST,[](AsyncWebServerRequest *request){ request->send(200, "text/plain", ""); }, handleFileUpload);  
server.onFileUpload([](AsyncWebServerRequest * request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
if (!index)  PRINTF("UploadStart: %s\n", filename.c_str());
PRINTF("%s", (const char*)data);
if (final)  PRINTF("UploadEnd: %s (%u)\n", filename.c_str(), index + len);
});
server.onRequestBody([](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
if (!index)  PRINTF("BodyStart: %u\n", total);
PRINTF("%s", (const char*)data);
if (index + len == total)  PRINTF("BodyEnd: %u\n", total);
}); 
//server.serveStatic("/", SPIFFS, "/Sindex.html.gz","max-age=86400").setFilter(check_Auth);//.setFilter(FilterStandalone);
/*server.serveStatic("/", SPIFFS, "/Mindex.html.gz","max-age=86400").setFilter(FilterMaster);*/
server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){   
                              if(!check_Auth(request))  request->requestAuthentication();
                               else
                               {    
                                 {///if(STANDALONE) {
                                      if(!handleFileRead("/Sindex.html", request)) request->send(404, "text/plain", "Sindex.html FileNotFound");
                                      }                                                                     
                               }   
                                             
                                });
//server.serveStatic("/jquery.3.4.js", SPIFFS, "/jquery.3.4.js.gz"); 
server.serveStatic("/images/logo.png", SPIFFS, "/images/logo.png" ); 

server.onNotFound([](AsyncWebServerRequest *request) {
PRINTF("Not found: %s\r\n", request->url().c_str());
AsyncWebServerResponse *response = request->beginResponse(200);
          response->addHeader("Connection", "close");
          response->addHeader("Access-Control-Allow-Origin", "*");
if (!handleFileRead(request->url(), request))     request->send(404, "text/plain", "FileNotFound");
}); 
/*
events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()) PRINTF("Client reconnected! Last message ID that it gat is: %u\n", client->lastId()); 
    client->send("hello!",NULL,millis(),1000);//send event with message "hello!", id current millis and set reconnect delay to 1 second
  });
  */
// server.addHandler(&events);
                     WS.onEvent(webSocketEvent);
 server.addHandler(&WS);
 server.begin();
   
 
  delay(500);
  #ifdef EnableMQTT
     Mqtt_setting();
 #endif 
  
 setup_ini();
}

void loop ( void ) {  if(!UPDATING){if(EnabaleOTA) ArduinoOTA.handle();} 

/* 
SerialEvent();
 if(WifiMode=='S' && EnableTelegram && !client_active){//////////////// ------------------   Check bot command  ------------------
  if((millis()-LastSendBotUpdate)>Update_Bot_interval)  {  check_bot_command(); LastSendBotUpdate=millis();  }
}
 if(NewTelegramCommand) PerformCommand();

 if(Automatic )  do_key_stetting(Ego_Seconds+millis()/1000);///////// -----------  do_key_stetting  ------------------
  if(WifiMode=='S' && DoRegisterTel ){ //came out of websocket 
									// RegisterTelPass =false; TelegramRegistered=true; 
									  delay(400);
									  RegisterBotID(); 
								  // WS.ping(MainWSclinet);
									if(RegisterationDone) {
														   DoRegisterTel=false;
														   TelegramRegistered=true;
														   UpdateContent("/data/device.inf",compose());
														   }
									
								  } 
if(WifiMode=='S' && RegisterationDone )  {
										  PRINTLN("HttpResult="+HttpResult);
										  WS.text(MainWSclinet,"REG$"+HttpResult);
										 if(HttpResult=="OK")	 UpdateContent("/data/device.inf",compose()); 
										 RegisterationDone=false; 
									  }
 
if(PendingUpdating) {PendingUpdating=false; PRINTLN("check_update"); check_update(0);} 
 */ 

app_loop();
  
}

int main(int argc, char **argv)
{
  setup ();
  loop ();
  return 0;
}
