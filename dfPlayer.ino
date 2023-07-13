//#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

HardwareSerial Serial_df(2);
DFRobotDFPlayerMini myDFPlayer;
 
void setup() {
    Serial_df.begin(9600);
    Serial.begin(115200);

    if (!myDFPlayer.begin(Serial_df, false))
  {
    Serial.println(F("Not initialized:"));
    Serial.println(F("1. Check the DFPlayer Mini connections"));
    Serial.println(F("2. Insert an SD card"));
    //für esp8266
    //while(true);
  }else Serial.println(F("DFPlayer Mini online."));
  
   myDFPlayer.volume(15);
   myDFPlayer.play();
}
 
void loop() {

  
}