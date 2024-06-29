




// Sender 




#include <ESP8266WiFi.h>
#include <espnow.h>
#include<DHT.h>

#define DHTPIN 16   //D0
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);


int w = 14;   //D5
int x = 12;   //D6
int y = 13;   //D7
int z = 3;  //RX
int light = 2; // D4

// REPLACE WITH RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x40, 0x91, 0x51, 0x55, 0xA4, 0x76};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a[32];
  String st;
  float t;
  String sh;
  float h;

  // CAR data string

  String L;
  String R;
  String F;
  String B;
  String S;
  String Light;
  
  
  
} struct_message;

// Create a struct_message called myData
struct_message myData;

unsigned long lastTime = 0;  
unsigned long timerDelay = 1;  // send readings timer

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  //Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    //Serial.println("Delivery success");
     // Car sending dat variable

     if (digitalRead(w) == LOW) {
      Serial.println("LEFT");
      myData.L = "L";
  } 

     else if (digitalRead(x) == LOW) {
      Serial.println("RIGHT");
      myData.R = "R";
  } 

     else if (digitalRead(y) == LOW) {
      Serial.println("FORWARD");
      myData.F = "F";
  } 

     else if (digitalRead(z) == LOW) {
      Serial.println("BACKWARD");
      myData.B = "B";
  } 

     else if (digitalRead(light) == LOW) {
      Serial.println("LIGHT");
      myData.Light = "Light";
  } 

     else{
      Serial.println("STOP");
      myData.S = "S";
  } 
  }
  else{
    Serial.println("Delivery fail");
  }
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  dht.begin();
  
  pinMode(w,INPUT_PULLUP);
  pinMode(x,INPUT_PULLUP);
  pinMode(y,INPUT_PULLUP);
  pinMode(z,INPUT_PULLUP);
 
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
  //   float humidity = dht.readHumidity();
  // float temperature = dht.readTemperature();
  
 // if ((millis() - lastTime) >= timerDelay) {
    // Serial.print("Temperature = ");
    // Serial.println(temperature);
    // Serial.print("Humidity = ");
    // Serial.println(humidity);
    // Set values to send
    // strcpy(myData.a, "Temperature and Humidity");
    // myData.st = "Temperature";
    // myData.t = temperature;
    // myData.sh = "Humidity";
    // myData.h = humidity;


    

    // Send message via ESP-NOW
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

//     lastTime = millis();
//  }
}