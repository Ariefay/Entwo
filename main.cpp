#include <Arduino.h>

const int led1 = 27;
const int led2 = 26;

const int onTime1 = 1000;
const int onTime2 = 2000;

const int offTime1 = 1000;
const int offTime2 = 2000;

bool led1Run = false;
bool led2Run = false;

String inputString;
bool StringComplete;

void ledCurrent();
void SerialEventHandler();
void commandEvent (String data);



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
    while (!Serial) {

    ; // wait for serial port to connect. Needed for native USB port only

  }
  Serial.println("Hello, ESP32!");
  pinMode(led1, OUTPUT); 
  pinMode(led2, OUTPUT); 

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);


}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10); // this speeds up the simulation

  // ledCurrent();
  SerialEventHandler();

}

void SerialEventHandler(){
// IF Pertama Untuk Menyalakan dan Mematika LED
  if(Serial.available() > 0) // if there is data comming
    { 
    while (Serial.available()){
      char inChar = (char)Serial.read();
      if ((inChar != '\n')&&(inChar != 'r')){
        inputString += inChar;
        Serial.print(inChar);
      }
      if(inChar == '\n'){
        StringComplete = true;
        Serial.println(inputString);
        commandEvent(inputString);
        inputString = "";

      
      }
    }
  }
}

void commandEvent (String data){
  //LED;1 LED;0
  String command;
  String order;
  int pinOrder;
  String val;
  int value;
  
  byte separator1 = data.indexOf(';');
  byte separator2 = data.indexOf('/');
  command = data.substring(0, separator1);
  order = data.substring(separator1+1);
  pinOrder = order.toInt();
  val = data.substring(separator2+1);
  value = val.toInt();

  Serial.print("Command = ");
  Serial.println(command);

  Serial.print("Pin Order Led = ");
  Serial.println(pinOrder);

  Serial.print("Value = ");
  Serial.println(value);

  
if(command == "ON")
    {    
      Serial.println("LED is turned ON"); // send action to Serial Monitor 
      if (pinOrder == 1)
      {
      led1Run = true;
      digitalWrite(led1, HIGH); // turn on LED
      analogWrite(led1, value);
      Serial.println("LED 1 is turned ON"); // send action to Serial Monitor 
      
      
      }
      else if (pinOrder == 2)
      {
        led2Run = true;
        digitalWrite(led2, HIGH); // turn on LED
        analogWrite(led2, value);
      Serial.println("LED 2 is turned ON"); // send action to Serial Monitor 
      }
      
      
    }
  else if(command == "OFF")
    {
      Serial.println("LED is turned OFF"); // send action to Serial Monitor  
    if (pinOrder == 1)
      {
      led1Run = false;
      digitalWrite(led1, LOW); // turn on LED
      analogWrite(led1, value);
      Serial.println("LED 1 is turned OFF"); // send action to Serial Monitor 
      }
      else if (pinOrder == 2)
      {
        led2Run = false;
        digitalWrite(led2, LOW); // turn on LED
        analogWrite(led2, value);
      Serial.println("LED 2 is turned OFF"); // send action to Serial Monitor 
      }

    } else {
      Serial.println("Command, Pin Order, and Value is Wrong");
    }




// IF Kedua Untuk Blink LED

  if(led1Run == true){
  unsigned long currentMillis = millis();
   if (currentMillis % (onTime1 + offTime1) < onTime1) {
        digitalWrite(led1, HIGH);
      } else {
        digitalWrite(led1, LOW);
      } 
  } else if(led1Run == false){
    digitalWrite(led1, LOW);
  }

  if(led2Run == true){
  unsigned long currentMillis = millis();
   if (currentMillis % (onTime2 + offTime2) < onTime2) {
        digitalWrite(led2, HIGH);
      } else {
        digitalWrite(led2, LOW);
      } 
  } else if(led2Run == false){
    digitalWrite(led2, LOW);
  }





}



