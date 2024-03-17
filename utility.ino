String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void setupInitialColor() {
  int r = random(255);
  int g = random(255);
  int b = random(255);

  default_color = pixel_ring.Color(r, g, b);
  active_color = default_color;

  String color_string = String(r) + "," + String(g) + "," + String(b);
  color_characteristic.writeValue(color_string);
}

void setMode(int modeIndex) {
  switch (modeIndex) {
    case 0:
      active_light_mode = CHASE;
      break;

    case 1:
      active_light_mode = TRAIL;
      break;

    case 2:
      active_light_mode = FADE;
      break;

    case 3:
      active_light_mode = MANUAL;
      break;

    default:
      break;
  }
}