//////////////////////////////////////////////////
// P A T T E R N S
//////////////////////////////////////////////////

//////////////////////////////////////////////////
// Flash
//////////////////////////////////////////////////

void drawFlash(CRGB Cd){
  for (int i = 0; i < num_leds; i++){
    leds[i] = CRGB(0,0,0);
  }
  FastLED.show();
  FastLED.delay(150);
  for (int i = 0; i < num_leds; i++){
    leds[i] = Cd;
  }
  FastLED.show();
  FastLED.delay(150);
}

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
// 0. Confetti
//////////////////////////////////////////////////
void drawConfetti(int _start_pixel) {
 
    fadeToBlackBy(leds, num_leds, 2);

    EVERY_N_MILLISECONDS( 100 ) {
      int num_confetti = 2 + (interval_width/5);
      for (int i = 0; i < num_confetti; i++){
        random16_add_entropy(random());
        int pos = random16(num_leds);
        int hue = gHue + random8(64);
        float sat = random8(128) + (gSat/2);

        leds[pos + _start_pixel] += CHSV(hue, sat, random8(225)+30);
      }
    }
    FastLED.setBrightness(gLum);

    // bang a random secret message
    EVERY_N_MILLISECONDS(rand_message_interval){
      drawSecretMessage(true);
      UpdateLEDS();
      FastLED.delay(60);
      // fade out
      for (int i = 0; i < 20; i++){
        fadeToBlackBy(leds, num_leds, 10);
        UpdateLEDS();
      }
      drawSecretMessage(true);
      UpdateLEDS();
      FastLED.delay(600);
      rand_message_interval = random16(30000, 120000);
    }
}

//////////////////////////////////////////////////
// 1. Rainbow
//////////////////////////////////////////////////
void drawRainbow(int _start_pixel) {
  float wave = sin((float)gHue/255);
  wave *= 5;
  wave *= (float)(anim_speed/100);

    for (int i = 0; i < num_leds; i++){
      int pos = i + _start_pixel;
      float width = map(float(interval_width), 0, 100, .01, 6.);

      int hue = map(i, 0, num_leds, 0, 255);
      hue = (hue * (width+wave)) + gHue;
      uint8_t sat_wave = cubicwave8(i + (gHue/width));
      uint8_t sat = gSat - (cubicwave8(((i) * (width+sat_wave)) + gHue)/3);

      pos = FlipPosition(pos);
      leds[pos] = CHSV(hue, sat, 255);
    }
    FastLED.setBrightness(gLum);
}

//////////////////////////////////////////////////
// 2. PingLength
//////////////////////////////////////////////////
void drawPingLength(int _start_pixel, int _ID, int _length){

    float s = (float)2/(104-anim_speed);
    if (s > .08) s = .08;  // have to clamp the value...why?
    t[_ID] += EaseIn(t[_ID], _length, s); // easing in +1 or 2 extra looks nicer imo
    
    int pos = _start_pixel + int(t[_ID]);
    int pos_b = _start_pixel + int(d_t[_ID]);
    int dif = pos - pos_b;

    // reverse for strip 2
    pos = FlipPosition(pos);
    pos_b = FlipPosition(pos_b);
    if (_ID == 1) dif = pos_b - pos;

    // set leading pixel
    leds[pos] = CHSV(gHue,gSat/2,255);
    
    random16_add_entropy(random());
    int rand_hue1 = gHue + random8(10);

    if (dif > 0) {      // light up skipped pixels
      for (int i = 0; i < dif; i++) {
        int rx = random8(20);
        if (_ID==1) leds[pos+i] = CHSV(gHue+rx,gSat/1.5,255);
        else if (pos-i > 0) leds[pos-i] = CHSV(gHue+rx,gSat/1.5,255);
      }
    }
    leds[pos_b-1] = CHSV(rand_hue1, gSat/2, 200);

    d_t[_ID] = t[_ID];

    // reset numbers if it reached the end 
    // sync them up when interval width is less than 5
    int length_mapped = map(interval_width, 1, 100, 1, 55);
    if (t[_ID] >= num_leds_strip || t[_ID] > _length-1){
      if (interval_width <= 5){
        for (int i = 0; i < 3; i++){
          t[i] = 0;
          strip_ctrl[i] = 55;
        }
      }
      else {
        t[_ID] = 0;
        d_t[_ID] = 0;
        strip_ctrl[_ID] = 55 - random8(length_mapped); // set next length
      }
    }

}

//////////////////////////////////////////////////
// 3. PingCenter
//////////////////////////////////////////////////
void drawPingCenter(int _start_pixel, int _ID){
  
    int center_pixel = _start_pixel + (num_leds_strip/2);
    int a = 104-anim_speed;
    float ping_spd = (float)2/a;

    // change this so it only goes halfway...please...
    t[_ID] += EaseIn(t[_ID], 51, ping_spd); // easing in +1 or 2 extra looks nicer imo

    // reset numbers if it reached the end if BLAST MODE is not enabled
    if (toggle_blastMode == 0){
      if (t[_ID] >= 48) t[_ID] = 0;
    }

    // get position and previous position ints
    int pos = center_pixel + ((int)t[_ID]/2);
    int pos_b = center_pixel + ((int)d_t[_ID]/2);
    
    int pos_neg = center_pixel - ((int)t[_ID]/2)-1;
    int pos_neg_b = center_pixel - ((int)d_t[_ID]/2)-1;

    // Set the leading edge of the pulse
    leds[pos] = CHSV(gHue,gSat/2,255);
    leds[pos_neg] = CHSV(gHue,gSat/2,255);
    
    // light up skipped pixels
    if (pos > pos_b) {      
      for (int i = 1; i < pos - pos_b; i++) {
        int rx = random8(20);
        if (pos-i > 0) {
          leds[pos-i] = CHSV(gHue+rx,200,255);
          leds[pos_neg+i] = CHSV(gHue+rx,200,255);
        }
      }
    }

    // light up caboose pixel for color trail
    random16_add_entropy(random());
    int rand_hue1 = gHue + random8(10);
    int hue2_o = (random8(5) - 5) * 2 ;
    int rand_hue2 = rand_hue1 + hue2_o;
    leds[pos_b-1] = CHSV(rand_hue1, gSat/2, 200);
    leds[pos_neg_b+1] = CHSV(rand_hue2, gSat/2, 200);    

    // set previous timestep to current timestep
    d_t[_ID] = t[_ID];

    fadeToBlackBy( leds + _start_pixel, num_leds_strip, 30);

}

//////////////////////////////////////////////////
// 4. Noise
//////////////////////////////////////////////////
void drawNoise(){

  float freq = map(float(interval_width), 1, 100, .01, 20);
  int xscale = noise_xscale * freq;
  int yscale = noise_xscale * freq;

  // how varied the colors are, sync these up with anim speed and interval
  // get presets to play nice
  int h_scale = gSat;
  int h_offset = gHue * 4;

  // fill the led array 2/16-bit noise values
  // leds, width, height, serpentine, 
  // octaves, x, xscale, y, yscale
  // time, hue_octaves, hue_x, hue_xscale, hue_y, hue_yscale,
  // hue_time, blend, hue_shift
  fill_2dnoise16(LEDS.leds(), num_leds_strip, 3, kMatrixSerpentineLayout,
                noise_octaves, noise_x, xscale, noise_y, yscale, 
                noise_v_time, noise_hue_octaves, noise_hxy, h_scale, noise_hxy, h_scale, 
                noise_hue_time, true, h_offset);

  FastLED.setBrightness(gLum);
  UpdateLEDS();

  // adjust the intra-frame time values
  float time_mult = map(float(anim_speed), 1, 100, .001, 2);
  float hue_mult = map(float(gSat), 1, 255, .1, 2);
  noise_x += noise_x_speed;
  noise_y += noise_y_speed;
  noise_v_time += noise_time_speed * time_mult;
  noise_hue_time += (noise_hue_speed );
}

//////////////////////////////////////////////////
// Morse code
//////////////////////////////////////////////////
void drawSecretMessage(bool _bang){
  fadeToBlackBy(leds, num_leds, 5);

  int interval;
  if (_bang) interval = 0;
  else interval = random8(50,100);

  // if being banged, clear pixels first
  if (_bang){
    for (int i = 0; i < num_leds; i++){
      leds[i] = CRGB(0,0,0);
    }
    UpdateLEDS();
  }

  // bright tips
  for (int i = 0; i < 3; i++){
    for (int j = 0; j < 10; j++){
      int pos = j + strip_start[i];
      int pos_b = strip_start[i] + num_leds_strip - j;
      pos = FlipPosition(pos);
      pos_b = FlipPosition(pos_b);

      int rnd = random8(gLum/4);
      int val = gLum-((gLum/18)*j);
      val -= rnd;

      leds[pos] = CHSV(0,0,val);
      leds[pos_b] = CHSV(0,0,val);
    }
  }

  // message
  EVERY_N_MILLISECONDS(interval){
    for (int j = 0; j < 3; j++){
      for (int i = 0; i < 27; i++){
        if (secret_message[i] == 1){
          int pos = i + strip_start[j] + 11;
          if (pos >= strip_start[1] && pos < strip_start[2]) {
            pos = map(pos, strip_start[1], strip_start[2], strip_start[2], strip_start[1]);
            pos--;
          }
          int value = random8((gLum/2)+20, gLum+30);
          int hue_o = random8(8);
          hue_o = (hue_o - 6) * hue_o;
          hue_o = (i*5)+gHue+hue_o;
          int sat_low = gSat - 75;
          int sat_high = gSat + 10;
          if (sat_low < 1) sat_low = 1;
          if (sat_high > 255) sat_high = 255;
          leds[pos] = CHSV(hue_o, random8(sat_low,sat_high), value);
        }
      }
    }
  }

}

// A FUNKY WEIRD PATTERN
void drawModWave(int _ID){
  // for (int i = 0; i <= num_leds; i++){
  //   uint8_t s = cubicwave8(abs(gHue));
  //   int wave1 = cubicwave8(pow(i*2,2)+(s*10)); // (width of wave) + (oscillation speed)
  //   int p = FlipPosition(i);
  //   leds[p] = CHSV(p,255,wave1);
  // }
  for (int i = 0; i < num_leds; i++){
    int pos = i;
    pos += random8(3);
    // pos = FlipPosition(pos);
    pos = pos % 49;
    uint8_t hue_wave = cubicwave8((pos*2) + gHue);
    uint8_t sat_wave = cubicwave8((pos*2) + (gHue/2));
    sat_wave = sat_wave - (255-gSat);
    if (pos%abs(gHue%49) == 0) leds[i] = CHSV(hue_wave,sat_wave,255);
  }
  FastLED.setBrightness(gLum);
  UpdateLEDS();
}

//////////////////////////////////////////////////
// 2. Agent Ping Length
//////////////////////////////////////////////////
void drawAgentLength(int id){

  for (int i = 0; i < 2; i++){
    ping[i].animate(anim_speed, interval_width);
    int pos = ping[i].getPos();
    int val = ping[i].getValue();
    float vel = ping[i].getVelocity();
    val = 255-val;
    leds[pos] = CHSV(gHue,gSat/2,val);
  }

///

    // float s = (float)2/(104-anim_speed);
    // if (s > .08) s = .08;  // have to clamp the value...why?
    // t[_ID] += EaseIn(t[_ID], _length, s); // easing in +1 or 2 extra looks nicer imo
    
    // int pos = _start_pixel + int(t[_ID]);
    // int pos_b = _start_pixel + int(d_t[_ID]);
    // int dif = pos - pos_b;

    // // reverse for strip 2
    // pos = FlipPosition(pos);
    // pos_b = FlipPosition(pos_b);
    // if (_ID == 1) dif = pos_b - pos;

    // // set leading pixel
    // leds[pos] = CHSV(gHue,gSat/2,255);
    
///

    // random16_add_entropy(random());
    // int rand_hue1 = gHue + random8(10);

    // if (dif > 0) {      // light up skipped pixels
    //   for (int i = 0; i < dif; i++) {
    //     int rx = random8(20);
    //     if (_ID==1) leds[pos+i] = CHSV(gHue+rx,gSat/1.5,255);
    //     else if (pos-i > 0) leds[pos-i] = CHSV(gHue+rx,gSat/1.5,255);
    //   }
    // }
    // leds[pos_b-1] = CHSV(rand_hue1, gSat/2, 200);

    // d_t[_ID] = t[_ID];

    // // reset numbers if it reached the end 
    // // sync them up when interval width is less than 5
    // int length_mapped = map(interval_width, 1, 100, 1, 55);
    // if (t[_ID] >= num_leds_strip || t[_ID] > _length-1){
    //   if (interval_width <= 5){
    //     for (int i = 0; i < 3; i++){
    //       t[i] = 0;
    //       strip_ctrl[i] = 55;
    //     }
    //   }
    //   else {
    //     t[_ID] = 0;
    //     d_t[_ID] = 0;
    //     strip_ctrl[_ID] = 55 - random8(length_mapped); // set next length
    //   }
    // }

}

//////////////////////////////////////////////////
// Rain
//////////////////////////////////////////////////
// void drawRain(){
  
// }

