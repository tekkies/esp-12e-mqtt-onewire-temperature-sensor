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
};


IState* setState(String stateName, int sizeForTypeCheck);
IState *state;

#define SET_STATE(stateName) setState(#stateName, sizeof(stateName))

#endif