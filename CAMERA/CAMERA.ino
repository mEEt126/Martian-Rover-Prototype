//-----------------------------------------------------------------------------------CAMERA-----------------------------------------------------------------// 

#include <Servo.h>

Servo camera1; // create servo object to control a CAMERA1
Servo camera2; // create servo object to control a CAMERA2

int pos_camera1 = 0;    // variable to store the camera1_position
int pos_camera2 = 0;    // variable to store the camera2_position
char camera;

void setup() 
{
  Serial.begin(38400);
  camera1.attach(5);  // attaches the servo on pin 4 to the camera1
  camera2.attach(6);  // attaches the servo on pin 5 to the camera2
//  pinMode(buttonPin, OUTPUT);
}

void loop() 
{
  
 if(Serial.available()>0)  
{
  camera = Serial.read();
  Serial.println(camera);
  if(camera == '6')
  {
      Serial.println('a');
      for (pos_camera1 = 0; pos_camera1 <= 200; pos_camera1 += 1)
      { 
        camera1.write(pos_camera1);              
        delay(30);                                
      }
      Serial.println(pos_camera1);
  } 
  else if(camera == '7')  
  {
     Serial.println('b');
      for (pos_camera1 = 200; pos_camera1 >= 0; pos_camera1 -= 1)
      { 
       camera1.write(pos_camera1);              
        delay(30);                       
      }
      Serial.println(pos_camera1);
  } 
  else if(camera == '8') 
  {
     Serial.println('c');
      for (pos_camera2 = 0; pos_camera2 <= 200; pos_camera2 += 1)
      { 
        camera2.write(pos_camera2);              
        delay(25);                      
      }
  }
  else if(camera == '9')
  {
   Serial.println('d'); 
    for (pos_camera2 = 200; pos_camera2 >= 0; pos_camera2 -= 1)
      { 
        camera2.write(pos_camera2);              
        delay(25);                       
      }
  } 
  
} 
}

