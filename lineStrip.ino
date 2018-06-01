#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6
#define PIXELCOUNT 182
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXELCOUNT, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

bool Pixels[PIXELCOUNT], nextPixels[PIXELCOUNT];

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(255);
  for (uint8_t i = 0; i < (PIXELCOUNT); i++) {
    Pixels[i] = false;
    nextPixels[i] = false;
  }
}

void loop() {
  //rainbowCycle(1);
  //ColorTest();
  //ZigZag();
  if (Listen()) {
    newFlare();
  }
  Shift();
  ShowFlare();
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < PIXELCOUNT; i++) {
      strip.setPixelColor(i, Wheel((((i * 256 / PIXELCOUNT) + j) & 255)));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

//Static Color Test, fill strip with rainbow and color reversal at the middle of the strip
void ColorTest() {
  for (uint16_t i = 0;                        i < (PIXELCOUNT / 2); i++) {
    strip.setPixelColor(i, Wheel((i * 256 / PIXELCOUNT) & 255));
  }

  for (uint16_t i = PIXELCOUNT; i > (PIXELCOUNT / 2); i--) {
    strip.setPixelColor(i, Wheel((i * 256 / PIXELCOUNT) & 255));
  }
  strip.show();
}

void ZigZag() {
  bool zig = false;
  for (uint8_t i = 0; i < PIXELCOUNT; i++) {
    if (zig) {
      strip.setPixelColor(i, 255, 0, 0);
      zig = false;
    }
    else {
      strip.setPixelColor(i, 0, 0, 255);
      zig = true;
    }
  }
  strip.show();
}

bool Listen() {
  String received = "";
  if (Serial.available() > 0) { //get input from serial
    //delay(25);
    received = Serial.readString();
  }
  if (received == "1") {
    return true;
  }
  else {
    return false;
  }
}

void newFlare() {
  if (Listen()) {
    Pixels[0] = true;
  }
}

void Shift() {
  for (int8_t i = 0; i < PIXELCOUNT; i++) {
    if (Pixels[i]) {
      Pixels[i] = false;
      if (i + 1 < PIXELCOUNT) {
        nextPixels[i + 1] = true;
      }
    }
  }
  for (int8_t i = 0; i < PIXELCOUNT; i++) {
    Pixels[i] = nextPixels[i];
    nextPixels[i] = false;
  }
}

void ShowFlare() {
  for (uint8_t i = 0; i < PIXELCOUNT; i++) {
    if (Pixels[i]) {
      strip.setPixelColor(i, 255, 255, 255);
    }
  }
}
