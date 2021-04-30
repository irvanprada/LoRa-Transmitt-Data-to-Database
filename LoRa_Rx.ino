#include <SPI.h>
#include <LoRa.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>

#define ss 15
#define rst 16
#define dio0 2

String LoRaMessage;
String Arus;
String Daya;

const char* ssid = "Popo"; 
const char* password = "123098aab";
char server[] = "192.168.43.236"; 

WiFiClient client; 

void setup() {
  Serial.begin(9600);
  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  Serial.println("Server started");
  Serial.print(WiFi.localIP());
  delay(1000);
  Serial.println("connecting...");
  
  while (!Serial);

  Serial.println("LoRa Receiver");
  LoRa.setPins(ss, rst, dio0);

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
      Data();
      Sending_To_phpmyadmindatabase();
  }
}

void Sending_To_phpmyadmindatabase()   //CONNECTING WITH MYSQL
 {
   if (client.connect(server, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    Serial.print("GET /Sct013.php?Daya=");
    client.print("GET /Sct013.php?Daya=");     //YOUR URL
    Serial.println(Daya);
    client.print(Daya);
    client.print("&Arus=");
    Serial.println("&Arus=");
    Serial.println(Arus);
    client.print(Arus);
    client.print(" ");      //SPACE BEFORE HTTP/1.1
    client.print("HTTP/1.1");
    client.println();
    client.println("Host: Your Local IP");
    client.println("Connection: close");
    client.println();
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  delay(10000);
 }

 void Data () {
    while (LoRa.available()) {
     String LoRaData = LoRa.readString();
     //Serial.print(LoRaData);

     int pos1 = LoRaData.indexOf('/');
     Arus = LoRaData.substring(0, pos1);
     Daya = LoRaData.substring(pos1 +1);

    }
    //Serial.println(Arus);
    //Serial.print(Daya);
}
