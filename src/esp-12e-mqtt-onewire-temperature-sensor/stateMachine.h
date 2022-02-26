#ifndef stateMachine_h
#define stateMachine_h


class IState {
  public: 
    String stateName;






    char **exitStates;
    const char* a="Alpha";
    const char* b="beta";



    IState(String name) {
      stateName = name;
      exitStates = new char*[4];
      exitStates[0]=(char*)a;
      exitStates[1]=(char*)b;
    }
    
    virtual void execute() {
      Serial.println(stateName);
    }
    
    

    char** validExitStates() {
    if(exitStates == NULL) {
      Serial.println("validExitStates:NULL");
    } else {
      Serial.println("validExitStates:NOT NULL");
    }
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