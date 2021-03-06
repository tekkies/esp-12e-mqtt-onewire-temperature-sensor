#include <ESP8266WiFi.h>
#include "src\arduino-mqtt\MQTT.h"
#include "src\OneWire\OneWire.h"

#include "config.h"
#include "secrets.h"

#include "stateMachine.h"
#include "oneWireStateMachine.h"


const char version[] = "1.2.0";


ADC_MODE(ADC_VCC);

WiFiClient net;
MQTTClient client(1024);
String json;




class SuccessState : IState {
  public: 
    SuccessState(String name) : IState(name) {}
    void execute();
};

class FailState : IState {
  public: 
    FailState(String name) : IState(name) {}
    void execute();
};

class PublishMqttState : IState {
  public: 
    PublishMqttState(String name) : IState(name) {}
    void execute();
};

void SuccessState::execute() {
  IState::execute();
  

  ESP.deepSleep(sleepSeconds * 1e6); 
}

void FailState::execute() {
  IState::execute();
}


void PublishMqttState::execute() {
  Serial.print("w");
  if(client.connected()) {

    json += "\"mV\":";
    json += ESP.getVcc();

    json += ",\"build\":\"v";
    json += version;
    json += " ";
    json += __DATE__;
    json += " ";
    json += __TIME__;
    json += "\"";
    
    json += ",\"sleep\":";
    json += sleepSeconds;

    json += ",\"heap\":";
    json += ESP.getFreeHeap();

    json += ",\"rssi\":";
    json += WiFi.RSSI();

    json += "}";

    Serial.println("");
    Serial.println("Publish JSON");
    Serial.println(json);

    String mqttTopic = mqttTopicBase + WiFi.macAddress();
    
    client.publish(mqttTopic, json);
    Serial.println("Published");
    setState("SuccessState");
  }
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
  DECLARESTATE(PublishMqttState),
  NULL
};


void temperatureCallback(byte address[], float temperature, String sensorModel) {

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


    json += "\"" + String(hexstr) + "\":{\"C\":" + temperature + ",\"model\":\""+sensorModel+"\"},";


}


void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
 
  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.
}

void setup() {
 
  json = "{";
  Serial.begin(115200);
  Serial.println("setup");  

  WiFi.begin(ssid, pass);
  client.begin(mqttServer, net);
  client.onMessage(messageReceived);

  oneWireContext = new OneWireContext();
  oneWireContext->successExitState = "PublishMqttState";
  oneWireContext->failExitState = "PublishMqttState";
  oneWireContext->callback = temperatureCallback;

  setState("InitOneWire");
}


void loop() {
  //Serial.println(state->stateName);  
  if(WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi Connected");
    if (!client.connected()) {
      Serial.println("mqtt not connected");
      client.connect("arduino", "public", "public");
      //client.subscribe("/hello");
      //client.unsubscribe("/hello");
    } else {
      Serial.println("mqtt is connected");
    }
  }
  client.loop();
  state->execute();
}


// // Example based on...
// // This example uses an Adafruit Huzzah ESP8266
// // to connect to shiftr.io.
// //
// // You can check on your device after a successful
// // connection here: https://www.shiftr.io/try.
// //
// // by Jo??l G??hwiler
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
