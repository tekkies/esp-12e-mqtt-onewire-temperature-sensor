#ifndef stateMachine_h
#define stateMachine_h


class IState {
  public: 
    String stateName;
    IState(String name) {
      stateName = name;
    }
    virtual void execute() {
      Serial.println(stateName);
    }
    
    /*virtual char*[] validExitStates() {
      char *colour[4] = { "Blue", "Red",
                             "Orange", "Yellow" };
      char*[] a = color;                    
      return colour;
    }*/
};

class DelayState : IState {
  private:
    int endMs;
    String exitState;
  public: 
    DelayState(String name) : IState(name) {}
    void execute();
    void reset(int ms, String nextState);
};

IState* setState(String stateName);

#endif