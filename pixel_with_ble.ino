#include <Adafruit_NeoPixel.h>
#include <RotaryEncoder.h>

//  Peripherals
#define NEO_PIXEL_PIN 2
#define ROTARY_ENCA 3
#define ROTARY_ENCB 4
#define BUTTON_PIN_CENTER 6

// Initialization
Adafruit_NeoPixel pixel_ring(12, NEO_PIXEL_PIN, NEO_GRB + NEO_KHZ800);
RotaryEncoder encoder(ROTARY_ENCB, ROTARY_ENCA, RotaryEncoder::LatchMode::FOUR3);

unsigned long previous_millis = 0;
uint32_t color = 0;
int active_pixel = 0;
int brightness = 0;
int fade_amount = 5;
int button_center_state = LOW;

// constants
const uint32_t SELECTED_COLOR = pixel_ring.Color(random(255), random(255), random(255));
const int INTERVAL = 30;
const int MAX_BRIGHTNESS = 100;

enum pixelMode { CHASE,
                 TRAIL,
                 FADE,
                 MANUAL };

pixelMode active_light_mode;

// Setup
void setup() {
  Serial.begin(9600);
  pixel_ring.begin();
  setupPixels();
  pixel_ring.show();

  pinMode(NEO_PIXEL_PIN, OUTPUT);
  digitalWrite(NEO_PIXEL_PIN, LOW);

  pinMode(BUTTON_PIN_CENTER, INPUT_PULLUP);

  // to remove
  active_light_mode = MANUAL;
  color = SELECTED_COLOR;
}

// Main Loop
void loop() {
  buttonHandler();

  unsigned long current_millis = millis();

  if (current_millis - previous_millis >= INTERVAL) {
    previous_millis = current_millis;
    lightingModeHandler();
  }

  rotaryHandler();
}