#ifndef _AWSOCKET_H
#define _AWSOCKET_H

#define MAX_WS_CLIENT  5
#define CLIENT_NONE     0
#define CLIENT_ACTIVE   1

//String check_update(byte);

String compose_js(){

String s= LoginUser+"$"+MyIP+"$"+SerialPart+"$"+Device_info[Hostname]+"$"+Device_info[ST_ssid]+"$"+Device_info[AP_ssid]+"$"+Device_info[Static_IP]+"$"+Device_info[AP_Server_IP]+"$"+Device_info[Master_IP]+"$"+          
		 "";
return s;		 
} 

void parse_webSocket(AsyncWebSocketClient * client, String income,uint32_t num){
     byte eqI1 = income.indexOf('=');
 String cammand = income.substring(0, eqI1); 
 String value   = income.substring(eqI1+1);  
 
 if(cammand=="SGT")  { // [-Standalone-]
	Device_info[Last_store_date]=value.substring(2,8);  // 
	Device_info[Last_store_time]=value.substring(8);                           
	Ego_Seconds=Device_info[Last_store_time].toInt();   PRINT(F(" Base second="));PRINTLN(Ego_Seconds);   //77666000  
	String respond= "RST$"+compose_js();		
	
	  PRINTLN(respond);                                     
	client->text(respond); 
 } 
 else if(cammand=="RST")  client->text("RST$"+compose_js()); 
 //'''''''''''''''''''''''''''''''''''''''''   page setting     ''''''''''''''''''''''''''''''''''''''''''''''''''''''''
// else if(cammand=="UDM")  { UPDATING=true; check_update(num); UPDATING=false; } 
 else if(cammand=="TTS")  {/*         set the  enable remote  sensor IP*/}
 //'''''''''''''''''''''''''''''''''''''''''   wifi setting     ''''''''''''''''''''''''''''''''''''''''''''''''''''''''
 else if(cammand=="WFM")  {if(value[0]=='S') WifiMode='S'; else WifiMode='A'; PRINT("WifiMode="); PRINTLN(WifiMode);} 
 else if(cammand=="SIP")  Device_info[Master_IP]=value;
 else if(cammand=="STI")  Device_info[Static_IP]=value; 
 else if(cammand=="SHN")  Device_info[Hostname]=value;  
 else if(cammand=="APN")  Device_info[AP_ssid]=value; 
 else if(cammand=="APP")  Device_info[AP_pass]=value;
 else if(cammand=="SNW") {  // SSID SSIDPASS Select Network user&pass        [-Standalone-]     
						       eqI1 = value.indexOf('&');
						 Device_info[ST_ssid]= value.substring(0, eqI1); 
						 Device_info[ST_pass]= value.substring(eqI1+1);  						    
						}
 else if(cammand=="SNT") { if(value[0]=='S') STANDALONE=1; else   STANDALONE=0; EEPROM.write(STANDALONE_p, STANDALONE);EEPROM.commit();}
 else if(cammand=="SVE")  { client->text("SVE$"); UpdateContent("/data/device.inf",compose()); }
 //''''''''''''''''''''''''''''''''''''''''' app websocket command     ECM   '''''''''''''''''''''''''''''''''''''''''''''''''''''
else if(cammand=="R42") { RN42=value+"\r\n"; SENDCODE=true;}
else if(cammand=="ECM")  Serial.print("$$$");
 
//else if(cammand=="RNR")  //Reset Module with pin 0 {digitalWrite(0,LOW);}
else if(cammand=="KEY")   Serial.print(value[0]);////KEY=x1 (1<<(value[1]-'0')) 
else if(cammand=="SCK")   { keyboardPress((byte) value[0],(byte)0x0);}// Serial1.print((byte)value.toInt());  }//SCK=O

else if(cammand=="JTE")   {if(value[0]=='E') RecieveMode=true; else RecieveMode=false;}
else if(cammand=="JSL")   {if(!RecieveMode) return; gameJoyPress((byte)0x0,(1<<(value[0]-'0')) & 0xFF);}//JST=4
else if(cammand=="JSR")   {if(!RecieveMode) return; gameJoyPress((1<<(value[0]-'0')) & 0xFF,(byte)0x0);}//JST=4
else if(cammand=="JSA")   {if(!RecieveMode) return; eqI1 = value.indexOf(','); 
                           signed int b= (value.substring(eqI1+1)).toInt();
                           signed int g= value.toInt();
                           gameJoyMove(0,0,g,b);
                           //Serial1.print(g);Serial1.print(b);
                           }//gas JSG=255
else if(cammand=="ARC")  {ArduniDataFor=value;   //use Arduino data for calibration(CW), for joystick(JO) for nothing(ID) 
                                 if(ArduniDataFor[0]=='C') { InitializedCalibration=false;	CalibrationCode=ArduniDataFor[1];}//calibration
              					    else if(ArduniDataFor[0]=='S') { ArduniDataFor[0]='I'; SaveCalibrationDO=true;}//SA ignore & save calibration
                            else if(value=="RE")           {  ArduniDataFor[0]='I'; SendCalibrationData=true;}
                            //JO  Execute Data  ,default value
                            //ID Ignor Data  Stop I2C
                            //MX,MW set mean value 
                          }
						  
				//"SC"   Wire.write(calib_buffer); DoCalibration("CW");"RS" restor_setting(); "YE" EnableY_Axis 
else if(cammand=="ACO")   {Command2Arduino[0]=value[0]; Command2Arduino[1]=value[1];}						  
   //  set axis  buttons  
else if(cammand=="XJB")   { EnableAxisBotton = value.toInt(); EEPROM.write(EnableAxisBotton_p, EnableAxisBotton); EEPROM.commit();PRINT("EnableAxisBotton=");PRINTLN(EnableAxisBotton);}  
else if(cammand=="SGB")   {        GasButton = value.toInt(); EEPROM.write(GasButton_p, GasButton);               EEPROM.commit();PRINT("GasButton=");PRINTLN(GasButton);}
else if(cammand=="SBB")   {       BreakBtton = value.toInt(); EEPROM.write(BreakBtton_p, BreakBtton);             EEPROM.commit();PRINT("BreakBtton=");PRINTLN(BreakBtton);}
  //set wheel              
else if(cammand=="SLW")   { WheelButtonLeft = value.toInt(); EEPROM.write(WheelButtonLeft_p, WheelButtonLeft); EEPROM.commit();PRINT("WheelButtonLeft=");PRINTLN(WheelButtonLeft);}
else if(cammand=="SRW")   { WheelButtonRight= value.toInt(); EEPROM.write(WheelButtonRight_p,WheelButtonRight);EEPROM.commit();PRINT("WheelButtonRight=");PRINTLN(WheelButtonRight);}
//set gas  break axis
else if(cammand=="SGX")   {          GasAxis= value.toInt(); EEPROM.write(GasAxis_p    ,GasAxis);EEPROM.commit();PRINT("GasAxis=");PRINTLN(GasAxis);}//0,2,4,8
else if(cammand=="SBX")   {        BreakAxis= value.toInt(); EEPROM.write(BreakAxis_p,BreakAxis);EEPROM.commit();PRINT("BreakAxis=");PRINTLN(BreakAxis);}//0,1,2,3
// only affected  right button 
else if(cammand=="SRB") {}

else if(cammand=="RNR")   HareReset=true;
}

bool client_active=false ;


void  webSocketEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *payload, size_t length) {
     if (type == WS_EVT_DISCONNECT) {
									  PRINTF("WS URL=%s ID=%u Disconnected: %u\n", server->url(), client->id());
									  client_active=false;
                  //  client->close();
									 // client->ping();
									  /*
									for (uint8_t i=0; i<MAX_WS_CLIENT ; i++) {
																			  if (ws_client[i].id == client->id() )
																				{
                                       
																				ws_client[i].id = 0;
																				ws_client[i].state = CLIENT_NONE;												
																				break; // Exit for loop
																			  }
																			}
	 */
								}
else if (type == WS_EVT_CONNECT) { 
							 	PRINTF("Client %u added with ip=", client->id());	PRINTLN(client->remoteIP());	
									client->ping();
									MainWSclinet=client->id();
                 // client->keepAlivePeriod(2);
									/*byte index=0;
								  for (  index=0; index<MAX_WS_CLIENT ; index++) 
								  {
									  if (ws_client[index].id == 0 )  
												   {
													ws_client[index].id = MainWSclinet=client->id();
													ws_client[index].state = CLIENT_ACTIVE;
													PRINTF("Client %u added  at index[%d] with ip=", client->id(), index);	Serial.println(client->remoteIP());												 
													client_active=true;
													//client->ping();
													break;  
												   }
									}
									if(index>=MAX_WS_CLIENT) { PRINTLN("Client request rejected ");}//client->ping();
									*/						
                                }
else if (type == WS_EVT_DATA)  {
					AwsFrameInfo * info = (AwsFrameInfo*)arg;
					String msg = "";
					if (info->final && info->index == 0 && info->len == length){
														  if (info->opcode == WS_TEXT) { for (size_t i = 0; i < info->len; i++) msg += (char)payload[i]; }
														  else { // Binary
      															char buff[3];
      															for (size_t i = 0; i < info->len; i++) 
                                            															{
                                            															  sprintf(buff, "%02x ", (uint8_t)payload[i]);
                                            															  msg += buff;
                                            															}
      														  }
														  //  PRINTF("URL%s client:%u:%s[%lu]:", server->url(), client->id(), (info->opcode == WS_TEXT) ? "TXT" : "BIN", info->len);//URL/ws client:1:TXT[1073647444]:ECM
														  //  PRINTF("%s\r\n", msg.c_str());
														 	PRINTF("[[WS]]%s\r\n",msg.c_str());
														}
					else 			       {
											  if (info->index == 0) 
											  { // Message start
												//if (info->num == 0) //PRINTF("ws[%s][%u] %s-message start\n", server->url(), client->id(), (info->message_opcode == WS_TEXT) ? "text" : "binary");
													//	 		PRINTF("ws[%s][%u] frame[%u] start[%llu]\n", server->url(), client->id(), info->num, info->len);
											  }
											  // Continue message
											 //  PRINTF("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT) ? "text" : "binary", info->index, info->index + length);
											  if (info->opcode == WS_TEXT) 
											  { // Text
												for (size_t i = 0; i < info->len; i++)  msg += (char)payload[i];
											  }
											  else 
											  { // Binary
												char buff[3];
												for (size_t i = 0; i < info->len; i++) 
												{
												  sprintf(buff, "%02x ", (uint8_t)payload[i]);
												  msg += buff;
												}
											  }
											 //  PRINTF("%s\r\n", msg.c_str());
											  if ((info->index + length) == info->len) 
											  { // Message end
										 //		PRINTF("ws[%s][%u] frame[%u] end[%llu]\n", server->url(), client->id(), info->num, info->len);
												if (info->final) 
												{
										//	 	  PRINTF("ws[%s][%u] %s-message end\n", server->url(), client->id(), (info->message_opcode == WS_TEXT) ? "text" : "binary");
												}
											  }
											}
					
					parse_webSocket(client, msg,client->id()); 
							  msg.~String();  /*
					for (byte index=0; index<MAX_WS_CLIENT ; index++){
					  if (ws_client[index].id == client->id() ) // if client registered before parse its message
						  {
							parse_webSocket(client, msg,client->id()); 
							  msg.~String();   
						  }  
					}  
					*/
				  }
}

#endif
