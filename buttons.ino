void buttonHandler() {
  // Debounce
  if (digitalRead(BUTTON_PIN_CENTER) == HIGH && button_center_state == LOW) {
    button_center_state = HIGH;
    setupPixels();

    switch (active_light_mode) {
      case CHASE:
        active_light_mode = TRAIL;
        break;

      case TRAIL:
        active_light_mode = FADE;
        break;

      case FADE:
        active_light_mode = MANUAL;
        break;

      case MANUAL:
        active_light_mode = CHASE;
        break;
    }

  } else {
    button_center_state = digitalRead(BUTTON_PIN_CENTER);
  }
}