#include <Servo.h>

Servo lidServo; 
//pins
int btnPin = 3; 
int lidPin = 6;

//values
int servoPos = 0; 
int btnVal = 0;
int delta = 4; //rate of change in servo angle
int fastDelta = 10; //for slamming shut and whipping open
int peekDelta = 1; //super slow for peeking out

void setup() {
  lidServo.attach(lidPin); 
  pinMode(btnPin, INPUT_PULLUP); 
  pinMode(LED_BUILTIN, OUTPUT);
  //verify servoPos is 0
  lidServo.write(0);
}

void loop() {
  btnVal = digitalRead(btnPin); 

  if (btnVal == HIGH){
    while (servoPos != 0){
      servoPos = moveNegative(lidServo, servoPos, delta);
    } //make sure back to zero
    digitalWrite(LED_BUILTIN, LOW); 
  } else {
    //btn is closed - start servo
    if (servoPos < 180){
      servoPos = movePositive(lidServo, servoPos, delta);
    } else {
      digitalWrite(LED_BUILTIN, HIGH);
    }//if/else for servoPos
  }//if/else for btn status
}

/**
* Move Servo Positive
* 
* @param servo The servo to increment
* @param pos An integer, the position variable for the given servo
* @param delta An integer, the value by which to increment the angle of the servo. Max 10
* 
*/
int movePositive(Servo servo, int pos, int delta) {
  //TODO - remove when finsihed
  digitalWrite(LED_BUILTIN, HIGH);  //here for testing&prototypes
  pos += delta;
  servo.write(pos);
  delay(15);
  return pos;
}  //movePositive

/**
* Move Servo Negative
* 
* @param servo The servo to decrement
* @param pos An integer, the position variable for the given servo
* @param delta An integer, the value by which to decrement the angle of the servo. Max 10 
* 
*/
int moveNegative(Servo servo, int pos, int delta) {
  pos -= delta;
  servo.write(pos);
  delay(15);
  return pos; 
}  //moveNegative
