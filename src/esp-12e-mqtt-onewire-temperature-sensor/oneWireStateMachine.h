#ifndef oneWireStateMachine_h
#define oneWireStateMachine_h

#include "stateMachine.h"

class OneWireState : IState {
    public:
      OneWireState(String name) : IState(name) {}
      void execute() {
        IState::execute();
      }  
};

class InitOneWire : OneWireState {
  public: 
    InitOneWire(String name) : OneWireState(name) {}
    void execute();
};



#endif

