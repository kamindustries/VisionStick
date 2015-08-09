class Agent{
  public:
    int start_pos, end_pos;
    float t, dt, spd;
    Agent();
    Agent(int _start_pos, int _end_pos, float _speed);
    void animate(int _anim_speed, int _interval_width);
    float EaseIn(float _value, float _target, float _speed);
    int getVelocity();
    int getPos();
    int getValue();
    int FlipPosition(int _var);
};

Agent::Agent(){
    start_pos = 0;
    end_pos = 48;
    spd = .05;
    t = 0;
    dt = 0;
}  

Agent::Agent(int _start_pos, int _end_pos, float _speed){
    start_pos = _start_pos;
    end_pos = _end_pos;
    spd = _speed;
    t = 0;
    dt = 0;
}

int Agent::getPos(){
    return FlipPosition(int(t));
}

int Agent::getValue(){
    int diff = int(t) - start_pos;
    diff = map(diff, 0, end_pos, 0, 255);
    return diff;
}

int Agent::getVelocity(){
    float diff = t - dt;
    int range = end_pos - start_pos;
    diff = map(diff, 0, range, 0, 1.0);
    return diff;
}

void Agent::animate(int _anim_speed, int _interval_width){
    spd = (float)2/(104-_anim_speed);
    if (spd > 0.08) spd = 0.08;
    t += EaseIn(t, end_pos, spd);

    // reset numbers if it reached the end 
    // sync them up when interval width is less than 5
    int length_mapped = map(_interval_width, 1, 100, 1, 55);
    if (t >= 48 || t > end_pos-1){
      t = 0;
      dt = 0;
      end_pos = 55 - random8(length_mapped); // set next length
    }

}


float Agent::EaseIn(float _value, float _target, float _speed) {
    float dx = _target - _value;
    dx *= _speed;
    return dx;
}

//////////////////////////////////////////////////
// Flip position for snake layout
//////////////////////////////////////////////////
int Agent::FlipPosition(int _var){
  int output = _var;
  if (_var >= 48 && _var < 96) {
    output = map(_var, 48, 96, 96, 48);
    output--;
  }
  return output;
}