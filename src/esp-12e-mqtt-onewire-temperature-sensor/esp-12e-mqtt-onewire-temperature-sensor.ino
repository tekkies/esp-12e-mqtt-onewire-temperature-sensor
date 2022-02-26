#include <ESP8266WiFi.h>
#include "src\arduino-mqtt\MQTT.h"
#include "src\OneWire\OneWire.h"

#include "config.h"
#include "secrets.h"

#include "stateMachine.h"
#include "oneWireStateMachine.h"

String json;




class SuccessState : IState {
  public: 
    SuccessState(String name) : IState(name) {}
    void execute();
};

void SuccessState::execute() {
  IState::execute();
  Serial.println("Sleeping for 5 seconds");
  ESP.deepSleep(5e6); 
}

class FailState : IState {
  public: 
    FailState(String name) : IState(name) {}
    void execute();
};

void FailState::execute() {
  IState::execute();
  Serial.println("Sleeping for 5 seconds");
  ESP.deepSleep(5e6); 
}


#define DECLARESTATE(aState) (IState*)(new aState(#aState))
IState *states[] = 
{
  DECLARESTATE(InitOneWire),
  DECLARESTATE(IdentifyOneWireDevice),
  DECLARESTATE(SuccessState),
  DECLARESTATE(FailState),
  DECLARESTATE(DelayState),
  DECLARESTATE(ReadTemperature),
  NULL
};



void setup() {
  Serial.begin(115200);
  Serial.println("setup");  
  WiFi.begin(ssid, pass);

  oneWireContext = new OneWireContext();
  oneWireContext->successExitState = "SuccessState";
  oneWireContext->failExitState = "FailState";

  setState("InitOneWire");
}




void loop() {
  Serial.println("loop");  
  state->execute();
}


// // Example based on...
// // This example uses an Adafruit Huzzah ESP8266
// // to connect to shiftr.io.
// //
// // You can check on your device after a successful
// // connection here: https://www.shiftr.io/try.
// //
// // by Joël Gähwiler
// // https://github.com/256dpi/arduino-mqtt


// WiFiClient net;
// MQTTClient client;

// unsigned long lastMillis = 0;

// void connect() {
//   Serial.print("checking wifi...");
//   while (WiFi.status() != WL_CONNECTED) {
//     Serial.print(".");
//     delay(1000);
//   }

//   Serial.print("\nconnecting...");
//   while (!client.connect("arduino", "public", "public")) {
//     Serial.print(".");
//     delay(1000);
//   }

//   Serial.println("\nconnected!");

//   client.subscribe("/hello");
//   // client.unsubscribe("/hello");
// }

// void messageReceived(String &topic, String &payload) {
//   Serial.println("incoming: " + topic + " - " + payload);

//   // Note: Do not use the client in the callback to publish, subscribe or
//   // unsubscribe as it may cause deadlocks when other things arrive while
//   // sending and receiving acknowledgments. Instead, change a global variable,
//   // or push to a queue and handle it in the loop after calling `client.loop()`.
// }

// void setup() {

//   // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported
//   // by Arduino. You need to set the IP address directly.
//   client.begin("test.mosquitto.org", net);
//   client.onMessage(messageReceived);

//   connect();
// }

// void loop() {
//   client.loop();
//   delay(10);  // <- fixes some issues with WiFi stability

//   if (!client.connected()) {
//     connect();
//   }

//   // publish a message roughly every second.
//   if (millis() - lastMillis > 1000) {
//     lastMillis = millis();
//     client.publish("/tekkies.co.uk/hello", "world");
//   }
// }











// // Based on OneWire DS18S20, DS18B20, DS1822 Temperature Example



// //
// // http://www.pjrc.com/teensy/td_libs_OneWire.html
// //
// // The DallasTemperature library can do all this work for you!
// // https://github.com/milesburton/Arduino-Temperature-Control-Library


// void setup(void) {
// }

// void loop(void) {
//   byte i;
//   byte present = 0;
//   

//   float celsius, fahrenheit;
  
 

//   ds.reset();
//   ds.select(addr);
//   ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  
//   delay(1000);     // maybe 750ms is enough, maybe not
//   // we might do a ds.depower() here, but the reset will take care of it.
  
//   present = ds.reset();
//   ds.select(addr);    
//   ds.write(0xBE);         // Read Scratchpad

//   Serial.print("  Data = ");
//   Serial.print(present, HEX);
//   Serial.print(" ");
//   for ( i = 0; i < 9; i++) {           // we need 9 bytes
//     data[i] = ds.read();
//     Serial.print(data[i], HEX);
//     Serial.print(" ");
//   }
//   Serial.print(" CRC=");
//   Serial.print(OneWire::crc8(data, 8), HEX);
//   Serial.println();

//   // Convert the data to actual temperature
//   // because the result is a 16 bit signed integer, it should
//   // be stored to an "int16_t" type, which is always 16 bits
//   // even when compiled on a 32 bit processor.
//   int16_t raw = (data[1] << 8) | data[0];
//   if (type_s) {
//     raw = raw << 3; // 9 bit resolution default
//     if (data[7] == 0x10) {
//       // "count remain" gives full 12 bit resolution
//       raw = (raw & 0xFFF0) + 12 - data[6];
//     }
//   } else {
//     byte cfg = (data[4] & 0x60);
//     // at lower res, the low bits are undefined, so let's zero them
//     if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
//     else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
//     else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
//     //// default is 12 bit resolution, 750 ms conversion time
//   }
//   celsius = (float)raw / 16.0;
//   fahrenheit = celsius * 1.8 + 32.0;
//   Serial.print("  Temperature = ");
//   Serial.print(celsius);
//   Serial.print(" Celsius, ");
//   Serial.print(fahrenheit);
//   Serial.println(" Fahrenheit");
// }