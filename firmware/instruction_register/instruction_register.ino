#include <LiquidCrystal.h>

// shift registers
const int CP_PIN = 4;
const int DATA_IN_PIN = 14;
const int CE_PIN = 3;
const int PL_PIN = 5;

// control signals
const int IR1_IN_PIN = 10;
const int IR2_IN_PIN = 9;
const int IR3_IN_PIN = 8;

// button
const int SW_MODE_PIN = 2;

// lcd
const int RS_PIN = 1;
const int E_PIN = 0;
const int D4_PIN = 19;
const int D5_PIN = 18;
const int D6_PIN = 17;
const int D7_PIN = 16;

LiquidCrystal lcd(RS_PIN, E_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);

bool ir1_in_state, ir2_in_state, ir3_in_state;

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

void setup() {

  // shift registers
  pinMode(CP_PIN, OUTPUT);
  digitalWrite(CP_PIN, LOW);
  pinMode(CE_PIN, OUTPUT);
  digitalWrite(CE_PIN, LOW);
  pinMode(PL_PIN, OUTPUT);
  digitalWrite(PL_PIN, HIGH);
  pinMode(DATA_IN_PIN, INPUT);

  // control signals
  pinMode(IR1_IN_PIN, INPUT);
  pinMode(IR2_IN_PIN, INPUT);
  pinMode(IR3_IN_PIN, INPUT);
  
  // button TMP
  pinMode(SW_MODE_PIN, INPUT_PULLUP);

  // lcd
  lcd.begin(16, 2);
  lcd.clear();

  ir1_in_state = false;
  ir2_in_state = false;
  ir3_in_state = false;
  
}

char hexChar(uint8_t nibble) {
  if (nibble <= 9) return '0' + nibble;
  return 'A' + nibble - 10;
}

void updateDisplay() {
  loadWord();
  uint8_t q1 = readByte();
  uint8_t q2 = readByte();
  uint8_t q3 = readByte();
  lcd.clear();
  lcd.print(hexChar((q1 >> 4) & 0x0f));
  lcd.print(hexChar(q1 & 0x0f));
  lcd.print("  ");
  lcd.print(hexChar((q2 >> 4) & 0x0f));
  lcd.print(hexChar(q2 & 0x0f));
  lcd.print("  ");
  lcd.print(hexChar((q3 >> 4) & 0x0f));
  lcd.print(hexChar(q3 & 0x0f));
}

void loop() {
  // bool change = false;
  // if (digitalRead(IR1_IN_PIN) != ir1_in_state) {
  //   ir1_in_state = !ir1_in_state;
  //   change = true;
  // }
  // if (digitalRead(IR2_IN_PIN) != ir2_in_state) {
  //   ir2_in_state = !ir2_in_state;
  //   change = true;
  // }
  // if (digitalRead(IR3_IN_PIN) != ir3_in_state) {
  //   ir3_in_state = !ir3_in_state;
  //   change = true;
  // }

  // if (change) {
  //   updateDisplay();
  // }

  // delay(10);

  static int counter = 0;
  lcd.clear();
  lcd.print("Hello, World!");
  lcd.setCursor(0, 1);
  lcd.print("counter: ");
  lcd.print(counter++);
  delay(2000);
}
