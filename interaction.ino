void Interaction(){

  //Pattern up
    if(button1.uniquePress()){
      pattern_num++;
      if (pattern_num > num_patterns-1) pattern_num = 0;
      ResetVars();
    }

    // Down in brightness or chroma
    if(button3.isPressed()){
      if (!button2.isPressed()) gLum = IncrementInt(gLum, false, 3, 1, 0, 255, 25, 235);
      else gSat = IncrementInt(gSat, false, 3, 1, 1, 255, 25, 235);
    
    }

    // Up in brightness
    // Up in chroma
    if (button4.isPressed()){
      // if (!button2.isPressed()) gLum = IncrementInt(gLum, true, 20, 5, 0, 255, 25, 235);
      if (!button2.isPressed()) gLum = IncrementInt(gLum, true, 3, 1, 0, 255, 25, 235);
      else gSat = IncrementInt(gSat, true, 3, 1, 1, 255, 25, 235);
      
    }
    // if (button4.held(1000)){
    //   drawFlash();
    // }
    
    // Toggle hue pause
    if(button6.uniquePress()){
      if (toggle_pause == 0) toggle_pause = 1;
      else toggle_pause = 0;
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

