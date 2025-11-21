#include <HardwareSerial.h>
#include <LiquidCrystal_I2C.h> 

// 1. Setup UWB on REMAPPED pins for WROVER (RX=4, TX=18)
// We use "Serial2" but map it to custom pins
HardwareSerial UWBSerial(2); 
#define UWB_RX_PIN 4  // Connect UWB TX to this
#define UWB_TX_PIN 18 // Connect UWB RX to this

// 2. Setup LCD (Address 0x27 is common, if it fails try 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);
  
  // --- Init LCD ---
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Booting...");

  // --- Init UWB ---
  // Important: Define the custom pins here
  UWBSerial.begin(115200, SERIAL_8N1, UWB_RX_PIN, UWB_TX_PIN);
  delay(1000);

  lcd.setCursor(0, 0);
  lcd.print("Config Anchor...");
  
  // --- Configure Local Module as ANCHOR ---
  sendAT("AT+MODE=1"); 
  delay(500);
  sendAT("AT+NETWORKID=FREENOVE");
  delay(500);
  sendAT("AT+ADDRESS=ANCHOR1");
  delay(500);

  lcd.clear();
  lcd.print("Radar Active");
}

void loop() {
  // --- 1. Ping the Tag ---
  // We assume the other module is named TAG001
  UWBSerial.println("AT+ANCHOR_SEND=TAG001,5,PING");

  // --- 2. Read Response ---
  String response = readResponse();
  
  // --- 3. Parse Distance ---
  // Response format: +ANCHOR_RCV:TAG001,5,DIST:150.5,PING
  int distIdx = response.indexOf("DIST:");
  
  lcd.setCursor(0, 0);
  lcd.print("Target: TAG001");
  
  if (distIdx > 0) {
    // Find the comma after the distance to isolate the number
    int endIdx = response.indexOf(",", distIdx);
    String distStr = response.substring(distIdx + 5, endIdx);
    float distCm = distStr.toFloat();
    
    // Update LCD
    lcd.setCursor(0, 1);
    lcd.print("Dist: " + String(distCm) + " cm    "); // Extra spaces clear old text
    
    // Print to Serial for debugging
    Serial.println("Distance: " + String(distCm) + "cm");
  } else {
    // Tag not found
    lcd.setCursor(0, 1);
    lcd.print("Searching...    ");
  }
  
  delay(100); // Fast refresh
}

void sendAT(String cmd) {
  UWBSerial.println(cmd);
  // Clear buffer to keep it clean
  while(UWBSerial.available()) UWBSerial.read(); 
}

String readResponse() {
  String r = "";
  long t = millis();
  // Wait 100ms for a reply
  while(millis() - t < 100) { 
    while(UWBSerial.available()) r += (char)UWBSerial.read();
  }
  return r;
}