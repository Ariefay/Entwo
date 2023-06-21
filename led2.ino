const int led1 = 33;
const int led2 = 25;
const int led3 = 26;
const int led4 = 27;

const int onTime1 = 1000;
const int onTime2 = 2000;
const int onTime3 = 3000;
const int onTime4 = 5000;

const int offTime1 = 1000;
const int offTime2 = 2000;
const int offTime3 = 3000;
const int offTime4 = 5000;

bool led1Run = false;
bool led2Run = false;
bool led3Run = false;
bool led4Run = false;

int parameter = 0;
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
  pinMode(led1, OUTPUT); 
  pinMode(led2, OUTPUT); 
  pinMode(led3, OUTPUT); 
  pinMode(led4, OUTPUT); 

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);

  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(led1, ledChannel);

}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10); // this speeds up the simulation

  ledCurrent();



}


void ledCurrent(){



// IF Pertama Untuk Menyalakan dan Mematika LED
  if(Serial.available()) // if there is data comming
    {
    String command = Serial.readStringUntil(';'); // read string until meet newline character
    Serial.print("LED Command : ");
    Serial.println(command);
    
  if(command == "ON1")
    {    
      led1Run = true;
      digitalWrite(led1, HIGH); // turn on LED
      Serial.println("LED 1 is turned ON"); // send action to Serial Monitor 
    }
  else if(command == "OFF1")
    {
      led1Run = false;
      digitalWrite(led1, LOW);  // turn off LED
      Serial.println("LED 1 is turned OFF"); // send action to Serial Monitor  
    }
  else if(command == "ON2")
    {    
      led2Run = true;
      digitalWrite(led2, HIGH); // turn on LED
      Serial.println("LED 2 is turned ON"); // send action to Serial Monitor 
    }
  else if(command == "OFF2")
    {
      led2Run = false;
      digitalWrite(led2, LOW);  // turn off LED
      Serial.println("LED 2 is turned OFF"); // send action to Serial Monitor  
    }
  else if(command == "ON3")
    {    
      led3Run = true;
      digitalWrite(led3, HIGH); // turn on LED
      Serial.println("LED 3 is turned ON"); // send action to Serial Monitor 
    }
  else if(command == "OFF3")
    {
      led3Run = false;
      digitalWrite(led3, LOW);  // turn off LED
      Serial.println("LED 3 is turned OFF"); // send action to Serial Monitor  
    }
   else if(command == "ON4")
     {    
       led4Run = true;
       digitalWrite(led4, HIGH); // turn on LED
       Serial.println("LED 4 is turned ON"); // send action to Serial Monitor 
     }
   else if(command == "OFF4")
     {
       led4Run = false;
       digitalWrite(led4, LOW);  // turn off LED
       Serial.println("LED 4 is turned OFF"); // send action to Serial Monitor  
     }

        else {
      Serial.println("INVALID COMMAND");
      }

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

  if(led3Run == true){
  unsigned long currentMillis = millis();
   if (currentMillis % (onTime3 + offTime3) < onTime3) {
        digitalWrite(led3, HIGH);
      } else {
        digitalWrite(led3, LOW);
      } 
  } else if(led3Run == false){
    digitalWrite(led3, LOW);
  }
  
   if(led4Run == true){
   unsigned long currentMillis = millis();
    if (currentMillis % (onTime4 + offTime4) < onTime4) {
         digitalWrite(led4, HIGH);
       } else {
         digitalWrite(led4, LOW);
       } 
   } else if(led4Run == false){
     digitalWrite(led4, LOW);
   }


// Brightness LED
  if(Serial.available()>0){
   String brightness = Serial.readString();
   
   if(brightness.startsWith("SET1/")){
      int parameter = brightness.substring(5).toInt(); 
      parameter = constrain (parameter, 0, 255);
//      analogWrite(led1, parameter);
      ledcWrite(ledChannel,parameter);
      Serial.print("NOW Set:");
      Serial.println(parameter);
    } else {
      Serial.print("INVALID BRIGHTNESS");
      }

  }




  
}
