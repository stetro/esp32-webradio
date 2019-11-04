#include <player.h>

void Player::init() {
  Serial.println("[Player] init player");
  out = new AudioOutputI2S();
  out->SetGain(0.05f);
  out->SetOutputModeMono(true);

  preallocate_buffer = malloc(preallocate_buffer_size);
  preallocate_codec = malloc(preallocate_codec_size);
}

void Player::setup(int station) {
  Serial.printf("[Player] Setup player station %s\n", stations[station][NAME_INDEX]);
  free_memory();
  file = new AudioFileSourceICYStream(stations[station][URL_INDEX]);
  buff = new AudioFileSourceBuffer(file, preallocate_buffer, preallocate_buffer_size);
  mp3 = new AudioGeneratorMP3(preallocate_codec, preallocate_codec_size);
}

void Player::play() { mp3->begin(buff, out); }

void Player::loop() {
  static int lastms = 0;

  if (mp3->isRunning()) {
    if (millis() - lastms > 1000) {
      lastms = millis();
      Serial.printf("Running for %d ms...\n", lastms);
      Serial.flush();
    }
    if (!mp3->loop()) {
      mp3->stop();
    }
  } else {
    Serial.printf("MP3 done\n");
    delay(1000);
  }
}

void Player::free_memory() { Serial.printf_P(PSTR("[Player] Before stop...Free mem=%d\n"), ESP.getFreeHeap()); }

void Player::stop() {
  if (mp3) {
    mp3->stop();
    delete mp3;
    mp3 = NULL;
  }
  if (file) {
    file->close();
    delete file;
    file = NULL;
  }
  if (buff) {
    buff->close();
    delete buff;
    buff = NULL;
  }
}