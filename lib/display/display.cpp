#include <display.h>

void Display::setup() {
  Serial.println("[Display] setup oled display");
  lcd = new SSD1306Wire(DISPLAY_ADDRESS, PIN_WIRE_SDA, PIN_WIRE_SCL);
  lcd->init();
  lcd->flipScreenVertically();
  lcd->setFont(ArialMT_Plain_10);
}

void Display::status(const char *message) {
  lcd->setTextAlignment(TEXT_ALIGN_CENTER);
  lcd->setFont(ArialMT_Plain_10);
  lcd->drawString(64, 3 * 17, message);
}

void Display::station(const char *message) {
  lcd->setTextAlignment(TEXT_ALIGN_CENTER);
  lcd->setFont(ArialMT_Plain_16);
  lcd->drawString(64, 2 * 17, message);
}

void Display::clear() { lcd->clear(); }

void Display::show_time() {
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    strftime(time_buffer, 9, "%H:%M", &timeinfo);
  }
  lcd->setTextAlignment(TEXT_ALIGN_CENTER);
  lcd->setFont(ArialMT_Plain_24);
  lcd->drawString(64, 0, time_buffer);
}

void Display::configuration() {
  lcd->setTextAlignment(TEXT_ALIGN_CENTER);
  lcd->setFont(ArialMT_Plain_16);
  lcd->drawString(64, 0 * 17, "Not Connected");
  lcd->drawString(64, 1 * 17, "Connect via Wifi");
  lcd->drawString(64, 2 * 17, "Open 192.168.4.1");
}

void Display::display() { lcd->display(); }