#ifndef oneWireStateMachine_h
#define oneWireStateMachine_h

#include "src\OneWire\OneWire.h"
#include "stateMachine.h"

class OneWireContext {
  public:
    OneWire *oneWire;
    String successExitState;
    String failExitState;
    byte addr[8];
    String addressString;
    byte type_s;
    int conversionTimeout;
    void (*callback)(byte address[], float temperature);
};

extern OneWireContext *oneWireContext;

class InitOneWire : IState {
  public: 
    InitOneWire(String name) : IState(name) {}
    void execute();
};

class OneWireSearch : IState {
  public: 
    OneWireSearch(String name) : IState(name) {}
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

