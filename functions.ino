//////////////////////////////////////////////////
// F U N C T I O N S
//////////////////////////////////////////////////

void UpdateLEDS(){
  FastLED.show();
  FastLED.delay(1000/FRAMES_PER_SECOND); 
}

void ResetVars(){
  p1 = 0;
  p1_b = 0;
}

///////////////////////////////////
// EASE IN
///////////////////////////////////
float EaseIn(float _value, float _target, float _speed){
  float dx = _target - _value;
  dx *= _speed;
  return dx;
}

// Rainbow
//
void drawRainbow(int _start_pixel, int _pixel_count) { 
  
    for (int i = 0; i < _pixel_count; i++){
      uint8_t sat = (i+1) * (255/_pixel_count);
      int wave = cubicwave8((i*2)+gHue);
      if (wave < 1) wave = 1;
      leds[i + _start_pixel] = CHSV(sat, wave, wave);
    }
//    FastLED.setBrightness(30);
}

// Confetti
//
void drawConfetti(int _start_pixel, int _pixel_count) {
 
    fadeToBlackBy( leds + _start_pixel, _pixel_count, 1);

    EVERY_N_MILLISECONDS( 100 ) {
      for (int i = 0; i < 10; i++){
        int pos = random16(_pixel_count);
        leds[pos + _start_pixel] += CHSV( gHue -32 + random8(64), 200, random8(200));
      }
    }
}


// PingFromCenter
//
void drawPingFromCenter(int _start_pixel, int _pixel_count, int strip_num){
    
//    fadeToBlackBy( leds + _start_pixel, _pixel_count, 30);
    int center_pixel = _start_pixel + (_pixel_count/2);
    int end_pixel = (_start_pixel + 48);
    
//    for (int i = 0; i <= _pixel_count/2; i++){  
//      int wave1 = cubicwave8((i*5)+(gHue*5)); // (width of wave) + (oscillation speed)
//      
//      leds[center_pixel + i] = CHSV(center_pixel+(i*2), 200, wave1);
//      leds[center_pixel - i] = CHSV(center_pixel-(i*2), 200, wave1);
//    }
    
//    for (int i = 0; i <= _pixel_count; i++){
//      uint8_t s = cubicwave8(abs(gHue));
//      int wave1 = cubicwave8((i*5)+abs(s*2)); // (width of wave) + (oscillation speed)
//      int pos = (wave1/256) * 48;
//      leds[i] = CHSV(255,255,wave1);
//    }

    // THIS ONE WORKS
//    int d = 20;
//    for (int i = 0; i <= end_pixel/2; i++){
//      d += EaseIn(d, 600, 0.005);
//      leds[center_pixel + i] = CRGB(255,255,255);
//      leds[center_pixel - i] = CRGB(255,255,255);
//      FastLED.show();
//      FastLED.delay(d);
//      fadeToBlackBy( leds + _start_pixel, _pixel_count, 60);    
//    }

    // A FUNKY WEIRD PATTERN
//    for (int i = 0; i < end_pixel; i++){
//      if (led_ctrl[i]%abs(gHue%48) == 0) leds[i] = CRGB(255,255,255);
//    }
//    UpdateLEDS();
//    fadeToBlackBy( leds + _start_pixel, _pixel_count, 60);

    p1 += EaseIn(p1, 48, 0.05);
    if (p1 >= 47) p1 = 0;
    int pos = int(p1);
    int pos_b = int(p1_b);
    leds[pos] = CRGB(255,255,255);
    if (pos > pos_b) {      // light up skipped pixels
      for (int i = 0; i < pos - pos_b; i++) {
        if (pos-i > 0) leds[pos-i] = CRGB(255,255,255);
      }
    }
    p1_b = p1;
    UpdateLEDS();
    fadeToBlackBy( leds + _start_pixel, _pixel_count, 60);
  

//    int led_start = center_pixel;
//    int led_end   = _pixel_count;
//    int led_range = led_end - led_start;
//    for(int i = 0; i < led_range; i++) {  // move thru range
//        fract8 f = (256 * i) / led_range;  // convert range to "fract8" percentage
//        fract8 eased = ease8InOutCubic( f ) ; // get eased version
//        int pos = led_start + (eased * led_range)/256;
//        int pos2 = led_start - (eased * led_range)/256;
//        leds[ pos ] = CRGB(255,0,0);
//        leds[ pos2 ] = CRGB(255,0,0);
//        UpdateLEDS();
//        fadeToBlackBy( leds + _start_pixel, _pixel_count, 30);
//    }

//    FastLED.setBrightness(50);
}
