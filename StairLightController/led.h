#ifndef led_h
#define led_h

#include "Arduino.h"
#include <Adafruit_PWMServoDriver.h>

class LED : public Adafruit_PWMServoDriver {
  public:
    LED(uint8_t addr);
    void init(uint16_t freq, uint8_t twbr);
    void updatePWM();
    void setAllOffPWM();
    int channel_0 = 0;
    int channel_1 = 0;
    int channel_2 = 0;
    int channel_3 = 0;
    int channel_4 = 0;
    int channel_5 = 0;
    int channel_6 = 0;
    int channel_7 = 0;
    int channel_8 = 0;
    int channel_9 = 0;
    int channel_10 = 0;
    int channel_11 = 0;
    int channel_12 = 0;
    int channel_13 = 0;
    int channel_14 = 0;
    int channel_15 = 0;
};

#endif
