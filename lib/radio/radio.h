#if !defined(RADIO_H)
#define RADIO_H

#include <ESP32Encoder.h>
#include <display.h>
#include <player.h>

class Radio {
 public:
  void init_display() { display.setup(); }
  void init_encoder();
  void init_player();
  void display_connecting();
  void display_configuration();
  void display_station(const char* status = "");
  void loop();

 private:
  void render();
  void handle_encoder_input();
  Player player;
  Display display;
  ESP32Encoder encoder;

  int station_index = 1;
  int station_change_millis = 0;
  int playing_station_index = 0;
  long last_rendering_millis = 0;
};

#endif  // RADIO_H
