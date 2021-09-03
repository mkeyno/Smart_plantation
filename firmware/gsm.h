#ifndef _GSM_H
#define _GSM_H
                     

SoftwareSerial SIM(RxD, TxD, false, 1024);


#define Arduino_add 9

#include <Ticker.h> 
Ticker _Tic_sensor; //_Tic_sensor.attach(30, check_sensor); request_data

String Arduino_Data="";



 void Get_Data(const char *com)
{
  Serial.print(F("Aquier data=")); 
  //Serial.print(com.length ()); 
//byte buf[3];
 // com.getBytes(buf, sizeof(buf));
//for(byte i=0;i<sizeof(buf);i++) buf[i]=(byte)com[i];
  Wire.beginTransmission(Arduino_add);
  Wire.write((byte*)com,8); 
  Wire.endTransmission();  // Moved up here by frippe75
  delay(4);
  // GET RESPONSE 
  Wire.requestFrom(Arduino_add, 20); //address, quantity  , sendStop=true  
 Arduino_Data="";
  while (Wire.available())
  {
    delay(1);
    char c = (char)Wire.read();   
      Arduino_Data += c;   
  }
  Serial.println(Arduino_Data); 
}

/////////////////////////////////////////////////////--------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////         End   ESP function            ///////////////////////////////////////
void send_SMS(const char* text)
{  
   sendCmdAndWaitForResp("AT+CMGF=1\r\n","OK",2); 
   String s="AT+CMGS=\""+PHONE+"\"\r\n";
  sendCmdAndWaitForResp(s.c_str(),">",2); 
  SIM.write(text);
  SIM.println((char)26);
}
void set_action_in_time_span(String com)
{
  
}
void set_time_span(String com)
{
  
}
char set_relay_status(String com)
{//SRS-01
  Serial.println("set relay :"+com);
  byte num_r=com[0]-'0';
  byte status=com[1]-'0';
  if(!num_r) // motor relay
            {
              if(digitalRead(Motor_Relay)!=status) { digitalWrite(Motor_Relay,status); return 'O';}
              else { String s="Already "; if(status) s+="On"; else s+="Off";  Serial.println(s);  send_SMS(s.c_str());
              return 'N';
              }
            }
 else       {
              String s="SRS@"+com;
             Get_Data(s.c_str());
             return Arduino_Data[0];
            }
}

void set_scenario(String com)
{
  byte v=com.toInt();  
 
  {
    
    
  }
 // else
  
}
void set_user_pass(String old_user, String old_pass,String com)
{
//  Serial.print("change user pass to"); //
       byte q=com.indexOf('?');
  String user=com.substring(0,q);
  String pass=com.substring(q+1);
  //Serial.println(user+"& "+pass);
  RemoveUser(old_user,old_pass);
  AddUser(user,pass,"Admin");
  send_status_report();
}

void send_status_report(void)
{
  Serial.println("Send SMS Report");
  String s="Owner:"+ OWNER+"\nMotor Status:"+String(digitalRead(Motor_Relay))+"\nTemp=:";
  send_SMS(s.c_str());
}
void SMS_exe(String user, String pass, String exe,String com)
{////user11.pass,com-val14   
       if(exe=="GRP") send_status_report();
  else if(exe=="SUP") set_user_pass(user,pass,com);
  else if(exe=="SSN") set_scenario(com);
  else if(exe=="SRS") set_relay_status(com);
 
  else if(exe=="STS") set_time_span(com);
  else if(exe=="SAC") set_action_in_time_span(com);  
}

String read_SIM(byte t, int n)
 {
  String col="";
  int i=0;
unsigned long timerStart=0;
              timerStart = millis();
              while((millis()- timerStart)<t*1000)
              {
               char c=(char)SIM.read(); 
               Serial.print(c);
               col+=c;
               if(i++>n) break;
              }
  return col;
 }
int sendCmdAndWaitForResp(const char* cmd, const char *resp, unsigned timeout)
{
  SIM.write(cmd);
 //Serial.println(cmd);
 SIM_BUFF="";
int len = strlen(resp);
    byte sum=0,Location=0;
    unsigned long timerStart,timerEnd;
    timerStart = millis(); 
     
    while(1) {
        if(SIM.available()) {
                        char c = SIM.read();
                        Serial.write(c);
                        if(c<32) SIM_BUFF+='_';
                        else     SIM_BUFF+=c;
                        Location++;
                        sum = (c==resp[sum]) ? sum+1 : 0;
                        if(sum == len) break;
                      }
       timerEnd = millis();
      // Serial.print(".");
        if((timerEnd - timerStart) > 1000 * timeout) return 0; 
        yield();
      }
    while(SIM.available())    SIM.read(); 
    return Location-len+1;  
}
String read_sim(void)
{
   String sim_buf="";
   Serial.print("[->\n");
  while (SIM.available() > 0) 
                           { 
                           char c = (char)SIM.read();
                            sim_buf+=c;
                            Serial.print(c);
                           }
  //Serial.print(sim_buf.length());
  Serial.println("\n<-]");
 // SENS_WEBSOCKET("SIM$"+sim_buf) ;
 return sim_buf;
}
void SENS_WEBSOCKET(String s)
{
if((millis()-last_attemp_to_send_WS)>2 || SIM_BUFF.length()>1024)
  {
   // Serial.println("[WS]"+s);
  webSocket.sendTXT(client_num,s.c_str());
  last_attemp_to_send_WS=millis();
  }  
 else  Serial.println("[NWS]");
}

void parse_sim(String _simcome)
{
 //Serial.println("parse("+_simcome+")");
 if(_simcome.indexOf("+CMT")>0) {
                                  Serial.print("new sms come, num =");
                                  byte n=_simcome.indexOf(",");
                                  byte num=_simcome.substring(n+1).toInt();                                  
                                  Serial.println(num);
                                  // AT+CMGF=1     Set SMS system into text mode
                                  if(sendCmdAndWaitForResp("AT+CMGF=1\r\n","OK",2))       { Serial.println("-->"); Serial.println(SIM_BUFF);  Serial.println("   <--");SIM_BUFF=""; }
                                  String s="AT+CMGR="+String(num)+"\r\n";
                                  if(sendCmdAndWaitForResp(s.c_str(),"OK",3))  
                                  // if(sendCmdAndWaitForResp("AT+CMGL=\"ALL\"\r\n","OK",2))
                                                                                            { // Serial.println("-->"); Serial.println(SIM_BUFF);  Serial.println("   <--");
                                                                                            // SIM_BUFF.trim();
                                                                                             byte p=SIM_BUFF.indexOf("@@");
                                                                                             byte e=SIM_BUFF.indexOf("OK");
                                                                                             if(p)
                                                                                                   {                                                                                                        
                                                                                                    String com=SIM_BUFF.substring(p+2,e-4); //Serial.print(p); Serial.print("--");Serial.print(e);
                                                                                                    //user11.pass,com-val14
                                                                                                    p=com.indexOf('.');
                                                                                                    e=com.indexOf(',');
                                                                                                    String user=com.substring(0,p);
                                                                                                    String pass=com.substring(p+1,e);
                                                                                                    p=com.indexOf('-');
                                                                                                    String exe=com.substring(e+1,p);
                                                                                                    String val=com.substring(p+1);
                                                                                                    
                                                                                                    Serial.println("User="+user+"\nPass="+pass+"\nCommand="+exe+"\nValue="+val); 
                                                                                                    if(check_user_pass(user,pass)) SMS_exe(user,pass,exe,val);
                                                                                                    //SENS_WEBSOCKET(com);
                                                                                                   }
                                                                                           SIM_BUFF=""; 
                                                                                           }
                                  //if(sendCmdAndWaitForResp("AT+CMGR=1\r\n","OK",2)) {  Serial.println("-->"); Serial.println(SIM_BUFF);  Serial.println("   <--"); webSocket.sendTXT(client_num,("SMS$"+SIM_BUFF).c_str()); SIM_BUFF=""; }
                                  if(num>14)  sendCmdAndWaitForResp("AT+CMGDA=\"DEL ALL\"\r\n","OK",2);
                                 }
else if(_simcome.indexOf("+CLIP")>0) Serial.println("new call");
}
void serialEvent() 
{
  while (Serial.available())
  {   
     char inChar = (char)Serial.read();  
    inputString += inChar;
     if (inChar == '\n') stringComplete = true; 
  }
}


#endif






