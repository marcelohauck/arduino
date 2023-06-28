#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"

#define RXD2 16
#define TXD2 17
#define pinoAnalog A0 // sensor de solo 1
#define a1 2 // Ventilação auxiliar
#define a2 3 // Umidificador
#define a3 4 // exaustor auxiliar
#define b1 6 // bomba d'água
#define b2 7 // solenoide 1
#define b3 8 // solenoide 2
#define DHTPIN 9
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

int ValAnalogIn; // Introduz o valor analógico ao código
int valor_analogico = analogRead(pinoAnalog);
int analogSoloSeco = 800;
int analogSoloMolhado = 200;
int percSoloSeco = 0;
int percSoloMolhado = 100;

void setup() {

  Serial.begin(9600);
  Serial.println("mendigrow 2.37");
  dht.begin();
  pinMode(a1, OUTPUT);
  pinMode(a2, OUTPUT);
  pinMode(a3, OUTPUT);
  pinMode(b1, OUTPUT);
  pinMode(b2, OUTPUT);
  digitalWrite(a2, HIGH);
  digitalWrite(a3, HIGH);
  digitalWrite(b1, HIGH);
  digitalWrite(b2, HIGH);
  digitalWrite(b3, HIGH);
}

void loop() {

delay(10000);

  ValAnalogIn = analogRead(pinoAnalog); // Relaciona o valor analógico com o recebido do sensor
  int Percentual = map(ValAnalogIn, 800, 0, 0, 200); // Relaciona o valor analógico à porcentagem
  ValAnalogIn = constrain(analogRead(pinoAnalog), analogSoloMolhado, analogSoloSeco);
  Percentual = map(ValAnalogIn, analogSoloMolhado, analogSoloSeco, percSoloMolhado, percSoloSeco);

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  if (dht.readTemperature() >= 27.0) {
              digitalWrite(a1, LOW);
              Serial.print("temperatura alta: ");
              Serial.print(t);
              Serial.println(F(" c"));              

  } else if (dht.readTemperature() <= 22.0) {
              digitalWrite(a1, HIGH);
              Serial.print("temperatura baixa: ");
              Serial.print(t);
              Serial.println(F(" c"));  
              
  } if (dht.readHumidity() >= 50.0) {
              digitalWrite(a3, LOW);
              digitalWrite(a2, HIGH);
              Serial.print("umidade alta: ");
              Serial.print(h);
              Serial.println("%");
                  
  } else if (dht.readHumidity() <= 42.0) {
              digitalWrite(a3, HIGH);
              digitalWrite(a2, LOW);
              Serial.print("umidade baixa: ");
              Serial.print(h);
              Serial.println("%");
              
/*  } if (analogRead(pinoAnalog) >= 600) {

             Serial.print("Solo seco: ");
             Serial.print(Percentual);
             Serial.println("%");
                 
       while (analogRead(pinoAnalog) >= 350) {
                   digitalWrite(b1, LOW);
                   digitalWrite(b2, LOW);                             
  }
  */
 } else {
              Serial.print("temperatura ok: ");
              Serial.print(t);
              Serial.println(F(" c"));          
              Serial.print("umidade ok: ");
              Serial.print(h);
              Serial.println("%");  
              Serial.print("Solo ok: ");
              Serial.print(Percentual);
              Serial.println("%"); 
 }

 
delay (60000);
}
