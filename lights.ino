void setupPixels() {
  active_pixel = 0;
  active_color = default_color;
  pixel_ring.clear();
  pixel_ring.setBrightness(30);
  pixel_ring.show();
}

void lightingModeSwitchHandler() {
  setupPixels();

  if (active_light_mode == MANUAL) {
    showLights(true, active_pixel, active_color);
  }

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

  int mode = (int)(active_light_mode);
  String json = createNotifyValue(mode);
  mode_characteristic.writeValue(json);
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

  pixel_ring.setBrightness(brightness);

  for (int i = 0; i < pixel_ring.numPixels(); i++) {
    showLights(false, i, active_color);
  }
}

void colorChase(bool show_trail) {
  showLights(!show_trail, active_pixel, active_color);
  active_pixel++;

  if (active_pixel > pixel_ring.numPixels()) {
    active_pixel = 0;
    active_color = ((show_trail && active_color) == 0) ? default_color : 0;
  }
}