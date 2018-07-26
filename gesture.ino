#include <SparkFun_APDS9960.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#define APDS9960_INT    2 // Needs to be an interrupt pin
SparkFun_APDS9960 apds = SparkFun_APDS9960();
//connects to WiFi and data base
const char* ssid     = "<YourSSID>";
const char* password = "<YourPassword>";
int isr_flag = 0;
const char* host = "<IPAddress>";
//declaring varible for gesture
String gesture = "left";
void setup() {
  Serial.begin(115200);
 
  Serial.println();
  Serial.println(F("--------------------------------"));
  Serial.println(F("SparkFun APDS-9960 - GestureTest"));
  Serial.println(F("--------------------------------"));

  if ( apds.init() ) {
    Serial.println(F("APDS-9960 initialization complete")); }
  else {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
  }
  
//communicates to serial monitor that the gesture sensor is running 
  if ( apds.enableGestureSensor(true) ) {
    Serial.println(F("Gesture sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during gesture sensor init!"));
  }
  
delay(10);
// printing out that connection is being checked 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  // loop for continuous WiFi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
int value = 0;
//begins loop for continuous WiFi connection and data from the sensor
void loop() {
  delay(1000);
  handleGesture();
  Serial.print("connecting to ");
  Serial.println(host);
  
  
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
//gets file from termainal then displays on webpage of url below
  String url = "/gest.php?gesture=" + (gesture);
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  Serial.println();
  Serial.println("closing connection");
}
void interruptRoutine() {
  isr_flag = 1;
}
// declares what will be printed on the serial monitor for each individual gesture
void handleGesture() {
    if ( apds.isGestureAvailable() ) {
    switch ( apds.readGesture() ) {
      case DIR_UP:
        gesture = "UP";
        Serial.println("UP");
        break;
      case DIR_DOWN:
        gesture = "DOWN";
        Serial.println("DOWN");
        break;
      case DIR_LEFT:
        gesture = "LEFT";
        Serial.println("LEFT");
        break;
      case DIR_RIGHT:
        gesture = "RIGHT";
        Serial.println("RIGHT");
        break;
      case DIR_NEAR:
        gesture = "NEAR";
        Serial.println("NEAR");
        break;
      case DIR_FAR:
        gesture = "FAR";
        Serial.println("FAR");
        break;
      default:
        gesture = "NONE";
        Serial.println("NONE");
    }
  }
}