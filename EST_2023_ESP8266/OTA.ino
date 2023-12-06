
#include <ESPAsync_WiFiManager.h>  

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>

#include <EEPROM.h>

#define EEPROM_SIZE 512

struct WiFiCredentials {
  char ssid[32];
  char password[64];
};
#define AP_ONLY 1
#define WiFi_AP 0


WiFiCredentials wifiCredentials;

void saveCredentialsToEEPROM() {
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.put(0, wifiCredentials);
  EEPROM.commit();
  EEPROM.end();
}

void loadCredentialsFromEEPROM() {
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(0, wifiCredentials);
  EEPROM.end();
}

#include <ESPAsyncDNSServer.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <AsyncTCP.h>

#endif

#include <AsyncElegantOTA.h>

const char* ElegantOTAUser = "admin";
const char* ElegantOTAPassword = "Yonova2020";


// const char* ssid = "........";
// const char* password = "........";
#ifdef ESP8266
String _hostname = "Yonova-" + String(ESP.getChipId(), HEX);
#else  //ESP32
String _hostname = "Yonova-" + String(ESP_getChipId(), HEX);
#endif


AsyncWebServer YONOVAwebServer(80);

// AsyncWebServer server(80);
const byte DNS_PORT = 53;
AsyncDNSServer dnsServer;
AsyncDNSServer dns;
// void setup() {
//   YonovaOTASetup("EST2023");
// }

void YonovaOTASetup(String ssid, boolean WiFiType) {
  // put your setup code here, to run once:
  // Serial.begin(115200);
  // while (!Serial)
  //   ;
  delay(200);

  if (WiFiType == WiFi_AP) {

    ESPAsync_WiFiManager ESPAsync_wifiManager(&YONOVAwebServer, &dnsServer, "AutoConnectAP");
    //ESPAsync_wifiManager.resetSettings();   //reset saved settings
    //ESPAsync_wifiManager.setAPStaticIPConfig(IPAddress(192,168,186,1), IPAddress(192,168,186,1), IPAddress(255,255,255,0));
    _hostname.toUpperCase();
    // WiFi.begin();
#if defined(ESP8266)
    loadCredentialsFromEEPROM();
    // Connect to Wi-Fi network
    WiFi.begin(wifiCredentials.ssid, wifiCredentials.password);

#elif defined(ESP32)
    WiFi.begin();
#endif
    for (int i = 0; i < 150; i++) {
      delay(100);
      if (WiFi.status() == WL_CONNECTED) {
        break;
        Serial.print(F("Connected"));
      }
    }

    if (WiFi.status() != WL_CONNECTED) {
      ESPAsync_wifiManager.autoConnect(_hostname.c_str());
    }
    // ESPAsync_wifiManager.autoConnect(_hostname.c_str());
    if (WiFi.status() == WL_CONNECTED) {
#if defined(ESP8266)
      // Save credentials to EEPROM
      strncpy(wifiCredentials.ssid, WiFi.SSID().c_str(), sizeof(wifiCredentials.ssid));
      strncpy(wifiCredentials.password, WiFi.psk().c_str(), sizeof(wifiCredentials.password));
      saveCredentialsToEEPROM();
#endif
      // Serial.print(F("Connected. Local IP: "));
      // Serial.println(WiFi.localIP());
    } else {
      // Serial.println(ESPAsync_wifiManager.getStatus(WiFi.status()));
    }
  }
  AsyncElegantOTA.begin(&YONOVAwebServer, ElegantOTAUser, ElegantOTAPassword);  // Start AsyncElegantOTA
  YONOVAwebServer.begin();
  // Serial.println("HTTP server started");
  // delay(3000);

  IPAddress apIP(192, 168, 4, 1);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid.c_str());
  // delay(5000);

  // dnsServer.stop();
  // start DNS server for a specific domain name
  dns.start(DNS_PORT, "*", apIP);
}

// void loop() {}