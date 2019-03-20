#include "MPU9250.h"
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

// an MPU9250 object with the MPU-9250 sensor on I2C bus 0 with address 0x68
MPU9250 IMU(Wire, 0x68);
int status;

void setup() {
  // serial to display data
  Serial.begin(115200);
  delay(10);
  pinMode(buttonPin, INPUT);
  while (!Serial) {}

  // start communication with IMU
  status = IMU.begin();
  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while (1) {}
  }
  Serial.println();
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
    IMU.readSensor();
    float a = analogRead(A0);
    float c = tempsensor.readTempC();
    float d = analogRead(A0);
    // display the data
    float x = IMU.getAccelX_mss();
    float y = IMU.getAccelY_mss();
    float z = IMU.getAccelZ_mss();
    stringOne = "Acc," + String(micros()) + "," + "X," + String(x) + "," + "Y," + String(y) + "," + "Z," + String(z) + "," + "T," + String(c) + "," + "H," + String(d);
    stringOne.toCharArray(charBuf, 100);
    Udp.beginPacket(ipd, port);
    Udp.write(charBuf);
    Udp.endPacket();
    Serial.print(stringOne);
    Serial.println();
    delay(5);
  } else {
    Serial.print("Turn on the ACC to start Recording");
    Serial.println();
  }
}
