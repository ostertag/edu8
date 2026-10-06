#include "SimpleButton.h"

SimpleButton::SimpleButton(int pin) {
  _pin = pin;
  _lastButtonState = HIGH;
  _currentButtonState = HIGH;
  _lastDebounceTime = 0;
  _clickCount = 0;
  _isPressing = false;
  _longPressTriggered = false;
}

void SimpleButton::begin() {
  pinMode(_pin, INPUT_PULLUP);
}

ButtonEvent SimpleButton::update() {
  ButtonEvent event = NONE;
  bool reading = digitalRead(_pin);

  // Debounce logic
  if (reading != _lastButtonState) {
    _lastDebounceTime = millis();
  }

  if ((millis() - _lastDebounceTime) > DEBOUNCE_DELAY) {
    if (reading != _currentButtonState) {
      _currentButtonState = reading;

      if (_currentButtonState == LOW) { // Pressed
        _isPressing = true;
        _pressStartTime = millis();
        _longPressTriggered = false;
      } else { // Released
        _isPressing = false;
        _releaseTime = millis();
        if (!_longPressTriggered) {
          _clickCount++;
        }
      }
    }
  }

  _lastButtonState = reading;

  // Check for Long Press while holding
  if (_isPressing && !_longPressTriggered) {
    if (millis() - _pressStartTime > LONG_PRESS_TIME) {
      _longPressTriggered = true;
      _clickCount = 0;
      return LONG_PRESS;
    }
  }

  // Check for Single vs Double Click after release
  if (!_isPressing && _clickCount > 0) {
    if (millis() - _releaseTime > DOUBLE_CLICK_GAP) {
      if (_clickCount == 1) event = SINGLE_CLICK;
      else if (_clickCount >= 2) event = DOUBLE_CLICK;
      
      _clickCount = 0; // Reset
    }
  }

  return event;
}