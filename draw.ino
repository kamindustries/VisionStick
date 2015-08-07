//////////////////////////////////////////////////
// A N I M A T E
//////////////////////////////////////////////////
void loop()
{ 
  Interaction();

  // Global periodic updates
  EVERY_N_MILLISECONDS( 50 ) {
    if (toggle_pause == 0) {
      // special case for ping length
      if (pattern_num == 2) gHue -= ((anim_speed+1)/30);
      // special case for ping center
      if (pattern_num == 3) gHue -= ((anim_speed+1)/10);
      else gHue -= ((anim_speed+1)/2);
    }
  }

  // Add entropy to random number generator
  random16_add_entropy( random());
    
  // Confetti
  if (pattern_num == 0) {
    drawConfetti(strip_start[0]);
    UpdateLEDS();
  }

  // Rainbow
  else if (pattern_num == 1){
    drawRainbow(strip_start[0]);
    UpdateLEDS();
  }

  // Ping length
  else if (pattern_num == 2) {
    fadeToBlackBy(leds, num_leds, 30);
    drawPingLength(strip_start[0], 0, strip_ctrl[0]);
    drawPingLength(strip_start[1], 1, strip_ctrl[1]);
    drawPingLength(strip_start[2], 2, strip_ctrl[2]);
    FastLED.setBrightness(gLum);
    UpdateLEDS();
  }
  
  // Ping center
  else if (pattern_num == 3) {
    drawPingCenter(strip_start[0], 0);
    drawPingCenter(strip_start[1], 1);
    drawPingCenter(strip_start[2], 2);
    FastLED.setBrightness(gLum);    
    UpdateLEDS();
  }

  // Noise
  else if (pattern_num == 4) {
    drawNoise();
  }


}

