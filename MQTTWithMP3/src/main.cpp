#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// WiFi
const char *ssid = "DEV UJI_COBA";    // Enter your Wi-Fi name
const char *password = "ujicobadulu"; // Enter Wi-Fi password

// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *topic = "emqx/esp32";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;

// Declaration Object
EspSoftwareSerial::UART swSer;
// Declaration BaudRate
constexpr uint32_t TESTBPS = 9600;
// Declaration MP3 Module
DFRobotDFPlayerMini myDFPlayer;

// Declaration Parsing
String inputString;
bool StringComplete;

WiFiClient espClient;
PubSubClient client(espClient);
void commandEvent(String data);
void callback(char *topic, byte *payload, unsigned int length);

void setup()
{
  Serial.begin(TESTBPS);
  // Software serial with D18 as RX and D19 as TX
  swSer.begin(TESTBPS, EspSoftwareSerial::SWSERIAL_8N1, 18, 19);
  // Correction MP3 Module
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

  // Connecting to a WiFi network
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
  client.publish(topic, "Hi, I'm ESP32 ^^");
  client.subscribe(topic);
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  String messagePayload;
  for (int i = 0; i < length; i++)
  {
    messagePayload += (char)payload[i];
  }
  Serial.println(messagePayload);
  commandEvent(messagePayload);
  Serial.println("-----------------------");
}

void commandEvent(String data)
{

  // Declaration Function
  String command;
  String order;
  int trackNumber;
  int VolumeMP3 = 20;

  // Parsing Data
  byte separator = data.indexOf(';');
  command = data.substring(0, separator);
  order = data.substring(separator + 1);
  trackNumber = order.toInt();

  Serial.print("Task = ");
  Serial.println(command);

  Serial.print("Order Record Number = ");
  Serial.println(trackNumber);

  // Parsing Work
  if (command == "STOP")
  {
    Serial.println("MP3 STOP");
    myDFPlayer.reset();
  }
  else if (command == "PLAY")
  {
    Serial.println("MP3 PLAY");
    myDFPlayer.volume(VolumeMP3);
    myDFPlayer.play(trackNumber);
  }
  else
  {
    Serial.println("Task is Wrong");
    myDFPlayer.reset();
  }
}

void loop()
{
  client.loop();
}
