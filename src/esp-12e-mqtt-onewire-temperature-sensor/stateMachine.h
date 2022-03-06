#ifndef stateMachine_h
#define stateMachine_h


class IState {
  public: 
    String stateName;
    char **exitStates;



    IState(String name) {
      stateName = name;
      exitStates = new char*[4];
      exitStates[0]="Charlie";
      exitStates[1]="Delta";
      exitStates[2]=NULL;

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