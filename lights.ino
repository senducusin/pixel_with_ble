void setupPixels() {
  active_pixel = 0;
  color = SELECTED_COLOR;
  pixel_ring.clear();
  pixel_ring.setBrightness(30);
  pixel_ring.show();
}

void lightingModeHandler() {
  switch (active_light_mode) {
    case CHASE:
      colorChase(false);
      break;

    case TRAIL:
      colorChase(true);
      break;

    case FADE:
      colorFade();
      break;

    default:
      break;
  }
}

void showLights(bool show_single_pixel, int pixel_index, uint32_t color) {
  if (show_single_pixel) {
    pixel_ring.clear();
  }

  pixel_ring.setPixelColor(pixel_index, color);
  pixel_ring.show();
}

void colorFade() {
  brightness = brightness + fade_amount;

  if (brightness <= 0 || brightness >= MAX_BRIGHTNESS) {
    fade_amount = -fade_amount;
  }

  Serial.println(brightness);
  pixel_ring.setBrightness(brightness);

  for (int i = 0; i < pixel_ring.numPixels(); i++) {
    showLights(false, i, color);
  }
}

void colorChase(bool show_trail) {
  showLights(!show_trail, active_pixel, color);
  active_pixel++;

  if (active_pixel > pixel_ring.numPixels()) {
    active_pixel = 0;
    color = (show_trail && color) == 0 ? SELECTED_COLOR : 0;
  }
}