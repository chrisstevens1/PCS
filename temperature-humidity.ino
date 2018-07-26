#include <ESP8266WiFi.h>
#include "SparkFun_Si7021_Breakout_Library.h"
#include <Wire.h>
 // These above label the libraries in which the functions are derived
const char* ssid     = "<YourSSID>";
const char* password = "<YourPassword>";
const char* host = "<IPAddress>";
float humidity = 0;
float tempf = 0;
 // variables for temperature and humidity are created.
//Create Instance of SI7021 temp and humidity sensor
Weather sensor;
//-------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
void getWeather()
{
  // Measure Relative Humidity from the Si7021
  humidity = sensor.getRH();
  // Measure Temperature from the Si7021
  tempf = sensor.getTempF();
  // Temperature is measured every time RH is requested.
  // It is faster, therefore, to read it from previous RH
  
}
//---------------------------------------------------------------
void printInfo()
{
//This function prints the weather data out to the default Serial Port
  Serial.print("Temp:");
  Serial.print(tempf);
  Serial.print("F, ");
  Serial.print("Humidity:");
  Serial.print(humidity);
  Serial.println("%");
}
void setup() 
{
  Serial.begin(115200);
  delay(10);
 
   
    
  
    //Initializes the I2C sensors and ping them
    sensor.begin();
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
// indicates whether the device is connected to wifi or not.
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
int value = 0;
void loop() 
{
  delay(5000);
  Serial.print("connecting to ");
  Serial.println(host);
    //Get readings from all sensors
    getWeather();
    printInfo();
    delay(1000);
  // Use WiFiClient class to create Transmission control protocol connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
    
  }
  
  // We now create a URI for the request, so it knows where to send the information on our spcific network.
  String url = "/sending.php?temp=" + String (tempf) + "&humidity=" + String (humidity);
  
  
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