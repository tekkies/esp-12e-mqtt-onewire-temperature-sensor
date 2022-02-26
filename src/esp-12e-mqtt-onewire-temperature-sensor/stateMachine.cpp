#include <ESP8266WiFi.h>
#include "stateMachine.h"


IState *state=NULL;

IState* setState(String stateName) {
    IState *candidateState=NULL;
    int stateIndex = 0;
    while(states[stateIndex] != NULL)
    {
        if(states[stateIndex]->stateName == stateName) {
            candidateState = states[stateIndex];
            break;
        }
        stateIndex++;
    }

    if(candidateState == NULL) {
        Serial.print("State not found " + stateName);
    }

    if(state!=NULL) {
        char **pExitStates = state->validExitStates();
        Serial.print("State Exit Check:");
        Serial.println(state->stateName);

        if(pExitStates != NULL) {
            Serial.println("setState:NOT NULL");
            Serial.println(pExitStates[0]);
        } else {
            Serial.println("setState:NULL");
        }
    }
    state = candidateState;
    return state;
}




void DelayState::reset(int ms, String nextState) {
    endMs = millis()+ms;
    exitState = nextState;
}

#include "oneWireStateMachine.h"

void DelayState::execute(){
    int remaining = endMs - millis();
    Serial.print("  ready ");

    Serial.print(oneWireContext->oneWire->read_bit());

    Serial.print("  remaining ");
    

    Serial.println(remaining);
    if(remaining <= 0) {
        Serial.print("Delay complete - jumping to state ");
        Serial.println(exitState);
        setState(exitState);
    }
}


