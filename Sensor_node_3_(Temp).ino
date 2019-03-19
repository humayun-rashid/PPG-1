#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
const int buttonPin = 14;
#include "Adafruit_MCP9808.h"
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();

const char* ssid     = "ITtest";
const char* password = "testITtest";
const char* ipd = "192.168.137.1";
const int port = 33333;

WiFiUDP Udp;
unsigned long tim = micros();
unsigned long tic = micros();
int packageCount = 0;
char charBuf[755];
String stringOne = "";
int j = 10000;

int buttonState = 0;

void setup() {
  // serial to display data
  Serial.begin(115200);
  delay(10);
  pinMode(buttonPin, INPUT);
  while (!Serial) {}


  if (!tempsensor.begin())
  {
    Serial.println("Couldn't find MCP9808!");
    while (1);
  }
  Serial.println();

  Serial.print("Connecting to ");
  Serial.println(ssid);

  //WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(2000);
}

void loop() {
  // read the sensor
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    float c = tempsensor.readTempC();
    stringOne = "Temp," + String(micros()) + "," + "T," + String(c);
    stringOne.toCharArray(charBuf, 100);
    Udp.beginPacket(ipd, port);
    Udp.write(charBuf);
    Udp.endPacket();
    Serial.print(stringOne);
    Serial.println();
    delay(5);
  } else {
    Serial.print("Turn on the Temparature Sensor to start Recording");
    Serial.println();
  }
}
