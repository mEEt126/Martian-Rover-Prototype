#include <Adafruit_BMP085.h>
#include <DHT22.h>
#include <HX711.h>
#include <cactus_io_AM2302.h>
#include <EEPROM.h>
#include "Wire.h"    // imports the wire library for talking over I/2C 
#include <Servo.h>

#define AM2302_PIN 4
#define DOUT  3
#define CLK  2
Servo camera1; // create servo object to control a CAMERA1
Servo camera2; // create servo object to control a CAMERA2
int pos_camera1 = 0;    // variable to store the camera1_position
int pos_camera2 = 0;    // variable to store the camera2_position

 
HX711 scale(DOUT, CLK);
float calibration_factor = -736504.18;
Adafruit_BMP085 mySensor;  // create sensor object called mySensor
int airquality = 0;
float tempC;  // Variable for holding temp in C
float tempF;  // Variable for holding temp in F
float pressure; //Variable for holding pressure reading
int address = 0;
byte valg;
byte valh;
byte vali;
byte valj;
byte valk;
byte valm;
byte valn;
int Sensor_pin ;


int sensorValue=0;
long int Output_data;
int inByte;
AM2302 dht(AM2302_PIN);

void transmit_sensor_data(float temp,float pressure,int humid,int moist,float load,int gas)
{
  String sensor;
  String seperator = ":";
  sensor = seperator + temp + seperator + pressure + seperator + humid + seperator + moist + seperator + load + seperator + gas + seperator; 
  Serial.println(sensor);
}



long int transferData()
  {  
     valg= EEPROM.read(0x31);
     Serial.print('g');
    // Serial.print(valg);

      valh= EEPROM.read(0x41);
     Serial.print('h');
     //Serial.print(valh);
     
      vali= EEPROM.read(0x51);
     Serial.print('i');
    // Serial.print(vali);

      valj= EEPROM.read(0x61);
     Serial.print('j');
     //Serial.print(valj);

      valk= EEPROM.read(0x71);
     Serial.print('k');
     //Serial.print(valk);

      valm= EEPROM.read(0x81);
     Serial.print('m');
     //Serial.print(valm);

     transmit_sensor_data(valg,valh,vali,valj,valk,valm);
  }
void setup()
{
  UCSR0B&=~(1<<TXEN0);
  Serial.begin(38400);
  dht.begin();
  mySensor.begin();   //initialize mySensor
  int sensorValue = 0;
  //Serial.println("Press T to tare");
  scale.set_scale(-736504.18);  //Calibration Factor obtained from first sketch
  scale.tare();
  camera1.attach(5);  // attaches the servo on pin 4 to the camera1
  camera2.attach(6);  // attaches the servo on pin 5 to the camera2
}

float temp_pressure()
{
  tempC = mySensor.readTemperature(); //  Read Temperature
  pressure =mySensor.readPressure(); //Read Pressure
  /*return (tempC);*/
  EEPROM.write(0x31,tempC);
  EEPROM.write(0x41,pressure);
  //Serial.println(tempC);
 //Serial.println(pressure);
  transferData();
}
int soil_moisture()
{
 float M = analogRead(Sensor_pin);
 
  M = map(M,550,10,0,100);
  EEPROM.write(0x51,M);
  //Serial.println(M);
  transferData();
 
}

int humidity_sens(){
  dht.readHumidity();
  // Check if any reads failed and exit early (to try again).
  if (isnan(dht.humidity)) {
    
  Serial.println("AM2302 sensor read failure!");
  return;
  }
  
  float H = dht.humidity; 
  EEPROM.write(0x61,H);
  //return (H);
  transferData();
  
}
void load_sens()
{
  Serial.println("Press T to tare");
 int  W = (scale.get_units(), 3);
   if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == 't' || temp == 'T')
      scale.tare();  //Reset the scale to zero      
  }
  EEPROM.write(0x71,scale.get_units());
  //Serial.print(scale.get_units(), 3);
  transferData();
}
int gas_sens()
{
  
 int sensorValue = analogRead(A1);
 EEPROM.write(0x81,sensorValue);
// Serial.println(transferData());
transferData();
}

void loop()
{

  if(Serial.available()>0)
  {
    int inByte = Serial.read();
  
  switch (inByte)
  {
    case '0':
    UCSR0B|=(1<<TXEN0);
    temp_pressure();
    break;
    
    case '1':
    UCSR0B|=(1<<TXEN0);
    temp_pressure();
    
    case '2':
    UCSR0B|=(1<<TXEN0);
    soil_moisture();
    break;
    
    case '3':
    UCSR0B|=(1<<TXEN0);
    humidity_sens();
    break;
    
    case '4':
    UCSR0B|=(1<<TXEN0);
    load_sens();
    break;
    
    case '5':
    UCSR0B|=(1<<TXEN0);
    gas_sens();
    break;
    
    case '6':
    UCSR0B|=(1<<TXEN0);
     Serial.println('6');
      for (pos_camera1 = 0; pos_camera1 <= 200; pos_camera1 += 1)
      { 
        camera1.write(pos_camera1);              
        delay(30);                                
      }
      Serial.println(pos_camera1);
      break;

     case '7':
     UCSR0B|=(1<<TXEN0);
      Serial.println('7');
      for (pos_camera1 = 200; pos_camera1 >= 0; pos_camera1 -= 1)
      { 
       camera1.write(pos_camera1);              
        delay(30);                       
      }
      Serial.println(pos_camera1);
      break;

     case '8':
     UCSR0B|=(1<<TXEN0);
       Serial.println('8');
      for (pos_camera2 = 0; pos_camera2 <= 200; pos_camera2 += 1)
      { 
        camera2.write(pos_camera2);              
        delay(30);                      
      }
      break;

     case '9':
     UCSR0B|=(1<<TXEN0);
      Serial.println('9'); 
    for (pos_camera2 = 200; pos_camera2 >= 0; pos_camera2 -= 1)
      { 
        camera2.write(pos_camera2);              
        delay(30);                       
      }
     break;

    default:
    UCSR0B&=~(1<<TXEN0);
    break ;
   }
  }
  EEPROM.write(address, inByte);
  address = address + 1;
  if (address == EEPROM.length())
  {
    address = 0;
  }
  
}
