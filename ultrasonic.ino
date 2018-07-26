#define TOPHATSENSORPIN A0 //Ambient light sensor reading 
#include <ESP8266WiFi.h>
#include "SparkFun_Si7021_Breakout_Library.h"
#include <Wire.h>
#include <String>
// libraries needed are decalred above, so that the code knows where to get specific functions.
const char* ssid     = "<YourSSID>";
const char* password = "<YourPassword>";
// wifi connection initialized
const char* host = "<IPAddress>";
float distance = 0;
float LED_PIN=2;
void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(TOPHATSENSORPIN,  INPUT);   
  Serial.begin(9600);
      
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address:");
  Serial.println(WiFi.localIP());
}
 
void loop() {
  int obstruction;
  distance = analogRead(TOPHATSENSORPIN); //Read light level
  Serial.println(distance);           
  if (distance < 100) {  // if there is an obstruction within 100 mm the light turns on
    obstruction = 1;
    digitalWrite(LED_PIN, HIGH);
  }
  else {
    digitalWrite(LED_PIN, LOW);
    obstruction = 0;
  }
  // turn the LED on (HIGH is the voltage level)
  //Display reading in serial monitor
  delay(1000);
  {

  Serial.print("connecting to ");
  Serial.println(host);
    //Get readings from all sensors
  // Use WiFiClient class to create Transmission control protocol connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;}
    // establishes URL as the recipient for the code for our specific network
    String url = "/tophatsend.php?obstruction=" + String(obstruction);
  
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis(); // disconnects from server temporarily in order to create a loop of information
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
}

int value = 0;

  }
  
  Serial.println();
  Serial.println("closing connection"); // closes connection to the server
}
 