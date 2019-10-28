#include <player.h>

void Player::setup() {
  Serial.println("[Player] Setup player");
  file = new AudioFileSourceICYStream(STREAM_URL);

  buff = new AudioFileSourceBuffer(file, 131072);
  out = new AudioOutputI2S();
  mp3 = new AudioGeneratorMP3();
}

void Player::play(){
  mp3->begin(buff, out);
}

void Player::loop() {
  static int lastms = 0;

  if (mp3->isRunning()) {
    if (millis() - lastms > 1000) {
      lastms = millis();
      Serial.printf("Running for %d ms...\n", lastms);
      Serial.flush();
    }
    if (!mp3->loop())
      mp3->stop();
  } else {
    Serial.printf("MP3 done\n");
    delay(1000);
  }
}