#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"

// Relé e equipamentos externos

#define pinoAnalog A0 // sensor de solo
#define a1 9 // bomba d'água
#define a2 3 // umidificador
#define a3 4 // coolers auxiliares
#define a4 5 // exaustores auxiliares
#define b1 6 // solenoide 1

//Sensor de ar

#define DHTPIN A1
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// DISPLAY

const byte SCREEN_WIDTH = 128;
const byte SCREEN_HEIGHT = 64;
const int OLED_RESET = -1;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int ValAnalogIn; // Introduz o valor analógico ao código
int valor_analogico = analogRead(pinoAnalog);
int analogSoloSeco = 800;
int analogSoloMolhado = 200; 
int percSoloSeco = 0; 
int percSoloMolhado = 100;

void setup() {

  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 5);
  display.display();

  //RELÉ

  pinMode(a1, OUTPUT);
  digitalWrite(a1, HIGH);
  pinMode(a2, OUTPUT);
  digitalWrite(a2, HIGH);
  pinMode(a3, OUTPUT);
  digitalWrite(a3, HIGH);
  pinMode(a4, OUTPUT);
  digitalWrite(a4, HIGH);
  pinMode(b1, OUTPUT);
  digitalWrite(b1, HIGH);

  // Sensor de ar

  dht.begin();
}

void loop() {


  //sensor de ar

  delay(10000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);


  // sensor solo

  ValAnalogIn = analogRead(pinoAnalog); // Relaciona o valor analógico com o recebido do sensor
  int Percentual = map(ValAnalogIn, 800, 0, 0, 200); // Relaciona o valor analógico à porcentagem

  ValAnalogIn = constrain(analogRead(pinoAnalog), analogSoloMolhado, analogSoloSeco);
  Percentual = map(ValAnalogIn, analogSoloMolhado, analogSoloSeco, percSoloMolhado, percSoloSeco);


  //temperatura

  if (dht.readTemperature() >= 27.0) {

    digitalWrite(a3, LOW);

    display.clearDisplay(); // Clear the buffer.
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 1);
    display.println("COOLER ON");
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 16);
    display.print("solo:   ");
    display.print(Percentual);
    display.println("%");
    display.print("ar:     ");
    display.print(h);
    display.println("%");
    display.print("temp:   ");
    display.print(t);
    display.println(F("c"));
    display.print("rega:   ");
    display.print(digitalRead(a1));
    display.print(" | cooler: ");
    display.println(digitalRead(a3));
    display.print("exaust: ");
    display.print(digitalRead(a4));
    display.print(" | umid: ");
    display.println(digitalRead(a2));
    display.display();

    Serial.print("COOLER ON");
    Serial.print(" | solo: ");
    Serial.print(Percentual);
    Serial.print("% | ");
    Serial.print(analogRead(A0));
    Serial.print(" | ar: ");
    Serial.print(h);
    Serial.print("%");
    Serial.print(" | temp: ");
    Serial.print(t);
    Serial.print(F(" c"));
    Serial.print(" | rega:  ");
    Serial.print(digitalRead(a1));
    Serial.print(" | cooler: ");
    Serial.print(digitalRead(a3));
    Serial.print(" | exaust: ");
    Serial.print(digitalRead(a4));
    Serial.print(" | umidif: ");
    Serial.println(digitalRead(a2));

    delay (60000);

  }  if (dht.readTemperature() <= 22.0) {

    digitalWrite(a3, HIGH);

    display.clearDisplay(); // Clear the buffer.
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 1);
    display.println("COOLER OFF");
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 16);
    display.print("solo:   ");
    display.print(Percentual);
    display.println("%");
    display.print("ar:     ");
    display.print(h);
    display.println("%");
    display.print("temp:   ");
    display.print(t);
    display.println(F("c"));
    display.print("rega:   ");
    display.print(digitalRead(a1));
    display.print(" | cooler: ");
    display.println(digitalRead(a3));
    display.print("exaust: ");
    display.print(digitalRead(a4));
    display.print(" | umid: ");
    display.println(digitalRead(a2));
    display.display();

    Serial.print("COOLER OFF");
    Serial.print(" | solo: ");
    Serial.print(Percentual);
    Serial.print("% | ");
    Serial.print(analogRead(A0));
    Serial.print(" | ar: ");
    Serial.print(h);
    Serial.print("%");
    Serial.print(" | temp: ");
    Serial.print(t);
    Serial.print(F(" c"));
    Serial.print(" | rega:  ");
    Serial.print(digitalRead(a1));
    Serial.print(" | cooler: ");
    Serial.print(digitalRead(a3));
    Serial.print(" | exaust: ");
    Serial.print(digitalRead(a4));
    Serial.print(" | umidif: ");
    Serial.println(digitalRead(a2));

    delay (60000);

    //umidade

  } if (dht.readHumidity() >= 54.0) {

    digitalWrite(a4, LOW);
    digitalWrite(a2, HIGH);

    //sensor de ar
    delay(10000);
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float f = dht.readTemperature(true);
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }
    float hif = dht.computeHeatIndex(f, h);
    float hic = dht.computeHeatIndex(t, h, false);
    // sensor solo
    ValAnalogIn = analogRead(pinoAnalog);
    int Percentual = map(ValAnalogIn, 800, 0, 0, 200);
    ValAnalogIn = constrain(analogRead(pinoAnalog), analogSoloMolhado, analogSoloSeco);
    Percentual = map(ValAnalogIn, analogSoloMolhado, analogSoloSeco, percSoloMolhado, percSoloSeco);

    display.clearDisplay(); // Clear the buffer.
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 1);
    display.println("EXAUSTOR ON");
    display.println("UMIDIFICADOR OFF");
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 16);
    display.print("solo:   ");
    display.print(Percentual);
    display.println("%");
    display.print("ar:     ");
    display.print(h);
    display.println("%");
    display.print("temp:   ");
    display.print(t);
    display.println(F("c"));
    display.print("rega:   ");
    display.print(digitalRead(a1));
    display.print(" | cooler: ");
    display.println(digitalRead(a3));
    display.print("exaust: ");
    display.print(digitalRead(a4));
    display.print(" | umid: ");
    display.println(digitalRead(a2));
    display.display();

    Serial.print("EXAUSTOR ON, UMIDIFICADOR OFF");
    Serial.print(" | solo: ");
    Serial.print(Percentual);
    Serial.print("% | ");
    Serial.print(analogRead(A0));
    Serial.print(" | ar: ");
    Serial.print(h);
    Serial.print("%");
    Serial.print(" | temp: ");
    Serial.print(t);
    Serial.print(F(" c"));
    Serial.print(" | rega:  ");
    Serial.print(digitalRead(a1));
    Serial.print(" | cooler: ");
    Serial.print(digitalRead(a3));
    Serial.print(" | exaust: ");
    Serial.print(digitalRead(a4));
    Serial.print(" | umidif: ");
    Serial.println(digitalRead(a2));

    delay (60000);

  } if (dht.readHumidity() <= 42.0) {

    digitalWrite(a2, LOW);
    digitalWrite(a4, HIGH);

    //sensor de ar
    delay(10000);
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float f = dht.readTemperature(true);
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }
    float hif = dht.computeHeatIndex(f, h);
    float hic = dht.computeHeatIndex(t, h, false);
    // sensor solo
    ValAnalogIn = analogRead(pinoAnalog);
    int Percentual = map(ValAnalogIn, 800, 0, 0, 200);
    ValAnalogIn = constrain(analogRead(pinoAnalog), analogSoloMolhado, analogSoloSeco);
    Percentual = map(ValAnalogIn, analogSoloMolhado, analogSoloSeco, percSoloMolhado, percSoloSeco);

    display.clearDisplay(); // Clear the buffer.
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 1);
    display.println("UMIDIFICADOR ON");
    display.println("EXAUSTOR OFF");
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 16);
    display.print("solo:   ");
    display.print(Percentual);
    display.println("%");
    display.print("ar:     ");
    display.print(h);
    display.println("%");
    display.print("temp:   ");
    display.print(t);
    display.println(F("c"));
    display.print("rega:   ");
    display.print(digitalRead(a1));
    display.print(" | cooler: ");
    display.println(digitalRead(a3));
    display.print("exaust: ");
    display.print(digitalRead(a4));
    display.print(" | umid: ");
    display.println(digitalRead(a2));
    display.display();

    Serial.print("UMIDIFICADOR ON, EXAUSTOR OFF");
    Serial.print(" | solo: ");
    Serial.print(Percentual);
    Serial.print("% | ");
    Serial.print(analogRead(A0));
    Serial.print(" | ar: ");
    Serial.print(h);
    Serial.print("%");
    Serial.print(" | temp: ");
    Serial.print(t);
    Serial.print(F(" c"));
    Serial.print(" | rega:  ");
    Serial.print(digitalRead(a1));
    Serial.print(" | cooler: ");
    Serial.print(digitalRead(a3));
    Serial.print(" | exaust: ");
    Serial.print(digitalRead(a4));
    Serial.print(" | umidif: ");
    Serial.println(digitalRead(a2));

    delay (60000);

    // Rega

  }  if (analogRead(pinoAnalog) >= 500) {

    //sensor de ar
    delay(10000);
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float f = dht.readTemperature(true);
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }
    float hif = dht.computeHeatIndex(f, h);
    float hic = dht.computeHeatIndex(t, h, false);
    // sensor solo
    ValAnalogIn = analogRead(pinoAnalog);
    int Percentual = map(ValAnalogIn, 800, 0, 0, 200);
    ValAnalogIn = constrain(analogRead(pinoAnalog), analogSoloMolhado, analogSoloSeco);
    Percentual = map(ValAnalogIn, analogSoloMolhado, analogSoloSeco, percSoloMolhado, percSoloSeco);

    while (analogRead(pinoAnalog) >= 350) {

      digitalWrite(a1, LOW);
      digitalWrite(b1, LOW);

      display.clearDisplay(); // Clear the buffer.
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 1);
      display.println("IRRIGACAO ATIVA");
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 16);
      display.print("solo:   ");
      display.print(Percentual);
      display.println("%");
      display.print("ar:     ");
      display.print(h);
      display.println("%");
      display.print("temp:   ");
      display.print(t);
      display.println(F("c"));
      display.print("rega:   ");
      display.print(digitalRead(a1));
      display.print(" | cooler: ");
      display.println(digitalRead(a3));
      display.print("exaust: ");
      display.print(digitalRead(a4));
      display.print(" | umid: ");
      display.println(digitalRead(a2));
      display.display();

      Serial.print("IRRIGACAO ATIVA");
      Serial.print(" | solo: ");
      Serial.print(Percentual);
      Serial.print("% | ");
      Serial.print(analogRead(A0));
      Serial.print(" | ar: ");
      Serial.print(h);
      Serial.print("%");
      Serial.print(" | temp: ");
      Serial.print(t);
      Serial.print(F(" c"));
      Serial.print(" | rega:  ");
      Serial.print(digitalRead(a1));
      Serial.print(" | cooler: ");
      Serial.print(digitalRead(a3));
      Serial.print(" | exaust: ");
      Serial.print(digitalRead(a4));
      Serial.print(" | umidif: ");
      Serial.println(digitalRead(a2));

      delay (900000); //tempo de rega

      //sensor de ar
      delay(10000);
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      float f = dht.readTemperature(true);
      if (isnan(h) || isnan(t) || isnan(f)) {
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
      }
      float hif = dht.computeHeatIndex(f, h);
      float hic = dht.computeHeatIndex(t, h, false);
      // sensor solo
      ValAnalogIn = analogRead(pinoAnalog);
      int Percentual = map(ValAnalogIn, 800, 0, 0, 200);
      ValAnalogIn = constrain(analogRead(pinoAnalog), analogSoloMolhado, analogSoloSeco);
      Percentual = map(ValAnalogIn, analogSoloMolhado, analogSoloSeco, percSoloMolhado, percSoloSeco);

      digitalWrite(a1, HIGH);
      digitalWrite(b1, HIGH);

      display.clearDisplay(); // Clear the buffer.
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 1);
      display.println("IRRIGACAO PAUSADA");
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 16);
      display.print("solo:   ");
      display.print(Percentual);
      display.println("%");
      display.print("ar:     ");
      display.print(h);
      display.println("%");
      display.print("temp:   ");
      display.print(t);
      display.println(F("c"));
      display.print("rega:   ");
      display.print(digitalRead(a1));
      display.print(" | cooler: ");
      display.println(digitalRead(a3));
      display.print("exaust: ");
      display.print(digitalRead(a4));
      display.print(" | umid: ");
      display.println(digitalRead(a2));
      display.display();

      Serial.print("IRRIGACAO PAUSADA");
      Serial.print(" | solo: ");
      Serial.print(Percentual);
      Serial.print("% | ");
      Serial.print(analogRead(A0));
      Serial.print(" | ar: ");
      Serial.print(h);
      Serial.print("%");
      Serial.print(" | temp: ");
      Serial.print(t);
      Serial.print(F(" c"));
      Serial.print(" | rega:  ");
      Serial.print(digitalRead(a1));
      Serial.print(" | cooler: ");
      Serial.print(digitalRead(a3));
      Serial.print(" | exaust: ");
      Serial.print(digitalRead(a4));
      Serial.print(" | umidif: ");
      Serial.println(digitalRead(a2));

      delay (2700000); // tempo de pausa

    }

  }  else {

    digitalWrite(a1, HIGH);
    digitalWrite(b1, HIGH);

    //sensor de ar
    delay(10000);
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float f = dht.readTemperature(true);
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }
    float hif = dht.computeHeatIndex(f, h);
    float hic = dht.computeHeatIndex(t, h, false);
    // sensor solo
    ValAnalogIn = analogRead(pinoAnalog);
    int Percentual = map(ValAnalogIn, 800, 0, 0, 200);
    ValAnalogIn = constrain(analogRead(pinoAnalog), analogSoloMolhado, analogSoloSeco);
    Percentual = map(ValAnalogIn, analogSoloMolhado, analogSoloSeco, percSoloMolhado, percSoloSeco);

    display.clearDisplay(); // Clear the buffer.
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 1);
    display.println("   MENDIGROW 2.3");
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 16);
    display.print("solo:   ");
    display.print(Percentual);
    display.println("%");
    display.print("ar:     ");
    display.print(h);
    display.println("%");
    display.print("temp:   ");
    display.print(t);
    display.println(F("c"));
    display.print("rega:   ");
    display.print(digitalRead(a1));
    display.print(" | cooler: ");
    display.println(digitalRead(a3));
    display.print("exaust: ");
    display.print(digitalRead(a4));
    display.print(" | umid: ");
    display.println(digitalRead(a2));
    display.display();

    Serial.print("   MENDIGROW 2.3");
    Serial.print(" | solo: ");
    Serial.print(Percentual);
    Serial.print("% | ");
    Serial.print(analogRead(A0));
    Serial.print(" | ar: ");
    Serial.print(h);
    Serial.print("%");
    Serial.print(" | temp: ");
    Serial.print(t);
    Serial.print(F(" c"));
    Serial.print(" | rega:  ");
    Serial.print(digitalRead(a1));
    Serial.print(" | cooler: ");
    Serial.print(digitalRead(a3));
    Serial.print(" | exaust: ");
    Serial.print(digitalRead(a4));
    Serial.print(" | umidif: ");
    Serial.println(digitalRead(a2));
  }
  delay (900000);

}
