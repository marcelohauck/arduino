#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define pinoAnalog A0
//#define pinoAnalog1 A1
#define pinoRele 5
// #define pinoRele1 6

// DISPLAY

const byte SCREEN_WIDTH = 128; // OLED display width, in pixels
const byte SCREEN_HEIGHT = 64; // OLED display height, in pixels
const int OLED_RESET = -1; // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int ValAnalogIn; // Introduz o valor analógico ao código
//int ValAnalogIn1; // Introduz o valor analógico ao código

int valor_analogico = analogRead(pinoAnalog);
int analogSoloSeco = 1023; //VALOR MEDIDO COM O SOLO SECO (VOCÊ PODE FAZER TESTES E AJUSTAR ESTE VALOR)
int analogSoloMolhado = 300; //VALOR MEDIDO COM O SOLO MOLHADO (VOCÊ PODE FAZER TESTES E AJUSTAR ESTE VALOR)
int percSoloSeco = 0; //MENOR PERCENTUAL DO SOLO SECO (0% - NÃO ALTERAR)
int percSoloMolhado = 100; //MAIOR PERCENTUAL DO SOLO MOLHADO (100% - NÃO ALTERAR)

/*int valor_analogico1 = analogRead(pinoAnalog1);
int analogSoloSeco1 = 1000; //VALOR MEDIDO COM O SOLO SECO (VOCÊ PODE FAZER TESTES E AJUSTAR ESTE VALOR)
int analogSoloMolhado1 = 300; //VALOR MEDIDO COM O SOLO MOLHADO (VOCÊ PODE FAZER TESTES E AJUSTAR ESTE VALOR)
int percSoloSeco1 = 0; //MENOR PERCENTUAL DO SOLO SECO (0% - NÃO ALTERAR)
int percSoloMolhado1 = 100; //MAIOR PERCENTUAL DO SOLO MOLHADO (100% - NÃO ALTERAR)
*/
void setup() {

  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C
  display.clearDisplay(); // Clear the buffer.
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 5);
  display.display();

  //RELÉ

  pinMode(pinoRele, OUTPUT); // Declara o pinoRele como Saída
//  pinMode(pinoRele1, OUTPUT); // Declara o pinoRele como Saída
  digitalWrite(pinoRele, HIGH); // Põem o pinoRele em estado Alto
//  digitalWrite(pinoRele1, HIGH); // Põem o pinoRele em estado Alto
}

void loop() {

  ValAnalogIn = analogRead(pinoAnalog); // Relaciona o valor analógico com o recebido do sensor
  int Percentual = map(ValAnalogIn, 1023, 0, 0, 300); // Relaciona o valor analógico à porcentagem
//  ValAnalogIn1 = analogRead(pinoAnalog1); // Relaciona o valor analógico com o recebido do sensor
 // int Percentual1 = map(ValAnalogIn1, 1023, 0, 0, 300); // Relaciona o valor analógico à porcentagem


  ValAnalogIn = constrain(analogRead(pinoAnalog), analogSoloMolhado, analogSoloSeco); //MANTÉM ValAnalogIn DENTRO DO INTERVALO (ENTRE analogSoloMolhado E analogSoloSeco)
  Percentual = map(ValAnalogIn, analogSoloMolhado, analogSoloSeco, percSoloMolhado, percSoloSeco); //EXECUTA A FUNÇÃO "map" DE ACORDO COM OS PARÂMETROS PASSADOS

 // ValAnalogIn1 = constrain(analogRead(pinoAnalog1), analogSoloMolhado1, analogSoloSeco1); //MANTÉM  DENTRO DO INTERVALO (ENTRE analogSoloMolhado E analogSoloSeco)
//  Percentual1 = map(ValAnalogIn1, analogSoloMolhado1, analogSoloSeco1, percSoloMolhado1, percSoloSeco1); //EXECUTA A FUNÇÃO "map" DE ACORDO COM OS PARÂMETROS PASSADOS


  //OG PRENSADO  KUSH


  if (analogRead(pinoAnalog) >= 650) {


    while (analogRead(pinoAnalog) >= 385) {

      digitalWrite(pinoRele, LOW);
      display.clearDisplay(); // Clear the buffer.
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 2);
      display.println("     P1 ATIVA");
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 16);
      display.println("OG PRENSADO  KUSH");
      display.print("Umidade: ");
      display.print(Percentual);
      display.println("%");
      display.print("Sensor: ");
      display.print(analogRead(A0));
      display.print(" |  R:  ");
      display.println(digitalRead(pinoRele));
  //  display.println("PLANTA 2");
  //  display.print("Umidade: ");
  //  display.print(Percentual1);
  //  display.println("%");
  //  display.print("Sensor: ");
  //  display.print(analogRead(A1));
  //  display.print(" |  R:  ");
  //  display.println(digitalRead(pinoRele1));
      display.display();

      Serial.print("OG PRENSADO  KUSH:  ");
      Serial.print("Read: ");
      Serial.print(analogRead(A0));
      Serial.print(" |  Umidade: ");
      Serial.print(Percentual);
      Serial.print("%");
      Serial.print(" |  Rele:  ");
      Serial.println(digitalRead(pinoRele));
  //  Serial.print(" PLANTA 2:  ");
  //  Serial.print("Read: ");
  //  Serial.print(analogRead(A1));
  //  Serial.print(" |  Umidade: ");
  //  Serial.print(Percentual1);
  //  Serial.print("%");
  //  Serial.print(" |  Rele:  ");
  //  Serial.println(digitalRead(pinoRele1));

      delay (600000);

      digitalWrite(pinoRele, HIGH);
      display.clearDisplay(); // Clear the buffer.
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 2);
      display.println("   IRRIGACAO PAUSADA");
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 16);
      display.println("OG PRENSADO  KUSH");
      display.print("Umidade: ");
      display.print(Percentual);
      display.println("%");
      display.print("Sensor: ");
      display.print(analogRead(A0));
      display.print(" |  R:  ");
      display.println(digitalRead(pinoRele));
  //  display.println("PLANTA 2");
  //  display.print("Umidade: ");
  //  display.print(Percentual1);
  //  display.println("%");
  //  display.print("Sensor: ");
  //  display.print(analogRead(A1));
  //  display.print(" |  R:  ");
  //  display.println(digitalRead(pinoRele1));
      display.display();

      Serial.print("OG PRENSADO  KUSH:  ");
      Serial.print("Read: ");
      Serial.print(analogRead(A0));
      Serial.print(" |  Umidade: ");
      Serial.print(Percentual);
      Serial.print("%");
      Serial.print(" |  Rele:  ");
      Serial.println(digitalRead(pinoRele));
  //  Serial.print(" PLANTA 2:  ");
  //  Serial.print("Read: ");
  //  Serial.print(analogRead(A1));
  //  Serial.print(" |  Umidade: ");
  //  Serial.print(Percentual1);
  //  Serial.print("%");
  //  Serial.print(" |  Rele:  ");
  //  Serial.println(digitalRead(pinoRele1));

      delay (3600000);

    }

/*
    //PLANTA 2
    
  }  else if (analogRead(pinoAnalog1) >= 750) {

      while (analogRead(pinoAnalog1) >= 400) {

        digitalWrite(pinoRele1, LOW);
        display.clearDisplay(); // Clear the buffer.
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0, 2);
        display.println("     P2 ATIVA");
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0, 16);
        display.println("OG PRENSADO  KUSH");
        display.print("Umidade: ");
        display.print(Percentual);
        display.println("%");
        display.print("Sensor: ");
        display.print(analogRead(A0));
        display.print(" |  R:  ");
        display.println(digitalRead(pinoRele));
        display.println("PLANTA 2");
        display.print("Umidade: ");
        display.print(Percentual1);
        display.println("%");
        display.print("Sensor: ");
        display.print(analogRead(A1));
        display.print(" |  R:  ");
        display.println(digitalRead(pinoRele1));
        display.display();

        Serial.print("OG PRENSADO  KUSH:  ");
        Serial.print("Read: ");
        Serial.print(analogRead(A0));
        Serial.print(" |  Umidade: ");
        Serial.print(Percentual);
        Serial.print("%");
        Serial.print(" |  Rele:  ");
        Serial.print(digitalRead(pinoRele));
        Serial.print(" PLANTA 2:  ");
        Serial.print("Read: ");
        Serial.print(analogRead(A1));
        Serial.print(" |  Umidade: ");
        Serial.print(Percentual1);
        Serial.print("%");
        Serial.print(" |  Rele:  ");
        Serial.println(digitalRead(pinoRele1));

        delay (600000);

        digitalWrite(pinoRele1, HIGH);
        display.clearDisplay(); // Clear the buffer.
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0, 2);
        display.println("   IRRIGACAO PAUSADA");
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0, 16);
        display.println("OG PRENSADO  KUSH");
        display.print("Umidade: ");
        display.print(Percentual);
        display.println("%");
        display.print("Sensor: ");
        display.print(analogRead(A0));
        display.print(" |  R:  ");
        display.println(digitalRead(pinoRele));
        display.println("PLANTA 2");
        display.print("Umidade: ");
        display.print(Percentual1);
        display.println("%");
        display.print("Sensor: ");
        display.print(analogRead(A1));
        display.print(" |  R:  ");
        display.println(digitalRead(pinoRele1));
        display.display();

        Serial.print("OG PRENSADO  KUSH:  ");
        Serial.print("Read: ");
        Serial.print(analogRead(A0));
        Serial.print(" |  Umidade: ");
        Serial.print(Percentual);
        Serial.print("%");
        Serial.print(" |  Rele:  ");
        Serial.print(digitalRead(pinoRele));
        Serial.print(" PLANTA 2:  ");
        Serial.print("Read: ");
        Serial.print(analogRead(A1));
        Serial.print(" |  Umidade: ");
        Serial.print(Percentual1);
        Serial.print("%");
        Serial.print(" |  Rele:  ");
        Serial.println(digitalRead(pinoRele1));

        delay (3600000);

      }
  
  */
        }else{

        digitalWrite(pinoRele, HIGH);
  //      digitalWrite(pinoRele1, HIGH);

        display.clearDisplay(); // Clear the buffer.
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0, 2);
        display.println("   MENDIGROW 2.0");
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0, 16);
        display.println("OG PRENSADO  KUSH");
        display.print("Umidade: ");
        display.print(Percentual);
        display.println("%");
        display.print("Sensor: ");
        display.print(analogRead(A0));
        display.print(" |  R:  ");
        display.println(digitalRead(pinoRele));
  //  display.println("PLANTA 2");
  //  display.print("Umidade: ");
  //  display.print(Percentual1);
  //  display.println("%");
  //  display.print("Sensor: ");
  //  display.print(analogRead(A1));
  //  display.print(" |  R:  ");
  //  display.println(digitalRead(pinoRele1));
        display.display();

        Serial.print("OG PRENSADO  KUSH:  ");
        Serial.print("Read: ");
        Serial.print(analogRead(A0));
        Serial.print(" |  Umidade: ");
        Serial.print(Percentual);
        Serial.print("%");
        Serial.print(" |  Rele:  ");
        Serial.println(digitalRead(pinoRele));
  //  Serial.print(" PLANTA 2:  ");
  //  Serial.print("Read: ");
  //  Serial.print(analogRead(A1));
  //  Serial.print(" |  Umidade: ");
  //  Serial.print(Percentual1);
  //  Serial.print("%");
  //  Serial.print(" |  Rele:  ");
  //  Serial.println(digitalRead(pinoRele1));
      }

      delay (600000); // Estabelece o tempo de 1s para reinicializar a leitura

    }
