   

    // ESP 01
    // Receiver  


#include <Wire.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <LiquidCrystal_I2C.h>

//LiquidCrystal_I2C lcd(0x27, 20, 4);

LiquidCrystal_I2C lcd(0x27, 16, 2); // for 16*2 LCD I2C

// #include <LiquidCrystal.h>  // for no I2C

//LiquidCrystal lcd(4, 0, 2, 14, 12, 13); // RS=D2, EN=D3, D4=D4, D5=D5, D6=D6, D7=D7 pin configuration

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    char a[32];
    String st;
    float t;
    String sh;
    float h;

    float pm1;
    String spm1;
    float pm25;
    String spm25;
    float pm10;
    String spm10;

} struct_message;

// Create a struct_message called myData
struct_message myData;

 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  Wire.begin(1,3);
  // Wire.begin(0,2); // Initialize I2C communication with SDA on GPIO 14 and SCL on GPIO 15
 // pinMode(LED_BUILTIN, OUTPUT);
 pinMode(1, OUTPUT);
  //lcd.begin(16, 2); // for no I2C // Initialize the LCD columns and rows

  lcd.init();
  lcd.backlight();
  lcd.clear(); // Clear the LCD screen
  lcd.setCursor(0, 0);
  //lcd.setCursor(0, 1);
  lcd.cursor();  
  lcd.print(" Connecting ");
  lcd.setCursor(0, 1);
  //lcd.setCursor(0, 2);
  lcd.print("  TO SENDER... ");
  digitalWrite(1, LOW); 
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
    Serial.println();
    // digitalWrite(LED_BUILTIN, HIGH); 
    
    Serial.print("Board MAC Address:  ");
    //WiFi.mode(WIFI_AP_STA);
    Serial.println(WiFi.macAddress());
    delay(900);
     digitalWrite(1, HIGH);
  

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    lcd.clear(); // Clear the LCD screen
    // digitalWrite(LED_BUILTIN, LOW); 
    
    digitalWrite(1, HIGH);
    lcd.setCursor(0, 1);
    lcd.print(" CONNECTION");
    lcd.setCursor(0, 2);
    lcd.print("  FAILED ");
    delay(60);
    //digitalWrite(LED_BUILTIN, HIGH); 
    digitalWrite(1, LOW);
    delay(60);
    //digitalWrite(LED_BUILTIN, LOW); 
    digitalWrite(1, HIGH);
    delay(900);
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
  //  digitalWrite(LED_BUILTIN, HIGH); 
  //   delay(60);
  //   digitalWrite(LED_BUILTIN, LOW); 
}

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  digitalWrite(1, LOW); 
  //   delay(60);
  //   digitalWrite(LED_BUILTIN, HIGH);
  Serial.print("Bytes received: ");
  Serial.println(len);
  lcd.clear();
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
  delay(30);
  lcd.setCursor(0, 0);
  lcd.print("T:");
  //lcd.print(myData.t,1);
  lcd.print(myData.t,0);
  lcd.print((char)223);
  lcd.print("C");

  lcd.setCursor(8, 0);
  // lcd.setCursor(12, 0);
  lcd.print("H:");
  lcd.print(myData.h,0);
  lcd.print("%");

    lcd.setCursor(0, 1);
    lcd.print("PM1.0:");
    lcd.print(myData.pm1);
    lcd.print("(ug/m3)");

     //lcd.setCursor(0, 2);
    // lcd.print("PM2.5 : ");
    // lcd.print(myData.pm25);
    // lcd.print("(ug/m3)");

    // lcd.setCursor(0, 3);
    // lcd.print("PM10 : ");
    // lcd.print(myData.pm10);
    // lcd.print("(ug/m3)");


}


void loop() {
  
}

