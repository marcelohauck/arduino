#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"

#define RXD2 16 // Esp32 transmissão 
#define TXD2 17 // Esp32 recepção
#define a1 4  // solenoide 1
#define a2 5  // solenoide 2
#define a3 6  // bomba d'água
#define RE 7  // sensor de solo (leitura)
#define DE 8  // sensor de solo (escrita)
#define b1 A0 // aquecedor 
#define b2 A1 // Umidificador
#define b3 A2 // Ventilação auxiliar
#define b4 A3 // exaustor auxiliar
#define DHTPIN 9 // Sensor temperatura e umidade
#define DHTTYPE DHT22 // define template biblioteca do sensor DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {

  Serial.begin(9600);
  Serial.println("mendigrow 3.0");
  dht.begin();
  pinMode(a1, OUTPUT);
  pinMode(a2, OUTPUT);
  pinMode(a3, OUTPUT);
  pinMode(b1, OUTPUT);
  pinMode(b2, OUTPUT);
  pinMode(b3, OUTPUT);
  pinMode(b4, OUTPUT);
  digitalWrite(a1, HIGH);
  digitalWrite(a2, HIGH);
  digitalWrite(a3, HIGH);
  digitalWrite(b1, HIGH);
  digitalWrite(b2, HIGH);
  digitalWrite(b3, HIGH);
  digitalWrite(b4, HIGH);
}

void loop() {

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  if (dht.readTemperature() >= 29.0) {
    digitalWrite(b1, HIGH);
    digitalWrite(b3, LOW);
    char nomeEvento[] = "temperatura alta";
    char nomeAtividade[] = "aquecedor off | cooler 2 on";
    message();
    Serial.print("evento:");
    Serial.println(nomeEvento);
    Serial.print("atividade:");
    Serial.println(nomeAtividade);

  } if (dht.readTemperature() <= 20.0) {
    digitalWrite(b1, LOW);
    digitalWrite(b3, HIGH);
    char nomeEvento[] = "temperatura baixa";
    char nomeAtividade[] = "aquecedor on | cooler 2 off";
    message();
    Serial.print("evento:");
    Serial.println(nomeEvento);
    Serial.print("atividade:");
    Serial.println(nomeAtividade);

  } if (dht.readHumidity() >= 60.0) {
    digitalWrite(b2, HIGH);
    digitalWrite(b4, LOW);
    char nomeEvento[] = "umidade alta";
    char nomeAtividade[] = "umidificador off | exaustor 2 on ";
    message();
    Serial.print("evento:");
    Serial.println(nomeEvento);
    Serial.print("atividade:");
    Serial.println(nomeAtividade);

  } if (dht.readHumidity() <= 42.0) {
    digitalWrite(b2, LOW);
    digitalWrite(b4, HIGH);
    char nomeEvento[] = "umidade baixa";
    char nomeAtividade[] = "umidificador on | exaustor 2 off";
    message();
    Serial.print("evento:");
    Serial.println(nomeEvento);
    Serial.print("atividade:");
    Serial.println(nomeAtividade);

    /* } else if (analogRead(pinoAnalog) <= 400) {
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
    */
  }else{

  delay (60000);
}
void message() {

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
  //        Serial.print("umidSolo:");
  //        Serial.println(Percentual);
  //        Serial.print("sensorSolo:");
  //        Serial.println(valor_analogico);

}
