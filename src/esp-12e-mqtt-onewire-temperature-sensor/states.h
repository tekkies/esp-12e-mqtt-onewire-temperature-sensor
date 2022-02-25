#ifndef states_h
#define states_h

#include "stateMachine.h"

class EndState : IState {
  public: 
    EndState(String name) : IState(name) {}
    void execute();
};

#endif