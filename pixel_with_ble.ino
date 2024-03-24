#include <Adafruit_NeoPixel.h>
#include <RotaryEncoder.h>
#include <ArduinoBLE.h>
#include <ArduinoJson.h>

//  Peripherals
#define NEO_PIXEL_PIN 2
#define ROTARY_ENCA 3
#define ROTARY_ENCB 4
#define BUTTON_PIN_CENTER 6

// Initialization
Adafruit_NeoPixel pixel_ring(12, NEO_PIXEL_PIN, NEO_GRB + NEO_KHZ800);
RotaryEncoder encoder(ROTARY_ENCB, ROTARY_ENCA, RotaryEncoder::LatchMode::FOUR3);

unsigned long previous_millis = 0;
int active_pixel = 0;
int brightness = 0;
int fade_amount = 5;

uint32_t default_color;
uint32_t active_color;

int button_center_state = LOW;
const int BLE_BTN_CENTER_DEFAULT_STATE = 0;

// constants

const int INTERVAL = 30;
const int MAX_BRIGHTNESS = 100;

enum pixelMode { CHASE,
                 TRAIL,
                 FADE,
                 MANUAL };

pixelMode active_light_mode;

// BLE
BLEService input_service("50CB2718-83F9-4EFC-B35B-2544BB24D8B6");
BLEStringCharacteristic rotary_position_characteristic("961C5B3C-4E94-4CEE-84B3-E84A5D036950", BLERead | BLENotify, 25);
BLEStringCharacteristic button_center_characteristic("28E0C7B8-95E4-4C69-939E-0A7BF68606C5", BLERead | BLENotify, 25);
// add some sensors?

BLEService output_service("CD457CC4-F310-4D7B-ADDD-3291067B12EC");
BLEStringCharacteristic color_characteristic("D0DF504E-B877-427C-B014-F22DDEFD387A", BLERead | BLEWrite, 25);
BLEStringCharacteristic mode_characteristic("F4F7790A-6C50-40C6-9A75-37A0F78A6FB3", BLERead | BLEWrite | BLENotify, 25);
// add brightness control

// Setup
void setup() {
  Serial.begin(9600);
  pixel_ring.begin();
  setupPixels();
  pixel_ring.show();

  pinMode(NEO_PIXEL_PIN, OUTPUT);
  digitalWrite(NEO_PIXEL_PIN, LOW);

  pinMode(BUTTON_PIN_CENTER, INPUT_PULLUP);

  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");

    while (1);
  }

  setupBLE();

  // to remove
  active_light_mode = MANUAL;
  setupInitialColor();
  
}

// Main Loop
void loop() {
  buttonHandler();
  BLE.poll();

  unsigned long current_millis = millis();

  if (current_millis - previous_millis >= INTERVAL) {
    previous_millis = current_millis;
    lightingModeHandler();
  }

  rotaryHandler();
}
