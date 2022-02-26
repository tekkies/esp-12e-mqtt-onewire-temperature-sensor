#ifndef oneWireStateMachine_h
#define oneWireStateMachine_h

#include "src\OneWire\OneWire.h"
#include "stateMachine.h"

class OneWireContext {
  public:
    OneWire *oneWire;
    byte addr[8];
    String successExitState;
    String failExitState;
    byte type_s;
    int conversionTimeout;
};

extern OneWireContext *oneWireContext;

class InitOneWire : IState {
  public: 
    InitOneWire(String name) : IState(name) {}
    void execute();
};

class IdentifyOneWireDevice : IState {
  public: 
    IdentifyOneWireDevice(String name) : IState(name) {}
    void execute();
};

class WaitForTemperature : IState {
  public: 
    WaitForTemperature(String name) : IState(name) {}
    void execute();
};


#endif

