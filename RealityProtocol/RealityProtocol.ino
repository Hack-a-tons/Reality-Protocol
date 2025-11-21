#include <HardwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include "config.h"            // Import your settings
#include "SmartWiFiConnect.h"  // Import WiFi logic

// --- HARDWARE DEFINITIONS ---
// UWB Remapped Pins for ESP32-WROVER
#define UWB_RX_PIN 4  
#define UWB_TX_PIN 18
HardwareSerial UWBSerial(2);

// LCD Address (Try 0x27 or 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);

int currentTagIndex = 0;

void setup() {
  Serial.begin(115200);
  
  // 1. Init LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Reality Protocol");
  lcd.setCursor(0, 1);
  lcd.print("Booting...");
  delay(1000);

  // 2. Smart WiFi Connection
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scanning WiFi...");
  
  String connectedSSID = connectToStrongestNetwork();
  
  lcd.clear();
  if (WiFi.status() == WL_CONNECTED) {
    lcd.setCursor(0, 0);
    lcd.print("WiFi: OK");
    lcd.setCursor(0, 1);
    lcd.print(connectedSSID);
    Serial.println("WiFi Connected: " + connectedSSID);
  } else {
    lcd.setCursor(0, 0);
    lcd.print("WiFi: FAILED");
    lcd.setCursor(0, 1);
    lcd.print("Offline Mode");
    Serial.println("WiFi Failed. Continuing in Offline Mode.");
  }
  delay(2000); // Let user read the status

  // 3. Init UWB
  UWBSerial.begin(115200, SERIAL_8N1, UWB_RX_PIN, UWB_TX_PIN);
  lcd.clear();
  lcd.print("Config UWB...");
  
  // Configure Local Module as ANCHOR
  sendAT("AT+MODE=1"); 
  delay(200);
  sendAT("AT+NETWORKID=FREENOVE");
  delay(200);
  sendAT("AT+ADDRESS=ANCHOR1");
  delay(500);

  lcd.clear();
}

void loop() {
  // Cycle through tags defined in config.h
  String targetTag = TARGET_TAGS[currentTagIndex];
  
  // 1. Ping the current tag
  // "2" is data length (just a ping), "Q" is dummy payload
  UWBSerial.print("AT+ANCHOR_SEND=");
  UWBSerial.print(targetTag);
  UWBSerial.println(",2,Q");

  // 2. Read Response
  String response = readResponse();
  
  // 3. Parse Distance
  int distIdx = response.indexOf("DIST:");
  
  lcd.setCursor(0, 0);
  lcd.print("Target: " + targetTag + "   "); // Spaces clear leftovers
  
  if (distIdx > 0) {
    // Parse logic
    int endIdx = response.indexOf(",", distIdx);
    String distStr = response.substring(distIdx + 5, endIdx);
    float distCm = distStr.toFloat();
    
    // Update LCD
    lcd.setCursor(0, 1);
    lcd.print("Dist: " + String(distCm) + " cm    ");
    Serial.println(targetTag + ": " + String(distCm) + "cm");
    
    // Note: We only switch tags if we DON'T find this one? 
    // Or switch constantly? 
    // For "Radar" effect, let's switch constantly to scan the room.
    currentTagIndex++;
  } else {
    lcd.setCursor(0, 1);
    lcd.print("Searching...    ");
    
    // Move to next tag to keep scanning
    currentTagIndex++;
  }

  // Loop index back to 0 if we hit the end of the array
  if (currentTagIndex >= NUM_TAGS) currentTagIndex = 0;
  
  delay(200); 
}

void sendAT(String cmd) {
  UWBSerial.println(cmd);
  while(UWBSerial.available()) UWBSerial.read(); 
}

String readResponse() {
  String r = "";
  long t = millis();
  while(millis() - t < 150) { // Wait 150ms for response
    while(UWBSerial.available()) r += (char)UWBSerial.read();
  }
  return r;
}