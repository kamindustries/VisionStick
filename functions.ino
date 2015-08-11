//////////////////////////////////////////////////
// F U N C T I O N S
//////////////////////////////////////////////////

//////////////////////////////////////////////////
// Update LEDs
//////////////////////////////////////////////////
void UpdateLEDS(){
  FastLED.show();
  FastLED.delay(1000/FRAMES_PER_SECOND); 
}

//////////////////////////////////////////////////
// Presets
//////////////////////////////////////////////////
void Preset(int _num){
  // low/chill
  if (_num == 0){
    gLum = 50;
    gSat = 220;
    anim_speed = 10;
    interval_width = 10;
    ResetVars();
  }
  // high/woah
  if (_num == 1){
    gLum = 235;
    gSat = 255;
    anim_speed = 95;
    interval_width = 90;
    ResetVars();
  }
}

//////////////////////////////////////////////////
// Reset Vars
//////////////////////////////////////////////////
void ResetVars(){
  for (int i = 0; i < 3; i++){
    t[i] = 0;
    d_t[i] = 0;
    strip_ctrl[i] = 55;
    rand_message_interval = 30000;
  }
}

//////////////////////////////////////////////////
// Ease In
//////////////////////////////////////////////////
float EaseIn(float _value, float _target, float _speed){
  float dx = _target - _value;
  dx *= _speed;
  return dx;
}

//////////////////////////////////////////////////
// Increment Integer
//////////////////////////////////////////////////
int IncrementInt(int _var, bool _dir, int _inc, int _safe_inc, int _min, int _max, 
                  int _safe_neg, int _safe_pos){
  if (_dir){
    if (_var > _safe_pos) _var += _safe_inc;
    else _var += _inc;
    if (_var > _max) _var = _max;
  }
  else {
    if (_var < _safe_neg) _var -= _safe_inc;
    else _var -= _inc;
    if (_var < _min) _var = _min;
  }
  return _var;
}

//////////////////////////////////////////////////
// Flip position for snake layout
//////////////////////////////////////////////////
int FlipPosition(int _var){
  int output = _var;
  if (_var >= strip_start[1] && _var < strip_start[2]) {
    output = map(_var, strip_start[1], strip_start[2], strip_start[2], strip_start[1]);
    output--;
  }
  return output;
}

int CheckDirection(int _pos){
  if (_pos >= strip_start[1] && _pos < strip_start[2]) {
    return -1;
  }
  else return 1;
}

// void BlendHue(){
//   for (int i = 1; i < num_leds-1; i++){
//     int pos = FlipPosition(i);
//     int dir = CheckDirection(pos);
//     int diff = ((-2*leds_hsv[i].h)%255) + leds_hsv[i+dir].h + leds_hsv[i+dir].h;
//     diff /= 2;
//     diff = ((diff+leds_hsv[i].h)%255);
    
//     leds_hsv[pos].h = diff;
//     CRGB rgb;
//     hsv2rgb_rainbow(leds_hsv[pos], rgb);
//     leds[pos] = rgb;
//   }
// }