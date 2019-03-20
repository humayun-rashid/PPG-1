#include <Wire.h>
#include "MAX30105.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

MAX30105 particleSensor;

const int buttonPin = 14;
int buttonState = 0;

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

void setup()
{
  Serial.begin(115200);
  delay(10);
  pinMode(buttonPin, INPUT);
  Serial.println("MAX30105 Presence Sensing Example");

  // Initialize sensor
  if (particleSensor.begin(Wire, I2C_SPEED_FAST) == false) //Use default I2C port, 400kHz speed
  {
    Serial.println("PPG Sensor to colelct heart rate");
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

  //Setup to sense up to 18 inches, max LED brightness
  byte ledBrightness = 0xFF; //Options: 0=Off to 255=50mA
  byte sampleAverage = 1; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  int sampleRate = 1600; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 2048; //Options: 2048, 4096, 8192, 16384

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
  particleSensor.setPulseAmplitudeRed(0); //Turn off Red LED
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
}

void loop()
{
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    stringOne = "PPG-1," + String(micros()) + "," + String(particleSensor.getIR());
    stringOne.toCharArray(charBuf, 30);
    Udp.beginPacket(ipd, port);
    Udp.write(charBuf);
    Udp.endPacket();
    Serial.print(particleSensor.getIR());
    Serial.println();
  } else {
    Serial.print("Turn on the PPG to start Recording");
    Serial.println();
  }
}
