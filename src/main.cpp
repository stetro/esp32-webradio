#include <Arduino.h>
#include <HTTPClient.h>
#include <TimeLib.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <display.h>
#include <time.h>

#include <configuration.h>
// #include <player.h>
#include <ESP32Encoder.h>

#define WIFI_TIMEOUT_CONFIGURATION_S 60

// Player player;
Display display;

ESP32Encoder encoder;

Configuration configuration;
ConfigurationData *configuration_data;
bool configuration_mode_enabled = false;

const char *ntp_server = "europe.pool.ntp.org";
const long gmt_offset_sec = 3600;
const int daylight_offset_sec = 0;

#define STATION_COUNT 7

const char stations[STATION_COUNT][16] = {"domradio.de", "1Live", "WDR 2", "WDR 3", "WDR 4", "WDR 5", "Deutschlandfunk"};
int station_index = 0;
int32_t old_count = 0;

void setup() {

  // Setup Serial on 115200 Baud
  Serial.begin(115200);

  // Setup Encoder
  ESP32Encoder::useInternalWeakPullResistors = false;
  encoder.clearCount();
  encoder.attachHalfQuad(27, 26);

  // Setup Display
  display.setup();
  display.clear();
  display.status("Connecting Wifi...");
  display.display();

  // Load configuration and check validity
  configuration_data = configuration.get_configuration();
  if (!(configuration_data->eeprom_check[0] == 'O' &&
        configuration_data->eeprom_check[1] == 'K') ||
      configuration_data->configuration_version != CONFIGURATION_VERSION) {
    configuration.init();
    configuration_mode_enabled = true;
    display.clear();
    display.configuration();
    display.display();
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
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

  // Player
  // player.setup();
  // player.play();
}

void render() {
  display.clear();
  display.show_time();
  display.station(stations[station_index]);
  display.status("Buffering ...");
  display.display();
}

void loop() {
  if (configuration_mode_enabled) {
    delay(100);
    return;
  }
  if (old_count != encoder.getCount()) {
    if (old_count > encoder.getCount()) {
      station_index = (station_index + 1) % STATION_COUNT;
    } else {
      station_index = (station_index - 1) % STATION_COUNT;
      if (station_index < 0) {
        station_index = STATION_COUNT-1;
      }
    }
    old_count = encoder.getCount();
    Serial.printf("Encoder %d %d \n", encoder.getCount(), station_index);
  }
  render();
  delay(200);
}
