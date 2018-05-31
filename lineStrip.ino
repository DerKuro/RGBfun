#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(182, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

uint8_t Pixels[(strip.numPixels())], nextPixels(strip.numPixels());

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(255);
  
}

void loop() {
  //rainbowCycle(1);
  //ColorTest();
  //ZigZag();
  Flare(20);  
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((((i * 256 / strip.numPixels()) + j) & 255)));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

//Static Color Test, fill strip with rainbow and color reversal at the middle of the strip
void ColorTest() {
  for(uint16_t i=0;                        i< (strip.numPixels()/2); i++) {
    strip.setPixelColor(i, Wheel((i * 256 / strip.numPixels()) & 255));
  }
  
  for(uint16_t i=strip.numPixels(); i>(strip.numPixels()/2); i--) {
    strip.setPixelColor(i, Wheel((i * 256 / strip.numPixels()) & 255));
  }
  strip.show();  
}

void ZigZag(){
  bool zig=false;
  for(uint8_t i=0; i<strip.numPixels();i++){
    if (zig){
    strip.setPixelColor(i,255,0,0);
    zig=false;
    }
    else{
    strip.setPixelColor(i,0,0,255);
    zig=true;
    }
  }
  strip.show();
}

bool Listen(){
  String received = "";
  if (Serial.available() > 0){ //get input from serial
    //delay(25);
  received = Serial.readString();
  }
  if (received =="1"){
    return true;
  }
  else{
    return false;
  }
}

void Flare(uint8_t wait){
  if (Listen()){
    for(int16_t i,j,k,l,m,n,o,p,q,r=0; i<strip.numPixels()+5;i++){
      strip.setPixelColor(i,255,255,255);
      strip.setPixelColor(i-1,200,200,200);
      strip.setPixelColor(i-2,150,150,150);
      strip.setPixelColor(i-3,150,150,150);
      strip.setPixelColor(i-4,150,150,150);
      strip.setPixelColor(i-5,0,0,0);
      strip.show();
      if(i<3)
        delay(100);
      if(i>3 && i<8)
        delay(50);
      if(i>8 && i<100)
        delay(0);
      if(i>130)
        delay(8);
    }
  }  
}
