#include <Arduino.h>
#include <WiFi.h>
#include "driver/uart.h"
#include <Firebase_ESP_Client.h>
#include "time.h"

#define RXD2 16
#define TXD2 17

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -10800;   //Replace with your GMT offset (seconds)
const int   daylightOffset_sec = 21600;  //Replace with your daylight offset (seconds)
void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "troco internet por picole"
#define WIFI_PASSWORD "almeida2021"

// Insert Firebase project API Key
#define API_KEY "AIzaSyBW8bMJnTRuJ0sc8S3hD20ya6CJjBqSbkA"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://mendigrow-default-rtdb.firebaseio.com"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

   //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
 // if (Firebase.signUp(&config, &auth, "marcelohauck@gmail.com", "Mh-130182")) {
    Serial.println("ok");
    signupOK = true;
  }
  else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {

  while (Serial2.available() == 0) {}     //wait for data available
  String arduino = Serial.readString();  //read until timeout
  arduino.trim();   
  
   if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    // Write an Int number on the database path test/int
    if (Firebase.RTDB.setString(&fbdo, "Ar/temperatura", arduino)){
      printLocalTime();
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
      Serial.println(arduino);

    }
    else {
      printLocalTime();
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  }
  }
