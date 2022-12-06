# RoombaControllerDOC
 Documentatie iRobot Roomba 600 serie

Tom Koevoets
Avans Hogeschool, Breda
Communication & Multimedia Design

Patricia Nagtzaam
Avans Hogeschool, Breda
Communication & Multimedia Design

In opdracht van Mark Meeuwenoord en Lectoraat Robotisering en Sensoring Avans Hogeschool

iRobot Roomba 600 documentation (open interface)
https://www.irobotweb.com/~/media/MainSite/PDFs/About/STEM/Create/iRobot_Roomba_600_Open_Interface_Spec.pdf

Controlling a Roomba Robot with Arduino and Android
https://www.instructables.com/Controlling-a-Roomba-Robot-With-Arduino-and-Androi/

RoombaDefines.h geraadpleegd van:
https://github.com/simoneluconi/Roomba_ESP8266_Ctrl/tree/master/Roomba_ESP8266_Ctrl

<img width="778" alt="roomba_connection01" src="https://user-images.githubusercontent.com/45975088/205992098-a21abd0e-028f-4ba6-a18a-cae4391084fe.png">

**In het kort**
In het komende document geven we uitleg over hoe wij een iRobot Roomba serie 600 (698) hebben aangestuurd met C++ via Arduino en de 7-pin connector op de Roomba zelf.

De basics
Om een Roomba via je telefoon te kunnen besturen moet je hem kunnen verbinden met de plaatselijke wifi. Bij ons op Avans is dat aardig lastig, maar via het medialab konden we het MAC-adres van de Roomba in het systeem zetten: (ziet er zo ongeveer uit; 00:E9:3A:7A:0C:C9) en hebben we hem op AvansWlan aangesloten. Hierdoor is deze iRobot Roomba te gebruiken overal op Avans, je hoeft alleen maar te verbinden via het wifi netwerk. Je hoeft hiervoor alleen maar een account te maken en de Roomba hieraan toe te voegen, je kan hem zelfs een naam geven.

Als je de Roomba hebt verbonden met je mobiele telefoon kan je hem normaal besturen en programma’s laten draaien vanuit de app. Wij hadden het probleem dat de Roomba niet opstartte als hij niet verbonden was met de Wi-fi. Als de Wi-fi wel meewerkt heb je een normaal werkende Roomba. 

De controles en besturing worden duidelijk uitgelegd in de open interface die iRobot zelf publiceert van de Roomba (vooraan in het bestand gelinkt)

Arduino verbinden met de Roomba
Je kan een Arduino heel simpel verbinden met de Roomba, er zit een female 7-pin connector onder de bovenste kap van de Roomba. Haal je deze kap eraf kan je makkelijk bij de connector.

![pin-connect](https://user-images.githubusercontent.com/45975088/205993383-3e462c48-713d-4bf5-8e05-428909aba4ba.png)


Met deze afbeeldingen kan je een Arduino makkelijk verbinden met de Roomba. Hieronder zie je hoe de female 7-pin connector werkt. Je gebruikt 1 voor Power. LET OP dit is geen 5V maar unregulated, GEBRUIK hiervoor de vin pin op je Arduino. Anders doet je Arduino het niet lang meer. Pin 3 en 4 zijn voor Tx en Rx, als je data wilt uploaden naar je Arduino zal je deze uit de Roomba 7-pin connector moeten halen anders zal je een foutmelding krijgen.
Pin 5 is voor de Baud Rate, het is ook heel belangrijk dat als je een nieuwe code wilt uploaden naar de Arduino dat je de baudrate van de Roomba veranderd naar 19200 (de baudrate staat normaal op 115200), dit doe je door de clean-button op de Roomba 10 a 12 sec in te drukken als de Roomba uit staat tot je een aflopend deuntje hoort, doe je dit niet dan snapt de Roomba de code niet. Als laatste heb je Pin 6, dit is GND en deze kan je simpel verbinden met de GND.

<img width="630" alt="pinUitleg" src="https://user-images.githubusercontent.com/45975088/205993519-c26acdfa-909a-418a-a01a-9d9f3732b657.png">


Arduino code 
Bij het schrijven van de code hebben we veel gebruik gemaakt van de open source documentation van iRobot zelf en een vergelijkbaar document waarmee een Roomba aangestuurd kan worden via Arduino, Bluetooth en Android.

De code waarmee de Roomba werkt is heel simpel, het bestaat namelijk uit een lijst van commands die je kan sturen naar de Roomba, elk van deze commands is uitgebreid uitgelegd in de documentatie. Hiermee kan je zelf de complexiteit van de code bedenken.

Ook heb je een aantal #include nodig in je bestand; namelijk <SoftwareSerial.h> en “roombaDefines.h” die gelinkt is boven aan het bestand. Hiervoor is dit stukje code nodig, hiermee vertel je de Roomba wat de Rx en Tx pin zijn en hoe er gecommuniceerd kan worden met de Roomba.

<img width="269" alt="Bt_uitleg1" src="https://user-images.githubusercontent.com/45975088/205997259-6c387139-a9bb-4bc6-8511-36d2cf7e3e05.png">

Wij gebruiken de simpelste variant van deze code; opstarten, vooruit rijden, achteruit rijden, draaien en stoppen. Het makkelijkst is om hiervoor void functies te schrijven, die je daarna kan opvragen in de setup of loop. 

Om de Roomba te laten starten gebruiken we Safe start, hiervoor stuur je eerst de Roomba start(128) en vervolgens safe Mode (131), als je full control(132) wilt over de Roomba stopt hij niet meer als de Roomba van een trap afrijdt of word opgetild. 

<img width="368" alt="startsafe()" src="https://user-images.githubusercontent.com/45975088/205993827-b09006a8-a4ea-4ba7-9bff-82a9366b8ce8.png">

Nu de Roomba is opgestart kan je de volgende functies starten, wij gebruiken naar voren rijden, achteruit rijden en halt. Ze komen allemaal uit de code drive(137) van Roomba. Het nadeel is dat ze alleen maar hexadecimal lezen. Voor de Velocity kan dit maximaal 255, dus 200 is goed zodat je de Roomba langer kan laten rijden. Via een online convertor konden we de waardes vertalen naar Hex, de volgorde ervan is lastig in te vullen, je splitst het nummer op over high en low velocity byte achter de 0x. 

Als je de code(137) plus 4 data doorstuurt kan je de Roomba vooruit of achteruit laten rijden, halt is daar de makkelijke variant van “0x00” staat gelijk aan “0”, hierdoor stoppen alle rij functies en stopt de Roomba.

<img width="454" alt="bt_uitleg3" src="https://user-images.githubusercontent.com/45975088/205997504-e6b957dd-753e-4773-b988-975b1dae9c2c.png">

Nu moet het zijn gelukt om de Roomba te Safe starten, vooruit en achteruit laten rijden. 

Om de Roomba te kunnen laten draaien op zijn plaats gebruiken we drive direct(145), er zijn meerder manieren mogelijk om de Roomba te laten draaien, deze staan allemaal duidelijk vermeld in de iRobot Documentatie.

<img width="270" alt="bt_uitleg4" src="https://user-images.githubusercontent.com/45975088/205997735-5125e4fe-1261-4d3a-a739-5f098ade0fe1.png">

Met driveWheels() kun je bepalen welke kan de Roomba op moet draaien, dit kan in een range van -500 tot 500. Dit kun je aansturen door de 2 waardes te veranderen;

<img width="262" alt="bt_uitleg5" src="https://user-images.githubusercontent.com/45975088/205997815-39bb8a83-81c5-48df-82b2-66bae6df30be.png">

Door hier een bepaalde delay aan te geven kan je precies bepalen hoe ver de Roomba mag draaien voordat hij weer vooruit of achteruit rijdt. 

Connect Arduino met HC-05 Bluetooth module
Een uitleg over hoe je de Roomba kan aansturen via een Android. Dit hebben we samen gedaan met een HC-05 bluetooth en een oude Samsung. 

Om te beginnen had ik op de Android een applicatie geïnstalleerd “Arduino bluetooth controller” waarmee ik over bluetooth tekens kon sturen naar mijn Arduino. Deze tekens; A, V, L, R, 0 en 1 had ik gekoppeld aan functies. Zodat we de Roomba op afstand met Bluetooth konden besturen. 

Om de HC-05 te verbinden kun je dit
schema hieronder volgen:

![schema_btController_Tom](https://user-images.githubusercontent.com/45975088/205997982-33e098ef-09b5-423f-835d-e961c1d20583.png)

Als je de HC-05 hebt geïnstalleerd kan je deze uitlezen met een string + char. 

<img width="208" alt="Bt_uitleg" src="https://user-images.githubusercontent.com/45975088/205998097-39cdcb0e-ee69-4090-997b-8270e174323d.png">

Met deze 2 functies werkt de Bluetooth module, “inputString” is de data die word verzonden vanuit de Android. Door functies te koppelen aan deze “tekens” is het mogelijk om de Roomba te besturen via een simpele contoller.

<img width="454" alt="bt_uitleg6" src="https://user-images.githubusercontent.com/45975088/205998328-85244e4f-01cc-4243-bc11-45c67687027d.png">

Het enige wat je dan nodig hebt zijn een heel aantal what-ifs, dat zodra er een A, V, L, R, 0 of 1 wordt verstuurd dat de Roomba weet wat hij moet doen.

<img width="271" alt="bt_uitleg7" src="https://user-images.githubusercontent.com/45975088/205998369-b3dbbb6a-f163-4b76-979c-0d9316201b87.png">

De uiteindelijke code zorgt er dan voor dat de Roomba op afstand bestuurd kan worden met een Android via Arduino en een HC-05 bluetooth module. 





Code C++ 	Controller Arduino & Android

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







