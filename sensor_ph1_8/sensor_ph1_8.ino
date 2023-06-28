#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>

// Define software serial pins
#define RX_PIN 16
#define TX_PIN 17
#define RE 8
#define DE 7
// Create software serial object
SoftwareSerial mySerial(RX_PIN, TX_PIN);

// Initialize OLED display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  // Start serial communication
  Serial.begin(9600);

  // Start software serial communication
  mySerial.begin(9600);

  // Initialize OLED display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void loop() {
  // Request data from the sensor
  mySerial.write("01 03 00 00 00 07 85 DB");

  // Wait for sensor to respond
  delay(1000);

  // Check if sensor data is available
  if (mySerial.available() > 0) {
    // Read sensor data
    byte data[17];
    for (int i = 0; i < 17; i++) {
      data[i] = mySerial.read();
    }

    // Calculate sensor values
    float ph = (float)((data[3] << 8) | data[4]) / 100.0;
    float temp = (float)((data[5] << 8) | data[6]) / 100.0;
    float hum = (float)((data[7] << 8) | data[8]) / 100.0;
    float nitrogen = (float)((data[9] << 8) | data[10]) / 100.0;
    float phosphorus = (float)((data[11] << 8) | data[12]) / 100.0;
    float calcium = (float)((data[13] << 8) | data[14]) / 100.0;

    // Print sensor values to serial monitor
    Serial.print("pH: ");
    Serial.print(ph);
    Serial.print(", Temperature: ");
    Serial.print(temp);
    Serial.print(", Humidity: ");
    Serial.print(hum);
    Serial.print(", Nitrogen: ");
    Serial.print(nitrogen);
    Serial.print(", Phosphorus: ");
    Serial.print(phosphorus);
    Serial.print(", Calcium: ");
    Serial.println(calcium);

    // Display sensor values on OLED display
    display.clearDisplay();
    display.setCursor(0, 5);
    display.print("pH: ");
    display.print(ph);
    display.print(", Temperature: ");
    display.print(temp);
    display.print(", Humidity: ");
    display.print(hum);
    display.print(", Nitrogen: ");
    display.print(nitrogen);
    display.print(", Phosphorus: ");
    display.print(phosphorus);
    display.print(", Calcium: ");
    display.print(calcium);
    display.display();
  }

  // Wait before requesting sensor data again
  delay(5000);
}