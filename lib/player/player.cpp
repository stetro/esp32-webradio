#include <player.h>

void Player::init() {
  Serial.println("[Player] init player");
  SPI.begin();
  player = new VS1053(VS1053_CS, VS1053_DCS, VS1053_DREQ);
  player->begin();
  player->printDetails("[Player]");
  player->switchToMp3Mode();
  player->setVolume(VOLUME);
  if (player->isChipConnected()) {
    Serial.println("[Player] Chip is connected");
  } else {
    Serial.println("[Player] Chip is not connected");
  }
  client = new WiFiClient();
}

void Player::play(int station) {
  Serial.printf("[Player] Setup player station %s\n", stations[station].name);
  player->startSong();

  if (!client->connect(stations[station].host, 80)) {
    Serial.println("Connection failed");
    return;
  }

  client->print(String("GET ") + stations[station].path + " HTTP/1.1\r\n" + "Host: " + stations[station].host + "\r\n" +
                "Connection: close\r\n\r\n");
}

void Player::loop() {
  if (client->available() > 0) {
    uint8_t bytesread = client->read(mp3buff, MP3_BUFFER_SIZE);
    player->playChunk(mp3buff, bytesread);
  }
}

void Player::stop() {
  Serial.printf("[Player] Stop\n");
  player->stopSong();
  client->stop();
}