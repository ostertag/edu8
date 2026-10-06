#include "MAX7219.h"
#include "SimpleButton.h"

// SPI CS
const int CS_PIN = 14;

// shift registers
const int CP_PIN = 1;
const int DATA_IN_PIN = 3;
const int CE_PIN = 4;
const int PL_PIN = 19;

// button
const int SW_MODE_PIN = 2;

MAX7219 display(CS_PIN);
SimpleButton swMode(SW_MODE_PIN);


unsigned long last;
uint16_t value;
bool isHex;


void loadWord() {
    digitalWrite(PL_PIN, LOW);
    delay(1);
    digitalWrite(PL_PIN, HIGH);
    delay(1);
}

uint8_t readByte() {
    uint8_t val = 0;
    for (int i = 7; i >= 0; i--) {
        val |= (digitalRead(DATA_IN_PIN) << i);
        digitalWrite(CP_PIN, HIGH);
        delay(1);
        digitalWrite(CP_PIN, LOW);
        delay(1);
    }
    return val;
}

uint8_t flip(int num, uint8_t val) {
  uint8_t result = 0;
  for (int i = 0; i < num; i++) {
    result |= ((val >> i) & 1) << (num-1-i);
  }
  return result;
}

void displayDec(uint16_t val) {
  display.clearDisplay();
  for (int i = 0; i < 5; i++) {
      int d = val % 10;
      val /= 10;
      display.setDigit(i, d);
      if (val == 0) break;
  }
}

void displayHex(uint16_t val) {
  display.clearDisplay();
  for (int i = 0; i < 4; i++) {
      int d = val & 0x0f;
      val >>= 4;
      display.setDigit(i, d);
      // if (val == 0) break;
  }
}

void startupAnimation() {
  display.clearDisplay();
  for (int i = 0; i < 8; i++) display.setSegments(i, SEG_DP);
  delay(500);
  static byte order[] = {SEG_D, SEG_E, SEG_F, SEG_A, SEG_B, SEG_C, SEG_D, SEG_G};
  for (int i = 0; i < sizeof(order); i++) {
      display.clearDisplay();
      for (int j = 0; j < 8; j++) display.setSegments(j, order[i]);
      delay(200);
  }
  delay(300);
  display.clearDisplay();
}


void showType() {
  display.clearDisplay();
  display.setSegments(7, SEG_C | SEG_D | SEG_E | SEG_F | SEG_G); // b
  display.setSegments(6, SEG_G); // -
  display.setDigit(5, 1);
  if (isHex) {
    display.setDigit(4, 6);
    // display.setSegments(7, SEG_B, SEG_C, SEG_E, SEG_F, SEG_G); // H
    // display.setSegments(6, SEG_A, SEG_D, SEG_E, SEG_F, SEG_G); // E
  } else {
    display.setDigit(4, 0);
    // display.setSegments(7, SEG_B, SEG_C, SEG_D, SEG_E, SEG_G); // d
    // display.setSegments(6, SEG_A, SEG_D, SEG_E, SEG_F, SEG_G); // E
    // display.setSegments(5, SEG_A, SEG_D, SEG_E, SEG_F); // C
  }
  delay(750);
}

void displayValue(uint16_t value) {
  isHex ? displayHex(value) : displayDec(value);
}





void setup() {
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);

  pinMode(CP_PIN, OUTPUT);
  digitalWrite(CP_PIN, LOW);
  pinMode(CE_PIN, OUTPUT);
  digitalWrite(CE_PIN, LOW);
  pinMode(PL_PIN, OUTPUT);
  digitalWrite(PL_PIN, HIGH);
  pinMode(DATA_IN_PIN, INPUT);

  swMode.begin();
  display.begin();
  display.setBrightness(8);

  last = 0;
  value = 0;
  isHex = false;

  startupAnimation();

  showType();
  displayValue(value);
}

void loop() {
  unsigned long now = millis();
  if (now - last >= 100) {
    loadWord();
    uint8_t lsbRaw = readByte();
    uint16_t msbRaw = readByte();
    value = 0;
    value |= lsbRaw & 0xf0;
    value |= flip(4, lsbRaw & 0x0f) & 0x0f;
    value |= (msbRaw & 0xf0) << 8;
    value |= (flip(4, msbRaw & 0x0f) & 0x0f) << 8;
    displayValue(value);
    last = now;
  }

  ButtonEvent event = swMode.update();
  switch (event) {
    case SINGLE_CLICK:
      isHex = !isHex;
      showType();
      break;
    case DOUBLE_CLICK:
      break;
    case LONG_PRESS:
      break;
    default:
      break;
  }

  delay(10);
}
