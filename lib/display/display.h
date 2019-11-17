#if !defined(DISPLAY_H)
#define DISPLAY_H

#include <SSD1306Wire.h>
#include <Wire.h>
#include <Time.h>

#define PIN_WIRE_SDA 22                                                                                                                                                                                          
#define PIN_WIRE_SCL 21 
#define DISPLAY_ADDRESS 0x3C

class Display {
public:
  void setup();
  void status(const char *message);
  void station(const char *message);
  void configuration();
  void clear();
  void show_time();
  void display();

private:
  SSD1306Wire *lcd;
  char time_buffer[9];
};

#endif // DISPLAY_H
