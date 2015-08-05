#include "FastLED.h"
#include <Button.h>


#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN  11
#define CLK_PIN   12
#define LED_TYPE    APA102
#define COLOR_ORDER BRG
#define FRAMES_PER_SECOND  60

const int num_leds = 144;
const int num_leds_strip = 48;
CRGB leds[num_leds];

int led_ctrl[num_leds];

int gLum = 96;
int gSat = 255;
uint8_t gHue = 0; // rotating "base color" used by both patterns
int anim_speed = 10;
int interval_width = 100;
int rainbow_anim = 5;
int toggle_pause = 0;
int toggle_cycle = 0;
int toggle_chroma = 0;
int toggle_sync = 0;
int toggle_interval = 0;

int pattern_num = 0;
int num_patterns = 4;
float t[3]; //current timestep
float d_t[3]; //previous timestep

// Buttons
Button button1 = Button(10, BUTTON_PULLUP_INTERNAL, true, 80);
Button button2 = Button(9, BUTTON_PULLUP_INTERNAL, true, 80);
Button button3 = Button(8, BUTTON_PULLUP_INTERNAL, true, 80);
Button button4 = Button(7, BUTTON_PULLUP_INTERNAL, true, 80);
Button button5 = Button(6, BUTTON_PULLUP_INTERNAL, true, 80);
Button button6 = Button(5, BUTTON_PULLUP_INTERNAL, true, 80);
Button button7 = Button(4, BUTTON_PULLUP_INTERNAL, true, 80);
Button button8 = Button(3, BUTTON_PULLUP_INTERNAL, true, 80);

//////////////////////////////////////////////////
// S E T U P
//////////////////////////////////////////////////
void setup() {
  delay(3000); // 3 second delay for recovery

//  Serial.begin(9600);
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, num_leds).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(gLum);

  for (int i = 0; i < num_leds; i++){
    led_ctrl[i] = i;
  }
}

//////////////////////////////////////////////////
// A N I M A T E
//////////////////////////////////////////////////
void loop()
{
  Interaction();
  
  int left_start = 0;
  int mid_start = num_leds/3;
  int right_start = mid_start*2;

  if (pattern_num == 0) {
    drawConfetti(left_start, num_leds);
    UpdateLEDS();
  }
  else if (pattern_num == 1){
    drawRainbow(left_start, num_leds);
    UpdateLEDS();
  }
  else if (pattern_num == 2) {
    drawPingLength(left_start, num_leds_strip, 0);
//    drawPingFromCenter(mid_start, num_leds_strip, 1);
//    drawPingFromCenter(right_start, num_leds_strip, 2);
//    UpdateLEDS();
//    fadeToBlackBy( leds, num_leds, 30);  
  }
  else if (pattern_num == 3) {
    drawPingCenter(left_start, num_leds_strip, 0);
  }

  EVERY_N_MILLISECONDS( 50 ) {
    if (toggle_pause == 0) gHue -= ((anim_speed+1)/2);
  }

}


