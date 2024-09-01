//
// Created by kgrin on 12/9/2023.
//

#ifndef HUB_C_VERSION_STATE_H
#define HUB_C_VERSION_STATE_H


static char armedMsg[] = "Armed";
static char disarmedMsg[] = "Disarmed";
static char errorMsg[] = "Error";
static char waitingMsg[] = "Waiting";

enum State {
    ARMED,
    DISARMED,
    ERROR,
    WAITING
};

typedef enum State State;

char* getName(State *state) {
    char* rtn;
    switch (*state) {
        case ARMED:
            rtn = armedMsg;
            break;
        case DISARMED:
            rtn = disarmedMsg;
            break;
        case ERROR:
            rtn = errorMsg;
            break;
        case WAITING:
            rtn = waitingMsg;
            break;
    }
    return rtn;
}
char* getStateName(State state) {
    char* rtn;
    switch (state) {
        case ARMED:
            rtn = armedMsg;
            break;
        case DISARMED:
            rtn = disarmedMsg;
            break;
        case ERROR:
            rtn = errorMsg;
            break;
        case WAITING:
            rtn = waitingMsg;
            break;
    }
    return rtn;
}
#endif //HUB_C_VERSION_STATE_H
