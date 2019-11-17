#if !defined(PLAYER_H)
#define PLAYER_H

#include <VS1053.h>
#include <HTTPClient.h>

#define VS1053_CS 5
#define VS1053_DCS 16
#define VS1053_DREQ 4
#define VOLUME 80
#define MP3_BUFFER_SIZE 32

#define STATION_COUNT 7
#define NAME_INDEX 0
#define URL_INDEX 1
#define LOST_SYNCRONSIATION_STATUS 257

const char stations[STATION_COUNT][2][100] = {
    {"domradio.de", "http://dom.audiostream.io/domradio/1000/mp3/64/domradio"},
    {"1Live", "http://wdr-edge-2018.fra-lg.cdn.addradio.net/wdr/1live/live/mp3/56/stream.mp3"},
    {"WDR 2", "http://wdr-edge-201a.fra-lg.cdn.addradio.net/wdr/wdr2/suedwestfalen/mp3/56/stream.mp3"},
    {"WDR 3", "http://wdr-edge-201a.fra-lg.cdn.addradio.net/wdr/wdr3/live/mp3/56/stream.mp3"},
    {"WDR 4", "http://wdr-edge-201a.fra-lg.cdn.addradio.net/wdr/wdr4/live/mp3/56/stream.mp3"},
    {"WDR 5", "http://wdr-edge-201a.fra-lg.cdn.addradio.net/wdr/wdr5/live/mp3/56/stream.mp3"},
    {"Deutschlandfunk", "http://dradio-edge-2095.fra-lg.cdn.addradio.net/dradio/dlf/live/mp3/64/stream.mp3"}};

class Player {
 public:
  void init();
  void loop();
  void play(int station);
  void stop();
  static void on_status_update(void *context, int code, const char *string);

 private:
  VS1053* player;
  WiFiClient* client;
  
  uint8_t mp3buff[MP3_BUFFER_SIZE];
  int playing_index = 0;
};

#endif  // PLAYER_H
