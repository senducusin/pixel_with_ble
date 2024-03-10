void buttonHandler() {
  // Debounce
  if (digitalRead(BUTTON_PIN_CENTER) == HIGH && button_center_state == LOW) {
    button_center_state = HIGH;
    // button_center_characteristic.writeValue(1);
    lightingModeSwitchHandler();

  } else {
    button_center_state = digitalRead(BUTTON_PIN_CENTER);
    // button_center_characteristic.writeValue(0);
  }
}