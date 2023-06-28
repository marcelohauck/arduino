 /*******************************************************************************
*
*    Projeto: Hello Word! - COMUNICAÇÃO I2C
*    Data: 30/09/2020
*            http://squids.com.br/arduino
*
*******************************************************************************/
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


// const. LCD

const byte SCREEN_WIDTH = 128; // OLED display width, in pixels
const byte SCREEN_HEIGHT = 64; // OLED display height, in pixels
const int OLED_RESET = -1; // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//const. Sensor umidade

const int pinoSensor = A0; //PINO UTILIZADO PELO SENSOR
int valorLido; //VARIÁVEL QUE ARMAZENA O PERCENTUAL DE UMIDADE DO SOLO

int valor_analogico = analogRead(pinoSensor);
int analogSoloSeco = 900; //VALOR MEDIDO COM O SOLO SECO (VOCÊ PODE FAZER TESTES E AJUSTAR ESTE VALOR)
int analogSoloMolhado = 150; //VALOR MEDIDO COM O SOLO MOLHADO (VOCÊ PODE FAZER TESTES E AJUSTAR ESTE VALOR)
int percSoloSeco = 0; //MENOR PERCENTUAL DO SOLO SECO (0% - NÃO ALTERAR)
int percSoloMolhado = 100; //MAIOR PERCENTUAL DO SOLO MOLHADO (100% - NÃO ALTERAR)

void setup() {

 Serial.begin(9600); //INICIALIZA A SERIAL
 Serial.println("Lendo a umidade..."); //IMPRIME O TEXTO NO MONITOR SERIAL


display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C
display.clearDisplay(); // Clear the buffer.
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0,5);
display.println("FICA DUDU <3");

}


void loop(){

// PARÂMETROS SENSOR

valorLido = constrain(analogRead(pinoSensor),analogSoloMolhado,analogSoloSeco); //MANTÉM valorLido DENTRO DO INTERVALO (ENTRE analogSoloMolhado E analogSoloSeco)
valorLido = map(valorLido,analogSoloMolhado,analogSoloSeco,percSoloMolhado,percSoloSeco); //EXECUTA A FUNÇÃO "map" DE ACORDO COM OS PARÂMETROS PASSADOS

display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0,5);
display.println("FICA DUDU <3");
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0,28);
display.print("Sensor: ");
display.println(analogRead(A0));
Serial.print("Sensor: ");
Serial.println(analogRead(A0));
display.print("Umidade: ");
Serial.print("Umidade: ");
display.print(valorLido);
Serial.print(valorLido);
display.println("%");
Serial.println("%");
display.display();
delay(10000);
display.clearDisplay(); // Clear the buffer.
}
