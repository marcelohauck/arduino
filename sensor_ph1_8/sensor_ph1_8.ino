#include <Wire.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"

#define b2 A2  // solenoide bomba
#define b3 A3  // solenoide tanque
#define RXD2 16
#define TXD2 17
#define RE 8
#define DE 7
#define RX_BUFF_SIZE 30
#define DHTPIN 9
#define DHTTYPE DHT22

// Create the SSD1306 display object
const byte SCREEN_WIDTH = 128;  // OLED display width, in pixels
const byte SCREEN_HEIGHT = 64;  // OLED display height, in pixels
const int OLED_RESET = -1;      // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

DHT dht(DHTPIN, DHTTYPE);

enum RS485States {
  ST_IDLE = 0,
  ST_RXDATA,
  ST_CHECKDATA

};

const uint16_t crc16_tab[] PROGMEM = {
  0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
  0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
  0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
  0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
  0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
  0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
  0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
  0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
  0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
  0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
  0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
  0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
  0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
  0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
  0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
  0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
  0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
  0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
  0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
  0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
  0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
  0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
  0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
  0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
  0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
  0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
  0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
  0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
  0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
  0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
  0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
  0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};

const byte requestframe[] = { 0x01, 0x03, 0x00, 0x00, 0x00, 0x07 };  //CRC is added in TX logic

uint8_t values[RX_BUFF_SIZE];
char
  szStr[10];

SoftwareSerial mod(2, 3);  // RX, TX ( Creates a new SoftwareSerial object ) (for Uno)
//SoftwareSerial mod(10,3); // RX, TX ( Creates a new SoftwareSerial object ) (for Mega2560)

void setup() {
  Serial.begin(115200);
  Serial.println("Mendigrow 1.8");
  mod.begin(4800);
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);
  pinMode(b2, OUTPUT);
  pinMode(b3, OUTPUT);
  digitalWrite(b2, HIGH);
  digitalWrite(b3, HIGH);
  dht.begin();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C
  display.clearDisplay();                     // Clear the buffer.
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 5);
  display.display();
  // Initialize the display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

}  //setup

void loop() {


  ModSM();

}  //loop


void ModSM(void) {
  static uint32_t
    tFrameTimeout = 0ul,
    tSM = 0ul;
  static uint8_t
    dataLen,
    rxIdx,
    state = ST_IDLE;
  uint16_t
    crc;
  uint32_t
    tNow = millis();
  uint8_t
    ch;

  switch (state) {
    case ST_IDLE:
      if ((tNow - tSM) >= 3000ul) {
        //flush any stray characters from RX buffer
        while (mod.available())
          mod.read();

        tSM = tNow;
        digitalWrite(DE, HIGH);
        digitalWrite(RE, HIGH);

        delayMicroseconds(100);
        crc = CRC16_Modbus(requestframe, sizeof(requestframe));
        mod.write(requestframe, sizeof(requestframe));
        mod.write(crc & 0xff);
        mod.write((crc >> 8) & 0xff);
        // delay(20);  //allow msg to go before switching DE & RE
        delayMicroseconds(100);  //allow msg to go before switching DE & RE
        digitalWrite(DE, LOW);
        digitalWrite(RE, LOW);

        tFrameTimeout = tNow;  //init message timeout
        rxIdx = 0;
        dataLen = 0;
        state = ST_RXDATA;

      }  //if

      break;

    case ST_RXDATA:
      if (mod.available()) {
        ch = mod.read();
        switch (rxIdx) {
          case 0:  //address code
            //an attempt to filter leading zeros in reply
            //look for 0x01 address code
            if (ch == 0x01) {
              //got the address code
              values[rxIdx++] = ch;

            }  //if
            break;

          case 1:  //function code
            //if function code is not 0x03, we're misaligned
            if (ch != 0x03)
              rxIdx = 0;
            else
              values[rxIdx++] = ch;
            break;

          case 2:
            //number of data + CRC bytes to follow
            dataLen = ch + 2;  // <--- change this line to add '2' to the dataLength
            values[rxIdx++] = ch;
            break;

          default:
            //RX data and CRC
            values[rxIdx++] = ch;
            if (rxIdx == RX_BUFF_SIZE)
              rxIdx--;

            dataLen--;
            if (dataLen == 0)
              state = ST_CHECKDATA;

            break;

        }  //switch rx index

      }  //if available

      //if we haven't completed the message after 1.5-seconds just return to idle state
      if ((tNow - tFrameTimeout) >= 1500ul) {
        Serial.println("Message Timeout");
        state = ST_IDLE;

      }  //if

      break;

    case ST_CHECKDATA:
      //show RXed message
      // Serial.print( "Msg: " );
      for (crc = 0; crc < rxIdx; crc++) {
        sprintf(szStr, "%02X", values[crc]);
        //    Serial.print( szStr );

      }  //for
      Serial.println();

      //check the CRC
      crc = CRC16_Modbus(values, values[2] + 3);  //+2 for header, +1 for value[2] itself
      if (((uint8_t)(crc & 0x0ff) == values[rxIdx - 2]) && ((uint8_t)((crc >> 8) & 0x0ff) == values[rxIdx - 1])) {
        //msg is good; print results
        PrintData();

      }  //if
      else {
        Serial.println("Bad CRC");

      }  //else

      state = ST_IDLE;

      break;


  }  //switch

}  //ModSM

uint16_t CRC16_Modbus(uint8_t *pData, uint8_t Len) {
  //CRC16 calculation
  uint16_t crc = 0xFFFF;

  while (Len--)
    crc = (crc >> 8) ^ (pgm_read_word(&crc16_tab[(crc ^ *pData++) & 0xFF]));

  return crc;

}  //CRC16_Modbus+                                                                                            gwg ygqqqqqy



void PrintData(void) {
  int16_t val;

  //0 index 3     moisture content
  val = (values[3] << 8) + values[4];
  float humidityVal = (float)val / 10.0;  // Declare a new variable to keep the humidity value
  Serial.print("Soil Humidity....: ");
  Serial.print(humidityVal, 1);
  Serial.println("%");

  // Print to the display
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Soil Humidity....: ");
  display.print(humidityVal, 1);
  display.println("%");
  display.display();

  // Print messages based on the humidity value
  if (humidityVal > 70.0) {
    Serial.println("Umidade alta");
    display.println("Umidade alta");
    display.display();
    delay(60000);
  } else if (humidityVal < 25.0) {
    Serial.println("Umidade baixa, irrigação ativa");
    display.println("Umidade baixa, irrigação ativa");
    display.display();
    while (humidityVal < 62.0) {
      digitalWrite(b2, LOW);
      digitalWrite(b3, LOW);
      delay(300000);

      ModSM();                             // Update the values array
      val = (values[3] << 8) + values[4];  // Recalculate val
      humidityVal = (float)val / 10.0;     // Recalculate humidityVal

      Serial.print("Soil Humidity....: ");
      Serial.print(humidityVal, 1);
      Serial.println("%");
      display.print("Soil Humidity....: ");
      display.print(humidityVal, 1);
      display.println("%");
      display.display();
      digitalWrite(b2, HIGH);
      digitalWrite(b3, HIGH);
      Serial.println("irrigação pausada");
      display.println("irrigação pausada");
      display.display();
      delay(3300000);

      ModSM();                             // Update the values array
      val = (values[3] << 8) + values[4];  // Recalculate val
      humidityVal = (float)val / 10.0;     // Recalculate humidityVal

      Serial.print("Soil Humidity....: ");
      Serial.print(humidityVal, 1);
      Serial.println("%");
      display.print("Soil Humidity....: ");
      display.print(humidityVal, 1);
      display.println("%");
      display.display();
    }
  }

  //1 index 5     temperature
  val = (values[5] << 8) + values[6];
  Serial.print("Soil Temperature.: ");
  Serial.print((float)val / 10.0, 1);
  Serial.println("oC");
  display.print("Soil Temperature.: ");
  display.print((float)val / 10.0, 1);
  display.println("oC");
  display.display();

  //3 index 9     PH
  val = (values[9] << 8) + values[10];
  float phVal = (float)val / 10.0;
  Serial.print("pH..........: ");
  Serial.print(phVal);
  Serial.println("");
  display.print("pH..........: ");
  display.print(phVal);
  display.println("");
  display.display();

  // Print messages based on the humidity value
  if (phVal > 7.0) {
    Serial.println("PH alcalino");
    display.println("PH alcalino");
    display.display();
  } else if (phVal < 5.8) {
    Serial.println("PH ácido");
    display.println("PH ácido");
    display.display();
  }

  //4 index 11    nitrogen content
  val = (values[11] << 8) + values[12];
  Serial.print("Nitrogen....: ");
  Serial.print(val);
  Serial.println("mg/kg");
  display.print("Nitrogen....: ");
  display.print(val);
  display.println("mg/kg");
  display.display();

  //5 index 13    phosphorus content
  val = (values[13] << 8) + values[14];
  Serial.print("Phosphorus..: ");
  Serial.print(val);
  Serial.println("mg/kg");
  display.print("Phosphorus..: ");
  display.print(val);
  display.println("mg/kg");
  display.display();

  //6 index 15    potassium content
  val = (values[15] << 8) + values[16];
  Serial.print("Potassium...: ");
  Serial.print(val);
  Serial.println("mg/kg");
  display.print("Potassium...: ");
  display.print(val);
  display.println("mg/kg");
  display.display();

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    display.println(F("Failed to read from DHT sensor!"));
    display.display();
    return;
  }

  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);
  Serial.print("Air humidity...: ");
  Serial.print(h);
  Serial.println("%");
  display.print("Air humidity...: ");
  display.print(h);
  display.println("%");
  display.display();
  Serial.print("Air temperature...: ");
  Serial.print(t);
  Serial.println(F(" c"));
  display.print("Air temperature...: ");
  display.print(t);
  display.println(F(" c"));
  display.display();

  delay(600000);
}  //PrintData
