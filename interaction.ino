void Interaction(){

// BUTTON 1
// Pattern up
// goes up or down when not in blast mode
// when in blast mode, button 1 fires a shot. holding button 2 advances 1 pattern 
  if(button1.uniquePress()){ 
    if ((!button2.isPressed() && toggle_blastMode == 0) || 
        (button2.isPressed() && toggle_blastMode == 1)){
      pattern_num++;
      if (pattern_num > num_patterns-1) pattern_num = 0;
    }
    // Pattern down
    else if (button2.isPressed() && toggle_blastMode == 0){
      pattern_num--;
      if (pattern_num < 0) pattern_num = num_patterns-1;
    }

    // Shoot individual pings in blast mode
    else if (!button2.isPressed() && toggle_blastMode == 1){
      shootBlast++;
      if (shootBlast > 6) shootBlast = 6;
      for (int i = 0; i < shootBlast*3; i++){
        if (ping[i].done==true) ping[i].ResetPair(interval_width, -1);
      }

      // Release the turbo!!~
      if (turbo > 0) {
        shootBlast = turbo;
        if (turbo == 4) shootBlast = 6;
        if (shootBlast > 6) shootBlast = 6;
        anim_speed = random8(30,50 + (turbo*10));
        interval_width = random8(40-(turbo*5),50 + (turbo*10));
        for (int i = 0; i < (shootBlast*3)+2; i++){
          ping[i].ResetPair(interval_width, -1);
          ping[i].done = false;
        }
        if (turbo == 4) {
          turbo = -1;
          interval_width = random8(75,90);
          anim_speed = random8(75,90);
          drawFlashFade(CHSV(random8(),random8(),255), 55, false);
        }
        else turbo = 0;
      }
    }
  }
// Toggle auto switching
  if (button1.held(1800) && button2.isPressed()){
    if (toggle_autoCycle == 0){
      toggle_autoCycle++;
      pattern_num++;
      if (pattern_num > num_patterns-1) pattern_num = 0;
      drawFlash();
    }
    else if (toggle_autoCycle == 1){
      toggle_autoCycle = 0;
      pattern_num++;
      if (pattern_num > num_patterns-1) pattern_num = 0;
      drawFlash(CRGB(255,0,0));
    }
  }
  if (button1.heldFor(2000) && toggle_blastMode == 1){
    if (turbo < 1) {
      turbo = 1;
      drawFlashFade(CHSV(85, 220, 128), 150, true);
    }
    if (button1.heldFor(4000) && toggle_blastMode == 1){
      if (turbo < 2) {
        turbo = 2;
        drawFlashFade(CHSV(85+30, 220, 200), 150, true);
      }
      if (button1.heldFor(6000) && toggle_blastMode == 1){
        if (turbo < 3){
          turbo = 3;
          drawFlashFade(CHSV(85+60, 220, 220), 150, true);
        }
        if (button1.heldFor(8000) && toggle_blastMode == 1){
          if (turbo < 4) {
            turbo = 4;
            drawFlashFade(CHSV(0, 0, 225), 150, true);
          }
        }
      }
    }
  }

// BUTTON 3: reduce brightness, saturation, cycle time, low preset
//
// Shorten auto cycle timer
  if (button3.uniquePress() && pattern_num == 99){
    // go down by 60 sec if we're already above 60 sec
    if (auto_cycle_timer >= 120) auto_cycle_timer -= 60;
    else auto_cycle_timer -= 10;
    if (auto_cycle_timer < 10) auto_cycle_timer = 10;
    // auto_cycle_timer = ClampValueLesser(auto_cycle_timer, 10);
  }
// Down in brightness or chroma
  else if(button3.isPressed() && pattern_num != 99){
    if (!button2.isPressed() && pattern_num != 99) {
      gLum = IncrementInt(gLum, false, 3, 1, 0, 255, 25, 235);
    }
    else if (button2.isPressed()) gSat = IncrementInt(gSat, false, 3, 1, 1, 255, 25, 235);
  }
// toggle low preset
  if (button3.held(3000) && button2.isPressed() && pattern_num != 99){
    Preset(0);
    gLum = 55;
    drawFlash(CRGB(40,40,40));
  }

// BUTTON 4: raise brightness, saturation, cycle time, high preset
//
// Lengthen auto cycle timer
  if (button4.uniquePress() && pattern_num == 99){
    if (auto_cycle_timer >= 60) auto_cycle_timer += 60;
    else auto_cycle_timer += 10;
    if (auto_cycle_timer > 240) auto_cycle_timer = 240;
    // auto_cycle_timer = ClampValueGreater(auto_cycle_timer, 240);
  }
// Up in brightness or chroma
  else if (button4.isPressed() && pattern_num != 99){
    if (!button2.isPressed()) {
      gLum = IncrementInt(gLum, true, 3, 1, 0, 255, 25, 235);
    }
    else if (button2.isPressed()) gSat = IncrementInt(gSat, true, 3, 1, 1, 255, 25, 235);
  }
// toggle high preset
  if (button4.held(3000) && button2.isPressed() && pattern_num != 99){
    Preset(1);
    gLum = 230;
    drawFlash(CRGB(255,255,255));
  }
  
// BUTTON 6: Pause, blast mode, cycle timer selection
//
// Toggle hue pause, exit auto time select mode, or switch sync modes
  if(button6.uniquePress()){
    if (toggle_pause == 0) toggle_pause = 1;
    else toggle_pause = 0;

    // exit auto time select mode
    if (pattern_num == 99) {
      pattern_num = old_pattern_num;
      toggle_pause = 0;
    }

    // switch sync modes
    if (button2.isPressed()){
      toggle_sync++;
      if (toggle_sync > 2) toggle_sync = 0;
      toggle_pause = 0;
      int flash_mult = toggle_sync+1;
      drawFlash(CRGB(5*flash_mult,100*flash_mult,80*flash_mult));
    }
  } 
// enter auto time select mode
  if (button6.held(2200) && !button2.isPressed()){
    old_pattern_num = pattern_num;
    pattern_num = 99;
  }
// Toggle blast mode and switch to ping length
  else if (button6.held(2200) && button2.isPressed()){
    if (toggle_blastMode == 0) {
      toggle_blastMode = 1;
      pattern_num = 2;
      drawFlash(CRGB(0,255,0));
    }
    else {
      toggle_blastMode = 0;
      drawFlash(CRGB(255,40,0));
    }
  }

// BUTTON 7: lower animation speed or interval width
//
  if(button7.isPressed()){
    if (!button2.isPressed()) anim_speed = IncrementInt(anim_speed, false, 2, 1, 1, 100, 10, 90);
    else interval_width = IncrementInt(interval_width, false, 2, 1, 1, 100, 10, 90);
  }

// BUTTON 8: raise animation speed or interval width
//
  if(button8.isPressed()){
    // variable, dir, increment, safe increment, min, max, pad neg, pad pos
    if (!button2.isPressed()) anim_speed = IncrementInt(anim_speed, true, 2, 1, 1, 100, 10, 90);
    else interval_width = IncrementInt(interval_width, true, 2, 1, 1, 100, 10, 90);
  }
 
}

