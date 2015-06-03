#include "Arduino.h"
#include "led.h"

#define CHANNEL_0  0
#define CHANNEL_1  1
#define CHANNEL_2  2
#define CHANNEL_3  3
#define CHANNEL_4  4
#define CHANNEL_5  5
#define CHANNEL_6  6
#define CHANNEL_7  7
#define CHANNEL_8  8
#define CHANNEL_9  9
#define CHANNEL_10  10
#define CHANNEL_11  11
#define CHANNEL_12  12
#define CHANNEL_13  13
#define CHANNEL_14  14
#define CHANNEL_15  15


LED::LED(uint8_t addr) : Adafruit_PWMServoDriver(addr) {

}

void LED::init(uint16_t freq, uint8_t twbr) {
  
  begin();
  setPWMFreq(freq);  // PWM frequency (maximum 1600)
  TWBR = twbr;       // I2C speed (12 = 400KHz)
  updatePWM();
}

void LED::updatePWM() {
  
  setPWM(CHANNEL_0, 0, channel_0);
  setPWM(CHANNEL_1, 0, channel_1);
  setPWM(CHANNEL_2, 0, channel_2);
  setPWM(CHANNEL_3, 0, channel_3);
  setPWM(CHANNEL_4, 0, channel_4);
  setPWM(CHANNEL_5, 0, channel_5);
  setPWM(CHANNEL_6, 0, channel_6);
  setPWM(CHANNEL_7, 0, channel_7);
  setPWM(CHANNEL_8, 0, channel_8);
  setPWM(CHANNEL_9, 0, channel_9);
  setPWM(CHANNEL_10, 0, channel_10);
  setPWM(CHANNEL_11, 0, channel_11);
  setPWM(CHANNEL_12, 0, channel_12);
  setPWM(CHANNEL_13, 0, channel_13);
  setPWM(CHANNEL_14, 0, channel_14);
  setPWM(CHANNEL_15, 0, channel_15);
  /*
    led.setPWM(0, 0, 256);
  led.setPWM(1, 0, 512);
  led.setPWM(2, 0, 768);
  led.setPWM(3, 0, 1024);
  led.setPWM(4, 0, 1280);
  led.setPWM(5, 0, 1536);
  led.setPWM(6, 0, 1792);
  led.setPWM(7, 0, 2048);
  led.setPWM(8, 0, 2304);
  led.setPWM(9, 0, 2560);
  led.setPWM(10, 0, 2816);
  led.setPWM(11, 0, 3072);
  led.setPWM(12, 0, 3328);
  led.setPWM(13, 0, 3584);
  led.setPWM(14, 0, 3840);
  led.setPWM(15, 0, 4096);
  */
  
}

void LED::setAllOffPWM() {
  
  channel_0 = 0;
  channel_1 = 0;
  channel_2 = 0;
  channel_3 = 0;
  channel_4 = 0;
  channel_5 = 0;
  channel_6 = 0;
  channel_7 = 0;
  channel_8 = 0;
  channel_9 = 0;
  channel_10 = 0;
  channel_11 = 0;
  channel_12 = 0;
  channel_13 = 0;
  channel_14 = 0;
  channel_15 = 0;
}
