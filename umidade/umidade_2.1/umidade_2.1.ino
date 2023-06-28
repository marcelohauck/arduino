#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"

// Sensor de solo e relé

#define pinoAnalog A0
#define pinoRele1 5 // bomba + solenoide planta 1
#define pinoRele2 6 // coolers auxiliares
#define pinoRele3 8 // exaustores auxiliares / umidificador



//Sensor de ar

#define DHTPIN 7
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

  pinMode(pinoRele1, OUTPUT);
  digitalWrite(pinoRele1, HIGH);
  pinMode(pinoRele2, OUTPUT);
  digitalWrite(pinoRele2, HIGH);
  pinMode(pinoRele3, OUTPUT);
  digitalWrite(pinoRele3, HIGH);

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


  ValAnalogIn = analogRead(pinoAnalog); // Relaciona o valor analógico com o recebido do sensor
  int Percentual = map(ValAnalogIn, 1023, 0, 0, 300); // Relaciona o valor analógico à porcentagem

  ValAnalogIn = constrain(analogRead(pinoAnalog), analogSoloMolhado, analogSoloSeco); //MANTÉM ValAnalogIn DENTRO DO INTERVALO (ENTRE analogSoloMolhado E analogSoloSeco)
  Percentual = map(ValAnalogIn, analogSoloMolhado, analogSoloSeco, percSoloMolhado, percSoloSeco); //EXECUTA A FUNÇÃO "map" DE ACORDO COM OS PARÂMETROS PASSADOS


  //OG PRENSADO  KUSH

  if (analogRead(pinoAnalog) >= 650) {

    while (analogRead(pinoAnalog) >= 385) {

      digitalWrite(pinoRele1, LOW);
      display.clearDisplay(); // Clear the buffer.
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 2);
      display.println("IRRIGACAO ATIVA");
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 16);
      display.print("solo: ");
      display.print(Percentual);
      display.println("%");
      display.print("ar: ");
      display.print(h);
      display.println("%");
      display.print("temp: ");
      display.print(t);
      display.println(F(" c"));
      display.print("rega:  ");
      display.println(digitalRead(pinoRele1));
      display.print("cooler: ");
      display.println(digitalRead(pinoRele2));
      display.print("exaust: ");
      display.println(digitalRead(pinoRele3));
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
      Serial.print(digitalRead(pinoRele1));
      Serial.print(" | cooler: ");
      Serial.print(digitalRead(pinoRele2));
      Serial.print(" | exaust: ");
      Serial.println(digitalRead(pinoRele3));

    }

  } else if (dht.readTemperature() >= 27.0) {

    while (dht.readTemperature() >= 23.0) {

      digitalWrite(pinoRele2, HIGH);
      
      display.clearDisplay(); // Clear the buffer.
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 2);
      display.println("IRRIGACAO ATIVA");
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 16);
      display.print("solo: ");
      display.print(Percentual);
      display.println("%");
      display.print("ar: ");
      display.print(h);
      display.println("%");
      display.print("temp: ");
      display.print(t);
      display.println(F(" c"));
      display.print("rega:  ");
      display.println(digitalRead(pinoRele1));
      display.print("cooler: ");
      display.println(digitalRead(pinoRele2));
      display.print("exaust: ");
      display.println(digitalRead(pinoRele3));
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
      Serial.print(digitalRead(pinoRele1));
      Serial.print(" | cooler: ");
      Serial.print(digitalRead(pinoRele2));
      Serial.print(" | exaust: ");
      Serial.println(digitalRead(pinoRele3));
    }


  } else if (dht.readTemperature() <= 22.0) {

          while (dht.readTemperature() <= 26.0) {

    digitalWrite(pinoRele2, LOW);
    
display.clearDisplay(); // Clear the buffer.
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 2);
      display.println("COOLER DESLIGADO");
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 16);
      display.print("solo: ");
      display.print(Percentual);
      display.println("%");
      display.print("ar: ");
      display.print(h);
      display.println("%");
      display.print("temp: ");
      display.print(t);
      display.println(F(" c"));
      display.print("rega:  ");
      display.println(digitalRead(pinoRele1));
      display.print("cooler: ");
      display.println(digitalRead(pinoRele2));
      display.print("exaust: ");
      display.println(digitalRead(pinoRele3));
      display.display();

      Serial.print("COOLER DESLIGADO");
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
      Serial.print(digitalRead(pinoRele1));
      Serial.print(" | cooler: ");
      Serial.print(digitalRead(pinoRele2));
      Serial.print(" | exaust: ");
      Serial.println(digitalRead(pinoRele3));
  }
  
  }else if (dht.readHumidity() >= 55.0) {

    while (dht.readHumidity() >= 50.0) {
      
    digitalWrite(pinoRele3, LOW);

    display.clearDisplay(); // Clear the buffer.
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 2);
      display.println("EXAUSTOR LIGADO");
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 16);
      display.print("solo: ");
      display.print(Percentual);
      display.println("%");
      display.print("ar: ");
      display.print(h);
      display.println("%");
      display.print("temp: ");
      display.print(t);
      display.println(F(" c"));
      display.print("rega:  ");
      display.println(digitalRead(pinoRele1));
      display.print("cooler: ");
      display.println(digitalRead(pinoRele2));
      display.print("exaust: ");
      display.println(digitalRead(pinoRele3));
      display.display();

      Serial.print("EXAUSTOR LIGADO");
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
      Serial.print(digitalRead(pinoRele1));
      Serial.print(" | cooler: ");
      Serial.print(digitalRead(pinoRele2));
      Serial.print(" | exaust: ");
      Serial.println(digitalRead(pinoRele3));
  }

  } else if (dht.readHumidity() <= 42.0) {

      while (dht.readHumidity() <= 50.0) {
      
    digitalWrite(pinoRele3, HIGH);

 display.clearDisplay(); // Clear the buffer.
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 2);
      display.println("EXAUSTOR DESLIGADO");
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 16);
      display.print("solo: ");
      display.print(Percentual);
      display.println("%");
      display.print("ar: ");
      display.print(h);
      display.println("%");
      display.print("temp: ");
      display.print(t);
      display.println(F(" c"));
      display.print("rega:  ");
      display.println(digitalRead(pinoRele1));
      display.print("cooler: ");
      display.println(digitalRead(pinoRele2));
      display.print("exaust: ");
      display.println(digitalRead(pinoRele3));
      display.display();

      Serial.print("EXAUSTOR DESLIGADO");
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
      Serial.print(digitalRead(pinoRele1));
      Serial.print(" | cooler: ");
      Serial.print(digitalRead(pinoRele2));
      Serial.print(" | exaust: ");
      Serial.println(digitalRead(pinoRele3));
  }

  } else {

    display.clearDisplay(); // Clear the buffer.
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 2);
    display.println("   MENDIGROW 2.0");
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 16);
    display.print("solo: ");
    display.print(Percentual);
    display.println("%");
    display.print("ar: ");
    display.print(h);
    display.println("%");
    display.print("temp: ");
    display.print(t);
    display.println(F(" c"));
    display.print("rega:  ");
    display.println(digitalRead(pinoRele1));
    display.print("cooler: ");
    display.println(digitalRead(pinoRele2));
    display.print("exaust: ");
    display.println(digitalRead(pinoRele3));
    display.display();

    Serial.print("OG PRENSADO  KUSH");
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
    Serial.print(digitalRead(pinoRele1));
    Serial.print(" | cooler: ");
    Serial.print(digitalRead(pinoRele2));
    Serial.print(" | exaust: ");
    Serial.println(digitalRead(pinoRele3));
  }
  delay (60000);

}
