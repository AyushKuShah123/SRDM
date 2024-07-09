 /*Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/
#include <esp_now.h>
#include <WiFi.h>
#include<DHT.h>

#define DHTPIN 5   //D5
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
// uint8_t broadcastAddress[] = {0x08, 0xF9, 0xE0, 0x7E, 0x6C, 0x28};
//uint8_t broadcastAddress[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a[32];
  String st;
  float t;
  String sh;
  float h;
  //String lcdd;
  int pm1;
  String spm1;
  int pm25;
  String spm25;
  int pm10;
  String spm10;
} struct_message;

// Create a struct_message called myData
struct_message myData;

unsigned long lastTime = 0;  
unsigned long timerDelay = 1500;  // send readings timer

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
   Serial2.begin(9600, SERIAL_8N1, 16, 17); // Initialize Serial2 for PMS7003 with custom pins
  delay(30);           // Wait for 3 seconds to allow the sensor to initialize
  Serial.println("Serial and Serial2 initialized");
  dht.begin();
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
  // Set values to send
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
   //if ((millis() - lastTime) > timerDelay) {
  //  if (Serial2.available() >= 32) { // Ensure at least 32 bytes are available to read
  //  // Serial.println("Data available on Serial2");

    // Check for start bytes
    if (Serial2.read() == 0x42) {
      if (Serial2.read() == 0x4D) {
        byte buffer[32];
        Serial2.readBytes(buffer, 32);

        int PM1 = buffer[10] * 256 + buffer[11];
        int PM25 = buffer[12] * 256 + buffer[13];
        int PM10 = buffer[14] * 256 + buffer[15];

        strcpy(myData.a, "Temperature and Humidity");
          myData.st = "Temperature";
          myData.t = temperature;
          myData.sh = "Humidity";
          myData.h = humidity;

          myData.spm1 = "PM1.0";
          myData.pm1 = PM1;
          myData.spm25 = "PM2.5";
          myData.pm25 = PM25;
          myData.spm10 = "PM10.0";
          myData.pm10 = PM10;

          Serial.print("Temperature = ");
          Serial.println(temperature);
          Serial.print("Humidity = ");
          Serial.println(humidity);

          
        
          

          Serial.print("PM1.0 :");
          Serial.println(PM1);
          Serial.print("PM2.5 :");
          Serial.println(PM25);
          Serial.print("PM10 :");
          Serial.println(PM10);
          //delay(30);
   
      //}
      }
    }
   
        // Set values to send
          

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   if (result != ESP_OK) {
    Serial.println("Error sending the data");
  
  }

     
  
 //lastTime = millis();
    delay(30);

  
}