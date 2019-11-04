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

void Player::play() {
  Serial.printf("[Player] Play\n");
  mp3->begin(buff, out);
}

void Player::loop() {
  static int last_free_memory = 0;

  if (mp3->isRunning()) {
    if (millis() - last_free_memory > 5000) {
      last_free_memory = millis();
      free_memory();
      Serial.printf("[Player] Running for %d ms...\n", last_free_memory);
      Serial.flush();
    }
    if (!mp3->loop()) {
      this->stop();
    }
  } else {
    Serial.printf("[Player] MP3 done\n");
  }
}

void Player::free_memory() { Serial.printf_P(PSTR("[Player] Free memory = %d bytes\n"), ESP.getFreeHeap()); }

void Player::stop() {
  Serial.printf("[Player] Stop\n");
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