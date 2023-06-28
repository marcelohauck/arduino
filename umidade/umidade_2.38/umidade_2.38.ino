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
  Serial.println("mendigrow 2.38");
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
              char nomeEvento[] = "temperatura alta";
              char nomeAtividade[] = "cooler auxiliar ativo";
              message();           
             Serial.print("evento:");   
             Serial.println(nomeEvento);
             Serial.print("atividade:");    
             Serial.println(nomeAtividade);
             
  } else if (dht.readTemperature() <= 22.0) {
              digitalWrite(a1, HIGH);
              char nomeEvento[] = "temperatura baixa";
              char nomeAtividade[] = "cooler auxiliar desativado";
              message();
             Serial.print("evento:");   
             Serial.println(nomeEvento);
             Serial.print("atividade:");    
             Serial.println(nomeAtividade);
              
  } if (dht.readHumidity() >= 50.0) {
              digitalWrite(a3, LOW);
              digitalWrite(a2, HIGH);
              char nomeEvento[] = "umidade alta";
              char nomeAtividade[] = "exaustor on umidificador off";              
              message();
             Serial.print("evento:");   
             Serial.println(nomeEvento);
             Serial.print("atividade:");    
             Serial.println(nomeAtividade);
                               
  } else if (dht.readHumidity() <= 42.0) {
              digitalWrite(a3, HIGH);
              digitalWrite(a2, LOW);
              char nomeEvento[] = "umidade baixa";
              char nomeAtividade[] = "exaustor off umidificador on";
              message();
             Serial.print("evento:");   
             Serial.println(nomeEvento);
             Serial.print("atividade:");    
             Serial.println(nomeAtividade);
             
  } if (analogRead(pinoAnalog) <= 400) {
             digitalWrite(b1, HIGH);
             digitalWrite(b2, HIGH);                
             char nomeEvento[] = "solo molhado";
             char nomeAtividade[] = "bomba e solenoide desligadas";
             message();
             Serial.print("evento:");   
             Serial.println(nomeEvento);
             Serial.print("atividade:");    
             Serial.println(nomeAtividade);
                           
  } else if (analogRead(pinoAnalog) >= 600) {
             char nomeEvento[] = "solo seco";
             char nomeAtividade[] = "bomba e solenoide ativas"; 
             message();
             Serial.print("evento:");   
             Serial.println(nomeEvento);
             Serial.print("atividade:");    
             Serial.println(nomeAtividade);
             
       while (analogRead(pinoAnalog) >= 350) {
                   digitalWrite(b1, LOW);
                   digitalWrite(b2, LOW);                             
  }
 } else {
void message();

}

delay (60000);
}
void message(){

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
  
              Serial.print("tempAr:");
              Serial.println(t);
              Serial.print("umidAr:");
              Serial.println(h);
              Serial.print("umidSolo:");
              Serial.println(Percentual);
              Serial.print("sensorSolo:");
              Serial.println(valor_analogico);  
                            
} 
