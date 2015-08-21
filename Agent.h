// undo attempts to accomidate pinging across strips
// confine to 55 pixels as before and add position according to strip # assignment 
// try and fix tail

class Agent{
  public:
    int start_pos, end_pos, pos, d_pos, hue_offset, dir, ID, stripNum;
    bool strip2, done;
    float t, dt, spd, speed_mult;
    Agent();
    Agent(int _start_pos, int _end_pos, float _speed);
    void draw(CRGB _leds[], int _anim_speed, int _interval_width, int _gHue, int _gSat,
              int _manual_start_pos, uint8_t _sync, bool _turbo);
    void drawPing(CRGB _leds[], int _gHue, int _gSat);
    bool checkIfDone();
    int checkIfDoneBlast();
    bool checkSerpentine(int _pos);
    void Reset(int _interval_width, int _manual_start_pos, int _manual_end_pos);
    void ResetPair(int _interval_width, int _manual_start_pos);
    float EaseIn(float _value, float _target, float _speed);
    float getVelocity();
    int getValue();
    int FixPosition(int _pos);
};

Agent::Agent(){
    start_pos = 0;
    end_pos = 48;
    spd = .05;
    t = 0;
    dt = 0;
    pos = 0;
    d_pos = 0;
    dir = 1;
    hue_offset = random8();
    stripNum = random8(3);
    speed_mult = 1.;
}  

Agent::Agent(int _start_pos, int _end_pos, float _speed){
    start_pos = _start_pos;
    end_pos = _end_pos;
    spd = _speed;
    pos = 0;
    d_pos = 0;
    t = 0;
    dt = 0;
    dir = 1;
    hue_offset = random8();
    if (ID < 3) stripNum = ID;
    else stripNum = random8(3); 
}

// Main draw call
void Agent::draw(CRGB _leds[], int _anim_speed, int _interval_width, int _gHue, int _gSat, 
                int _manual_start_pos = -1, uint8_t _sync = 0, bool _turbo = false){
    dt = t;
    spd = (float)2/(104-_anim_speed);
    if (spd > 0.08) spd = 0.08;
    spd *= speed_mult;
    t += EaseIn(t, end_pos, spd);

    pos = FixPosition(int(t + (stripNum*48)));
    d_pos = FixPosition(int(dt + (stripNum*48)));

    // Reset numbers if we reached the end
    if (_turbo == false){
      if (checkIfDone()) {
        if (_sync > 0) ResetPair(_interval_width, _manual_start_pos);
        else Reset(_interval_width, _manual_start_pos, -1);
      }
    }

    // lights up leading edge
    drawPing(_leds, _gHue, _gSat); 
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void Agent::drawPing(CRGB _leds[], int _gHue, int _gSat){
    float v = getVelocity() * _gSat;
    int hue = _gHue + hue_offset;
    int lum = getValue();
    float sat = (float)_gSat/1.5;

    _leds[pos] = CHSV(hue, sat, lum);

    // now do the tail
    int diff = int(t) - int(dt);
    if (dir == -1) diff = int(dt) - int(t);
    if (diff > 0) {      
      for (int i = 1; i <= diff; i++) {
        int tail_pos = pos-(i*dir);
        if (strip2) tail_pos = pos+(i*dir);  
        int rnd_hue = (2*(random8(10) - 10))+1;
        int tail_hue = hue + rnd_hue + i;

        // assign the tail color
        if (tail_pos > 0 && tail_pos < 144) {
          _leds[tail_pos] = CHSV(tail_hue, sat, lum);
        }
      }
    }
    // little trail tip extra
    // int rand_hue = hue + random8(25);
    // int forward = dir;
    // if (strip2) forward *= -1;
    // if (dir == 1) _leds[d_pos-forward] = CHSV(rand_hue, _gSat/2, lum);
    // else _leds[d_pos+forward] = CHSV(rand_hue, _gSat/2, lum);
}


bool Agent::checkIfDone(){
    if (t < 0 || t >= 48) return true;
    if (dir == 1){
      if (t > end_pos-2) return true;
      else return false;
    }
    else if (dir == -1){
      if (t < end_pos+2) return true;
      else return false;
    }
    else return false;
}

int Agent::checkIfDoneBlast(){
    if (t < 0 || t >= 48){ 
      done = true; 
      return 1;
    } 
    if (dir == 1){
      if (t > end_pos-2) {
        done = true; 
        return 1;
      } 
      else {
        done = false; 
        return 0;
      } 
    }
    else if (dir == -1){
      if (t < end_pos+2){ 
        done = true; 
        return 1; 
      }
      else {
        done = false; 
        return 0;
      } 
    }
    else {
      done = false; 
      return 0;
    } 
}

bool Agent::checkSerpentine(int _pos){
  if (_pos >= 48 && _pos < 96) return true;
  else return false;
}

void Agent::Reset(int _interval_width, int _manual_start_pos, int _manual_end_pos = -1){

    int min_length = map(_interval_width, 1, 100, 1, 30);
    // pick direction and start/end pos
    if (random(255)<128) {
      if (_manual_start_pos != -1) start_pos = _manual_start_pos;
      else start_pos = 0;
      if (_manual_end_pos != -1) end_pos = _manual_end_pos;
      else end_pos = random8(48-min_length,55);  
    }
    else {
      if (_manual_start_pos != -1) start_pos = _manual_start_pos;
      else start_pos = 48;
      if (_manual_end_pos != -1) end_pos = _manual_end_pos;
      else end_pos = 48 - random8(min_length, 55);
    }
    if (start_pos < end_pos) dir = 1;
    else dir = -1;

    stripNum = ID%3;

    t = start_pos;
    dt = start_pos;
    pos = start_pos;
    d_pos = start_pos;
    hue_offset = random8();
}

// Unidirectional reset
void Agent::ResetPair(int _interval_width, int _manual_start_pos){

    int min_length = map(_interval_width, 1, 100, 1, 30);
    // same direction, gets mirrored in app
    if (_manual_start_pos != -1) start_pos = _manual_start_pos;
    else start_pos = 0;

    end_pos = random8(48-min_length,55);
    dir = 1;

    stripNum = ID%3;

    t = start_pos;
    dt = start_pos;
    pos = start_pos;
    d_pos = start_pos;
    hue_offset = random8();
    speed_mult = 2.;
    done = false;
}

int Agent::getValue(){
    int val;
    if (dir == 1){
      val = abs(int(t) - start_pos);
      val = 255 - map(val, 0, end_pos-1, 1, 128);
    }
    else {
      val = abs(start_pos - int(t));
      val = 255 - map(val, 0, start_pos+1, 1, 128);
    }
    if (val < 0) val = 0;
    if (val > 255) val = 255;
    return val;
}

float Agent::getVelocity(){
    float v = abs(t - dt);
    // float range = 48;
    // v = kk_map(v, 0.0, 1., 0.0, 255.0);
    return v;
}

float Agent::EaseIn(float _value, float _target, float _speed) {
    float dx = _target - _value;
    dx *= _speed;
    return dx;
}


//////////////////////////////////////////////////
// Flip position for snake layout
//////////////////////////////////////////////////
int Agent::FixPosition(int _pos){
  int output = _pos;
  if (_pos >= 48 && _pos < 96) {
    output = map(_pos, 48, 96, 96, 48);
    output--;
  }
  if (output < 0) output = 0;
  if (output >= 144) output = 144;

  return output;
}