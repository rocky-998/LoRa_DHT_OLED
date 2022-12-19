#include <SPI.h>
#include <LoRa.h>
#include "DHT.h"
#include<String.h>


#define DHTPIN 4      // Selecting the pin at which we have connected DHT11
#define DHTTYPE DHT11 
 
 DHT dht ( DHTPIN, DHTTYPE );

//define the pins used by the transceiver module
#define ss 16
#define rst 14
#define dio0 26

int counter = 0;

void setup() {
  //initialize Serial Monitor

  dht.begin();
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Sender");
   Serial.println();
    Serial.println();
   Serial.println("Humidity and Temperature");
  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  
  //replace the LoRa.begin(---E-) argument with your location's frequency 
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(866E6)) {
    Serial.println(".");
    delay(500);
  }
   LoRa.setSpreadingFactor(10);
   // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);
float humidity = dht.readHumidity();
float temprature = dht.readTemperature();
 
Serial.print("current humidity=");
  Serial.print(humidity);
  Serial.print("%,");
  Serial.print("current temperature=");
  Serial.print(temprature);
  Serial.println("C");

 
  //Send LoRa packet to receiver
  LoRa.beginPacket();
  char s1[5];
  char s2[5];
  gcvt(humidity, 6, s1);
  gcvt(temprature, 6, s2);
  String s3 = s1;
 s3+="%   ";
 s3+=s2;
  
  LoRa.print(s3);
  Serial.print(s3);
  //  LoRa.print(humidity);
  LoRa.endPacket();

  counter++;

  delay(10000);
}
