/*
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/  
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/



    // ESP32 RECEIVER

#include <esp_now.h>
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
     char a[32];
    String st;
    float t;
    String sh;
    float h;

    int pm1;
    String spm1;
    int pm25;
    String spm25;
    int pm10;
    String spm10;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received

 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
   pinMode(2, OUTPUT);

   lcd.init();
  lcd.backlight();
  lcd.clear(); // Clear the LCD screen
  lcd.setCursor(0, 1);
  lcd.cursor();  
  lcd.print(" Connecting ");
  lcd.setCursor(0, 2);
  lcd.print("  TO SENDER... ");
  delay(900);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

   Serial.println();
    digitalWrite(2, HIGH); 
    Serial.print("Board MAC Address:  ");
    //WiFi.mode(WIFI_AP_STA);
    Serial.println(WiFi.macAddress());

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    lcd.clear(); // Clear the LCD screen
    digitalWrite(2, LOW); 
    lcd.setCursor(0, 1);
    lcd.print(" CONNECTION");
    lcd.setCursor(0, 2);
    lcd.print("  FAILED ");
    delay(60);
    digitalWrite(2, HIGH); 
    delay(60);
    digitalWrite(2, LOW); 
    delay(900);
    return;
  }
  // else{
  //    lcd.clear(); // Clear the LCD screen
  //   digitalWrite(2, HIGH); 
  //   lcd.setCursor(0, 1);
  //   lcd.print(" CONNECTED");
  // }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}


void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  //lcd.clear();
  Serial.print("Temperature: ");
  Serial.print(myData.t);
  Serial.print(" ");
  Serial.print((char)223);
  Serial.println("C");

  Serial.print("Humidity: ");
  Serial.print(myData.h);
  Serial.println(" %");

    Serial.print("PM1.0 :");
    Serial.print(myData.pm1);
    Serial.println("(ug/m3)");

    Serial.print("PM2.5 :");
    Serial.print(myData.pm25);
    Serial.println("(ug/m3)");

    Serial.print("PM10 :");
    Serial.print(myData.pm10);
    Serial.println("(ug/m3)");

  Serial.println();

  lcdDisplay();
}

void lcdDisplay(){
  lcd.clear(); // Clear the LCD screen
  delay(9);
  lcd.setCursor(0, 0);
  lcd.print("Tem: ");
  lcd.print(myData.t,1);
  
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(12, 0);
  lcd.print("Hum: ");
  lcd.print(myData.h,0);
  lcd.print("%");

    lcd.setCursor(0, 1);
    lcd.print("PM1.0: ");
    lcd.print(myData.pm1);
    lcd.setCursor(13, 1);
    lcd.print("(ug/m3)");

    lcd.setCursor(0, 2);
    lcd.print("PM2.5: ");
    lcd.print(myData.pm25);
    lcd.setCursor(13, 2);
    lcd.print("(ug/m3)");

    lcd.setCursor(0, 3);
    lcd.print("PM10 : ");
    lcd.print(myData.pm10);
    lcd.setCursor(13, 3);
    lcd.print("(ug/m3)");
    delay(300);

}
 
void loop() {

}