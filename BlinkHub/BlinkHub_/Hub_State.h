//
// Created by kgrin on 12/9/2023.
//

#ifndef HUB_C_VERSION_HUB_STATE_H
#define HUB_C_VERSION_HUB_STATE_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "State.h"
#include "Colors.h"



typedef enum State State;
typedef struct Hub_State Hub_State;
// MKRIoTCarrier carrier;


// uint32_t green = 0x002A4B13;
// uint32_t white = 0x00FFFFFF;
// uint32_t purp = 0x00FF00FF;
// uint32_t red = 0x00C40000;

// uint32_t off = Adafruit_DotStar::Color(0, 0, 0);
// uint32_t purp = Adafruit_DotStar::Color(48, 33, 73);
// uint32_t green = Adafruit_DotStar::Color(42, 75, 19);
// uint32_t red = Adafruit_DotStar::Color(196, 0, 0);
// uint32_t white = Adafruit_DotStar::Color(100, 100, 100);


struct Hub_State {
    State state;
    uint32_t color;
    int startPixel;
    char *msg;
    const char *details;
};

Hub_State* createHubState(Hub_State* currState) {
  if (currState == NULL) {
    printf("%s\n", "Creating new Hub State struct.");
    currState = (Hub_State*) malloc(sizeof(Hub_State));
    (currState->state) = WAITING;
    currState->color = purp;
    currState->msg = waitingMsg;
    currState->startPixel = 36;
  }
  return currState;
}


Hub_State* setState(Hub_State* currState, State newState) {

    if (currState == NULL) {
        printf("%s\n", "Creating new Hub State struct.");
        currState = (Hub_State*) malloc(sizeof(Hub_State));
    }

    (currState->state) = newState;
    switch (newState) {
        case ARMED:
            (currState->color) = green;
            currState->msg = armedMsg;
            currState->startPixel = 60;
            break;
        case DISARMED:
            (currState->color) = white;
            currState->msg = disarmedMsg;
            currState->startPixel = 24;
            break;
        case ERROR:
            (currState->color) = red;
            currState->msg = errorMsg;
            currState->startPixel = 60;
            break;
        case WAITING:
            (currState->color) = purp;
            currState->msg = waitingMsg;
            currState->startPixel = 36;
            break;
    }
    return currState;
}

char* getPrintName(State state) {
  return getStateName(state);
}

const char* getDetails(Hub_State* currState) {
  if (currState->state == ERROR) {
    return currState->details; 
  } else {
    return currState->msg;
  }
}

Hub_State* setDetails(Hub_State* currState, const char* deets) {
  currState->details = deets; 
  return currState; 
}


// Hub_State* buttonZeroPress();

// Hub_State* buttonOnePress();

// Hub_State* buttonTwoPress();

// void buttonThreePress();

// void buttonFourPress();


Hub_State* destroyState(Hub_State *hubState) {
    printf("Destroying Hub_State struct");
    hubState->msg = NULL;
    free(hubState->msg);

    hubState = NULL;
    return hubState;
}

#endif //HUB_C_VERSION_HUB_STATE_H
