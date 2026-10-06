#pragma once

#include <Arduino.h>

enum ButtonEvent { NONE, SINGLE_CLICK, DOUBLE_CLICK, LONG_PRESS };

class SimpleButton {
  public:
    SimpleButton(int pin);
    void begin();
    ButtonEvent update(); // Call this every loop()

  private:
    int _pin;
    bool _lastButtonState;
    bool _currentButtonState;
    unsigned long _lastDebounceTime;
    unsigned long _pressStartTime;
    unsigned long _releaseTime;

    int _clickCount;
    bool _isPressing;
    bool _longPressTriggered;

    static const unsigned long DEBOUNCE_DELAY = 50;
    static const unsigned long LONG_PRESS_TIME = 800;
    static const unsigned long DOUBLE_CLICK_GAP = 250;
};
