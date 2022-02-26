
#include "src\OneWire\OneWire.h"

#include "stateMachine.h"
#include "oneWireStateMachine.h"


OneWireContext *oneWireContext;

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
    
    Serial.print("  ROM =");
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
      Serial.println("  Chip = DS18S20/DS1820");
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
       setState(oneWireContext->failExitState);
       return;
  } 


   oneWireContext->oneWire->reset();
   oneWireContext->oneWire->select(oneWireContext->addr);
   oneWireContext->oneWire->write(0x44, 1);        // start conversion, with parasite power on at the end

   setState("WaitForTemperature");
   oneWireContext->conversionTimeout = millis()+1000;
}

  void WaitForTemperature::execute() {
  
  Serial.print("  ready=");
  uint8_t ready = oneWireContext->oneWire->read_bit();
  Serial.print(ready);
  
  Serial.print(" timeout = ");
  Serial.print(oneWireContext->conversionTimeout - millis());
  Serial.println();
  if(ready > 0)   {
    byte present = 0;
    present = oneWireContext->oneWire->reset();
    oneWireContext->oneWire->select(oneWireContext->addr);    
    oneWireContext->oneWire->write(0xBE);         // Read Scratchpad

    Serial.print("  Data = ");
    Serial.print(present, HEX);
    Serial.print(" ");
    byte data[12];
    for ( int i = 0; i < 9; i++) {           // we need 9 bytes
      data[i] = oneWireContext->oneWire->read();
      Serial.print(data[i], HEX);
      Serial.print(" ");
    }
    Serial.print(" CRC=");
    Serial.print(OneWire::crc8(data, 8), HEX);
    Serial.println();


    // Convert the data to actual temperature
    // because the result is a 16 bit signed integer, it should
    // be stored to an "int16_t" type, which is always 16 bits
    // even when compiled on a 32 bit processor.
    int16_t raw = (data[1] << 8) | data[0];
    if (oneWireContext->type_s) {
      raw = raw << 3; // 9 bit resolution default
      if (data[7] == 0x10) {
        // "count remain" gives full 12 bit resolution
        raw = (raw & 0xFFF0) + 12 - data[6];
      }
    } else {
      byte cfg = (data[4] & 0x60);
      // at lower res, the low bits are undefined, so let's zero them
      if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
      else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
      else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
      //// default is 12 bit resolution, 750 ms conversion time
    }
    float celsius = (float)raw / 16.0;
    float fahrenheit = celsius * 1.8 + 32.0;
    Serial.print("  Temperature = ");
    Serial.print(celsius);
    Serial.print(" Celsius, ");
    Serial.print(fahrenheit);
    Serial.println(" Fahrenheit");
    setState(oneWireContext->successExitState);
  } else {
    if(millis() > oneWireContext->conversionTimeout) {
      setState(oneWireContext->failExitState);
    }
  }
}