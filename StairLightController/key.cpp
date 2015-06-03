#include "Arduino.h"
#include "key.h"

Key::Key(int pin)
{
  pinMode(pin, INPUT_PULLUP);
  _pin = pin;
}

boolean Key::pressed()
{
  if(digitalRead(_pin)) return 0;
  else return 1; 
}



