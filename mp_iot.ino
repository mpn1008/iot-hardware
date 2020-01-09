#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "SocketIOClient.h"
#include "Ethernet.h"
#include "SPI.h"


SocketIOClient client;
const char* ssid     = "Greenwich-Guest";
const char* password =  "greenwichvn@123";

char host[] = "10.23.252.116";
int port = 3000;

StaticJsonDocument <200> doc;

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);


void setup(void) 
{
#ifndef ESP8266
  while (!Serial); 
#endif
 Serial.begin(9600);
  Serial.println("Accelerometer Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
   Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while(1);
  }

  /* Set the range to whatever is appropriate for your project */
  accel.setRange(ADXL345_RANGE_16_G);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500);
   Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

// Then connect to Socket server

  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    return;
  }
 if (client.connect(host,port))
  {
    Serial.println("Success");
  }
}

void loop(void) 
{
  /* Get a new sensor event */ 
  sensors_event_t event; 
  accel.getEvent(&event);
  String jsonString;
  doc["accel"] = event.acceleration.y ;
  serializeJson(doc, jsonString);
  client.emit("sensordata",jsonString);
  delay(500);
  client.monitor();
}
