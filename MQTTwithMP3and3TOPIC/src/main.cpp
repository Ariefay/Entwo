// Include Library
#include <WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <Arduino.h>

// Declaration Pin Led and Relay
const int ledPin = 4; // D4 pin Green Led
const int Relay = 27; // D27 pin Red Led and Relay

// Declaration Status Led
bool ledState = true;

// Interval Blink Led
const int onTime1 = 2000; // Interval 2 second
const int offTime1 = 2000;

// WiFi
const char *ssid = "NOCOLA_DEV_ATAS";   // Enter your Wi-Fi name
const char *password = "NOCOLADEV2021"; // Enter Wi-Fi password

// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *topic1 = "emqx/stop";
const char *topic2 = "emqx/play";
const char *topic3 = "emqx/blink";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;

// Declaration Object
EspSoftwareSerial::UART swSer;
// Declaration BaudRate
constexpr uint32_t TESTBPS = 9600;
// Declaration MP3 Module
DFRobotDFPlayerMini myDFPlayer;

// Declaration Function
WiFiClient espClient;
PubSubClient client(espClient);
void DFPlayerCommand(String data);
void StopCommand(String);
void BlinkCommand(String data);
void callback(char *topic, byte *payload, unsigned int length);

void setup()
{
  // Setup BaudRate
  Serial.begin(TESTBPS);
  // ESP Software serial with D18 as RX and D19 as TX
  swSer.begin(TESTBPS, EspSoftwareSerial::SWSERIAL_8N1, 18, 19);

  // SETUP DFPLAYER
  //  Correction MP3 Module
  if (!myDFPlayer.begin(swSer, false))
  {
    // MP3 Module No Detection
    Serial.println(F("Not initialized:"));
    Serial.println(F("1. Check the DFPlayer Mini connections"));
    Serial.println(F("2. Insert an SD card"));

    // MP3 Module Detection
  }
  else
    Serial.println(F("DFPlayer Mini online."));

  // SETUP WIFI AND MQTT
  //  Connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the Wi-Fi network");

  // connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);

  while (!client.connected())
  {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password))
    {
      Serial.println("Public EMQX MQTT broker connected");
    }
    else
    {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }

  // Publish and subscribe
  client.publish(topic1, "Hi, I'm ESP32. Function to Break the MP3");
  client.subscribe(topic1);
  client.publish(topic2, "Hi, I'm ESP32. Function to Play the MP3");
  client.subscribe(topic2);
  client.publish(topic3, "Hi, I'm ESP32. Function to ON or OFF Led and Relay");
  client.subscribe(topic3);

  // SETUP LED AND RELAY
  pinMode(ledPin, OUTPUT);
  pinMode(Relay, OUTPUT);
  digitalWrite(ledPin, HIGH);
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  String messagePayload;
  for (int i = 0; i < length; i++)
  {
    // input char payload to string messagePayload
    messagePayload += (char)payload[i];
  }
  Serial.println(messagePayload);
  // compare include topic with topic1
  if (strcmp(topic, topic1) == 0)
  {
    // messagePayload from topic1, call StopCommand Function
    StopCommand(messagePayload);
  }
  // compare include topic with topic2
  if (strcmp(topic, topic2) == 0)
  {
    // messagePayload from topic2, call DFPlayerCommand Function
    DFPlayerCommand(messagePayload);
  }
  // compare include topic with topic3
  if (strcmp(topic, topic3) == 0)
  {
    // messagePayload from topic3, call BlinkCommand Function
    BlinkCommand(messagePayload);
  }
}

void StopCommand(String)
{
  myDFPlayer.reset();
  Serial.println("MP3 STOP");
}

void DFPlayerCommand(String data)
{

  // Declaration Function
  String order;
  int trackNumber;
  int VolumeMP3 = 20;

  // Parsing Data and Change String to Integer
  order = data.substring(0);
  trackNumber = order.toInt();

  Serial.print("Track Number = ");
  Serial.println(trackNumber);

  // Serial.println("MP3 PLAY");
  myDFPlayer.volume(VolumeMP3);
  myDFPlayer.play(trackNumber);
  Serial.println("-----------------------");
}

void BlinkCommand(String data)
{
  // Declaration Function
  String dictate;
  String part;

  // Parsing Data
  byte separator2 = data.indexOf(';');
  dictate = data.substring(0, separator2);
  part = data.substring(separator2 + 1);

  Serial.print("Task = ");
  Serial.println(dictate);
  Serial.print("Item = ");
  Serial.println(part);

  // Parsing Work
  if (dictate == "ON")
  {
    if (part == "LED")
    {
      ledState = false;
      Serial.println("LED IS ON");
      digitalWrite(ledPin, LOW);
      Serial.println("-----------------------");
    }
    else if (part == "RELAY")
    {
      Serial.println("RELAY IS ON");
      digitalWrite(Relay, HIGH);
      Serial.println("-----------------------");
    }
    else
    {
      Serial.println("TASK IS WRONG, TRY AGAIN");
      Serial.println("-----------------------");
    }
  }
  else if (dictate == "OFF")
  {
    if (part == "LED")
    {
      ledState = true;
      Serial.println("LED IS OFF");
      digitalWrite(ledPin, HIGH);
      Serial.println("-----------------------");
    }
    else if (part == "RELAY")
    {
      Serial.println("RELAY IS OFF");
      digitalWrite(Relay, LOW);
      Serial.println("-----------------------");
    }
    else
    {
      Serial.println("TASK IS WRONG, TRY AGAIN");
      Serial.println("-----------------------");
    }
  }
  else
  {
    Serial.println("TASK IS WRONG, TRY AGAIN");
    Serial.println("-----------------------");
  }
}

void ledCurrent()
{

  if (ledState == false)
  {
    unsigned long currentMillis = millis();
    if (currentMillis % (onTime1 + offTime1) < onTime1)
    {
      digitalWrite(ledPin, LOW);
    }
    else
    {
      digitalWrite(ledPin, HIGH);
    }
  }
  else if (ledState == true)
  {
    digitalWrite(ledPin, HIGH);
  }
}

void loop()
{
  // Call Client to Connented with Wifi
  client.loop();
  // Call Function to Blink Led
  ledCurrent();
}