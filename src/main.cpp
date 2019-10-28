#include <Arduino.h>
#include <HTTPClient.h>
#include <TimeLib.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#include <configuration.h>
#include <time.h>

#define WIFI_TIMEOUT_CONFIGURATION_S 60

Configuration configuration;
ConfigurationData *configuration_data;
bool configuration_mode_enabled = false;

const char *ntp_server = "pool.ntp.org";
const long gmt_offset_sec = 0;
const int daylight_offset_sec = 0;

void setup() {

  // Setup Serial on 115200 Baud
  Serial.begin(115200);

  // Load configuration and check validity
  configuration_data = configuration.get_configuration();
  if (!(configuration_data->eeprom_check[0] == 'O' &&
        configuration_data->eeprom_check[1] == 'K') ||
      configuration_data->configuration_version != CONFIGURATION_VERSION) {
    configuration.init();
    configuration_mode_enabled = true;
    return;
  }

  // Start Wifi connection
  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.begin(configuration_data->ssid, configuration_data->password);
  Serial.print("[WIFI] Connecting to WiFi ");
  int wifi_connection_time = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    wifi_connection_time++;
    if (wifi_connection_time > WIFI_TIMEOUT_CONFIGURATION_S) {
      Serial.println("[WIFI] Run into Wifi Timeout, configuration");
      Serial.flush();
      WiFi.disconnect();
      configuration_mode_enabled = true;
      return;
    }
  }
  Serial.println("\n[WIFI] Connected");

  // Load remote NTP time
  configTime(gmt_offset_sec, daylight_offset_sec, ntp_server);
  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) {
    Serial.println("[Timer] Failed to obtain time");
    delay(300);
  }
  setTime(mktime(&timeinfo));
  Serial.printf("[Timer] %02d:%02d:%02d", hour(), minute(), second());
}

void loop() {
  if (configuration_mode_enabled) {
    delay(100);
    return;
  }
  delay(100);
}
