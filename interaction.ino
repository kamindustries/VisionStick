void Interaction(){

  //Pattern up
  if(button1.uniquePress()){
    pattern_num++;
    if (pattern_num >= num_patterns) pattern_num = 0;
    ResetVars();
  }

  // Pattern down
  if (button2.uniquePress()){
    pattern_num--;
    if (pattern_num < 0) pattern_num = num_patterns-1;
    ResetVars();
  }

  // Down in brightness or chroma
  if(button3.uniquePress()){
    if (toggle_chroma==0) gLum = IncrementInt(gLum, -1, 20, 5, 0, 255, 25, 235);
    else if (toggle_chroma==1) gSat = IncrementInt(gSat, -1, 20, 5, 1, 255, 25, 235);
  }

  // Up in brightness or chroma
  // Toggle chroma
  if (button4.uniquePress()){
    if (toggle_chroma==0) gLum = IncrementInt(gLum, 1, 20, 5, 0, 255, 25, 235);
    else if (toggle_chroma==1) gSat = IncrementInt(gSat, 1, 20, 5, 1, 255, 25, 235);
  }
  if (button4.held(1000)){
    if (toggle_chroma == 0) toggle_chroma = 1;
    else toggle_chroma = 0;
    drawFlash();
  }
  
//  Button 5 is kill    
//  if(button5.uniquePress()){
//  }

  // Toggle hue pause
  if(button6.uniquePress()){
    if (toggle_pause == 0) toggle_pause = 1;
    else toggle_pause = 0;
  }

  // Speed down
  if(button7.uniquePress()){
    anim_speed = IncrementInt(anim_speed, -1, 5, 1, 1, 100, 10, 90);
  }

  // Speed up
  if(button8.uniquePress()){
    // variable, dir, increment, safe increment, min, max, pad neg, pad pos
    anim_speed = IncrementInt(anim_speed, 1, 5, 1, 1, 100, 10, 90);
  }
 
}

