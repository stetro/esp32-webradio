#if !defined(PLAYER_H)
#define PLAYER_H

#include "AudioFileSourceBuffer.h"
#include "AudioFileSourceICYStream.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"

#define STREAM_URL                                                             \
  "https://wdr-dg-wdr-https-fra-dtag-cdn.sslcast.addradio.de/wdr/1live/live/"  \
  "mp3/128/stream.mp3"

class Player {
public:
  void setup();
  void loop();
  void play();

private:
  AudioGeneratorMP3 *mp3;
  AudioFileSourceICYStream *file;
  AudioFileSourceBuffer *buff;
  AudioOutputI2S *out;
};

#endif // PLAYER_H
