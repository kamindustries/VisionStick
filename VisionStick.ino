#include "FastLED.h"
#include <Button.h>
#include "Agent.h"


#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN  11
#define CLK_PIN   12
#define LED_TYPE    APA102
#define COLOR_ORDER BRG
#define FRAMES_PER_SECOND  60
#define COOLING  75
#define SPARKING 120

const uint8_t num_leds = 144;
const uint8_t num_leds_strip = 48;
const uint8_t num_pings = 20;
CRGB leds[num_leds];
Agent ping[num_pings];

uint8_t gLum = 96;
uint8_t gSat = 255;
int gHue = 0; // rotating "base color" used by both patterns
// uint8_t strip_ctrl[3];
uint8_t strip_start[3];

uint8_t anim_speed = 10;
uint8_t interval_width = 10;
uint8_t rainbow_anim = 5;
uint8_t toggle_pause = 0;
uint8_t toggle_autoCycle = 0;
uint8_t toggle_blastMode = 0;
uint8_t toggle_sync = 0;
uint8_t presetNum = 0;
int shootBlast = 0;
int turbo = 0;

int pattern_num = 0;
uint8_t num_patterns = 8;
uint8_t secret_message[27];
int rand_message_interval = 30000;

float t[3]; //current timestep
float d_t[3]; //previous timestep

// Param for different pixel layouts
#define kMatrixSerpentineLayout  true

// x,y, & time values
uint32_t noise_x,noise_y,noise_v_time,noise_hue_time,noise_hxy;

// Play with the values of the variables below and see what kinds of effects they
// have!  More octaves will make things slower.
// how many octaves to use for the brightness and hue functions
uint8_t noise_octaves=2;
uint8_t noise_hue_octaves=2;

// the 'distance' between points on the x and y axis
int noise_yscale=57771;
int noise_xscale=57771;

// the 'distance' between x/y points for the hue noise
int noise_hue_scale_x=10;
int noise_hue_scale_y=10;

// how fast we move through time & hue noise
int noise_time_speed=1111;
int noise_hue_speed=31;

// adjust these values to move along the x or y axis between frames
int noise_x_speed=311;
int noise_y_speed=1111;

// Buttons
Button button1 = Button(10, BUTTON_PULLUP_INTERNAL, true, 80);
Button button2 = Button(9, BUTTON_PULLUP_INTERNAL, true, 80);
Button button3 = Button(8, BUTTON_PULLUP_INTERNAL, true, 80);
Button button4 = Button(7, BUTTON_PULLUP_INTERNAL, true, 80);
//Button button5 = Button(6, BUTTON_PULLUP_INTERNAL, true, 80);
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

  // initialize values
  for (int i = 0; i < 3; i++){
    // strip_ctrl[i] = random8(55);
    strip_start[i] = num_leds_strip*i;
    t[i] = 0;
    d_t[i] = 0;
  }

  for (int i = 0; i < num_pings; i++){
    ping[i].ID = i;
  }


  // NOISE STUFF
  // initialize the x/y and time values
  random16_set_seed(8934);
  noise_hxy = (uint32_t)((uint32_t)random16() << 16) + (uint32_t)random16();
  noise_x = (uint32_t)((uint32_t)random16() << 16) + (uint32_t)random16();
  noise_y = (uint32_t)((uint32_t)random16() << 16) + (uint32_t)random16();
  noise_v_time = (uint32_t)((uint32_t)random16() << 16) + (uint32_t)random16();
  noise_hue_time = (uint32_t)((uint32_t)random16() << 16) + (uint32_t)random16();

  // SECRET MESSAGE
  for (int i = 0; i < 27; i++){
    if (i==0 || i==2 || i==3 || i==4 || i==6 || i==8 || i==12 || i==14 || i==16 || 
        i==20 || i==21 || i==22 || i==24 || i==26) {
      secret_message[i] = 1;
    }
    else secret_message[i] = 0;
  }

}

