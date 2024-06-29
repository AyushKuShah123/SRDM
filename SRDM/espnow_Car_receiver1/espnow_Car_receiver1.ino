


//Receiver


/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp8266-nodemcu-arduino-ide/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <ESP8266WiFi.h>
#include <espnow.h>

int w = 14;   //D5
int x = 12;   //D6
int y = 13;   //D7
int z = 3;  //RX
int light = 2; // D4

// Structure example to receive data
// Must match the sender structure
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

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  

    //    // Car sending dat variable

     if (myData.L == "L") {
      Serial.println("LEFT");
       digitalWrite(w, HIGH);
      digitalWrite(x, LOW);
      digitalWrite(y, LOW);
      digitalWrite(z, LOW);
      
     
  } 

     else if (myData.R == "R") {
      Serial.println("RIGHT");
      digitalWrite(w, LOW);
      digitalWrite(x, LOW);
      digitalWrite(y, HIGH);
      digitalWrite(z, LOW);
      
      
  } 

     else if (myData.F == "F") {
      Serial.println("FORWARD");
      digitalWrite(w, HIGH);
      digitalWrite(x, LOW);
      digitalWrite(y, HIGH);
      digitalWrite(z, LOW);
      
  } 

     else if (myData.B == "B") {
      Serial.println("BACKWARD");
       digitalWrite(w, LOW);
      digitalWrite(x, HIGH);
      digitalWrite(y, LOW);
      digitalWrite(z, HIGH);
      
      
  } 

     else if (myData.Light == "Light") {
      Serial.println("LIGHT");
      digitalWrite(light, LOW);
      
  } 

     else{
      Serial.println("STOP");
       digitalWrite(w, LOW);
      digitalWrite(x, LOW);
      digitalWrite(y, LOW);
      digitalWrite(z, LOW);
      
  } 


  // Serial.print("Bytes received: ");
  // Serial.println(len);
  // Serial.print("Temperature: ");
  // Serial.print(myData.t);
  // Serial.print(" ");
  // Serial.print((char)223);
  // Serial.println("C");
  // Serial.print("Humidity: ");
  // Serial.print(myData.h);
  // Serial.println(" %");
  
 
  
    

  

  
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
   pinMode(w,OUTPUT);
  pinMode(x,OUTPUT);
  pinMode(y,OUTPUT);
  pinMode(z,OUTPUT);
 
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  Serial.println();
   Serial.print("Board MAC Address:  ");
  //WiFi.mode(WIFI_AP_STA);
  Serial.println(WiFi.macAddress());

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);

    
   
}

void loop() {
  
}
