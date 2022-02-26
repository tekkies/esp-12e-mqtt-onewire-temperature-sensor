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
    
    virtual const String* validExitStates() {
     const String exitStates[] = {"abc", ""};
     return exitStates;
    }
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

extern IState *state;
extern IState *states[];


#endif