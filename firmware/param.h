#ifndef _PARAM_H
#define _PARAM_H

extern "C" uint32_t _SPIFFS_start;
extern "C" uint32_t _SPIFFS_end;


 
String Device_info[20];

 
#define   Modes    0
#define   Hostname    1
#define   ST_ssid    2
#define   ST_pass    3
#define   AP_ssid    4
#define   AP_pass    5
#define   Static_IP    6
#define   AP_Server_IP    7
#define   Master_IP    8
#define   Last_store_date    9
#define   Last_store_time    10
#define   MQTT_Server_IP    11
#define   MQTT_Topic    12
#define   MQTT_user    13
#define   MQTT_pass    14
#define   MQTT_port    15
#define   MQTT_clientid    16
#define   Zoon5    17
#define   TempZoon5    18




////////////EEPROM
#define STANDALONE_p      1
#define STATIC_p      2
#define Enable_log_p      3
#define Protcol_p      4
#define _MQTT_p      5
#define QOS_p      6
#define Retain_p      7
#define Send2Server_p      8

#define WheelButtonLeft_p   10
#define WheelButtonRight_p  11
#define GasAxis_p           12
#define BreakAxis_p         13
#define EnableAxisBotton_p  14
#define GasButton_p         15
#define BreakBtton_p        16
#define Calibration_p      30

byte reset_number=0;

byte  STANDALONE=1;
byte  STATIC=0;
byte  Enable_log=0;
byte  COOLINGsys=0;
byte  ReciveRemoteSensor=0;
byte  Protcol=0;
byte  _MQTT=1;
byte  QOS=1;
byte  Retain=1;
byte  Send2Server=0;

 






///////////////////////////////

byte  ZOON=0;
uint32_t TimesSpans[5]={27000,45000,63000,75600,86400};
unsigned long Ego_Seconds=27000;
 
 
String MyIP,MyMAC;
String PerfixID="";
String preSetUser="";
String preSetPass="";
 
byte MainWSclinet;
 

bool shouldReboot = false;   
bool UPDATING=false,PendingUpdating=false;;
bool EnabaleOTA=true; 

 
bool ADMIN=false;
bool ACCESS=false;
String USERS[5],PASS[5],LoginUser;
byte  IsAdmin[5];

 char WifiMode;

 /////////////////////////////////////////////////////////////////////////////////////////////////////////

 /////////////////////////////////////////////////////////////////////////////////////////////////////////
void restor_user_pass(void);
 void UpdateContent(String , String );
  #ifdef EnableMQTT
 void Mqtt_setting(void);
 #endif
 String formatBytes(size_t );
 void print_chip_info(void);
 void print_device_info(void);

 /////////////////////////////////////////////////////////////////////////////////////////////////////////
String read_line(String _path,byte line_num){
File f = SPIFFS.open(_path, "r");
if (!f) return "" ; 
byte counter=0; 
String line="";
while(f.available())//1AAA@THAT54545646|1$15:
{ 
 line = f.readStringUntil('\n'); 
 line.trim();
                         
 if(line.length() >2 && counter==line_num) break;
 counter++;
}
 f.close();  
 return line;
}

String compose(void){
Device_info[Modes]=String(WifiMode);
String s=
Device_info[Modes]+ "@"+
Device_info[Hostname]+ "@"+
Device_info[ST_ssid]+ "@"+
Device_info[ST_pass]+ "@"+
Device_info[AP_ssid]+ "@"+
Device_info[AP_pass]+ "@"+
Device_info[Static_IP]+ "@"+
Device_info[AP_Server_IP]+ "@"+
Device_info[Master_IP]+ "@"+
Device_info[Last_store_date]+ "@"+
Device_info[Last_store_time]+ "@"+
Device_info[MQTT_Server_IP]+ "@"+
Device_info[MQTT_Topic]+ "@"+
Device_info[MQTT_user]+ "@"+
Device_info[MQTT_pass]+ "@"+
Device_info[MQTT_port]+ "@"+
Device_info[MQTT_clientid]+ "@"+
Device_info[Zoon5]+ "@"+
Device_info[TempZoon5]+ "@"+

" ";
PRINTLN(s);
return s;
}



void decompose(String s){
byte As=0; //Modes = WifiMode,Function_Modde,Scenario
 WifiMode=s[0];           if(WifiMode!='S') WifiMode='A'; 
 
   As=s.indexOf('@'); s=s.substring(As+1);
 
As=s.indexOf('@');  Device_info[Hostname]=s.substring(0,As);     if(Device_info[Hostname].length()<3) Device_info[Hostname]="SmartIOT"; s=s.substring(As+1);
As=s.indexOf('@');  Device_info[ST_ssid]=s.substring(0,As);     if(Device_info[ST_ssid].length()<3) Device_info[ST_ssid]="NS"; s=s.substring(As+1);
As=s.indexOf('@');  Device_info[ST_pass]=s.substring(0,As);     if(Device_info[ST_pass].length()<3) Device_info[ST_pass]="NS"; s=s.substring(As+1);
As=s.indexOf('@');  Device_info[AP_ssid]=s.substring(0,As);     if(Device_info[AP_ssid].length()<3) Device_info[AP_ssid]="SmartTouch" +String(ESP.getChipId(), HEX); s=s.substring(As+1);
As=s.indexOf('@');  Device_info[AP_pass]=s.substring(0,As);     if(Device_info[AP_pass].length()<3) Device_info[AP_pass]="12345678"; s=s.substring(As+1);
As=s.indexOf('@');  Device_info[Static_IP]=s.substring(0,As);     if(Device_info[Static_IP].length()<3) Device_info[Static_IP]="192.168.4.0"; s=s.substring(As+1);
As=s.indexOf('@');  Device_info[AP_Server_IP]=s.substring(0,As);     if(Device_info[AP_Server_IP].length()<3) Device_info[AP_Server_IP]="192.168.4.1"; s=s.substring(As+1);
As=s.indexOf('@');  Device_info[Master_IP]=s.substring(0,As);     if(Device_info[Master_IP].length()<3) Device_info[Master_IP]="192.168.0.0"; s=s.substring(As+1);
As=s.indexOf('@');  Device_info[Last_store_date]=s.substring(0,As);     if(Device_info[Last_store_date].length()<3) Device_info[Last_store_date]="20160510"; s=s.substring(As+1);
As=s.indexOf('@');  Device_info[Last_store_time]=s.substring(0,As);     if(Device_info[Last_store_time].length()<3) Device_info[Last_store_time]="77660"; s=s.substring(As+1);
As=s.indexOf('@');  Device_info[MQTT_Server_IP]=s.substring(0,As);     if(Device_info[MQTT_Server_IP].length()<3) Device_info[MQTT_Server_IP]="192.168.1.100"; s=s.substring(As+1);
As=s.indexOf('@');  Device_info[MQTT_Topic]=s.substring(0,As);     if(Device_info[MQTT_Topic].length()<3) Device_info[MQTT_Topic]="top"; s=s.substring(As+1);
As=s.indexOf('@');  Device_info[MQTT_user]=s.substring(0,As);     if(Device_info[MQTT_user].length()<3) Device_info[MQTT_user]=""; s=s.substring(As+1);
As=s.indexOf('@');  Device_info[MQTT_pass]=s.substring(0,As);     if(Device_info[MQTT_pass].length()<3) Device_info[MQTT_pass]=""; s=s.substring(As+1);
As=s.indexOf('@');  Device_info[MQTT_port]=s.substring(0,As);     if(Device_info[MQTT_port].length()<3) Device_info[MQTT_port]="1883"; s=s.substring(As+1);
As=s.indexOf('@');  Device_info[MQTT_clientid]=s.substring(0,As);     if(Device_info[MQTT_clientid].length()<3) Device_info[MQTT_clientid]="1"; s=s.substring(As+1);
As=s.indexOf('@');  Device_info[Zoon5]=s.substring(0,As);     if(Device_info[Zoon5].length()<3) Device_info[Zoon5]="2700-4500-6300-7560-8640-"; s=s.substring(As+1);
As=s.indexOf('@');  Device_info[TempZoon5]=s.substring(0,As);     if(Device_info[TempZoon5].length()<3) Device_info[TempZoon5]="27-27-27-27-27-"; s=s.substring(As+1);

}
void restore_EEPROM(void){
 
reset_number=EEPROM.read(0); if(reset_number>10) reset_number=0;	

STANDALONE=EEPROM.read(STANDALONE_p);
STATIC=EEPROM.read(STATIC_p);
 Protcol=EEPROM.read(Protcol_p); if(Protcol>3)  Protcol=0;
_MQTT=EEPROM.read(_MQTT_p); if(_MQTT>10)  _MQTT=1;
QOS=EEPROM.read(QOS_p); if(QOS>2)  QOS=1;
Retain=EEPROM.read(Retain_p); if(Retain>10)  Retain=1;
Send2Server=EEPROM.read(Send2Server_p); if(Send2Server>1)  Send2Server=0;

RestorCalibration();

}
void restore_setting(void)  { 
 restore_EEPROM();
 String STRING=read_line(DeviceInfo,0);
 decompose(STRING);

 Ego_Seconds=Device_info[Last_store_time].toInt();   
 String dummy=Device_info[Zoon5];
 for(byte i=0;i<5;i++)
  {
                         byte Sa=dummy.indexOf('-');
                   TimesSpans[i]=dummy.substring(0,Sa).toInt()*10;//                   
  if( Ego_Seconds> TimesSpans[i]) ZOON++;
              dummy=dummy.substring(Sa+1);                
  } 

 preSetUser=String(ESP.getChipId());//
 
             preSetPass=String(ESP.getChipId(), HEX);
             preSetPass.toUpperCase();
			 if(preSetPass.length()<5) preSetPass="X"+preSetPass;
 SerialPart+=preSetPass;
 USERS[0]=preSetUser;
 PASS[0]=preSetPass;
 IsAdmin[0]=1;
 restor_user_pass();
 delay(100);
}
void store_setting(void){
String STRING=compose();
PRINT(F("save STRING:"));PRINTLN(STRING);
UpdateContent(DeviceInfo,STRING);	
PRINTLN(F("[Stored]"));
}
 void Fail_Safe(void){
 PRINTLN(F("ENTER SAFE MODE(Restarting.....................)"));
 EEPROM.write(STANDALONE_p, 1);
 EEPROM.write(STATIC_p,0);
 EEPROM.commit();
 String ss= "AM@SmartIOT@NS@NS@SmartTouch"+String(ESP.getChipId(), HEX)+
            "@12345678@192.168.4.0@192.168.4.1@192.168.0.0@192.168.0.0@192.168.0.1@top@user@pass@55@1@50@27@2700-4500-6300-7560-8640-@27-27-27-27-27-@";
 UpdateContent(DeviceInfo,ss);  
delay(1000);
 ESP.restart();              
}
 
#endif
