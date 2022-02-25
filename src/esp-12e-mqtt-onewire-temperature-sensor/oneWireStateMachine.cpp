
#include "src\OneWire\OneWire.h"

#include "stateMachine.h"
#include "oneWireStateMachine.h"




void InitOneWire::execute() {
    IState::execute();
    pinMode(4, OUTPUT);
    digitalWrite(4,HIGH);
    oneWireContext->oneWire = new OneWire(5);

    if ( !oneWireContext->oneWire->search(oneWireContext->addr)) {
        Serial.println("No more addresses.");
        Serial.println();
        oneWireContext->oneWire->reset_search();
        setState(oneWireContext->failExitState);
    }
    else
    {
        Serial.println("Found OneWire");
        setState("IdentifyOneWireDevice");
    }
}

void IdentifyOneWireDevice::execute() {
    IState::execute();
    
    Serial.print("ROM =");
    for(int i = 0; i < 8; i++) {
        Serial.write(' ');
        Serial.print(oneWireContext->addr[i], HEX);
    }

   if (OneWire::crc8(oneWireContext->addr, 7) != oneWireContext->addr[7]) {
       Serial.println("CRC is not valid!");
       setState(oneWireContext->failExitState);
       return;
   }
   setState(oneWireContext->successExitState);
}