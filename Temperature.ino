#include <BearSSLHelpers.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureAxTLS.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiServerSecureAxTLS.h>
#include <WiFiServerSecureBearSSL.h>
#include <WiFiUdp.h>

#include <DallasTemperature.h>
#include <OneWire.h>

#define ONE_WIRE_BUS 2
#define LED_BUILTIN 2
OneWire oneWire(ONE_WIRE_BUS);
const char* server = "api.thingspeak.com";
String apiKey ="";
// the setup function runs once when you press reset or power the board
DallasTemperature sensors(&oneWire);
const char* MY_SSID = ""; 
const char* MY_PWD = "";
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
   pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  connectWifi();
  sensors.begin();
}
void connectWifi()
{
  Serial.print("Connecting to "+*MY_SSID);
  WiFi.begin(MY_SSID, MY_PWD);
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("Connected");
  Serial.println("");  
}//end connect
// the loop function runs over and over again forever
void loop() {
  float temp;
    sensors.requestTemperatures();
  temp = sensors.getTempCByIndex(0);
  
  WiFiClient client;   
  if(client.connect(server,80)){
       Serial.println("WiFi Client connected ");
      String postStr = apiKey; 
      postStr +="&field1=";
      postStr+=String(temp)+"\n\n";
      String path = "/update?key=" + apiKey + "&field1=";  
    client.print(String("GET ")+path+String(temp)+" HTTP/1.1\r\n"+"Host: "+server+"\r\n"+"Connection: keep-alive\r\n\r\n");
  /*
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
*/
   Serial.print(postStr);
  }
 //client.stop();
 //delay(1000);
  delay(60000); 
}
