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

bool Pixels[PIXELCOUNT+1], nextPixels[PIXELCOUNT+1];
String received = "";
uint8_t counter = 0;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(0);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(255);
  for (uint8_t i = 0; i < (PIXELCOUNT); i++) {
    Pixels[i] = false;
    nextPixels[i] = false;
  }
}

void loop() {
  if (Serial.available() > 0) { //get input from serial
    Serial.readString();
    NewFlare();
  }
  counter++;
  ShowFlare();
  Shift();
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

void NewFlare() {
  Pixels[0] = true;
}

void Shift() {
  for (uint8_t i = 0; i < PIXELCOUNT+1; i++) {

    if (Pixels[i] && (i + 1 < PIXELCOUNT+1)) {
      nextPixels[i + 1] = true;
      Pixels[i] = false;
    }
  }
  for (uint8_t i = 0; i < PIXELCOUNT+1; i++) {
    Pixels[i] = nextPixels[i];
    nextPixels[i] = false;
  }
}

void ShowFlare() {
  for (uint8_t i = 0; i < (PIXELCOUNT+1); i++) {
    if (Pixels[i]) {
      strip.setPixelColor(i, Wheel(counter));
      strip.setPixelColor((i - 1), 0, 0, 0);
    }
  }
  strip.show();
}
