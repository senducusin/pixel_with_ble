void setupBLE() {
  BLE.setLocalName("LED Prototype");

  setupInputCharacteristics();
  setupOutputCharacteristics();

  color_characteristic.setEventHandler(BLEWritten, onColorCharacteristicsUpdate);
  mode_characteristic.setEventHandler(BLEWritten, onModeCharacteristicsUpdate);

  BLE.advertise();
}

void setupOutputCharacteristics() {
  // setup
  BLE.setAdvertisedService(output_service);

  output_service.addCharacteristic(color_characteristic);
  output_service.addCharacteristic(mode_characteristic);

  BLE.addService(output_service);

  // initial values
  // color_characteristic initial value is added in setupInitialColor()
  mode_characteristic.writeValue(0);
}

void setupInputCharacteristics() {
  // setup
  BLE.setAdvertisedService(input_service);

  input_service.addCharacteristic(rotary_position_characteristic);
  input_service.addCharacteristic(button_center_characteristic);

  BLE.addService(input_service);

  // initial values
  rotary_position_characteristic.writeValue(0);
  button_center_characteristic.writeValue(0);
}

void onColorCharacteristicsUpdate(BLEDevice central, BLECharacteristic characteristic) {
  int color_r = getValue(color_characteristic.value(), ',', 0).toInt();
  int color_g = getValue(color_characteristic.value(), ',', 1).toInt();
  int color_b = getValue(color_characteristic.value(), ',', 2).toInt();

  default_color = pixel_ring.Color(color_r, color_g, color_b);
  active_color = default_color;

  if (active_light_mode == MANUAL) showLights(true, active_pixel, active_color);
}

void onModeCharacteristicsUpdate(BLEDevice central, BLECharacteristic characteristic) {
  setupPixels();

  if (mode_characteristic.value() == 3) {
    showLights(true, active_pixel, active_color);
  }

  setMode(mode_characteristic.value());
}