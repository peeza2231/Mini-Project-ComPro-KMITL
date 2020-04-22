#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
 
#define RightMotorSpeed 4
#define RightMotorDir   0  
#define LeftMotorSpeed  5  
#define LeftMotorDir    2
 
char auth[] = "blynktoken";
char ssid[] = "No one";
char pass[] = "linsing607";

int minRange = 312;
int maxRange = 712;

int minSpeed = 450;
int maxSpeed = 1023;
int noSpeed = 0;
 
 
void moveControl(int x, int y)
{

  if(y >= maxRange && x >= minRange && x <= maxRange) //zataci R
  {
    digitalWrite(RightMotorDir,HIGH);  
    digitalWrite(LeftMotorDir,HIGH);
    analogWrite(RightMotorSpeed,maxSpeed); 
    analogWrite(LeftMotorSpeed,maxSpeed);
  }
  
  // move forward right
  else if(x >= maxRange && y >= maxRange)   //zataci R
  {
    digitalWrite(RightMotorDir,HIGH);
    digitalWrite(LeftMotorDir,HIGH);
   analogWrite(RightMotorSpeed,minSpeed); 
    analogWrite(LeftMotorSpeed,maxSpeed);
  }
 
  // move forward left
  else if(x <= minRange && y >= maxRange)
  {
    digitalWrite(RightMotorDir,HIGH);
    digitalWrite(LeftMotorDir,HIGH);
    analogWrite(RightMotorSpeed,maxSpeed); 
    analogWrite(LeftMotorSpeed,minSpeed);
  }
 
  // neutral zone
  else if(y < maxRange && y > minRange && x < maxRange && x > minRange)
  {
    analogWrite(RightMotorSpeed,noSpeed); 
    analogWrite(LeftMotorSpeed,noSpeed);
  }
 
 // move back
  else if(y <= minRange && x >= minRange && x <= maxRange)
  {
    digitalWrite(RightMotorDir,LOW);
    digitalWrite(LeftMotorDir,LOW);
   analogWrite(RightMotorSpeed,maxSpeed); 
    analogWrite(LeftMotorSpeed,maxSpeed);
  }
 
  // move back and right
 else if(y <= minRange && x <= minRange)
  {
   digitalWrite(RightMotorDir,LOW);
    digitalWrite(LeftMotorDir,LOW);
    analogWrite(RightMotorSpeed,minSpeed); 
    analogWrite(LeftMotorSpeed,maxSpeed);  
  }
 
  // move back and left
  else if(y <= minRange && x >= maxRange)
  {
    digitalWrite(RightMotorDir,LOW);
    digitalWrite(LeftMotorDir,LOW);
    analogWrite(RightMotorSpeed,maxSpeed); 
    analogWrite(LeftMotorSpeed,minSpeed);
  }
}
 
void setup()
{
  // initial settings for motors off and direction forward
  pinMode(RightMotorSpeed, OUTPUT);
  pinMode(LeftMotorSpeed, OUTPUT);
  pinMode(RightMotorDir, OUTPUT);
  pinMode(LeftMotorDir, OUTPUT);
  digitalWrite(RightMotorSpeed, LOW);
  digitalWrite(LeftMotorSpeed, LOW);
  digitalWrite(RightMotorDir, HIGH);
  digitalWrite(LeftMotorDir,HIGH);
 
  Blynk.begin(auth, ssid, pass);
 }
 
 
void loop()
{
  Blynk.run();
}
 
 
BLYNK_WRITE(V1)
{
  int x = param[0].asInt();
  int y = param[1].asInt();
  moveControl(x,y); 
}
