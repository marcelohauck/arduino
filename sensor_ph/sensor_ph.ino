#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// For the i2c supported Oled display module which is 128x64
#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define RE 8
#define DE 7

const byte O2[] = {0x01 , 0x03 , 0x00 , 0x00 , 0x00 , 0x02 , 0xC4 , 0x0B};

byte values[20];
SoftwareSerial mod(2, 3);

void setup() {
  Serial.begin(4800);
  mod.begin(4800);
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //initialize with the I2C addr 0x3C (128x64)
  delay(500);
  display.clearDisplay();
  display.setCursor(25, 15);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println(" Soil Sensor");
  display.setCursor(25, 35);
  display.setTextSize(1);
  display.print("Initializing");
  display.display();
  delay(2000);
}

void loop() {
  float val1 ;
  float val2 ;
  Calculate();
  val1 = ((values[5] * 256) + values[6]) * 0.1;
  display.clearDisplay();
  Serial.print("temperatura: ");
  Serial.print(val1);
  Serial.print(" C   ");
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 1);
  display.println("Parametros");
  display.setTextSize(1);
  display.setCursor(0, 25);
  display.print("temperatura: ");
  display.print(val1);
  display.print("C ");
  val2 = ((values[3] * 256) + values[4]) * 0.1;
  Serial.print("umidade: ");
  Serial.print(val2);
  Serial.println(" %   ");
  display.setTextSize(1);
  display.setCursor(0, 40);
  display.print("umidade: ");
  display.print(val2);
  display.print("% ");
  display.display();
  delay(30000);
}

byte Calculate() {
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  if (mod.write(O2, sizeof(O2)) == 8) {
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    for (byte i = 0; i < 11; i++) {
      //Serial.print(mod.read(),HEX);
      values[i] = mod.read();
      //Serial.print(values[i], HEX);
      //Serial.print(" ");
    }
    Serial.println();
  }
  return values[6];
}
