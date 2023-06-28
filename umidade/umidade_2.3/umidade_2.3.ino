#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"

// Relé e equipamentos externos

#define pinoAnalog A0 // sensor de solo
#define a1 2 // bomba d'água
#define a2 3 // umidificador
#define a3 4 // coolers auxiliares
#define a4 5 // exaustores auxiliares
#define b1 6 // solenoide 1

//Sensor de ar

#define DHTPIN A1
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// DISPLAY

const byte SCREEN_WIDTH = 128; // OLED display width, in pixels
const byte SCREEN_HEIGHT = 64; // OLED display height, in pixels
const int OLED_RESET = -1; // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int ValAnalogIn; // Introduz o valor analógico ao código

int valor_analogico = analogRead(pinoAnalog);
int analogSoloSeco = 1023; //VALOR MEDIDO COM O SOLO SECO (VOCÊ PODE FAZER TESTES E AJUSTAR ESTE VALOR)
int analogSoloMolhado = 300; //VALOR MEDIDO COM O SOLO MOLHADO (VOCÊ PODE FAZER TESTES E AJUSTAR ESTE VALOR)
int percSoloSeco = 0; //MENOR PERCENTUAL DO SOLO SECO (0% - NÃO ALTERAR)
int percSoloMolhado = 100; //MAIOR PERCENTUAL DO SOLO MOLHADO (100% - NÃO ALTERAR)

void setup() {

  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C
  display.clearDisplay(); // Clear the buffer.
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
  int Percentual = map(ValAnalogIn, 1023, 0, 0, 300); // Relaciona o valor analógico à porcentagem

  ValAnalogIn = constrain(analogRead(pinoAnalog), analogSoloMolhado, analogSoloSeco); //MANTÉM ValAnalogIn DENTRO DO INTERVALO (ENTRE analogSoloMolhado E analogSoloSeco)
  Percentual = map(ValAnalogIn, analogSoloMolhado, analogSoloSeco, percSoloMolhado, percSoloSeco); //EXECUTA A FUNÇÃO "map" DE ACORDO COM OS PARÂMETROS PASSADOS


  //temperatura

  if (dht.readTemperature() >= 27.0) {

    digitalWrite(a3, LOW);

    display.clearDisplay(); // Clear the buffer.
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 1);
    display.println("COOLER ATIVO");
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
    display.println(digitalRead(a1));
    display.print("cooler: ");
    display.println(digitalRead(a3));
    display.print("exaust: ");
    display.println(digitalRead(a4));
    display.display();

    Serial.print("COOLER ATIVO");
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
    Serial.println(digitalRead(a4));

  } if (dht.readTemperature() <= 22.0) {

    digitalWrite(a3, HIGH);

    display.clearDisplay(); // Clear the buffer.
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 1);
    display.println("COOLER INATIVO");
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
    display.println(digitalRead(a1));
    display.print("cooler: ");
    display.println(digitalRead(a3));
    display.print("exaust: ");
    display.println(digitalRead(a4));
    display.display();

    Serial.print("COOLER INATIVO");
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
    Serial.println(digitalRead(a4));


    //umidade

  } if (dht.readHumidity() >= 55.0) {

    digitalWrite(a4, LOW);
    digitalWrite(a2, HIGH);

    display.clearDisplay(); // Clear the buffer.
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 1);
    display.println("EXAUSTÃO REFORÇADA");
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
    display.println(digitalRead(a1));
    display.print("cooler: ");
    display.println(digitalRead(a3));
    display.print("exaust: ");
    display.println(digitalRead(a4));
    display.display();

    Serial.print("EXAUSTÃO REFORÇADA");
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
    Serial.println(digitalRead(a4));


  } if (dht.readHumidity() <= 42.0) {

    digitalWrite(a2, LOW);
    digitalWrite(a4, HIGH);

    display.clearDisplay(); // Clear the buffer.
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 1);
    display.println("UMIDIFICADOR ATIVO");
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
    display.println(digitalRead(a1));
    display.print("cooler: ");
    display.println(digitalRead(a3));
    display.print("exaust: ");
    display.println(digitalRead(a4));
    display.display();

    Serial.print("UMIDIFICADOR ATIVO");
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
    Serial.println(digitalRead(a4));


    // Rega

  }  if (analogRead(pinoAnalog) >= 650) {

    while (analogRead(pinoAnalog) >= 385) {

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
      display.println(digitalRead(a1));
      display.print("cooler: ");
      display.println(digitalRead(a3));
      display.print("exaust: ");
      display.println(digitalRead(a4));
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
      Serial.println(digitalRead(a4));

      delay (3000); //tempo de rega

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
      display.println(digitalRead(a1));
      display.print("cooler: ");
      display.println(digitalRead(a3));
      display.print("exaust: ");
      display.println(digitalRead(a4));
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
      Serial.println(digitalRead(a4));

      delay (30000); // tempo de pausa

    }

  } if ((analogRead(pinoAnalog) <= 385) && digitalRead(b1 == LOW)) {

    digitalWrite(a1, HIGH);
    digitalWrite(b1, HIGH);

    display.clearDisplay(); // Clear the buffer.
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 1);
    display.println("IRRIGACAO FINALIZADA");
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
    display.println(digitalRead(a1));
    display.print("cooler: ");
    display.println(digitalRead(a3));
    display.print("exaust: ");
    display.println(digitalRead(a4));
    display.display();

    Serial.print("IRRIGACAO FINALIZADA");
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
    Serial.println(digitalRead(a4));


  } else {

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
    display.println(digitalRead(a1));
    display.print("cooler: ");
    display.println(digitalRead(a3));
    display.print("exaust: ");
    display.println(digitalRead(a4));
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
    Serial.println(digitalRead(a4));

  }
  delay (10000);

}
