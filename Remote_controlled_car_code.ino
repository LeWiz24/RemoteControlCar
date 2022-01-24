#define BLYNK_PRINT Serial        
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>  // include the BLYNK library

#define RightMotorSpeed 5    // define variable controling the speed of the right motor
#define RightMotorDir   0    // define variable controling the direction of the right motor 
#define LeftMotorSpeed  4    // define variable controling the speed of the left motor 
#define LeftMotorDir    2    // define variable controling the direction of the left motor

char auth[] = "x4KLZ0bQeXj6qWmZgbPkq4oLzNKYR6IW";   // character array, authentication code 
char ssid[] = "ATTNyTEBUI";   // wifi name
char pass[] = "k6y43et%z66h";  // wifi password

int minRange = 312;   // minimum value of x and y coordinates, we can set any value we want to set the deadzone
int maxRange = 712; // maximum value of x and y coordinates  

int maxSpeed = 1020; // maximum speed
int noSpeed = 0;     // stop speed 


void moveControl(int x, int y)  // main part to control the car moving. Here we add the if - else conditions for each specific region to control the car, such as move straight forward/backward, turn left/right
{
  // move straight forward
  if(y >= maxRange && x >= minRange && x <= maxRange) 
  {
    digitalWrite(RightMotorDir,HIGH);  // HIGH/LOW : digital state, HIGH/LOW: turns Clockwise/Counter-Clockwise 
    digitalWrite(LeftMotorDir,HIGH);
    analogWrite(RightMotorSpeed,maxSpeed);  // both left and right motors should have the same speed (either min or max speed) so that the car can move straight forward
    analogWrite(LeftMotorSpeed,maxSpeed);
  }
 
  // move forward right
  else if(x >= maxRange && y >= maxRange)
  {
    digitalWrite(RightMotorDir,HIGH);
    digitalWrite(LeftMotorDir,HIGH);
   analogWrite(RightMotorSpeed,maxSpeed);  // since we want the car turn right, the right motor needs to spin slower then the left motor. The motor was treating both speed values the same so we stop the motors on one side instead.
    analogWrite(LeftMotorSpeed,noSpeed);  // The motor was treating both speed values the same so we stop the motors on the left spide instead of just slowing it down.
  }

  // move forward left
  else if(x <= minRange && y >= maxRange)
  {
    digitalWrite(RightMotorDir,HIGH);
    digitalWrite(LeftMotorDir,HIGH);
    analogWrite(RightMotorSpeed,noSpeed);  // similar explanation as the car turn right. Here we want the car turn left, the right motor's speed need to be faster than that of the right one.
    analogWrite(LeftMotorSpeed,maxSpeed);
  }

  // neutral zone or the central zone
  else if(y < maxRange && y > minRange && x < maxRange && x > minRange)
  {
    analogWrite(RightMotorSpeed,noSpeed);  // No movement
    analogWrite(LeftMotorSpeed,noSpeed);
  }

 // move back
  else if(y <= minRange && x >= minRange && x <= maxRange)
  {
    digitalWrite(RightMotorDir,LOW);    // the motors are wired in such a was that LOW spins them such that the car moves backward
    digitalWrite(LeftMotorDir,LOW);
   analogWrite(RightMotorSpeed,maxSpeed);  // same speed for both motors
    analogWrite(LeftMotorSpeed,maxSpeed);
  }

  // move back and right
 else if(y <= minRange && x <= minRange)
  {
   digitalWrite(RightMotorDir,LOW);
    digitalWrite(LeftMotorDir,LOW);       // same as before
    analogWrite(RightMotorSpeed,noSpeed);
    analogWrite(LeftMotorSpeed,maxSpeed); 
  }

  // move back and left
  else if(y <= minRange && x >= maxRange)
  {
    digitalWrite(RightMotorDir,LOW);
    digitalWrite(LeftMotorDir,LOW);       //same as before
    analogWrite(RightMotorSpeed,maxSpeed);
    analogWrite(LeftMotorSpeed,noSpeed);
  }

// stay on spot, but turn left
  else if(y >= minRange && y <= maxRange && x <= minRange)
  {
    digitalWrite(RightMotorDir,LOW);      // Spinning both the left and right motors in opposite directions makes the car turn on the spot.
    digitalWrite(LeftMotorDir,HIGH);      
    analogWrite(RightMotorSpeed,maxSpeed);
    analogWrite(LeftMotorSpeed,maxSpeed);
  }

  // stay on spot, but turn right
  else if(y >= minRange && y <= maxRange && x >= maxRange)
  {
    digitalWrite(RightMotorDir,HIGH);     // same as before
    digitalWrite(LeftMotorDir,LOW);
    analogWrite(RightMotorSpeed,maxSpeed);
    analogWrite(LeftMotorSpeed,maxSpeed);
  }
}

void setup()      // initialize the code, the code only set up once in the beginning
{
  Serial.begin(9600);   //baud rate for serial communication
  Blynk.begin(auth, ssid, pass); // blynk command to set up iternet access for the MCU
 
  // initial settings for motors off and direction forward (so tha car doesnt run away as soon as getting switch on)
  pinMode(RightMotorSpeed, OUTPUT);    // declaration the PINs as outputs
  pinMode(LeftMotorSpeed, OUTPUT);
  pinMode(RightMotorDir, OUTPUT);
  pinMode(LeftMotorDir, OUTPUT);
 
  digitalWrite(RightMotorSpeed, LOW);   // LOW = 0 : not spinning.
  digitalWrite(LeftMotorSpeed, LOW);
  digitalWrite(RightMotorDir, HIGH);
  digitalWrite(LeftMotorDir,HIGH);

 
 }


void loop()    // once the setup runs, loop will keep running continuously
{
 
  Blynk.run(); 
}


BLYNK_WRITE(V1)
{
  int x = param[0].asInt(); //read the first element of the parameter array as an integer X
  int y = param[1].asInt(); //read the second element of the parameter array as an integer Y
 /*
  Serial.print("x value is: ");
  Serial.println(x);                //For Testing
  Serial.print("y value is: ");
  Serial.println(y);
 */ 
  moveControl(x,y); //run the move subroutine
}
