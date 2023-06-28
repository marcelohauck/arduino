#include <FirebaseESP32.h>
#include  <WiFi.h>
#include "DHT.h"


#define FIREBASE_HOST "https://mendigrow-default-rtdb.firebaseio.com"
#define WIFI_SSID "troco internet por picole" // Change the name of your WIFI
#define WIFI_PASSWORD "almeida2021" // Change the password of your WIFI
#define FIREBASE_Authorization_key "AIzaSyBW8bMJnTRuJ0sc8S3hD20ya6CJjBqSbkA"

#define DHTPIN 14    

#define DHTTYPE DHT22   
DHT dht(DHTPIN, DHTTYPE);

FirebaseData firebaseData;
FirebaseJson json;

void setup() {

 Serial.begin(115200);
  dht.begin();
   WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
   Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Firebase.begin(FIREBASE_HOST,FIREBASE_Authorization_key);
  
}

void loop() {
 
  float hum = dht.readHumidity();
  float temp = dht.readTemperature();  
  
   if (isnan(hum) || isnan(temp)  ){
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print("°C");
  Serial.print(" Humidity: ");
  Serial.print(hum);
  Serial.print("%");
  Serial.println();

  Firebase.setFloat(firebaseData, "/test/float", temp);
  Firebase.setFloat(firebaseData, "/test/int", hum);
   delay(200);
}
