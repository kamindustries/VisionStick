void Interaction(){

// Pattern up
// goes up or down when not in blast mode
// when in blast mode, button 1 fires a shot. holding button 2 advances 1 pattern 
  if(button1.uniquePress()){ 
    if ((!button2.isPressed() && toggle_blastMode == 0) || 
        (button2.isPressed() && toggle_blastMode == 1)){
      pattern_num++;
      if (pattern_num > num_patterns-1) pattern_num = 0;
      ResetVars();
    }
    // Pattern down
    else if (button2.isPressed() && toggle_blastMode == 0){
      pattern_num--;
      if (pattern_num < 0) pattern_num = num_patterns-1;
      ResetVars();
    }
    else if (!button2.isPressed() && toggle_blastMode == 1){
      shootBlast++;
      if (shootBlast > 4) shootBlast = 4;
      for (int i = 0; i < shootBlast*3; i++){
        // for (int j = 1; j <= shootBlast; j++){
          if (ping[i].done==true) ping[i].ResetPair(interval_width, -1);
        // }
      }
    }
  }

// Toggle auto switching
  if (button1.held(1800) && button2.isPressed() && toggle_blastMode == 0){
    if (toggle_autoCycle == 0){
      toggle_autoCycle++;
      drawFlash();
    }
    else if (toggle_autoCycle == 1){
      toggle_autoCycle = 0;
      drawFlash(CRGB(255,0,0));
    }
  }

// Down in brightness or chroma
  if(button3.isPressed()){
    // toggle low preset
    if (button3.held(3000) && !button2.isPressed()){
      Preset(0);
      drawFlash(CRGB(40,40,40));
    }
    if (!button2.isPressed()) {
      gLum = IncrementInt(gLum, false, 3, 1, 0, 255, 25, 235);
    }
    else gSat = IncrementInt(gSat, false, 3, 1, 1, 255, 25, 235);
  }

// Up in brightness
// Up in chroma
  if (button4.isPressed()){
    // toggle high preset
    if (button4.held(3000) && !button2.isPressed()){
      Preset(1);
      drawFlash(CRGB(255,255,255));
    }
    if (!button2.isPressed()) {
      gLum = IncrementInt(gLum, true, 3, 1, 0, 255, 25, 235);
    }
    else gSat = IncrementInt(gSat, true, 3, 1, 1, 255, 25, 235);
  }
  
  
// Toggle hue pause
  if(button6.uniquePress()){
    if (toggle_pause == 0) toggle_pause = 1;
    else toggle_pause = 0;
    if (button2.isPressed()){
      toggle_sync++;
      if (toggle_sync > 2) toggle_sync = 0;
      toggle_pause = 0;
      int flash_mult = toggle_sync+1;
      drawFlash(CRGB(5*flash_mult,100*flash_mult,80*flash_mult));
    }
  }
  if (button6.held(2200) && button2.isPressed()){
    // blast mode goes here
    if (toggle_blastMode == 0) {
      toggle_blastMode = 1;
      drawFlash(CRGB(0,255,0));
    }
    else {
      toggle_blastMode = 0;
      drawFlash(CRGB(255,40,0));
    }
  }

// Speed down
  if(button7.isPressed()){
    if (!button2.isPressed()) anim_speed = IncrementInt(anim_speed, false, 2, 1, 1, 100, 10, 90);
    else interval_width = IncrementInt(interval_width, false, 2, 1, 1, 100, 10, 90);
  }

// Speed up
  if(button8.isPressed()){
    // variable, dir, increment, safe increment, min, max, pad neg, pad pos
    if (!button2.isPressed()) anim_speed = IncrementInt(anim_speed, true, 2, 1, 1, 100, 10, 90);
    else interval_width = IncrementInt(interval_width, true, 2, 1, 1, 100, 10, 90);
  }
 
}

