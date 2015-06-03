#ifndef memory_h
#define memory_h

#include "Arduino.h"
#include <EEPROM.h>

class Memory
{
  public:
    Memory();
    uint8_t Contrast, oldContrast;
    uint8_t BackLight, oldBackLight;
    uint8_t Control, oldControl;
    uint8_t Mode,oldMode;
    uint8_t Times, oldTimes;
    uint8_t SensorPIR, oldSensorPIR;
    void readContrast();
    void readBackLight();
    void readControl();
    void readMode();
    void readTimes();
    void readSensorPIR();
    void updateContrast();
    void updateBackLight();
    void updateControl();
    void updateMode();
    void updateTimes();
    void updateSensorPIR();
};

#endif
