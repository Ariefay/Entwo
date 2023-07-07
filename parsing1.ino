void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  
  byte i = 65;
  Serial.write(i);
  delay(100);

  

}
