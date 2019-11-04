#if !defined(PLAYER_H)
#define PLAYER_H

#include "AudioFileSourceBuffer.h"
#include "AudioFileSourceICYStream.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"

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
  void setup(int station);
  void loop();
  void play();
  void stop();
  void restart();
  void free_memory();
  static void on_status_update(void *context, int code, const char *string);

 private:
  AudioGeneratorMP3 *mp3;
  AudioFileSourceICYStream *file;
  AudioFileSourceBuffer *buff;
  AudioOutputI2S *out;

  const int preallocate_buffer_size = 16 * 1024;
  const int preallocate_codec_size = 85332;
  void *preallocate_buffer = NULL;
  void *preallocate_codec = NULL;
  int playing_index = 0;
  bool restart_station = false;
};

#endif  // PLAYER_H
