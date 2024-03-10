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

uint32_t default_color = pixel_ring.Color(random(255), random(0), random(0));
uint32_t active_color = 0;

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
// separate services for input and output peripherals
BLEService led_service("36944a3d-73cd-4ff4-860a-ac2cc04812c5");
BLEByteCharacteristic pixel_characteristic("595f7ddd-8e72-4989-a553-bdd922c5efff", BLERead | BLEWrite);
BLEByteCharacteristic rotary_characteristic("961c5b3c-4e94-4cee-84b3-e84a5d036950", BLERead | BLEWrite);
BLEByteCharacteristic button_center_characteristic("28e0c7b8-95e4-4c69-939e-0a7bf68606c5", BLERead | BLEWrite);
// BLECharCharacteristic color_characteristic("d0df504e-b877-427c-b014-f22ddefd387a", BLERead | BLEWrite);
BLEStringCharacteristic color_characteristic("d0df504e-b877-427c-b014-f22ddefd387a", BLERead | BLEWrite, 20);

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

  bleSetup();

  // to remove
  active_light_mode = MANUAL;
  active_color = default_color;
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
  // bleHandler();
}

// void bleHandler() {
//   if (button_center_characteristic.value() != BLE_BTN_CENTER_DEFAULT_STATE) {
//     lightingModeSwitchHandler();
//     button_center_characteristic.writeValue(0);
//   }

  // if (color_characteristic.value() != 0 && color_characteristic.value() != default_color) {
    // Serial.println(color_characteristic.getData());
    // active_color = color_characteristic.value();
    // default_color = color_characteristic.value();
//   // }
// }