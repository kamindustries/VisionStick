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
// Reset Vars
//////////////////////////////////////////////////
void ResetVars(){
  for (int i = 0; i < 3; i++){
    t[i] = 0;
    d_t[i] = 0;
    strip_ctrl[i] = 55;
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

