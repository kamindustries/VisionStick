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

void drawFlash(CHSV Cd){
  CRGB rgb;
  hsv2rgb_rainbow(Cd, rgb);
  drawFlash(rgb);
}

void drawFlashFade(CHSV Cd){
  for (int i = 0; i < num_leds; i++){
    leds[i] = CRGB(0,0,0);
  }
  FastLED.show();
  FastLED.delay(150);
  for (int i = 0; i < 3; i++){
    for (int j = 0; j < 48; j++){
      CHSV Cd_o = Cd;
      Cd_o.v = 255-(j*5);
      int pos = FlipPosition((i*48)+j);
      leds[pos] = Cd_o;
    }
  }
  FastLED.show();
  FastLED.delay(150);
}

// flames based off turbo thing
void drawTurboFlames(int _strength){
  int flame_extra = 0;
  if (_strength == 4) flame_extra = 8;
  int flame_length = random8(1,10 + (_strength*3) + flame_extra);
  for (int i = 0; i < flame_length; i++){
    for (int j = 0; j < 3; j++){
      int pos = FlipPosition(i+strip_start[j]);

      int flame_lum = random8(45*_strength, 63*_strength);
      int flame_hue_o = random8(_strength, _strength*3);

      leds[pos] = CHSV(random8(75-flame_hue_o,95+flame_hue_o), random8(128,220), flame_lum);
    }
  }
}

//////////////////////////////////////////////////
// 0. Confetti
//////////////////////////////////////////////////
void drawConfetti(int _start_pixel, bool _bang) {
 
    int max_fade_spd = map(interval_width, 1, 100, 2, 60);
    fadeToBlackBy(leds, num_leds, 2);
    int update_time = 100;
    if (_bang) update_time = 0;

    EVERY_N_MILLISECONDS( update_time ) {
      int num_confetti = 3 + (interval_width/15);
      for (int i = 0; i < num_confetti; i++){
        random16_add_entropy(random());
        int pos = random16(num_leds);
        int hue = gHue + random8(64);
        float sat = random8(40, gSat);

        leds[pos + _start_pixel] += CHSV(hue, sat, random8(30, 200));
      }
    }
    FastLED.setBrightness(gLum);

    // bang a random secret message
    EVERY_N_MILLISECONDS(rand_message_interval){
      drawSecretMessage(true);
      FastLED.setBrightness(255);
      UpdateLEDS();
      FastLED.delay(60);
      // fade out
      for (int i = 0; i < 20; i++){
        fadeToBlackBy(leds, num_leds, 10);
        UpdateLEDS();
      }
      drawSecretMessage(true);
      FastLED.setBrightness(255);
      UpdateLEDS();
      FastLED.delay(500);
      rand_message_interval = random16(60000, 120000);
    }
}

//////////////////////////////////////////////////
// 1. Rainbow
//////////////////////////////////////////////////
void drawRainbow(int _start_pixel) {

  float freq = float(interval_width)/100.;
  float max_spd = map(freq, .01, 1., .01, .3);
  max_spd = 1.-max_spd;
  float spd = (float(anim_speed)/1000.) * max_spd;
  float wave = sin((float(gHue)/255.) + (float(gHue)*spd));

  // broad wave controls the slow directional changes
  float broad_spd = spd;
  broad_spd *= 0.02;
  float broad_wave = sin(freq + (float(gHue)*broad_spd));

  for (int i = 0; i < num_leds; i++){
    int pos = i + _start_pixel;

    float i_f = float(i);
    float hue = (sin((i_f * freq * 0.5 * broad_wave) - (float(gHue)*spd))+1.)*128.;
    float sat = (sin(((i_f+92) * freq * 2.2 * broad_wave) - (float(gHue)*spd*0.65))+1.)*gSat/2.;
    sat += 100;
    if (sat > 255) sat = 255;

    pos = FlipPosition(pos);
    leds[pos] = CHSV(hue, sat, 255);
  }
  FastLED.setBrightness(gLum);
}

//////////////////////////////////////////////////
// 2. Agent Ping Length
//////////////////////////////////////////////////
void drawAgentLength(){

  int max_fade_spd = map(anim_speed, 1, 100, 40, 100);
  if (turbo != 0) max_fade_spd = 8;
  fadeToBlackBy(leds, num_leds, max_fade_spd);

  if (toggle_blastMode == 0){
    int max_pings = map(interval_width, 1, 100, 3, 8);
    for (int i = 0; i < max_pings; i++){
      ping[i].draw(leds, anim_speed, interval_width, gHue, gSat, -1, toggle_sync);
    }

    if (toggle_sync == 2){
      for (int i = 1; i < 3; i++){
        for (int j = 0; j < num_leds_strip; j++){
          int pos = FlipPosition((i*48)+j);
          int lookAtPt = FlipPosition(j);
          leds[pos] = leds[lookAtPt];
        }
      }
    }
  }
  
  // blast mode engage
  else {
    // turbo flames
    if (turbo > 0){
      drawTurboFlames(turbo);
    }

    if (shootBlast > 0){
      int blast_interval = interval_width + 30;
      if (blast_interval > 100) blast_interval = 100;
      for (int i = 0; i < (shootBlast*3)+2; i++){
        ping[i].draw(leds, anim_speed, blast_interval, gHue, gSat, -1, 1, true);
        if(ping[i].done == false) ping[i].checkIfDoneBlast();
      }
    }
    if (turbo == -1){
      drawConfetti(0, true);
      EVERY_N_MILLISECONDS(2000) turbo = 0;
    }

    if (shootBlast > 0){
      int check_done = 0;
      for (int i = 0; i < shootBlast*3; i++){
        if (ping[i].done == true) check_done++;
      }
      if (check_done/3 >= shootBlast) {
        shootBlast--;
        if (shootBlast <= 0) {
          shootBlast = 0;
        }
      }
    }
  }
  // end blast mode

  FastLED.setBrightness(gLum);
  UpdateLEDS();

}


//////////////////////////////////////////////////
// 3. PingCenter
//    unidirectional pings start halfway down the strip then mirrored
//////////////////////////////////////////////////
void drawAgentCenter(){
  
  int max_fade_spd = map(anim_speed, 1, 100, 40, 100);
  fadeToBlackBy(leds, num_leds, max_fade_spd);

  int max_pings = map(interval_width, 1, 100, 6, 12);
  for (int i = 0; i < max_pings; i++){
    // 23- start halfway, sync enables/disables random direction
    ping[i].draw(leds, anim_speed, interval_width, gHue, gSat, 23, 1);
  }

  // Mirroring- easy way of copying half the strip to its own other half
  // if toggle_sync==2 we overwrite all the strips with the first strip values
  for (int i = 0; i < 3; i++){
    for (int j = 0; j < num_leds_strip; j++){
      if (j < 24) {
        int pos = FlipPosition((i*48)+j);
        int lookAtPt = FlipPosition(((i+1)*48)-j);
        leds[pos] = leds[lookAtPt];
      }
      if (toggle_sync == 2 && i > 0){
        int pos = FlipPosition((i*48)+j);
        int lookAtPt = FlipPosition(48-j);
        leds[pos] = leds[lookAtPt];        
      }
    }
  }


  FastLED.setBrightness(gLum);
  UpdateLEDS();

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
  int h_scale = map(interval_width, 1, 100, 128, 255);
  int inv_offset = 12 - (anim_speed/10);
  int h_offset = gHue * inv_offset * 5;

  // fill the led array 2/16-bit noise values
  // leds, width, height, serpentine, 
  // octaves, x, xscale, y, yscale
  // time, hue_octaves, hue_x, hue_xscale, hue_y, hue_yscale,
  // hue_time, blend, hue_shift
  fill_2dnoise16(LEDS.leds(), num_leds_strip, 3, kMatrixSerpentineLayout,
                noise_octaves, noise_x, xscale, noise_y, yscale, 
                noise_v_time, noise_hue_octaves, noise_hxy, h_scale, noise_hxy, h_scale, 
                noise_hue_time, true, h_offset);

  FastLED.setBrightness(gLum+20);
  UpdateLEDS();

  // adjust the intra-frame time values
  float time_mult = map(float(anim_speed), 1, 100, .001, 5.);
  // float hue_mult = map(float(gSat), 1, 255, .1, 2);
  noise_x += noise_x_speed;
  noise_y += noise_y_speed;
  noise_v_time += noise_time_speed * time_mult;
  noise_hue_time += (noise_hue_speed * time_mult);
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
      int val = gLum-((gLum/10)*j);
      val -= rnd;
      if (val < 0) val = 0;
      if (val > 255) val = 255;

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
          int value = random8((gLum/2), gLum) + 40;
          if (value > 255) value = 255;
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
    if (pos%abs(gHue%49) == 0) leds[i] = CHSV(hue_wave,sat_wave,gLum);
  }
  FastLED.setBrightness(gLum);
  UpdateLEDS();
}



