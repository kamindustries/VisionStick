//////////////////////////////////////////////////
// P A T T E R N S
//////////////////////////////////////////////////

void drawFlash(){
  for (int i = 0; i < num_leds; i++){
    leds[i] = CRGB(0,0,0);
  }
  FastLED.show();
  FastLED.delay(150);
  for (int i = 0; i < num_leds; i++){
    leds[i] = CRGB(40,40,40);
  }
  FastLED.show();
  FastLED.delay(150);
}



//////////////////////////////////////////////////
// Confetti
//////////////////////////////////////////////////
void drawConfetti(int _start_pixel, int _pixel_count) {
 
    fadeToBlackBy( leds + _start_pixel, _pixel_count, 1);

    EVERY_N_MILLISECONDS( 100 ) {
      for (int i = 0; i < 10; i++){
        int pos = random16(_pixel_count);
        leds[pos + _start_pixel] += CHSV( gHue -32 + random8(64), gSat, random8(225)+30);
      }
    }

    FastLED.setBrightness(gLum);
}

//////////////////////////////////////////////////
// Rainbow
//////////////////////////////////////////////////
void drawRainbow(int _start_pixel, int _pixel_count) {
    for (int i = 0; i < _pixel_count; i++){
      uint8_t sat = (i+1) * (255/_pixel_count);
      int spd = (anim_speed-1) * 10;
      int wave = cubicwave8((i) + gHue + spd);
      if (wave < 1) wave = 1;
      leds[i + _start_pixel] = CHSV(wave, gSat, 255);
    }

    FastLED.setBrightness(gLum);

}

//////////////////////////////////////////////////
// PingLength
//////////////////////////////////////////////////
void drawPingLength(int _start_pixel, int _pixel_count, int strip_num){
  
    int center_pixel = _start_pixel + (_pixel_count/2);
    int a = 104-anim_speed;
    float s = (float)2/a;
     
    t[0] += EaseIn(t[0], 50, s); // easing in +1 or 2 extra looks nicer imo
    if (t[0] >= 48) t[0] = 0;
    int pos = int(t[0]);
    int pos_b = int(d_t[0]);
    leds[pos] = CRGB(255,255,255);
    
    if (pos > pos_b) {      // light up skipped pixels
      for (int i = 0; i < pos - pos_b; i++) {
        int rx = random8(20);
        if (pos-i > 0) leds[pos-i] = CHSV(gHue+rx,200,255);
      }
    }
    
    d_t[0] = t[0];
    UpdateLEDS();
    fadeToBlackBy( leds + _start_pixel, _pixel_count, 30);
    FastLED.setBrightness(gLum);
}

//////////////////////////////////////////////////
// PingCenter
//////////////////////////////////////////////////
void drawPingCenter(int _start_pixel, int _pixel_count, int strip_num){
  
    int center_pixel = _start_pixel + (_pixel_count/2);
    int a = 104-anim_speed;
    float s = (float)2/a;
     
    t[0] += EaseIn(t[0], 51, s); // easing in +1 or 2 extra looks nicer imo
    if (t[0] >= 48) t[0] = 0;
    
    int pos = center_pixel + ((int)t[0]/2);
    int pos_neg = center_pixel - ((int)t[0]/2)-1;
    int pos_b = center_pixel + ((int)d_t[0]/2);
    int pos_b_neg = center_pixel - ((int)d_t[0]/2)-1;
    
    leds[pos] = CRGB(255,255,255);
//    leds[pos] = CHSV(gHue/2,gSat,255);
    leds[pos_neg] = CRGB(255,255,255);

    int tail = (anim_speed/10)+1;
    tail *= (pos-pos_b);
    if (pos > pos_b-tail) {      // light up skipped pixels
      for (int i = 1; i < tail; i++) {
        int px = center_pixel + i;
        int px_neg = center_pixel -i;  
        if (pos-px > 0) {
          int d_hue = gHue + random8(tail*5);
          int d_hue_neg = gHue + random8(tail*5);
          int d_sat = ((i+1)*5)*(gHue/25);
          int d_val = 255-(i*tail*2);
          leds[pos-i] += CHSV(d_hue, d_sat, d_val);
          leds[pos_neg+i] += CHSV(d_hue_neg, d_sat, d_val);
        }
      }
    }
    fadeToBlackBy( leds + _start_pixel, _pixel_count, 30);
    FastLED.setBrightness(gLum);    
    d_t[0] = t[0];
    UpdateLEDS();

}
