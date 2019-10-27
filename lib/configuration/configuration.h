#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#define CONFIGURATION_DATA_BUFFER_SIZE 30
#define CONFIGURATION_VERSION 1

#include <EEPROM.h>

#include <AsyncTCP.h>
#include <WiFi.h>

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

struct ConfigurationData {
  char eeprom_check[3];
  int configuration_version;
  char ssid[CONFIGURATION_DATA_BUFFER_SIZE];
  char password[CONFIGURATION_DATA_BUFFER_SIZE];
};

class Configuration {
 private:
  char ssid_buffer[13];
  String ip_address;
  AsyncWebServer* server;
  ConfigurationData configuration_data;

 public:
  void init();
  ConfigurationData* get_configuration();
  void clear();
};

#endif  // CONFIGURATION_H
