#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include<LiquidCrystal_I2C.h>
#include "EmonLib.h"

LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3,POSITIVE);

String LoRaMessage ="";
int counter = 0;
EnergyMonitor emon1;
int V = 220;
int pin_sct = A0;
double I;

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print(" Tugas Akhir 2");
  lcd.setCursor(0,1);
  lcd.print("   Node_listrik");
  delay(3000);
  lcd.clear();
  emon1.current(pin_sct,0.66);
  while (!Serial);

  Serial.println("LoRa Sender");

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  LoRa.setTxPower(17);    //Set Power ---- 0-17dbm
  LoRa.setSignalBandwidth(125); // set bandwith/
  LoRa.setSpreadingFactor(12);
  LoRa.setCodingRate4(8);
  }
}

void loop() {
  I = (emon1.calcIrms(1480)*1.8);
  Serial.print("Arus (A) : ");
  Serial.print(I,1);
  Serial.print(" Daya (W): ");
  Serial.print((I*V),1);
  Serial.println("");
  lcd.setCursor(0,0);
  lcd.print("Arus (A): ");
  lcd.print(I,1);
  lcd.setCursor(0,1);
  lcd.print("Daya (W): ");
  lcd.print((I*V),1);
  Serial.print("Sending packet: ");
  Serial.println(counter);

  Data_1();

  counter ++;
  delay (10000);
}

void Data_1 () {
  LoRaMessage = String(I,1)+"/"+String((I*V),1);
  LoRa.beginPacket();
  LoRa.print(LoRaMessage);
  LoRa.endPacket();
}
