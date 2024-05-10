#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include <Arduino.h>

class ButtonHandler {
public:
  ButtonHandler(int pin);
  bool isButtonPressed();

private:
  int triggerPin;
};

#endif
