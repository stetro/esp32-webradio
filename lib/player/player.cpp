#include <player.h>

void Player::init() {
  out = new AudioOutputI2S();
  out->SetGain(0.05f);
  mp3 = new AudioGeneratorMP3();
}

void Player::setup(int station) {
  Serial.println("[Player] Setup player");
  file = new AudioFileSourceICYStream(stations[station][URL_INDEX]);
  buff = new AudioFileSourceBuffer(file, 8192 * 2);
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

void Player::stop() {
  mp3->stop();
  file->close();
  delete file;
  buff->close();
  delete buff;
}