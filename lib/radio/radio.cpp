#include <radio.h>

void Radio::init_decoder() {
  ESP32Encoder::useInternalWeakPullResistors = false;
  encoder.clearCount();
  encoder.attachHalfQuad(34, 35);
}

void Radio::init_player() {
  player.init();
  player.setup(station_index);
  player.play();
}

void Radio::display_connecting() {
  display.clear();
  display.status("Connecting Wifi...");
  display.display();
}

void Radio::display_configuration() {
  display.clear();
  display.configuration();
  display.display();
}

void Radio::display_station(const char* status) {
  display.clear();
  display.show_time();
  display.station(stations[station_index][NAME_INDEX]);
  display.status(status);
  display.display();
}

void Radio::handle_encoder_input() {
  if (last_encoder_count != encoder.getCount()) {
    if (last_encoder_count > encoder.getCount()) {
      station_index = (station_index + 1) % STATION_COUNT;
    } else {
      station_index = (station_index - 1) % STATION_COUNT;
      if (station_index < 0) {
        station_index = STATION_COUNT - 1;
      }
    }
    station_change_millis = millis();
    last_encoder_count = encoder.getCount();
    Serial.printf("Encoder %d %d \n", encoder.getCount(), station_index);
  }
}

void Radio::loop() {
  player.loop();

  // check every 300 ms if encoder has changed
  if (last_rendering_millis + 300 < millis()) {
    last_rendering_millis = millis();
    handle_encoder_input();
    display_station();
  }

  // check after 1s if channel should be changed
  if (station_index != playing_station_index && station_change_millis + 1000 < millis()) {
    playing_station_index = station_index;
    display_station("Buffering ...");
    player.stop();
    player.setup(station_index);
    player.play();
  }
}