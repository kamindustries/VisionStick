// undo attempts to accomidate pinging across strips
// confine to 55 pixels as before and add position according to strip # assignment 
// try and fix tail

class Agent{
  public:
    int start_pos, end_pos, pos, d_pos, hue_offset, dir, ID, stripNum;
    bool strip2;
    float t, dt, spd;
    Agent();
    Agent(int _start_pos, int _end_pos, float _speed);
    void draw(CRGB _leds[], int _anim_speed, int _interval_width, int _gHue, int _gSat);
    void drawPing(CRGB _leds[], int _gHue, int _gSat);
    bool checkIfDone();
    bool checkSerpentine(int _pos);
    void Reset(int _interval_width);
    float EaseIn(float _value, float _target, float _speed);
    float getVelocity();
    int getPos();
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
    if (ID < 3) stripNum = ID;
    else stripNum = random8(3);
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
void Agent::draw(CRGB _leds[], int _anim_speed, int _interval_width, int _gHue, int _gSat){
    dt = t;
    spd = (float)2/(104-_anim_speed);
    if (spd > 0.08) spd = 0.08;
    t += EaseIn(t, end_pos, spd);

    pos = FixPosition(int(t + (stripNum*48)));
    d_pos = FixPosition(int(dt + (stripNum*48)));

    // Reset numbers if we reached the end
    if (checkIfDone()) Reset(_interval_width);

    // lights up leading edge
    drawPing(_leds, _gHue, _gSat); 
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void Agent::drawPing(CRGB _leds[], int _gHue, int _gSat){
    float v = getVelocity() * _gSat;
    int hue = _gHue + hue_offset;
    int lum = getValue();
    int sat = _gSat/2;
    int diff = abs(t - (dt * dir));

    _leds[pos] = CHSV(hue, sat, lum);
    // now do the tail
    // if (diff > 0) {      
    //   for (int i = 0; i < diff; i++) {
    //     int rx = (2*(random8(10) - 10))+1;
    //     int tail_hue = hue + rx + i;
    //     int tail_pos = pos-dir-(i*dir);

    //     // tail_pos = FixPosition(tail_pos);
    //     // // check for serpentine
    //     if (checkSerpentine(pos)) {  
    //       tail_pos = pos+dir-(i*-dir);
    //     }

    //     // assign the tail color
    //     if (tail_pos >= 0 && tail_pos < 144) {
    //       _leds[tail_pos] = CHSV(tail_hue, sat, lum);
    //     }

    //   }
    // }

    int rand_hue = _gHue + random8(5);

    // little trail tip extra
    // int temp_dir = dir;
    // if (strip2) temp_dir *= -1;
    // if (temp_dir == 1) _leds[d_pos-1] = CHSV(rand_hue, _gSat/2, lum);
    // else _leds[d_pos+1] = CHSV(rand_hue, _gSat/2, lum);
    
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

bool Agent::checkSerpentine(int _pos){
  if (_pos >= 48 && _pos < 96) return true;
  else return false;
}

void Agent::Reset(int _interval_width){
    random16_add_entropy(random());

    // pick direction and start/end pos
    if (random(255)<128) {
      start_pos = 0;
      end_pos = random8(10,55);
      dir = 1;
    }
    else {
      start_pos = 48;
      end_pos = 48 - random8(5, 48) - 5;
      dir = -1;
    }
    if (ID < 3) stripNum = ID;
    else stripNum = random8(3);

    t = start_pos;
    dt = start_pos;
    pos = start_pos;
    d_pos = start_pos;
    hue_offset = random8();

}

int Agent::getPos(){
    return FixPosition(int(t));
}

int Agent::getValue(){
    int val;
    if (dir == 1){
      val = abs(int(t) - start_pos);
      val = 255 - map(val, 0, end_pos-1, 1, 255);
    }
    else {
      val = abs(start_pos - int(t));
      val = 255 - map(val, 0, start_pos+1, 1, 255);
    }
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