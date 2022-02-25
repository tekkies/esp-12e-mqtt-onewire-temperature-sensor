
#include "src\OneWire\OneWire.h"

#include "stateMachine.h"
#include "states.h"
#include "oneWireStateMachine.h"


OneWireContext *oneWireContext;

void InitOneWire::execute() {
    IState::execute();
    pinMode(4, OUTPUT);
    digitalWrite(4,HIGH);
    oneWireContext = new OneWireContext();
    oneWireContext->oneWire = new OneWire(5);


    if ( !oneWireContext->oneWire->search(oneWireContext->addr)) {
        Serial.println("No more addresses.");
        Serial.println();
        oneWireContext->oneWire->reset_search();
        SET_STATE(EndState);
    }
    else
    {
        Serial.println("Found OneWire");
        SET_STATE(IdentifyOneWireDevice);    
    }
}

void IdentifyOneWireDevice::execute() {
  IState::execute();
  
  Serial.print("ROM =");
  for(int i = 0; i < 8; i++) {
    Serial.write(' ');
     Serial.print(oneWireContext->addr[i], HEX);
  }
  SET_STATE(EndState);
}