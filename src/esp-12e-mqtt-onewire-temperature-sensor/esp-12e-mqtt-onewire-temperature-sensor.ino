#include <ESP8266WiFi.h>
#include "src\arduino-mqtt\MQTT.h"
#include "src\OneWire\OneWire.h"

#include "config.h"
#include "secrets.h"

#include "stateMachine.h"
#include "oneWireStateMachine.h"

WiFiClient net;
MQTTClient client;
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
  DECLARESTATE(WaitForTemperature),
  DECLARESTATE(OneWireSearch),
  NULL
};


void temperatureCallback(byte address[], float temperature) {

    Serial.println("");
    Serial.print("Callback: address=");

    //https://stackoverflow.com/a/14050569/270155
    char hexstr[201];
    int i;
    for (i=0; i<8; i++) {
        sprintf(hexstr+i*2, "%02x", address[i]);
    }
    hexstr[i*2] = 0;

    Serial.print(hexstr);
    Serial.print(" temperature=");
    Serial.println(temperature);
}


void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);

  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.
}





void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("arduino", "public", "public")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("/hello");
  // client.unsubscribe("/hello");
}



void setup() {
  Serial.begin(115200);
  Serial.println("setup");  
  WiFi.begin(ssid, pass);
  client.begin("test.mosquitto.org", net);
  client.onMessage(messageReceived);
  connect();

  oneWireContext = new OneWireContext();
  oneWireContext->successExitState = "SuccessState";
  oneWireContext->failExitState = "FailState";
  oneWireContext->callback = temperatureCallback;

  setState("InitOneWire");
}




void loop() {
  //Serial.println(state->stateName);  
  if (!client.connected()) {
    connect();
  }
  client.loop();
  state->execute();
  client.publish("/tekkies.co.uk/state", state->stateName);
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







// void loop() {
//   
//   delay(10);  // <- fixes some issues with WiFi stability



//   // publish a message roughly every second.
//   if (millis() - lastMillis > 1000) {
//     lastMillis = millis();
//     client.publish("/tekkies.co.uk/hello", "world");
//   }
// }
