#include <Arduino.h>

// Deklarasi Led
// Led Aktif "LOW"
const int led1 = 33;
const int led2 = 25;
// Led Aktif "HIGH"
const int led3 = 26;
const int led4 = 27;

// Deklarasi Interval Waktu
const int onTime1 = 1000;
const int onTime2 = 2000;
const int onTime3 = 3000;
const int onTime4 = 4000;
const int offTime1 = 1000;
const int offTime2 = 2000;
const int offTime3 = 3000;
const int offTime4 = 4000;

// Deklarasi Status
bool led1Run = false;
bool led2Run = false;
bool led3Run = false;
bool led4Run = false;

// Deklarasi Parsing
String inputString;
bool StringComplete;

// Deklarasi Fungsi
void ledCurrent();
void SerialEventHandler();
void commandEvent(String data);

void setup()
{
  // Inisialisasi Serial Monitor
  Serial.begin(9600);
  while (!Serial)
  {

    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Hello, ESP32!");

  // Inisialisasi Mode Led
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);

  // Inisialisasi Status Led
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  delay(5000);
}

void loop()
{
  // put your main code here, to run repeatedly:
  delay(10); // this speeds up the simulation

  // Pemanggilan Fungsi
  ledCurrent();
  SerialEventHandler();
}

void SerialEventHandler()
{
  // Pengecekan apakah ada data masuk atau inputan masuk
  if (Serial.available() > 0) // if there is data comming
  {

    while (Serial.available())
    {
      // Pembacaan hasil inputan
      char inChar = (char)Serial.read();
      if ((inChar != '\n') && (inChar != 'r'))
      {
        // Penampungan hasil inputan
        inputString += inChar;
        Serial.print(inChar);
      }
      if (inChar == '\n')
      {

        StringComplete = true;
        Serial.println(inputString);
        // Pemanggilan Fungsi
        commandEvent(inputString);
        inputString = "";
      }
    }
  }
}

void commandEvent(String data)
{

  // FUNGSI PARSING
  // Deklarasi Variable
  String command;
  String order;
  int pinOrder;
  String val;
  int value;

  // Pemisah data
  byte separator1 = data.indexOf(';');
  byte separator2 = data.indexOf('/');
  // Melakukan parsing
  command = data.substring(0, separator1);
  order = data.substring(separator1 + 1);
  pinOrder = order.toInt();
  val = data.substring(separator2 + 1);
  value = val.toInt();

  // Menunjukan hasil parsing
  Serial.print("Command = ");
  Serial.println(command);

  Serial.print("Pin Order Led = ");
  Serial.println(pinOrder);

  Serial.print("Value = ");
  Serial.println(value);

  // Pengerjaan perintah dari parsing
  //  IF STATUS ON
  if (command == "ON")
  {
    Serial.println("LED is turned ON"); // send action to Serial Monitor
    if (pinOrder == 1)
    {
      led1Run = true;
      digitalWrite(led1, LOW); // turn on LED
      // analogWrite(led1, value);
      Serial.println("LED 1 is turned ON"); // send action to Serial Monitor
    }
    else if (pinOrder == 2)
    {
      led2Run = true;
      digitalWrite(led2, LOW); // turn on LED
      // analogWrite(led2, value);
      Serial.println("LED 2 is turned ON"); // send action to Serial Monitor
    }
    else if (pinOrder == 3)
    {
      led3Run = true;
      digitalWrite(led3, HIGH);
      // analogWrite(led3, value);
      Serial.println("LED 3 is turned ON");
    }
    else if (pinOrder == 4)
    {
      led4Run = true;
      digitalWrite(led4, HIGH);
      // analogWrite(led4, value);
      Serial.println("LED 4 is turned ON");
    }
  }

  // IF Status OFF
  else if (command == "OFF")
  {
    Serial.println("LED is turned OFF"); // send action to Serial Monitor
    if (pinOrder == 1)
    {
      led1Run = false;
      digitalWrite(led1, HIGH); // turn on LED
      // analogWrite(led1, value);
      Serial.println("LED 1 is turned OFF"); // send action to Serial Monitor
    }
    else if (pinOrder == 2)
    {
      led2Run = false;
      digitalWrite(led2, HIGH); // turn on LED
      // analogWrite(led2, value);
      Serial.println("LED 2 is turned OFF"); // send action to Serial Monitor
    }
    else if (pinOrder == 3)
    {
      led3Run = false;
      digitalWrite(led3, LOW); // turn on LED
      // analogWrite(led3, value);
      Serial.println("LED 3 is turned OFF"); // send action to Serial Monitor
    }
    else if (pinOrder == 4)
    {
      led4Run = false;
      digitalWrite(led4, LOW); // turn on LED
      // analogWrite(led4, value);
      Serial.println("LED 4 is turned OFF"); // send action to Serial Monitor
    }
  }
  else
  {
    Serial.println("Command, Pin Order, and Value is Wrong");
  }
}

void ledCurrent()
{
  // FUNGSI LED MEMILIKI DELAY

  // LED 1
  if (led1Run == true)
  {
    unsigned long currentMillis = millis();
    if (currentMillis % (onTime1 + offTime1) < onTime1)
    {
      digitalWrite(led1, LOW);
    }
    else
    {
      digitalWrite(led1, HIGH);
    }
  }
  else if (led1Run == false)
  {
    digitalWrite(led1, HIGH);
  }

  // LED 2
  if (led2Run == true)
  {
    unsigned long currentMillis = millis();
    if (currentMillis % (onTime2 + offTime2) < onTime2)
    {
      digitalWrite(led2, LOW);
    }
    else
    {
      digitalWrite(led2, HIGH);
    }
  }
  else if (led2Run == false)
  {
    digitalWrite(led2, HIGH);
  }

  // LED 3
  if (led3Run == true)
  {
    unsigned long currentMillis = millis();
    if (currentMillis % (onTime3 + offTime3) < onTime3)
    {
      digitalWrite(led3, HIGH);
    }
    else
    {
      digitalWrite(led3, LOW);
    }
  }
  else if (led3Run == false)
  {
    digitalWrite(led3, LOW);
  }

  // LED 4
  if (led4Run == true)
  {
    unsigned long currentMillis = millis();
    if (currentMillis % (onTime4 + offTime4) < onTime4)
    {
      digitalWrite(led4, HIGH);
    }
    else
    {
      digitalWrite(led4, LOW);
    }
  }
  else if (led4Run == false)
  {
    digitalWrite(led4, LOW);
  }
}