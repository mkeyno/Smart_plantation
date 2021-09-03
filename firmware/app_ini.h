#ifndef _APPINI_H
#define _APPINI_H
                                              




#include <Wire.h>
#define SlaveDeviceId 9
#include "gsm.h"
 
 

 
#define reset_poz 0
#define mod_poz 1
#define static_poz 2
#define netmod_poz 3
#define scenario_poz 3
#define STORE_poz   5




#define LED_SIM 16
#define LED_MOTOR 2
#define digital_sensor 12
#define Motor_Relay 14

#define SDA_pin 5 
#define SCL_pin 4 //Wire.begin(SDA_pin,SCL_pin)


 #ifdef EnableSwSerial
     #define RxD 2  
	 #define TxD 13 
     #include <SoftwareSerial.h>
	 SoftwareSerial SIM(RxD, TxD, false, 64);
 #endif
 
byte pin_output[]={LED_MOTOR,digital_sensor,Motor_Relay,LED_SIM};
byte initial[]={LOW,HIGH,LOW,LOW} ;
byte LastStatus[sizeof(pin_output)];

int TEMP,HUMID,SOIL,POOL,WELL,VI[6],FACTOR,EFFI,POWER;
byte MOTOR,MANIFOLD,SECMAN,EQUIP;             
                    
void SerialEvent(void);
void ServerINI(void);
void Get_Data_from_Arduino(void);
/*************************************************************************************************/


/*
rows ][ columns 
     currentData  LastData max mean  min //5
gas  
break
wheel
rotate x
*/
uint16_t ArduinoData[4][5];

static byte NumberOfselectingData=0, button;

uint16_t MeanWheal,MeanRotationX,SMeanWheal=0,SMeanRotationX=0;

/*************************************************************************************************/ 
 void Joystick(int8_t BP_X1,int8_t BP_Y1,int8_t BP_X2,int8_t BP_Y2,byte BP_ST_BTN, byte BP_ND_BTN){
Serial.write((byte)0xFD); //Start HID Report
Serial.write((byte)0x6); //Length byte
Serial.write(BP_X1 & 0xFF); //First X coordinate
Serial.write(BP_Y1 & 0xFF); //First Y coordinate
Serial.write(BP_X2 & 0xFF); //Second X coordinate
Serial.write(BP_Y2 & 0xFF); //Second Y coordinate
Serial.write((byte)BP_ST_BTN); //First Button byte
Serial.write((byte)BP_ND_BTN); 
}

void gameJoyMove(int8_t BP_X1,int8_t BP_Y1,int8_t BP_X2,int8_t BP_Y2){
Serial.write((byte)0xFD); //Start HID Report
Serial.write((byte)0x6); //Length byte
Serial.write(BP_X1 & 0xFF); //First X coordinate
Serial.write(BP_Y1 & 0xFF); //First Y coordinate
Serial.write(BP_X2 & 0xFF); //Second X coordinate
Serial.write(BP_Y2 & 0xFF); //Second Y coordinate
Serial.write((byte)0x00); //First Button byte
Serial.write((byte)0x00); //Second Button byte
}


void gameJoyPress(byte BP_ST_BTN, byte BP_ND_BTN){
Serial.write((byte)0xFD); //Start HID Report
Serial.write((byte)0x6); //Length byte
Serial.write((byte)0x00);
Serial.write((byte)0x00);
Serial.write((byte)0x00);
Serial.write((byte)0x00);
Serial.write((byte)BP_ST_BTN); //First Button byte
Serial.write((byte)BP_ND_BTN); //Serial1.write((byte)BP_ND_BTN);//Second Button byte
 
}

void keyboardPress(byte BP_KEY,byte BP_MOD){
Serial.write((byte)0xFD); //Start HID Report
Serial.write((byte)0x9); //Length byte
Serial.write((byte)0x1); //Descriptor byte
Serial.write(BP_MOD); //Modifier byte
Serial.write((byte)0x00); //-
Serial.write(BP_KEY); //Send KEY
for(byte i = 0;i<5;i++){ //Send five zero bytes
                        Serial.write((byte)0x00);
                        }

}

void keyboardReleaseAll()      {keyboardPress((byte)0x00,(byte)0x00);   }
byte HEX2BYTE(char x0,char x1){
  byte a=0;
  if(x0 < '9') a=16*(x0 - '0');
  else         a=16*(x0 - '7');

   if(x1 < '9') a+=(x1 - '0');
  else           a+=(x1 - '7');
  
 // byte a= (16 * ((x0 < '9') ? x0 - '0' : x0 - '7')) + (x1 < '9') ? x1 - '0' : x1 - '7';
  return a;
}


void setup_ini(void);
void app_loop(void);
String RestorCalibration(void);
void SaveCalibration(char code);




#include "main.h"



void SaveCalibration(char code){
  byte row;
  switch (code){                  
          case 'G': row=0; break;
          case 'B': row=1; break;
          case 'W': row=2; break;
          case 'X': row=3; break;}
          
 EEPROM.write(Calibration_p+row*6  ,(byte)(ArduinoData[row][2]/255)); EEPROM.write(Calibration_p+row*6+1,(byte)(ArduinoData[row][2]%255)); //max 
 EEPROM.write(Calibration_p+row*6+2,(byte)(ArduinoData[row][3]/255)); EEPROM.write(Calibration_p+row*6+3,(byte)(ArduinoData[row][3]%255)); //mean
 EEPROM.write(Calibration_p+row*6+4,(byte)(ArduinoData[row][4]/255)); EEPROM.write(Calibration_p+row*6+5,(byte)(ArduinoData[row][4]%255)); //max
 


PRINT("SaveCalibration "); PRINT(row); PRINT(EEPROM.commit()?" Done\n":" Not\n");
/*
PRINT(EEPROM.read(Calibration_p+row*6    )*255+EEPROM.read(Calibration_p+row*6+1)); PRINT(",");PRINT(ArduinoData[row][2]);PRINT(" MAX:"); PRINT(ArduinoData[row][2]/255);PRINTLN(ArduinoData[row][2]%255);
PRINT(EEPROM.read(Calibration_p+row*6 +2 )*255+EEPROM.read(Calibration_p+row*6+3)); PRINT(",");PRINT(ArduinoData[row][3]);PRINT(" MEAN:");PRINT(ArduinoData[row][3]/255);PRINTLN(ArduinoData[row][3]%255);
PRINT(EEPROM.read(Calibration_p+row*6 +4 )*255+EEPROM.read(Calibration_p+row*6+5)); PRINT(",");PRINT(ArduinoData[row][4]);PRINT(" MIN:"); PRINT(ArduinoData[row][4]/255);PRINTLN(ArduinoData[row][4]%255);

*/}
String RestorCalibration(void){
  byte row;
  String s="";
for(row=0;row<4;row++)  {
  ArduinoData[row][2]=EEPROM.read(Calibration_p+row*6    )*255+EEPROM.read(Calibration_p+row*6+1); s+="M"+String(ArduinoData[row][2]);//max
  ArduinoData[row][3]=EEPROM.read(Calibration_p+row*6 + 2)*255+EEPROM.read(Calibration_p+row*6+3); s+="D"+String(ArduinoData[row][3]);//mean
  ArduinoData[row][4]=EEPROM.read(Calibration_p+row*6 + 4)*255+EEPROM.read(Calibration_p+row*6+5); s+="m"+String(ArduinoData[row][4]);//min
}

WheelButtonLeft =EEPROM.read(WheelButtonLeft_p);//143
WheelButtonRight=EEPROM.read(WheelButtonRight_p);//47
EnableAxisBotton=EEPROM.read(EnableAxisBotton_p);
GasButton=EEPROM.read(GasButton_p);
BreakBtton=EEPROM.read(BreakBtton_p);
 
GasAxis=EEPROM.read(GasAxis_p);//47
BreakAxis=EEPROM.read(BreakAxis_p);//47

  PRINTLN("Restor Calibration"); PRINTLN(s);
 return s;
}

void sendCommand2RN42(String _Data){
  
bool change=false;
int8_t gas,Break,Lgas,LBreak;
byte row;

    if(_Data[4]=='[' && _Data[21]==']')//S011[G424B064W390R410];if it valid 
    {  
      ArduinoData[0][0]  =(_Data[6]-'0' )*100+(_Data[7]-'0' )*10+(_Data[8]-'0' );//Gas 
      ArduinoData[1][0]  =(_Data[10]-'0')*100+(_Data[11]-'0')*10+(_Data[12]-'0');//Break
      ArduinoData[2][0]  =(_Data[14]-'0')*100+(_Data[15]-'0')*10+(_Data[16]-'0');//Wheel
      ArduinoData[3][0]  =(_Data[18]-'0')*100+(_Data[19]-'0')*10+(_Data[20]-'0');//RotateX
      
           if(ArduniDataFor[0]=='C'){//calibration for find min and max value CG
                      							  switch (ArduniDataFor[1]){								  
                                  														  case 'G': row=0; break;
                                  														  case 'B': row=1; break;
                                  														  case 'W': row=2; break;
                                  														  case 'X': row=3; break;}
                      								  
                      							  if(InitializedCalibration)
                      							  {
                                      
                      							  if(ArduinoData[row][0]>ArduinoData[row][2]) ArduinoData[row][2]=ArduinoData[row][0];      
                      							  if(ArduinoData[row][0]<ArduinoData[row][4]) ArduinoData[row][4]=ArduinoData[row][0];
                                      PRINT("M");PRINT(ArduinoData[row][2]);PRINT("m");PRINTLN(ArduinoData[row][4]);
                      							  }
                      							  else
                      							  {
                                      PRINT("InitializedCalibration ");PRINTLN(row);
                      								  ArduinoData[row][4]=65000,ArduinoData[row][2]=0;
                      								  InitializedCalibration=true;
                      							  }								  
                      							  
                                   }
      else if(ArduniDataFor[0]=='M' ){// set mean value for wheel and rotation X  MX,MW
                                                               switch (ArduniDataFor[1]){  
                                                                    case 'W': row=2; break;
                                                                    case 'X': row=3; break;}
                                                            ArduinoData[row][3]=ArduinoData[row][0];    
                                                            PRINTLN("set mean value");                                                     
                                                           }
      else if(ArduniDataFor[0]=='J' ){
                                        if(_Data[1]!=Last_get_Data[1] || _Data[2]!=Last_get_Data[2] ||_Data[3]!=Last_get_Data[3] ){ change=true; 
                                                                               if(_Data[1]=='0')  button=0;
                                                                          else if(_Data[1]=='1')  button=WheelButtonLeft;/// LEFTROTATE  1
                                                                          else if(_Data[1]=='2')  button=WheelButtonRight; /// LEFTROTATE  1
                                                                        }
                                         //         current value        Max                 Min
                                          gas=map(ArduinoData[0][0], ArduinoData[0][2], ArduinoData[0][4], -127, 127);
                                         Lgas=map(ArduinoData[0][1], ArduinoData[0][2], ArduinoData[0][4], -127, 127);
                                        
                                        Break=map(ArduinoData[1][0], ArduinoData[1][2], ArduinoData[1][4], 127, -127);
                                       LBreak=map(ArduinoData[1][1], ArduinoData[1][2], ArduinoData[1][4], 127, -127);
                                        
                                      if( (gas-Lgas)>10 || (gas-Lgas)<-10 || (Break-LBreak)>10 || (Break-LBreak)<-10)  change=true;                                          
                                             
                                     if(change){
                                            // S011[G420B058W360R432];Direction=0 Gas=110 Break=-47
                                       // char  databuffer[18];   
                                       // sprintf(databuffer,"S%c%02X%02X%c%c",_Data[1],gas,Break,_Data[6],_Data[7]);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//                                                 LHJ,LVJ,RHJ,RVJ,  
                                            // JostickAxis[]={0,0,0,0};  
                                               
                                           if(EnableAxisBotton) {
                                                                       if(gas<-70)   button+=GasButton;
                                                                  else if(Break<-70) button+=BreakBtton;
                                                                  Joystick(0,0,0,0,button,0);                               
                                                                 }
                                           else                 {
                                                                 memset(JostickAxis, 0, sizeof(JostickAxis)); 
                                                                 JostickAxis[GasAxis]=gas;   
                                                                 JostickAxis[BreakAxis]=Break;                                               
                                                                 Joystick(JostickAxis[0],JostickAxis[1],JostickAxis[2],JostickAxis[3],button,0);
                                                                 }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                       
                                          
                                          Serial1.print(_Data); Serial1.print(F(";Direction="));Serial1.print(_Data[1]);
                                          Serial1.print(F(" Gas="));Serial1.print(gas);  Serial1.print(F("("));Serial1.print(ArduinoData[0][2]);  Serial1.print(F(","));Serial1.print(ArduinoData[0][4]);Serial1.print(F(")"));
                                          Serial1.print(F(" Break="));Serial1.print(Break);Serial1.print(F("("));Serial1.print(ArduinoData[1][2]);  Serial1.print(F(","));Serial1.print(ArduinoData[1][4]);Serial1.print(F(")"));
                                           Serial1.println();
                                           WS.text(MainWSclinet,"ARD@"+_Data); 
                                          }
                                       }
         Last_get_Data=_Data;
      ArduinoData[0][1]  =ArduinoData[0][0];//Gas 
      ArduinoData[1][1]  =ArduinoData[1][0];//Break
      ArduinoData[2][1]  =ArduinoData[2][0];//Wheel
      ArduinoData[3][1]  =ArduinoData[3][0];//RotateX   
    }  
 

}

void Get_Data_from_Game_USB(){
 #ifdef EnableSwSerial
    inputSwString
    byte data[inputSwString.length()];
    inputSwString.getBytes(data, inputSwString.length());
    FfbOnUsbData( data, inputSwString.length())
  
 #endif

}

void Get_Data_from_Arduino()
{
  Wire.beginTransmission(SlaveDeviceId);
  Wire.write(Command2Arduino,sizeof(Command2Arduino)); 
  Wire.endTransmission();  // Moved up here by frippe75
  delay(5); 
  Wire.requestFrom(SlaveDeviceId, dataRecieveLength); //address, quantity  , sendStop=true  
 Arduino_get_Data="";
  while (Wire.available())
  {
    delay(1);
    char c = (char)Wire.read(); //  Serial1.write(c);
      Arduino_get_Data += c;   
  }
  Arduino_get_Data.trim();
  
   sendCommand2RN42(Arduino_get_Data);//S011[G420B058W360R432];Direction=0 Gas=110 Break=-47
   /*
  if (Arduino_get_Data.length()>4) {
                                     byte eqI1 = Arduino_get_Data.indexOf(']');
                                    if(eqI1<dataRecieveLength){ Arduino_get_Data = Arduino_get_Data.substring(0, eqI1);  sendCommand2RN42(Arduino_get_Data);}
                                    }*/

//  PRINTLN(Arduino_get_Data); 
}

void setup_ini(void){

 #ifdef EnableSwSerial
 SIM.begin(115200);
 #endif

 Wire.begin(SDA_pin,SCL_pin);
 delay(1000);
 
  for(byte i=0;i<5;i++)
  {
  if(sendCmdAndWaitForResp("AT\r\n","OK",2)) {Serial.println("PHONE is ready"); LINK=true; break;}
  //SIM_BUFF="";
  }
  
  sendCmdAndWaitForResp("AT+CMGF=1\r\n","OK",2);
  Serial.println("**>"); Serial.println(SIM_BUFF);  Serial.println("<**");//SIM_BUFF="";
 
  delay(500);
}

void app_loop(void){

 SerialEvent();
 #ifdef EnableSwSerial
 swSerialEvent()
 #endif
if(SENDCODE) {Serial.print(RN42);SENDCODE=false;}
if(HareReset){digitalWrite(0,LOW); delay(400); digitalWrite(0,HIGH);    HareReset=false;}
if((millis()-lastI2Crecieved)>(I2Cinterval) &&   ArduniDataFor[0]!='I' ){ Get_Data_from_Arduino(); lastI2Crecieved=millis(); }

if(SendCalibrationData) { WS.text(MainWSclinet,"RCA@"+RestorCalibration());  SendCalibrationData=false;}
if(SaveCalibrationDO) { SaveCalibration(CalibrationCode); WS.text(MainWSclinet,"CAS@"+String(CalibrationCode));  SaveCalibrationDO=false;}
if(SIM.available() > 0)  parse_sim(read_sim());//webSocket.sendTXT(client_num,read_sim().c_str()); 
}





#endif
