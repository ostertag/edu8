#include "MAX7219.h"

// Segment mapping for common cathode 7-segment (ABCDEFG)
// Map: 0b(DP)(A)(B)(C)(D)(E)(F)(G)
const uint8_t PROGMEM MAX7219::_hexFont[16] = {
    0x7E, // 0
    0x30, // 1
    0x6D, // 2
    0x79, // 3
    0x33, // 4
    0x5B, // 5
    0x5F, // 6
    0x70, // 7
    0x7F, // 8
    0x7B, // 9
    0x77, // A
    0x1F, // b
    0x4E, // C
    0x3D, // d
    0x4F, // E
    0x47  // F
};

MAX7219::MAX7219(uint8_t csPin) : _csPin(csPin) {}

void MAX7219::begin() {
    pinMode(_csPin, OUTPUT);
    digitalWrite(_csPin, HIGH);
    SPI.begin();
    
    sendData(REG_DISPLAY_TEST, 0x00); // Test off
    sendData(REG_DECODE_MODE, 0x00);  // Use manual segment control (No Decode)
    setScanLimit(0x07);               // Use all 8 digits
    setBrightness(0x08);              // Medium brightness
    shutdown(false);                  // Wake up
    clearDisplay();
}

void MAX7219::sendData(uint8_t address, uint8_t data) {
    digitalWrite(_csPin, LOW);
    SPI.transfer(address);
    SPI.transfer(data);
    digitalWrite(_csPin, HIGH);
}

void MAX7219::setBrightness(uint8_t intensity) {
    sendData(REG_INTENSITY, intensity & 0x0F);
}

void MAX7219::setScanLimit(uint8_t limit) {
    sendData(REG_SCAN_LIMIT, limit & 0x07);
}

void MAX7219::shutdown(bool status) {
    sendData(REG_SHUTDOWN, status ? 0x00 : 0x01);
}

void MAX7219::clearDisplay() {
    for (uint8_t i = 1; i <= 8; i++) {
        sendData(i, 0x00);
    }
}

void MAX7219::setSegments(uint8_t digit, uint8_t mask) {
    if (digit > 7) return;
    sendData(digit + 1, mask);
}

void MAX7219::setHex(uint8_t digit, uint8_t value, bool dot) {
    if (digit > 7 || value > 15) return;
    
    // Get segment pattern from Flash memory
    uint8_t pattern = pgm_read_byte(&_hexFont[value]);
    
    // If decimal point is needed, flip the MSB (bit 7)
    if (dot) pattern |= 0x80;
    
    // Register address for digit 0 is 0x01, digit 7 is 0x08
    sendData(digit + 1, pattern);
}

void MAX7219::setDigit(uint8_t digit, uint8_t value, bool dot) {
    setHex(digit, value, dot); // Re-use hex logic for 0-9
}