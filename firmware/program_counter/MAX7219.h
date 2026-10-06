#pragma once

#include <Arduino.h>
#include <SPI.h>

// MAX7219 Register Addresses
#define REG_NOOP        0x00
#define REG_DIGIT0      0x01
#define REG_DIGIT1      0x02
#define REG_DIGIT2      0x03
#define REG_DIGIT3      0x04
#define REG_DIGIT4      0x05
#define REG_DIGIT5      0x06
#define REG_DIGIT6      0x07
#define REG_DIGIT7      0x08
#define REG_DECODE_MODE 0x09
#define REG_INTENSITY   0x0A
#define REG_SCAN_LIMIT  0x0B
#define REG_SHUTDOWN    0x0C
#define REG_DISPLAY_TEST 0x0F

// Segment bitmasks for raw control
#define SEG_A  0b01000000
#define SEG_B  0b00100000
#define SEG_C  0b00010000
#define SEG_D  0b00001000
#define SEG_E  0b00000100
#define SEG_F  0b00000010
#define SEG_G  0b00000001
#define SEG_DP 0b10000000

class MAX7219 {
public:
    MAX7219(uint8_t csPin);
    
    void begin();
    void setBrightness(uint8_t intensity); // 0 to 15
    void setScanLimit(uint8_t limit);      // 0 to 7
    void shutdown(bool status);            // true = off, false = on
    void clearDisplay();
    
    // Display methods
    void setDigit(uint8_t digit, uint8_t value, bool dot = false);
    void setHex(uint8_t digit, uint8_t value, bool dot = false);

    // 'mask' is a combination of SEG_A, SEG_B, etc.
    void setSegments(uint8_t digit, uint8_t mask);

private:
    uint8_t _csPin;
    void sendData(uint8_t address, uint8_t data);
    
    // Segment map for Hex 0-F (No-decode mode)
    // Bits: DP A B C D E F G
    static const uint8_t PROGMEM _hexFont[16];
};
