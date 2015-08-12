//////////////////////////////////////////////////
// A N I M A T E
//////////////////////////////////////////////////
void loop()
{ 
  Interaction();

  // Global periodic updates
  EVERY_N_MILLISECONDS( 50 ) {
    if (toggle_pause == 0) {
      // special case for rainbow
      // if (pattern_num == 1) gHue -= (anim_speed/50);
      if (pattern_num == 1) gHue--;
      // special case for ping length
      if (pattern_num == 2) gHue -= ((anim_speed+1)/30);
      // special case for ping center
      if (pattern_num == 3) gHue -= ((anim_speed+1)/10);
      // special case for mod wave
      if (pattern_num == 6) gHue -= ((anim_speed)/30);

      if (pattern_num == 7) gHue -= ((anim_speed)/20);
      else gHue -= ((anim_speed+1)/2);
    }
  }
  EVERY_N_MILLISECONDS(20000) {
    // Random pattern
    if (toggle_autoCycle == 1) {
      pattern_num = random8(num_patterns);
      Preset(presetNum);
    }
    // Increment pattern
    // if (toggle_autoCycle == 2) {
      // pattern_num++;
      // skip secret message
      // if (pattern_num > num_patterns-1 || pattern_num == 5) pattern_num = 0;
      // ResetVars();
    // }
  }

  // Add entropy to random number generator
  random16_add_entropy( random());
    
  // Confetti
  if (pattern_num == 0) {
    drawConfetti(strip_start[0], false);
    UpdateLEDS();
  }

  // Rainbow
  else if (pattern_num == 1){
    drawRainbow(strip_start[0]);
    UpdateLEDS();
  }

  // Ping length
  else if (pattern_num == 2) {
    drawAgentLength();
  }
  
  // Ping center
  else if (pattern_num == 3) {
    drawAgentCenter();
  }

  // Noise
  else if (pattern_num == 4) {
    drawNoise();
  }

  else if (pattern_num == 5){
    drawSecretMessage(false);
    UpdateLEDS();
  }
  
  else if (pattern_num == 6){
    drawModWave(0);
    fadeToBlackBy(leds, num_leds, 60);
  }

}

