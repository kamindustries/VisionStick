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
        leds[pos + _start_pixel] += CHSV( gHue -32 + random8(64), 200, random8(200));
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
      if (wave < 1) wave = 5;
      leds[i + _start_pixel] = CHSV(wave, (gSat+wave)/2, wave);
    }

    FastLED.setBrightness(gLum);

}

//////////////////////////////////////////////////
// PingLength
//////////////////////////////////////////////////
void drawPingLength(int _start_pixel, int _pixel_count, int strip_num){
  
    int center_pixel = _start_pixel + (_pixel_count/2);
    int end_pixel = (_start_pixel + 48);

    int a = 104-anim_speed;
    float s = (float)2/a;
     
  
    t[0] += EaseIn(t[0], 48, s);
    if (t[0] >= 47) t[0] = 0;
    int pos = int(t[0]);
    int pos_b = int(d_t[0]);
    leds[pos] = CRGB(255,255,255);
    if (pos > pos_b) {      // light up skipped pixels
      for (int i = 0; i < pos - pos_b; i++) {
        if (pos-i > 0) leds[pos-i] = CRGB(255,255,255);
      }
    }
    
    d_t[0] = t[0];
    UpdateLEDS();
    fadeToBlackBy( leds + _start_pixel, _pixel_count, 60);
    FastLED.setBrightness(gLum);
}
