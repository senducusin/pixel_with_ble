void rotaryHandler() {
  if (active_light_mode != MANUAL) return;

  encoder.tick();

  static int position = 0;
  int new_position = encoder.getPosition();
  int direction = (int)(encoder.getDirection());

  if (position != new_position) {
    if (direction == 1) {
      // clockwise
      if (active_pixel < pixel_ring.numPixels() - 1) {
        active_pixel++;
      } else {
        active_pixel = 0;
      }

    } else if (direction == -1) {
      // counter-clockwise
      if (active_pixel > 0) {
        active_pixel--;
      } else {
        active_pixel = pixel_ring.numPixels() - 1;
      }
    }

    position = new_position;
    String value = createNotifyValue(position);
    rotary_position_characteristic.writeValue(value);
    showLights(true, active_pixel, active_color);
  }
}