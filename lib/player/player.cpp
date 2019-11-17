#include <player.h>

void Player::init() {
  Serial.println("[Player] init player");
  player = new VS1053(VS1053_CS, VS1053_DCS, VS1053_DREQ);
  player->begin();
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
  Serial.printf("[Player] Setup player station %s\n", stations[station][NAME_INDEX]);

  if (!client->connect("wdr-edge-2018.fra-lg.cdn.addradio.net", 80)) {
    Serial.println("Connection failed");
    return;
  }

  client->print(String("GET ") + "/wdr/1live/live/mp3/56/stream.mp3" + " HTTP/1.1\r\n" +
                "Host: " + "wdr-edge-2018.fra-lg.cdn.addradio.net" + "\r\n" + "Connection: close\r\n\r\n");

  // client->begin(stations[station][URL_INDEX]);
  // int statusCode = client->GET();
  // if (statusCode > 0) {
  //   if (statusCode == HTTP_CODE_OK) {
  //     Serial.println("[Player] Correctly connected");
  //   }
  // } else {
  //   Serial.println("[Player] Something went wrong");
  // }
  // playing_index = station;
}

void Player::loop() {
  if (client->available() > 0) {
    uint8_t bytesread = client->read(mp3buff, MP3_BUFFER_SIZE);
    player->playChunk(mp3buff, bytesread);
  }
}

void Player::stop() { Serial.printf("[Player] Stop\n"); }