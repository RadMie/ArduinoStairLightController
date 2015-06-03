#ifndef lcd_h
#define lcd_h

#include "Arduino.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

class LCD : public Adafruit_PCD8544 {

  public:
    LCD(int DC, int CS, int RST, int BK);
    void init(uint8_t contrast, uint8_t backlight);
    void setBacklight(uint8_t BK);
    void logo();
    void progressBar(uint8_t x, uint8_t y, uint8_t w, uint8_t h, boolean color, uint8_t progress);
    void stairs();
    void stairStep(uint16_t number);
    void scrollBar(uint8_t pos);
    void gird(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
    void window(char* nameText);
    void startPanel();
    void mainPanel(uint8_t Status, uint8_t Mode, boolean Down, boolean Up, uint8_t progress, uint16_t binStairStep);
    void mainMenu(uint8_t positionNumber);
    void menuLCDLight(uint8_t val);
    void menuLCDContrast(uint8_t val);
    void menuControl(uint8_t val);
    void menuMode(uint8_t val);
    void menuTimes(uint8_t val);
    void menuSensorPIR();
    void menuDemo();
  private:
    int _bk;
};

#endif
