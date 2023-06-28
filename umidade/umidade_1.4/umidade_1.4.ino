
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// DISPLAY

const byte SCREEN_WIDTH = 128; // OLED display width, in pixels
const byte SCREEN_HEIGHT = 64; // OLED display height, in pixels
const int OLED_RESET = -1; // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// SENSORES

const int pinoSensor = A0; //PINO UTILIZADO PELO SENSOR
//const int pinoSensor2 = A1; //PINO UTILIZADO PELO SENSOR
//const int pinoSensor3 = A2; //PINO UTILIZADO PELO SENSOR
int valorLido; //VARIÁVEL QUE ARMAZENA O PERCENTUAL DE UMIDADE DO SOLO
//int valorLido2; //VARIÁVEL QUE ARMAZENA O PERCENTUAL DE UMIDADE DO SOLO
//int valorLido3; //VARIÁVEL QUE ARMAZENA O PERCENTUAL DE UMIDADE DO SOLO

int valor_analogico = analogRead(pinoSensor);
int analogSoloSeco = 1000; //VALOR MEDIDO COM O SOLO SECO (VOCÊ PODE FAZER TESTES E AJUSTAR ESTE VALOR)
int analogSoloMolhado = 150; //VALOR MEDIDO COM O SOLO MOLHADO (VOCÊ PODE FAZER TESTES E AJUSTAR ESTE VALOR)
int percSoloSeco = 0; //MENOR PERCENTUAL DO SOLO SECO (0% - NÃO ALTERAR)
int percSoloMolhado = 100; //MAIOR PERCENTUAL DO SOLO MOLHADO (100% - NÃO ALTERAR)

//int valor_analogico2 = analogRead(pinoSensor2);
//int analogSoloSeco2 = 1000; //VALOR MEDIDO COM O SOLO SECO (VOCÊ PODE FAZER TESTES E AJUSTAR ESTE VALOR)
//int analogSoloMolhado2 = 150; //VALOR MEDIDO COM O SOLO MOLHADO (VOCÊ PODE FAZER TESTES E AJUSTAR ESTE VALOR)
//int percSoloSeco2 = 0; //MENOR PERCENTUAL DO SOLO SECO (0% - NÃO ALTERAR)
//int percSoloMolhado2 = 100; //MAIOR PERCENTUAL DO SOLO MOLHADO (100% - NÃO ALTERAR)

//int valor_analogico3 = analogRead(pinoSensor3);
//int analogSoloSeco3 = 1000; //VALOR MEDIDO COM O SOLO SECO (VOCÊ PODE FAZER TESTES E AJUSTAR ESTE VALOR)
//int analogSoloMolhado3 = 150; //VALOR MEDIDO COM O SOLO MOLHADO (VOCÊ PODE FAZER TESTES E AJUSTAR ESTE VALOR)
//int percSoloSeco3 = 0; //MENOR PERCENTUAL DO SOLO SECO (0% - NÃO ALTERAR)
//int percSoloMolhado3 = 100; //MAIOR PERCENTUAL DO SOLO MOLHADO (100% - NÃO ALTERAR)

// RELÉ

int porta_rele1 = 5;
// int porta_rele2 = 6;
// int porta_rele3 = 7;
// int porta_rele4 = 8;

//Armazena o estado do rele - 0 (LOW) ou 1 (HIGH)
int estadorele1 = 1;
//int estadorele2 = 1;
//int estadorele3 = 1;
//int estadorele4 = 1;

//Armazena o valor lido dos botoes
int leitura1 = 0;
//int leitura2 = 0;
//int leitura3 = 0;
//int leitura4 = 0;

void setup() {

//SERIAL

 Serial.begin(9600); 
 Serial.println("Lendo a umidade..."); //IMPRIME O TEXTO NO MONITOR SERIAL

//DISPLAY

display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C
display.clearDisplay(); // Clear the buffer.
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0,3);
display.println("     MENDIGROW");

//RELÉ

  pinMode(porta_rele1, OUTPUT); 
//  pinMode(porta_rele2, OUTPUT);
//  pinMode(porta_rele3, OUTPUT);
//  pinMode(porta_rele4, OUTPUT);

//Estado inicial dos reles - desligados
  digitalWrite(porta_rele1, HIGH);
//  digitalWrite(porta_rele2, HIGH);
}
 

void loop(){

// SENSOR

valorLido = constrain(analogRead(pinoSensor),analogSoloMolhado,analogSoloSeco); //MANTÉM valorLido DENTRO DO INTERVALO (ENTRE analogSoloMolhado E analogSoloSeco)
valorLido = map(valorLido,analogSoloMolhado,analogSoloSeco,percSoloMolhado,percSoloSeco); //EXECUTA A FUNÇÃO "map" DE ACORDO COM OS PARÂMETROS PASSADOS

//valorLido2 = constrain(analogRead(pinoSensor2),analogSoloMolhado2,analogSoloSeco2); //MANTÉM valorLido DENTRO DO INTERVALO (ENTRE analogSoloMolhado E analogSoloSeco)
//valorLido2 = map(valorLido2,analogSoloMolhado2,analogSoloSeco2,percSoloMolhado2,percSoloSeco2); //EXECUTA A FUNÇÃO "map" DE ACORDO COM OS PARÂMETROS PASSADOS

//valorLido3 = constrain(analogRead(pinoSensor3),analogSoloMolhado3,analogSoloSeco3); //MANTÉM valorLido DENTRO DO INTERVALO (ENTRE analogSoloMolhado E analogSoloSeco)
//valorLido3 = map(valorLido3,analogSoloMolhado3,analogSoloSeco3,percSoloMolhado3,percSoloSeco3); //EXECUTA A FUNÇÃO "map" DE ACORDO COM OS PARÂMETROS PASSADOS

// RELE

 if((valor_analogico >= 800) && (estadorele1 == 1)){
        digitalWrite(porta_rele1, LOW); 
        estadorele1 = 0;
 }else{ 
  
 if((valor_analogico <= 300) && (estadorele1 == 0)){
 
        digitalWrite(porta_rele1, HIGH);
        estadorele1 = 1;
        }
 }

// DISPLAY

display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0,2);
display.println("     MENDIGROW");
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0,16);
display.print("Umidade: ");
display.print(analogRead(A0));
display.print(" | ");
display.print(valorLido);
display.println("%");
display.print("Rele:  ");
//display.print(analogRead(A1));
display.print(estadorele1);
//display.print(valorLido2);
//display.println("%");
//display.print("Guerrera:  ");
//display.print(analogRead(A2));
//display.print(" | ");
//display.print(valorLido3);
//display.println("%");
display.display();

//SERIAL

Serial.print("Sensor 1: ");
Serial.print(analogRead(A0));
Serial.print("  ");
Serial.print("Umidade 1: ");
Serial.print(valorLido);
Serial.print("%");
Serial.print("  relé: ");
Serial.println(estadorele1);
//Serial.print("Sensor 2: ");
//Serial.print(analogRead(A1));
//Serial.print(" ");
//Serial.print("Umidade 2: ");
//Serial.print(valorLido2);
//Serial.println("%");
//Serial.print("  ");
//Serial.print("Sensor 3: ");
//Serial.print(analogRead(A2));
//Serial.print(" ");
//Serial.print("Umidade 3: ");
//Serial.print(valorLido3);

delay(10000);
display.clearDisplay(); // Clear the buffer.



 }
