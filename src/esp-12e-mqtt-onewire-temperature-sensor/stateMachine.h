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
    
    virtual String* validExitStates() {
      String exitStates[] = {"abc", ""};
      return exitStates;
      //String* exitStates = new String[2];
      //exitStates[0] = "Test 1";
      //exitStates[1] = "";
      //return exitStates;
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