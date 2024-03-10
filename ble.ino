void bleSetup() {
  BLE.setLocalName("LED Prototype");
  
  BLE.setAdvertisedService(led_service);

  led_service.addCharacteristic(pixel_characteristic);
  led_service.addCharacteristic(rotary_characteristic);
  led_service.addCharacteristic(button_center_characteristic);
  led_service.addCharacteristic(color_characteristic);

  BLE.addService(led_service);
  pixel_characteristic.writeValue(0);
  rotary_characteristic.writeValue(0);
  button_center_characteristic.writeValue(0);
  // color_characteristic.writeValue(default_color);

  color_characteristic.setEventHandler(BLEWritten, colorCharacteristicWritten);

  BLE.advertise();
}

void colorCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  // central wrote new value to characteristic, update LED
  Serial.print("Characteristic event, written: ");
  Serial.println(getValue(color_characteristic.value(), ',', 2));

  int color_r = getValue(color_characteristic.value(), ',', 0).toInt();
  int color_g = getValue(color_characteristic.value(), ',', 1).toInt();
  int color_b = getValue(color_characteristic.value(), ',', 2).toInt();

  default_color = pixel_ring.Color(color_r, color_g, color_b);
  active_color = default_color;

  if (active_light_mode == MANUAL) showLights(true, active_pixel, active_color);
}