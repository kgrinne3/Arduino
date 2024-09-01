#include <Servo.h>

Servo lidServo;
Servo armServo;
//pins
const int btnPin = 3;
const int lidPin = 6;
const int armPin = 5;

//CLEARANCES
const int minClearance = 15;     //min window of operation for armServo relative to the lidServo
const int maxClearance = 60;     //max window of operation for armServo relative to the lidServo

//values
int lidPos = 0;
int armPos = 0;
int btnVal = 0;

//lid stuff
const int lidMax = 90;     //max degrees to move the lidServo
int delta = 2.5;       //rate of change in servo angle
int fastDelta = 10;  //for slamming shut and whipping open
int peekDelta = 1;   //super slow for peeking out

//arm stuff
const int armMax = 90;           //max degrees to move the armServo

//THIS SHOULD BE A METHOD SO WE CAN UPDATE DELTA AS NEEDED 
int t1 = 0; //time of minClearance 
int t2 = 0; //time of maxClearance 
int armDelta = 0;  // distance  / time 


void setup() {
  //set baud rate for serial monitor
  Serial.begin(9600);
  Serial.println("Multi Servo");

  
  lidServo.attach(lidPin);
  armServo.attach(armPin);
  pinMode(btnPin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  //verify both servos are at 0
  lidServo.write(0);
  armServo.write(0);
}

void loop() {
  btnVal = digitalRead(btnPin);

  if (btnVal == HIGH) {



    while (lidPos > 0 || armPos > 0) {
      armPos = moveNegative(armServo, armPos, delta, 0);
      lidPos = moveNegative(lidServo, lidPos, delta, 1);
      if (digitalRead(btnPin) == LOW){
        break;
      }//btn pressed again 
      //should add peek, SLOW LID CLOSE, pause, and quick switch flip and lid slam here if the switch is hit before the lid finishes closing "fuck you man"

    }  //make sure back to zero
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    //btn is closed - start servo
    if (lidPos < lidMax){

//this is not working right 

      if (lidPos > minClearance && armPos < armMax){
        armDelta = updateArmDelta(delta); 
        armPos = movePositive(armServo, armPos, armDelta, 1);
        lidPos = movePositive(lidServo, lidPos, delta, 1);
      }else {
        lidPos = movePositive(lidServo, lidPos, delta, 1);
      }//if/else for armPos      
      digitalWrite(LED_BUILTIN, HIGH);
    }  //if/else for lidPos
  }    //if/else for btn status
}

/**
* Move Servo Positive
* 
* @param servo The servo to increment
* @param pos An integer, the position variable for the given servo
* @param delta An integer, the value by which to increment the angle of the servo. Max 10 
* @param delayed An integer, determines if delay is included. 0 = no delay; 1 = delay
* @return The updated position
* 
*/
int movePositive(Servo servo, int pos, int delta, int delayed) {
  //TODO - remove when finsihed
  digitalWrite(LED_BUILTIN, HIGH);  //here for testing&prototypes
  pos += delta;
  servo.write(pos);
  if (delayed == 1){
    delay(15);
  }//if delay
  return pos;
}  //movePositive

/**
* Move Servo Negative
* 
* @param servo The servo to decrement
* @param pos An integer, the position variable for the given servo
* @param delta An integer, the value by which to decrement the angle of the servo. Max 10 
* @param delayed An integer, determines if delay is included. 0 = no delay; 1 = delay
* @return The updated position
* 
*/
int moveNegative(Servo servo, int pos, int delta, int delayed) {
  pos -= delta;
  servo.write(pos);
  if (delayed ==1){
    delay(15);
  }//if delay
  return pos;
}  //moveNegative

/**
* Update armDelta
*
* @param delta An integer, the lidServo's current rate of change 
* @return The newly calculated armDelta value
*/
int updateArmDelta(int delta){
  t1 = minClearance / delta; //time of minClearance 
  t2 = maxClearance / delta; //time of maxClearance
  armDelta = armMax / (t2 - t1);  // distance  / time 
  return armDelta; 
}//updateArmDelta


