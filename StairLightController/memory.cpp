#include "Arduino.h"
#include "memory.h"

#define CONTRAST 0
#define BACKLIGHT 1
#define CONTROL 2
#define MODE 3
#define TIMES 4
#define SENSOR_PIR 5

Memory::Memory()
{
  readContrast();
  readBackLight();
  readControl();
  readMode();
  readTimes();
  readSensorPIR();
}

void Memory::readContrast() {
  Contrast = EEPROM.read(CONTRAST);
}
void Memory::readBackLight() {
  BackLight = EEPROM.read(BACKLIGHT);
}
void Memory::readControl() {
  Control = EEPROM.read(CONTROL); 
}
void Memory::readMode() {
  Mode = EEPROM.read(MODE);
}
void Memory::readTimes() {
  Times = EEPROM.read(TIMES);
}
void Memory::readSensorPIR() {
  SensorPIR = EEPROM.read(SENSOR_PIR); 
}
void Memory::updateContrast() {
  EEPROM.update(CONTRAST, Contrast);
}
void Memory::updateBackLight() {
  EEPROM.update(BACKLIGHT, BackLight);
}
void Memory::updateControl() {
  EEPROM.update(CONTROL, Control);
}
void Memory::updateMode() {
  EEPROM.update(MODE, Mode);
}
void Memory::updateTimes() {
  EEPROM.update(TIMES, Times);
}
void Memory::updateSensorPIR() {
  EEPROM.update(SENSOR_PIR, SensorPIR);
}

