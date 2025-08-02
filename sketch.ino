#include <Adafruit_SH110X.h>
#include <Adafruit_NeoPixel.h>
#include <RotaryEncoder.h>
#include <Wire.h>
#include "pitches.h"
#include <Keyboard.h>

// Create the neopixel strip with the built in definitions NUM_NEOPIXEL and PIN_NEOPIXEL
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_NEOPIXEL, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

// Create the OLED display
Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, &SPI1, OLED_DC, OLED_RST, OLED_CS);

// Create the rotary encoder
// RotaryEncoder encoder(PIN_ROTA, PIN_ROTB, RotaryEncoder::LatchMode::FOUR3);
// void checkPosition() {  encoder.tick(); } // just call tick() to check the state.
// our encoder position state
// int encoder_pos = 0;

int activeKey = PIN_SWITCH;

void setup() {
  // start keyboard
  Keyboard.begin();

  // start key backlight pixels
  pixels.begin();
  pixels.setBrightness(255);
  pixels.show();  // Initialize all pixels to 'off'

  // Start OLED
  display.begin(0, true);  // we dont use the i2c address but we will reset!
  display.display();

  // set all mechanical keys to inputs
  for (uint8_t i = 0; i <= 12; i++) {
    pinMode(i, INPUT_PULLUP);
  }

  // set rotary encoder inputs and interrupts
  // pinMode(PIN_ROTA, INPUT_PULLUP);
  // pinMode(PIN_ROTB, INPUT_PULLUP);
  // attachInterrupt(digitalPinToInterrupt(PIN_ROTA), checkPosition, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(PIN_ROTB), checkPosition, CHANGE);

  // text display tests
  display.setTextSize(1);
  display.setTextWrap(false);
  display.setTextColor(SH110X_WHITE, SH110X_BLACK);  // white text, black background

  // Enable speaker
  // pinMode(PIN_SPEAKER_ENABLE, OUTPUT);
  // digitalWrite(PIN_SPEAKER_ENABLE, HIGH);

  // Get ready to play some tones
  // pinMode(PIN_SPEAKER, OUTPUT);
  // digitalWrite(PIN_SPEAKER, LOW);
  // tone(PIN_SPEAKER, NOTE_C5, 100);  // tone1 - B5
  // delay(100);
  // tone(PIN_SPEAKER, NOTE_C6, 200); // tone2 - E6
  // delay(200);
}

// uint8_t j = 0;
// bool i2c_found[128] = {false};

String padLabels[] = { "", "Ab", "A", "Bb", "B", "C", "Db", "D", "Eb", "E", "F", "Gb", "G" };

String padKeys[] = { "x", "a", "A", "b", "B", "C", "d", "D", "e", "E", "F", "g", "G" };

uint32_t padOnColors[] = {
  pixels.Color(170, 61, 57),    // Ab
  pixels.Color(255, 195, 217),  // A
  pixels.Color(229, 238, 251),  // Bb
  pixels.Color(183, 255, 227),  // B
  pixels.Color(254, 224, 0),    // C
  pixels.Color(219, 194, 71),   // Db
  pixels.Color(216, 113, 7),    // D
  pixels.Color(83, 72, 137),    // Eb
  pixels.Color(200, 255, 0),    // E
  pixels.Color(74, 213, 254),   // F
  pixels.Color(212, 233, 255),  // Gb
  pixels.Color(77, 156, 105),   // G
};
uint32_t padOffColors[] = {
  pixels.Color(0, 0, 10),
  pixels.Color(0, 0, 10),
  pixels.Color(0, 0, 10),
  pixels.Color(0, 0, 10),
  pixels.Color(0, 0, 10),
  pixels.Color(0, 0, 10),
  pixels.Color(0, 0, 10),
  pixels.Color(0, 0, 10),
  pixels.Color(0, 0, 10),
  pixels.Color(0, 0, 10),
  pixels.Color(0, 0, 10),
  pixels.Color(0, 0, 10),
};

void loop() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(10);

  // encoder.tick();          // check the encoder
  // int newPos = encoder.getPosition();
  // if (encoder_pos != newPos) {
  //   Serial.print("Encoder:");
  //   Serial.print(newPos);
  //   Serial.print(" Direction:");
  //   Serial.println((int)(encoder.getDirection()));
  //   encoder_pos = newPos;
  // }
  // display.setCursor(0, 8);
  // display.print("Rotary encoder: ");
  // display.print(encoder_pos);

  // check encoder press
  if (!digitalRead(PIN_SWITCH)) {
    if (activeKey != PIN_SWITCH) {
      Keyboard.print(padKeys[0]);
      Keyboard.releaseAll();
      activeKey = PIN_SWITCH;
    }
    activeKey = PIN_SWITCH;
  }

  for (int i = 1; i <= 12; i++) {
    if (!digitalRead(i)) {  // switch pressed!
      if (activeKey != i) {
        Keyboard.print(padKeys[i]);
        Keyboard.releaseAll();
        activeKey = i;
      }
    }
    if (activeKey == i) {
      pixels.setPixelColor(i - 1, padOnColors[i - 1]);
      display.println(padLabels[i]);
    } else {
      pixels.setPixelColor(i - 1, padOffColors[i - 1]);
    }
  }

  // show neopixels
  pixels.show();

  // display oled
  display.display();
}
