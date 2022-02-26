#include <ESP8266WiFi.h>
#include "stateMachine.h"

IState *state;

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
  state = candidateState;
  return state;
}

void DelayState::reset(int ms, String nextState) {
    endMs = millis()+ms;
    exitState = nextState;
}

void DelayState::execute(){
    int remaining = endMs - millis();
    Serial.print("  remaining ");
    Serial.println(remaining);
    if(remaining <= 0) {
        Serial.print("Delay complete - jumping to state ");
        Serial.println(exitState);
        setState(exitState);
    }
}


