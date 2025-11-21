#ifndef SMART_WIFI_CONNECT_H
#define SMART_WIFI_CONNECT_H

#include <WiFi.h>
#include "config.h"

String connectToStrongestNetwork() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("Scanning for WiFi networks...");
  int n = WiFi.scanNetworks();
  
  if (n == 0) {
    Serial.println("No networks found.");
    return "";
  }

  Serial.print(n);
  Serial.println(" networks found.");

  int bestSignal = -999;
  int bestNetworkIndex = -1;
  int configIndex = -1;

  // Loop through scanned networks
  for (int i = 0; i < n; ++i) {
    String scanSSID = WiFi.SSID(i);
    int scanRSSI = WiFi.RSSI(i);
    
    // Check if this scanned network is in our config
    for (int j = 0; j < NUM_NETWORKS; j++) {
      if (scanSSID == KNOWN_NETWORKS[j].ssid) {
        Serial.print("Match found: ");
        Serial.print(scanSSID);
        Serial.print(" (");
        Serial.print(scanRSSI);
        Serial.println("dBm)");

        // If this is the strongest match so far, save it
        if (scanRSSI > bestSignal) {
          bestSignal = scanRSSI;
          bestNetworkIndex = i;
          configIndex = j;
        }
      }
    }
  }

  if (bestNetworkIndex != -1) {
    Serial.print("Connecting to strongest match: ");
    Serial.println(KNOWN_NETWORKS[configIndex].ssid);
    
    WiFi.begin(KNOWN_NETWORKS[configIndex].ssid, KNOWN_NETWORKS[configIndex].password);
    
    int retries = 0;
    while (WiFi.status() != WL_CONNECTED && retries < 20) {
      delay(500);
      Serial.print(".");
      retries++;
    }
    Serial.println("");
    
    if (WiFi.status() == WL_CONNECTED) {
      return String(KNOWN_NETWORKS[configIndex].ssid);
    }
  }

  return "";
}

#endif
