#include <SparkFun_MMA8452Q.h>
#include <ESP8266WiFi.h>
#include <Wire.h> // Must include Wire library for I2C

MMA8452Q accel;
// declaring the variables for the motor attached to the sensor and the value that the motor is producing
int motor = 4;
int vibrating = 1;
// set up for a limit on the vibraton
int count = 0;
int maxnum = 10;
String previous_orientation = "Left";
// connecting sensor to WiFi
const char* ssid     = "<YourSSID>";
const char* password = "<YourPassword>";
const char* host = "<IPAddress>";
String orientation = "right";

void setup() {
  Serial.begin(9600);
  Serial.println("MMA8452Q Test Code!");
 
  accel.init();
  delay(10);
  
// sets the pin on the ESP8266 Thing for the motor to output
  pinMode (motor, OUTPUT);
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
// printing out the x, y, and z planes from the sensors orientation in space
  void printAccels()
{
  Serial.print(accel.x, 3);
  Serial.print("\t");
  Serial.print(accel.y, 3);
  Serial.print("\t");
  Serial.print(accel.z, 3);
  Serial.print("\t");
}
void printCalculatedAccels()
{ 
  Serial.print(accel.cx, 3);
  Serial.print("\t");
  Serial.print(accel.cy, 3);
  Serial.print("\t");
  Serial.print(accel.cz, 3);
  Serial.print("\t");
}
// prints the orientation (such as "Landscape Left" or "Portrait Up") of the sensor
 void printOrientation()
 
{
  // declares what will be printed on the serial monitor and what actions will be communicated to the motor for each individual orientation
  byte pl = accel.readPL();
  switch (pl)
  {
  case PORTRAIT_U:
   orientation = "Portrait_Up";
   digitalWrite (motor, HIGH);
   Serial.print("Portrait Up");
   vibrating = 1;
    break;
  case PORTRAIT_D:
      orientation = "Portrait_Down";
      digitalWrite (motor, HIGH);
      Serial.print("Portrait Down");
      vibrating = 1;
    break;
  case LANDSCAPE_R:
      orientation = "Landscape_Left";
      digitalWrite (motor, HIGH);
      Serial.print("Landscape Left");
      vibrating = 1;
    break;
  case LANDSCAPE_L:
      orientation = "Landscape_Right";
      digitalWrite (motor, HIGH);
      Serial.print("Landscape Right");
      vibrating = 1;
    break;
  case LOCKOUT:
      orientation = "Flat";
      digitalWrite (motor, LOW);
      Serial.print("Flat");
      vibrating = 0;
    break;
  }
}
int value = 0;
// begins loop
void loop() {
 if (accel.available()){
    accel.read();
    printCalculatedAccels();
    printOrientation();}
 // makes sure when orientation is changed that the sensor keeps checked the orientation and restarts the timer
 if (previous_orientation != orientation) {
        previous_orientation = orientation;
        count = 0; }
    
 // if the vibration is longer than 10 seconds it will be turned off
 if (count > maxnum) {
  digitalWrite(motor, LOW);}
  count++;
    Serial.println(); 
  delay(1000);
  Serial.print("connecting to ");
  Serial.println(host);
  
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;}
//gets file from termainal then displays on webpage of url below
  String url = "/tilt.php?orientation=" + (orientation) + "&vibrating=" + String(vibrating);
  
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