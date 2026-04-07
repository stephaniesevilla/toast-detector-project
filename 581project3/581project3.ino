#include <FastLED.h>
#include <LiquidCrystal.h>

#define LED_PIN 6
#define NUM_LEDS 60
#define FSR_PIN A0
#define FSR_THRESHOLD 100

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
CRGB leds[NUM_LEDS];

void setAllLeds(int r, int g, int b) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(r, g, b);
  }
  FastLED.show();
}

void setup() {
  lcd.begin(16, 2);
  lcd.print("Ready to Toast!");
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(80);
  FastLED.clear();
  FastLED.show();
  Serial.begin(9600);
}

void loop() {
  int fsrValue = analogRead(FSR_PIN);
  Serial.println(fsrValue);

  if (fsrValue > FSR_THRESHOLD) {
    // Bright gold flash for CHEERS
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Gold;
    }
    FastLED.show();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("CHEERS!");
    lcd.setCursor(0, 1);
    lcd.print("Cheers everyone!");
    delay(3000);
    FastLED.clear();
    FastLED.show();
    lcd.clear();
    lcd.print("Ready to Toast!");
  } else {
    // Soft warm white-yellow gentle pulse in idle
    for (int b = 20; b <= 60; b++) {
      setAllLeds(b, b * 0.85, b * 0.4);
      delay(20);
      if (analogRead(FSR_PIN) > FSR_THRESHOLD) return;
    }
    for (int b = 60; b >= 20; b--) {
      setAllLeds(b, b * 0.85, b * 0.4);
      delay(20);
      if (analogRead(FSR_PIN) > FSR_THRESHOLD) return;
    }
  }
}