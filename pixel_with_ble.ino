#include <Adafruit_NeoPixel.h>
#include <RotaryEncoder.h>
#include <ArduinoBLE.h>

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
BLEService input_service("50cb2718-83f9-4efc-b35b-2544bb24d8b6");
BLEByteCharacteristic rotary_position_characteristic("961c5b3c-4e94-4cee-84b3-e84a5d036950", BLERead);
BLEByteCharacteristic button_center_characteristic("28e0c7b8-95e4-4c69-939e-0a7bf68606c5", BLERead);
// add some sensors?

BLEService output_service("cd457cc4-f310-4d7b-addd-3291067b12ec");
BLEStringCharacteristic color_characteristic("d0df504e-b877-427c-b014-f22ddefd387a", BLERead | BLEWrite, 20);
BLEByteCharacteristic mode_characteristic("595f7ddd-8e72-4989-a553-bdd922c5efff", BLERead | BLEWrite);
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
