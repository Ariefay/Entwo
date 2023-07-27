#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

//Declaration MP3 Module
HardwareSerial Serial_df(2);
DFRobotDFPlayerMini myDFPlayer;

// #if defined (ESP32)
// #define RX (18)     //GPIO18 to TX
// #define TX (19)     //GPIO19 to RX
//#define rxPin 18
//#define txPin 19

//Declaration Parsing
String inputString;
bool StringComplete;

//Declaration Function
void SerialEventHandler();
void commandEvent(String data);


// Set up a new SoftwareSerial object
SoftwareSerial mod(18, 19);
//SoftwareSerial mySerial (rxPin, txPin);
 

void setup() {
//Inisialisasi Serial Monitor
    Serial_df.begin(9600);
    Serial.begin(115200);
    
    // pinMode(rxPin, INPUT);
    // pinMode(txPin, OUTPUT);
    // mySerial.begin(115200);
//Correction MP3 Module 
    if (!myDFPlayer.begin(Serial_df, false))
  {
//MP3 Module No Detection
    Serial.println(F("Not initialized:"));
    Serial.println(F("1. Check the DFPlayer Mini connections"));
    Serial.println(F("2. Insert an SD card"));
    //für esp8266
    //while(true);
//MP3 Module Detection
  }else Serial.println(F("DFPlayer Mini online."));
  
  //  myDFPlayer.volume(100);
  //  myDFPlayer.play();

}
 
void loop() {

//Function Called
 SerialEventHandler();


  
}
//Detection Data Input
void SerialEventHandler(){
//
//Collection Data Input
 if(Serial.available ()> 0){
    while (Serial.available()){
      char inChar = (char)Serial.read();
      if((inChar != '\n') && (inChar != 'r')){
        inputString += inChar;
        Serial.print(inChar);
      }
      if (inChar =='\n'){
        StringComplete = true;
        Serial.println(inputString);
        commandEvent(inputString);
        inputString ="";
      }
    }
  }
}

void commandEvent(String data){

//Declaration Function
  String command;
  String order;
  int pinOrder;

//Parsing Data
  byte separator = data.indexOf(';');
  command = data.substring(0, separator);
  order = data.substring(separator + 1);
  pinOrder = order.toInt();


  Serial.print("Task =");
  Serial.println(command);

  Serial.print("Order Record Number =");
  Serial.println(pinOrder);

//Parsing Work
  if(command == "STOP"){
    Serial.println("MP3 STOP");
    myDFPlayer.reset();
  }
  else if (command == "PLAY"){
    Serial.println("MP3 PLAY");
    if (pinOrder == 1){
    myDFPlayer.volume(20); 
    myDFPlayer.play(1);
    }
    else if (pinOrder == 2){
    myDFPlayer.volume(20);
    myDFPlayer.play(2);
    }
  }
  else {
  Serial.println("Task is Wrong");
  myDFPlayer.reset();
  }
}