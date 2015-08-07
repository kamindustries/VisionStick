//////////////////////////////////////////////////
// P A T T E R N S
//////////////////////////////////////////////////

//////////////////////////////////////////////////
// Flash
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
// 0. Confetti
//////////////////////////////////////////////////
void drawConfetti(int _start_pixel) {
 
    fadeToBlackBy(leds, num_leds, 1);

    EVERY_N_MILLISECONDS( 100 ) {
      int num_confetti = 2 + (interval_width/10);
      for (int i = 0; i < num_confetti; i++){
        random16_add_entropy(random());
        int pos = random16(num_leds);
        int hue = gHue + random8(64);
        float sat = random8(128) + (gSat/2);

        leds[pos + _start_pixel] += CHSV(hue, sat, random8(225)+30);
      }
    }
    FastLED.setBrightness(gLum);
}

//////////////////////////////////////////////////
// 1. Rainbow
//////////////////////////////////////////////////
void drawRainbow(int _start_pixel) {
    for (int i = 0; i < num_leds; i++){
      int pos = i + _start_pixel;
      float width = map(float(interval_width), 0, 100, .01, 20);

      int hue = map(i, 0, num_leds, 0, 255);
      hue = (hue * width) + gHue;
      uint8_t sat = (i+1) * (255/num_leds);

      // reverse for strip 2
      if (pos >= strip_start[1] && pos < strip_start[2]) { 
        pos = map(pos, strip_start[1], strip_start[2], strip_start[2], strip_start[1]);
        pos--;
      }
      leds[pos] = CHSV(hue, gSat, 255);
    }
    FastLED.setBrightness(gLum);
}

//////////////////////////////////////////////////
// 2. PingLength
//////////////////////////////////////////////////
void drawPingLength(int _start_pixel, int _ID, int _length){

    int end_pixel = _start_pixel + num_leds_strip;
    float s = (float)2/(104-anim_speed);
    int length_mapped = map(interval_width, 1, 100, 1, 55);

    t[_ID] += EaseIn(t[_ID], _length, s); // easing in +1 or 2 extra looks nicer imo
    
    // reset numbers if it reached the end
    if (t[_ID] >= num_leds_strip || t[_ID] > _length-1){
      if (interval_width < 5) ResetVars();
      else t[_ID] = 0;
      strip_ctrl[_ID] = 55 - random8(length_mapped); // set next length
    }

    int pos = _start_pixel + int(t[_ID]);
    int pos_b = _start_pixel + int(d_t[_ID]);
    int dif = pos - pos_b;
    
    // reverse for strip 2
    if (pos >= strip_start[1] && pos < strip_start[2]) {
      pos = map(pos, strip_start[1], strip_start[2], strip_start[2], strip_start[1]);
      pos_b = map(pos_b, strip_start[1], strip_start[2], strip_start[2], strip_start[1]);
      pos--;
      pos_b--;
    }

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
    // fadeToBlackBy( leds + _start_pixel, num_leds_strip, 30);
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
    if (t[_ID] >= 48) t[_ID] = 0;
    
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

  // fill the led array 2/16-bit noise values
  fill_2dnoise16(LEDS.leds(), num_leds_strip, 3, kMatrixSerpentineLayout,
                noise_octaves,noise_x,xscale,noise_y,yscale,noise_v_time,
                noise_hue_octaves,noise_hxy,noise_hue_scale_x,noise_hxy,
                noise_hue_scale_y,noise_hue_time, true);

  // FastLED.show();
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
// 5. Fire
//////////////////////////////////////////////////
// void drawFire(int _start_pixel)
// {
//   random16_add_entropy( random());
//   // Array of temperature readings at each simulation cell
//   static byte heat[num_leds_strip];
//   int center_pixel = _start_pixel + (num_leds_strip/2);

//   // Step 1.  Cool down every cell a little
//   for( int i = 0; i < num_leds_strip; i++) {
//     heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / num_leds_strip/2) + 2));
//   }

//   // Step 2.  Heat from each cell drifts 'up' and diffuses a little
//   // for( int k= num_leds_strip/2 - 1; k >= 2; k--) {
//   for( int k = (num_leds_strip - 1)/2; k >= 2; k--) {
//     heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
//   }
//   for( int k = center_pixel; k <= num_leds_strip-1; k++) {
//     heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
//   }
//   // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
//   if( random8() < SPARKING ) {
//     int y = random8(4);
//     heat[y] = qadd8( heat[y], random8(160,255) );
//     y += num_leds_strip - 4;
//     heat[y] = qadd8( heat[y], random8(160,255) );
//   }

//   // Step 4.  Map from heat cells to LED colors
//   for( int j = 0; j < num_leds_strip; j++) {
//     // Scale the heat value from 0-255 down to 0-240
//     // for best results with color palettes.
//     byte colorindex = scale8( heat[j], 240);
//     leds[j] = ColorFromPalette( gPal, colorindex);
//   }
// }
//////////////////////////////////////////////////
// Morse code
//////////////////////////////////////////////////
void drawSecretMessage(){
  
}

//////////////////////////////////////////////////
// Sparks
//////////////////////////////////////////////////
void drawSparks(){
  
}

//////////////////////////////////////////////////
// Rain
//////////////////////////////////////////////////
void drawRain(){
  
}

