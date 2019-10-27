#include <configuration-site.h>
#include <configuration.h>

void Configuration::init() {
  // unique device identifier
  char efuse_mac_buffer[12];
  const char *efuse_mac = utoa(ESP.getEfuseMac(), efuse_mac_buffer, 10);

  sprintf(ssid_buffer, "d%s", efuse_mac);
  WiFi.softAP(ssid_buffer, NULL);

  ip_address = WiFi.softAPIP().toString();

  server = new AsyncWebServer(80);

  server->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("[Configuration] loading index.html");
    request->send(200, "text/html", INDEX_HTML);
  });

  server->on("/update", HTTP_POST, [](AsyncWebServerRequest *request) {
    Serial.println("[Configuration] updating configuration");

    EEPROM.begin(512);

    ConfigurationData data;

    String ssid = request->arg("ssid");
    Serial.println("[Configuration] ssid " + ssid);
    sprintf(data.ssid, "%s", ssid.c_str());

    String password = request->arg("password");
    Serial.println("[Configuration] password ***");
    sprintf(data.password, "%s", password.c_str());

    sprintf(data.eeprom_check, "OK");

    data.configuration_version = CONFIGURATION_VERSION;

    EEPROM.put(0, data);
    EEPROM.commit();

    request->send(200, "text/html",
                  "Configuration updated. Device is restarting.");
    Serial.flush();
    ESP.restart();
  });

  server->begin();

  Serial.printf("[Configuration] Running Soft AP at %s %s\n", ssid_buffer,
                WiFi.softAPIP().toString().c_str());
}

ConfigurationData *Configuration::get_configuration() {
  Serial.println("[Configuration] loading configuration");
  EEPROM.begin(512);
  EEPROM.get(0, configuration_data);

  Serial.printf("[Configuration] EEPROM %s \n",
                configuration_data.eeprom_check);
  Serial.printf("[Configuration] Version %d \n",
                configuration_data.configuration_version);
  Serial.printf("[Configuration] ssid %s \n", configuration_data.ssid);
  Serial.printf("[Configuration] password ***\n");

  return &configuration_data;
}

void Configuration::clear() {
  Serial.println("[Configuration] clear configuration");
  EEPROM.begin(512);
  ConfigurationData data;
  sprintf(data.eeprom_check, "**");
  data.configuration_version = 0;
  sprintf(data.ssid, "***");
  sprintf(data.password, "***");
  EEPROM.put(0, data);
  EEPROM.commit();
}
