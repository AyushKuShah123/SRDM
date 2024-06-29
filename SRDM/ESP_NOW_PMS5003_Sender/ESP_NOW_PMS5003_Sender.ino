

//// SENDER


#include <ESP8266WiFi.h>
#include <espnow.h>
#include<DHT.h>
#include "PMS.h"
#include "SoftwareSerial.h"


#define DHTPIN 2   //D4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

SoftwareSerial Serial1(3,1); // RX, TX
 
PMS pms(Serial1);
PMS::DATA data;

// REPLACE WITH RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xAC, 0x0B, 0xFB, 0xD9, 0x1D, 0x02};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a[32];
  String st;
  float t;
  String sh;
  float h;
  String lcdd;

  float pm1;
  String spm1;
  float pm25;
  String spm25;
  float pm10;
  String spm10;
  
  
} struct_message;

// Create a struct_message called myData
struct_message myData;

unsigned long lastTime = 0;  
unsigned long timerDelay = 900;  // send readings timer

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  dht.begin();
  
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}
 
void loop() {
    float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  float PM1=String(data.PM_AE_UG_1_0);
  float PM25=String(data.PM_AE_UG_2_5);
  float PM10=String(data.PM_AE_UG_10_0);
  
  if ((millis() - lastTime) > timerDelay) {
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

    // Set values to send
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
    
    

    // Send message via ESP-NOW
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

    lastTime = millis();
  }
}