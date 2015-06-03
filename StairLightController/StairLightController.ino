/*
███████╗████████╗ █████╗ ██╗██████╗     ██╗     ██╗ ██████╗ ██╗  ██╗████████╗     ██████╗ ██████╗ ███╗   ██╗████████╗██████╗  ██████╗ ██╗     ██╗     ███████╗██████╗ 
██╔════╝╚══██╔══╝██╔══██╗██║██╔══██╗    ██║     ██║██╔════╝ ██║  ██║╚══██╔══╝    ██╔════╝██╔═══██╗████╗  ██║╚══██╔══╝██╔══██╗██╔═══██╗██║     ██║     ██╔════╝██╔══██╗
███████╗   ██║   ███████║██║██████╔╝    ██║     ██║██║  ███╗███████║   ██║       ██║     ██║   ██║██╔██╗ ██║   ██║   ██████╔╝██║   ██║██║     ██║     █████╗  ██████╔╝
╚════██║   ██║   ██╔══██║██║██╔══██╗    ██║     ██║██║   ██║██╔══██║   ██║       ██║     ██║   ██║██║╚██╗██║   ██║   ██╔══██╗██║   ██║██║     ██║     ██╔══╝  ██╔══██╗
███████║   ██║   ██║  ██║██║██║  ██║    ███████╗██║╚██████╔╝██║  ██║   ██║       ╚██████╗╚██████╔╝██║ ╚████║   ██║   ██║  ██║╚██████╔╝███████╗███████╗███████╗██║  ██║
╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝╚═╝  ╚═╝    ╚══════╝╚═╝ ╚═════╝ ╚═╝  ╚═╝   ╚═╝        ╚═════╝ ╚═════╝ ╚═╝  ╚═══╝   ╚═╝   ╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚══════╝╚══════╝╚═╝  ╚═╝
*/                                                                                                                                                                      
#include <SPI.h>
#include <EEPROM.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Adafruit_PWMServoDriver.h>
#include "Timers.h"
#include "key.h"
#include "lcd.h"
#include "led.h"
#include "memory.h"

//#define DEBUG

#define MAIN_PANEL 0       // Main Panel
#define CONTROL 1          // on/off
#define MODE 2             // wave/cascade/elevator...
#define TIMES 3            // 0-25.5 second
#define LCD_LIGHT 4        // 0-100%
#define LCD_CONTRAST 5     // 0-100%
#define SENSOR_PIR 6       // 
#define DEMO 7             // DEMO1/DEMO2/DEMO3...
#define EXIT 8             // Exit

#define WAVE 10
#define CASCADE 11
#define ELEVATOR 12
#define RANDOM 13
#define ALL 14

Timers<10> timer;          // 10 x diff timer

Key KeyUP = Key(30);       // PIN INPUT 30
Key KeySEL = Key(32);      // PIN INPUT 32
Key KeyDOWN = Key(34);     // PIN INPUT 34
Key KeyLEFT = Key(36);     // PIN INPUT 36
Key KeyRIGHT = Key(35);    // PIN INPUT 35

LCD display = LCD(48,46,44,12);  // only Hardware SPI PIN(DC,CS,RST,Backlight)

LED led = LED(0x40);          // Address I2C, default 0x40

Memory memory = Memory();

uint8_t menu, old_menu = 0;
boolean menu_active, old_menu_active = 0;
uint16_t mainStairStep, old_mainStairStep = 0;  // (high byte * 256) + low byte
uint8_t mainProgress, old_mainProgress = 0;
boolean mainPirUp, old_mainPirUp = 0;
boolean mainPirDown, old_mainPirDown = 0;

void setup()   {
  
  #if defined(DEBUG)
    Serial.begin(19200);
  #endif
  
  display.init(memory.Contrast,memory.BackLight);
  //led.init(1600,12);
  
  display.startPanel();
  
  timer.attach(0, 150, Navigation);
  timer.attach(1, 150, Display);
  
  pinMode(13, OUTPUT);
}

void loop() { 
  
  timer.process();
  //ledProcess();
  
  #if defined(DEBUG)
    //Serial.print("menu:"); Serial.print(menu); Serial.print(", menu_active:"); Serial.println(menu_active);
  #endif
  
  digitalWrite(13, !digitalRead(13));
}

void Navigation() {
  
  if(KeyUP.pressed()) {
    if(menu_active == 0 && menu != MAIN_PANEL) {
      menu--;
      if(menu < CONTROL) menu = EXIT;
    }
  } else if(KeyDOWN.pressed()) {
    if(menu_active == 0 && menu != MAIN_PANEL) {
      menu++;
      if(menu > EXIT) menu = CONTROL;
    }
  } else if(KeySEL.pressed()) {
    if(menu_active == 0 && menu == MAIN_PANEL) menu = CONTROL;
    else if(menu_active == 0 && menu == EXIT) menu = MAIN_PANEL;
    else if(CONTROL <= menu <= EXIT) {
      menu_active = !menu_active;
      if(menu_active == 0) {
        switch(menu) {
          case CONTROL:
            //memory.updateControl();
            break;
          case MODE:
            //memory.updateMode();
            break;
          case TIMES:
            //memory.updateTimes();
            break;
          case LCD_LIGHT:
            //memory.updateBackLight();
            break;
          case LCD_CONTRAST:
            //memory.updateContrast();
            break;
        }
      }
    }
  } else if(KeyLEFT.pressed()) {
    if(menu_active == 1) {
      switch(menu) {
        case CONTROL:
          memory.Control = !memory.Control;
          break;
        case MODE:
          memory.Mode--;
          if(memory.Mode < WAVE) memory.Mode = ELEVATOR;
          break;
        case TIMES:
          memory.Times--;
          break;
        case LCD_LIGHT:
          memory.BackLight -= 2;
          display.setBacklight(memory.BackLight);
          break;
        case LCD_CONTRAST:
          memory.Contrast--;
          display.setContrast(memory.Contrast);
          break;
      }
    }
  } else if(KeyRIGHT.pressed()) {
    if(menu_active == 1) {
      switch(menu) {
        case CONTROL:
          memory.Control = !memory.Control;
          break;
        case MODE:
          memory.Mode++;
          if(memory.Mode > ELEVATOR) memory.Mode = WAVE;
          break;
        case TIMES:
          memory.Times++;
          break;
        case LCD_LIGHT:
          memory.BackLight += 2;
          display.setBacklight(memory.BackLight);
          break;
        case LCD_CONTRAST:
          memory.Contrast++;
          display.setContrast(memory.Contrast);
          break;
      }
    }
  } 
}
void Display() {
  
  if(menu == MAIN_PANEL) {
    if(menu != old_menu || memory.Control != memory.oldControl || memory.Mode != memory.oldMode || mainPirDown != old_mainPirDown || mainPirUp != old_mainPirUp || mainProgress != old_mainProgress || mainStairStep != old_mainStairStep) {
      display.mainPanel(memory.Control, memory.Mode, mainPirDown, mainPirUp, mainProgress, mainStairStep);
      old_menu = menu;
      memory.oldControl = memory.Control;
      memory.oldMode = memory.Mode;
      old_mainPirDown = mainPirDown;
      old_mainPirUp = mainPirUp;
      old_mainProgress = mainProgress;
      old_mainStairStep = mainStairStep;
      #if defined(DEBUG)
        Serial.println("main_panel");
      #endif
    }
  } else if (menu_active == 0) {
    if(menu != old_menu || menu_active != old_menu_active) {
      display.mainMenu(menu);
      old_menu = menu;
      old_menu_active = menu_active;
      #if defined(DEBUG)
        Serial.println("menu_active");
      #endif  
    }
  } else if(menu == CONTROL) {
    if(menu != old_menu || memory.Control != memory.oldControl || menu_active != old_menu_active) {
      display.menuControl(memory.Control);
      old_menu = menu;
      memory.oldControl = memory.Control;
      old_menu_active = menu_active;
      #if defined(DEBUG)
        Serial.println("control");
      #endif 
    }
  } else if(menu == MODE) {
    if(menu != old_menu || memory.Mode != memory.oldMode || menu_active != old_menu_active) {
      display.menuMode(memory.Mode);
      old_menu = menu;
      memory.oldMode = memory.Mode;
      old_menu_active = menu_active;
      #if defined(DEBUG)
        Serial.println("mode");
      #endif 
    }
  } else if(menu == TIMES) {
    if(menu != old_menu || memory.Times != memory.oldTimes || menu_active != old_menu_active) {
      display.menuTimes(memory.Times);
      old_menu = menu;
      memory.oldTimes = memory.Times;
      old_menu_active = menu_active;
      #if defined(DEBUG)
        Serial.println("times");
      #endif 
    }
  } else if(menu == LCD_LIGHT) {
    if(menu != old_menu || memory.BackLight != memory.oldBackLight || menu_active != old_menu_active) {
      display.menuLCDLight(memory.BackLight);
      old_menu = menu;
      memory.oldBackLight = memory.BackLight;
      old_menu_active = menu_active;
      #if defined(DEBUG)
        Serial.println("backlight");
      #endif 
    }
  } else if(menu == LCD_CONTRAST) {
    if(menu != old_menu || memory.Contrast != memory.oldContrast || menu_active != old_menu_active) {
      display.menuLCDContrast(memory.Contrast);
      old_menu = menu;
      memory.oldContrast = memory.Contrast;
      old_menu_active = menu_active;
      #if defined(DEBUG)
        Serial.println("contrast");
      #endif 
    }
  } else if(menu == SENSOR_PIR) {
    if(menu != old_menu || menu_active != old_menu_active) {
      display.menuSensorPIR();
      old_menu = menu;
      old_menu_active = menu_active;
      #if defined(DEBUG)
        Serial.println("sensor_pir");
      #endif 
    }
  } else if(menu == DEMO) {
    if(menu != old_menu || menu_active != old_menu_active) {
      display.menuDemo();
      old_menu = menu;
      old_menu_active = menu_active;
      #if defined(DEBUG)
        Serial.println("demo");
      #endif 
    }
  }
}
void ledProcess() {

  if(memory.Control) {
    
    switch(memory.Mode) {
      case WAVE:
        waveProcess();
        break;
      case CASCADE:
        cascadeProcess();
        break;
      case ELEVATOR:
        elevatorProcess();
        break;
    }
  }
  led.updatePWM();
}

void waveProcess() {
  
}

void cascadeProcess() {
  
}

void elevatorProcess() {
  
}
