#include <Time.h>
#include <TimeLib.h>

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
//const char* MY_SSID = ""; 
const char* MY_SSID = ""; 
const char* MY_PWD = "";
const char* MY_EC2 = "ec2-52-43-66-45.us-west-2.compute.amazonaws.com";
const char*port_num = "3000";
int FLAG = 0;
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
   pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  connectWifi();
  sensors.begin();

  setTime(00,18,0,19,3,19); 

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
   if(!FLAG){
    
    if(client.connect(server,80)){
       Serial.println("WiFi Client connected ");
      String postStr = apiKey; 
      postStr +="&field1=";
      postStr+=String(temp)+"\n\n";
      String path = "/update?key=" + apiKey + "&field1=";  
      client.print(String("GET ")+path+String(temp)+" HTTP/1.1\r\n"+"Host: "+server+"\r\n"+"Connection: keep-alive\r\n\r\n");
      Serial.print(postStr);
    }
    
   }
   else{
    int y = year();
    int m = month(),d = day();
    char EC2_str[30],pat1[20],pat2[20];
    sprintf(pat1,"%4d%02d%02d,%02d:%02d",y,m,d,hour(),minute());
    sprintf(pat2,"%.2lf",temp);
    sprintf(EC2_str,"%4d%02d%02d,%02d:%02d,%.2lf",y,m,d,hour(),minute(),temp);
    Serial.println(String(pat1)+","+String(pat2));
    String path="";
    if(client.connect(MY_EC2,3000)){
       Serial.println("WiFi EC2 connected ");
       path = "/data?date="+String(pat1)+"&temp="+String(pat2)+" HTTP/1.1\r\n"+"Host: "+MY_EC2+"\r\n"+"Connection: keep-alive\r\n\r\n";
       Serial.print(path);
       client.print(String("GET ")+path);
    } 
    Serial.print(path);
   }
    FLAG = !FLAG;
  
 //client.stop();
 
 //delay(1000);
 delay(30000);
  //delay(60000); 
}
