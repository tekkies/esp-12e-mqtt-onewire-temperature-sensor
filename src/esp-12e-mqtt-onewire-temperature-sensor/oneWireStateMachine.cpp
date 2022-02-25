
#include "src\OneWire\OneWire.h"

#include "stateMachine.h"
#include "states.h"
#include "oneWireStateMachine.h"


OneWire *oneWire;

void InitOneWire::execute() {
  OneWireState::execute();
  pinMode(4, OUTPUT);
  digitalWrite(4,HIGH);
  oneWire = new OneWire(5);
  SET_STATE(EndState);
}
