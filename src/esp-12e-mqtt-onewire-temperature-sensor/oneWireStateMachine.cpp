
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
    Serial.println("");

   if (OneWire::crc8(oneWireContext->addr, 7) != oneWireContext->addr[7]) {
       Serial.println("CRC is not valid!");
       setState(oneWireContext->failExitState);
       return;
   }

  // the first ROM byte indicates which chip
  switch (oneWireContext->addr[0]) {
    case 0x10:
      Serial.println("  Chip = DS18S20");  // or old DS1820
      oneWireContext->type_s = 1;
      break;
    case 0x28:
      Serial.println("  Chip = DS18B20");
      oneWireContext->type_s = 0;
      break;
    case 0x22:
      Serial.println("  Chip = DS1822");
      oneWireContext->type_s = 0;
      break;
    default:
      Serial.println("Device is not a DS18x20 family device.");
      return;
  } 




   setState(oneWireContext->successExitState);
}