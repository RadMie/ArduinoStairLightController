#include "Arduino.h"
#include "lcd.h"

LCD::LCD(int DC, int CS, int RST, int BK) : Adafruit_PCD8544(DC,CS,RST)
{
   pinMode(BK, OUTPUT);  // LCD PWM Backlight
   _bk = BK;
}

void LCD::init(uint8_t contrast, uint8_t backlight) {
  
  begin();                  // init
  setContrast(contrast);    // contrast
  clearDisplay();           // clears the screen and buffer
  setBacklight(backlight);  // backlight
}

void LCD::setBacklight(uint8_t BK) {
  
  analogWrite(_bk, BK);     // set PWM Backlight
}
//============================== COMPONENT ===========================================================

void LCD::logo() {
  
  setCursor(10,10);
  setTextColor(BLACK);
  setTextSize(1);
  println("STAIR LIGHT");
  setCursor(12,20);
  println("CONTROLLER");
}

void LCD::progressBar(uint8_t x, uint8_t y, uint8_t w, uint8_t h, boolean color, uint8_t progress) {
  
  drawLine(x+1, y, x+w-2, y, color);
  drawLine(x+1, y+h-1, x+w-2, y+h-1, color);
  drawLine(x, y+1, x, y+h-2, color);
  drawLine(x+w-1, y+1, x+w-1, y+h-2, color);
  fillRect(x+2, y+2, ((w-4)*progress)/254, h-4, color);
}

void LCD::stairs() {
  
  drawLine(3,39,3,43,BLACK);
  drawPixel(2,40,BLACK);
  drawLine(77,24,77,28,BLACK);
  drawPixel(76,25,BLACK);
  fillRect(79, 24, 3, 5, BLACK);
  drawLine(80,25,81,25,WHITE);
  drawPixel(80,27,WHITE);
  
  for(int i=1; i <= 16; i++) {
    
    drawRect((6*i)-i-3,46-i,6,20,BLACK);
  }
}

void LCD::stairStep(uint16_t bitnumber) {
  
  for(int i=1; i <= 16; i++) {
    
    if(bitnumber & (1 << i-1)) {
      
      fillRect((6*i)-i-1, 48-i, 2, 20, BLACK);
    } else {
      
      fillRect(79, 48-i, 2, 20, WHITE);
    }
  }
}

void LCD::scrollBar(uint8_t pos) {
  
  drawLine(79, 8, 82, 8, BLACK);
  drawLine(79, 47, 82, 47, BLACK);
  drawLine(78, 9, 78, 46, BLACK);
  drawLine(83, 9, 83, 46, BLACK);
  fillRect(80, 10, 2, 36, WHITE);
  if(pos == 0) {
    fillRect(80, 10, 2, 18, BLACK);
  } else {
    fillRect(80, 28, 2, 18, BLACK);
  }
}

void LCD::gird(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
  
  boolean c = 1;
  for(int i=y; i <= y+h; i++) {
    for(int j=x; j <= x+w; j++) {
      drawPixel(j, i, c);
      c = !c;
    }
  }
}

void LCD::window(char* nameText) {
  
  fillRoundRect(0,0,84,9,1,BLACK);
  drawRoundRect(0,0,84,48,1,BLACK);
  setCursor(3,1);
  setTextColor(WHITE,BLACK);
  setTextSize(1);
  println(nameText);
  setTextColor(BLACK);
  setCursor(4,38);
  print("<<");
  setCursor(34,38);
  print("SEL");
  setCursor(68,38);
  print(">>");
  display();
  
}
//============================== PANELS ===========================================================

void LCD::startPanel() {
  
  clearDisplay();
  logo();
  for(int i=1; i <= 254; i++) {
    
    progressBar(0,40,84,8, BLACK,i);
    display();
  }
}

void LCD::mainPanel(uint8_t Status, uint8_t Mode, boolean Down, boolean Up, uint8_t progress, uint16_t binStairStep) {
  
  clearDisplay();
  setCursor(0,0);
  setTextColor(BLACK);
  setTextSize(1);
  print("STATUS:");
  switch(Status) {
    case 0:
      print("OFF");
      break;
    case 1: 
      print("ON");
      break;
  }
  setCursor(0,8);
  print("MODE:");
  switch(Mode) {
    case 10:
      print("WAVE");
      break;
    case 11:
      print("CASCADE");
      break;
    case 12:
      print("ELEVATOR");
      break;
  } 
  progressBar(0, 16, 84, 6, 1, progress);
  setCursor(0,23);
  print("DOWN:"); print(Down ? "1" : "0");
  setCursor(0,31);
  print("UP:"); print(Up ? "1" : "0");
  stairs();
  stairStep(binStairStep);  // (high byte * 256) + low byte
  display();
}

void LCD::mainMenu(uint8_t positionNumber) {
  
  if(positionNumber > 8) positionNumber = 8;
  if(positionNumber == 0) return void();
  
  clearDisplay();
  setCursor(30,0);
  setTextColor(BLACK);
  setTextSize(1);
  println("MENU");
  gird(0,0,26,6);
  gird(56,0,28,6);
  
  if(positionNumber < 5) {  //top menu
    
    if(positionNumber == 1) {
      fillRoundRect(0, 9, 77, 9, 1,BLACK);
      setTextColor(WHITE,BLACK);
      setCursor(3,10);
      println("CONTROL");
    } else {
      setTextColor(BLACK);
      setCursor(3,10);
      println("CONTROL");
    }
    if(positionNumber == 2) {
      fillRoundRect(0, 19, 77, 9, 1,BLACK);
      setTextColor(WHITE,BLACK);
      setCursor(3,20);
      println("MODE");
    } else {
      setTextColor(BLACK);
      setCursor(3,20);
      println("MODE");
    }
    if(positionNumber == 3) {
      fillRoundRect(0, 29, 77, 9, 1,BLACK);
      setTextColor(WHITE,BLACK);
      setCursor(3,30);
      println("TIMES");
    } else {
      setTextColor(BLACK);
      setCursor(3,30);
      println("TIMES");
    }
    if(positionNumber == 4) {
      fillRoundRect(0, 39, 77, 9, 1,BLACK);
      setTextColor(WHITE,BLACK);
      setCursor(3,40);
      println("LCD LIGHT");
    } else {
      setTextColor(BLACK);
      setCursor(3,40);
      println("LCD LIGHT");
    }
    scrollBar(0);
    display();
    
  } else {  //bottom menu
  
    if(positionNumber == 5) {
      fillRoundRect(0, 9, 77, 9, 1,BLACK);
      setTextColor(WHITE,BLACK);
      setCursor(3,10);
      println("LCD CONTRAST");
    } else {
      setTextColor(BLACK);
      setCursor(3,10);
      println("LCD CONTRAST");
    }
    if(positionNumber == 6) {
      fillRoundRect(0, 19, 77, 9, 1,BLACK);
      setTextColor(WHITE,BLACK);
      setCursor(3,20);
      println("SENSOR PIR");
    } else {
      setTextColor(BLACK);
      setCursor(3,20);
      println("SENSOR PIR");
    }
    if(positionNumber == 7) {
      fillRoundRect(0, 29, 77, 9, 1,BLACK);
      setTextColor(WHITE,BLACK);
      setCursor(3,30);
      println("DEMO");
    } else {
      setTextColor(BLACK);
      setCursor(3,30);
      println("DEMO");
    }
    if(positionNumber == 8) {
      fillRoundRect(0, 39, 77, 9, 1,BLACK);
      setTextColor(WHITE,BLACK);
      setCursor(3,40);
      println("EXIT");
    } else {
      setTextColor(BLACK);
      setCursor(3,40);
      println("EXIT");
    }
    scrollBar(1);
    display();
  }
}

void LCD::menuLCDLight(uint8_t val) {
  
  clearDisplay();
  window("LCD LIGHT");
  setCursor(33,16);
  print(map(val,0,254,0,100)); print("%");
  progressBar(4,25,76,8, BLACK,val);
  display();
}

void LCD::menuLCDContrast(uint8_t val) {
  
  clearDisplay();
  window("LCD CONTRAST");
  setCursor(33,16);
  print(map(val,0,128,0,100)); print("%");
  progressBar(4,25,76,8, BLACK,map(val,0,128,0,255));
  display();
}

void LCD::menuControl(uint8_t val) {
  
  clearDisplay();
  window("CONTROL");
  setCursor(35,20);
  switch(val) {
    case 0:
      print("OFF");
      break;
    case 1:
      print("ON");
      break;
  }
  display();
}

void LCD::menuMode(uint8_t val) {
  
  clearDisplay();
  window("MODE");
  setCursor(22,20);
  switch(val) {
    case 10:
      print(" WAVE");
      break;
    case 11:
      print("CASCADE");
      break;
    case 12:
      print("ELEVATOR");
      break;
  }
  display();
}

void LCD::menuTimes(uint8_t val) {
  
  clearDisplay();
  window("TIMES");
  setCursor(30,20);
  print(val/10); print("."); print(val%10); print("s");
  display();
}

void LCD::menuSensorPIR() {
  
  clearDisplay();
  window("SENSOR PIR");
  display();
}

void LCD::menuDemo() {
  
  clearDisplay();
  window("DEMO");
  display();
}
