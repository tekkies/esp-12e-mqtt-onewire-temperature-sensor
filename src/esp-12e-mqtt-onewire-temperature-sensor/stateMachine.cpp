#include <ESP8266WiFi.h>
#include "stateMachine.h"



void DelayState::reset(int ms, String nextState) {
    endMs = millis()+ms;
    exitState = nextState;
}

void DelayState::execute(){
    int remaining = endMs - millis();
    Serial.print("  remaining ");
    Serial.println(remaining);
    if(remaining <= 0) {
        setState(exitState);
    }
}