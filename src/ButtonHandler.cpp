#include "ButtonHandler.h"

ButtonHandler::ButtonHandler(int pin) : triggerPin(pin) {}

bool ButtonHandler::isButtonPressed() {
  return digitalRead(triggerPin) == LOW;
}
