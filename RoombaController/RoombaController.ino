#include <SoftwareSerial.h>
#include "roombaDefines.h"

//Roomba Create2 connection
int rxPinRoomba = 10;
int txPinRoomba = 11;
SoftwareSerial Roomba(rxPinRoomba,txPinRoomba);

char junk;
String inputString="";

void startSafe() {
  Roomba.write(128); //start
  Roomba.write(131); //SAFE mode
}
void goForward() {
  Roomba.write(137);                  //Drive
  Roomba.write((byte)0x00);         //0x00c8 == 200 velocitye high byte
  Roomba.write(0xc8);             //Velocity low byte
  Roomba.write(0x80);           //Radius high byte
  Roomba.write((byte)0x00);   //Radius low byte
}
void goBackward() {
  Roomba.write(137);                  //Drive
  Roomba.write(0xff);               //0xff38 == -200 velocitye high byte
  Roomba.write(0x38);             //Velocity low byte
  Roomba.write(0x80);           //Radius high byte
  Roomba.write((byte)0x00);   //Radius low byte
}
void halt() {
  byte j = 0x00;
  Roomba.write(137);
  Roomba.write(j);
  Roomba.write(j);
  Roomba.write(j);
  Roomba.write(j);   
}
void driveWheels(int right, int left) {
clamp(right, -500, 500);
clamp(left, -500, 500);
Roomba.write(145);
Roomba.write(right >> 8);
Roomba.write(right);
Roomba.write(left >> 8);
Roomba.write(left);
}

void setup()                    // run once, when the sketch starts
{
 Serial.begin(9600); // set the baud rate to 9600, same should be of your Serial Monitor
 Roomba.begin(19200);
 pinMode(ddPin, OUTPUT);
}

void loop() {

  //READOUT incomming bluetooth message, then do task
  if(Serial.available()){
  while(Serial.available())
    {
      char inChar = (char)Serial.read(); //read the input
      inputString += inChar;        //make a string of the characters coming on serial
    }
    Serial.println(inputString);
    while (Serial.available() > 0)  
    { junk = Serial.read() ; }      // clear the serial buffer

    
    if(inputString == "R"){ //RECHTS
      driveWheels(-150,150); //range  -500/500
        
    } else if(inputString == "A"){  //ACHTERUIT
      goBackward();
      
    } else if(inputString =="V"){ // VOORUIT
      goForward();
      
    } else if(inputString == "L"){ //LINKS
      driveWheels(150,-150); //range  -500/500
      
    } else if(inputString == "0"){  //turn OFF 
      halt();
      
    } else if(inputString == "1"){ //turn ON
      startSafe();
    }
    
    inputString = "";
  }
}
