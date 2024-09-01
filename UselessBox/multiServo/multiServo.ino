#include <Servo.h>

Servo lidServo;
Servo armServo;
//pins
const int btnPin = 3;
const int lidPin = 6;
const int armPin = 5;

//CLEARANCES
const int minClearance = 15;  //min window of operation for armServo relative to the lidServo

//values
int lidPos = 0;
int armPos = 0;
int btnVal = 0;
int type = 0;

//lid stuff
const int lidMax = 90;  //max degrees to move the lidServo
int delta = 2.5;        //rate of change in servo angle


//arm stuff
const int armMax = 90;  //max degrees to move the armServo

unsigned long openDuration = 0;
unsigned long closeDuration = 0;


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
}  //setup

void loop() {


  btnVal = digitalRead(btnPin);

  // openDuration = pulseIn(btnPin, HIGH);
  // serialPrint(1, openDuration);
  // closeDuration = pulseIn(btnPin, LOW);
  // serialPrint(2, closeDuration);

  if (btnVal == HIGH) {
    // //MOVE NEGATIVE
    // //check positions
    // lidPos = lidServo.read();
    // armPos = armServo.read();
    // if (lidPos == 0 && armPos == 0) {
    //   digitalWrite(LED_BUILTIN, LOW);
    // } else {
    //   if (armPos == 0 && lidPos > 0) {
    //     type = 1;
    //   } else if (armPos <= (armMax - minClearance) && armPos > 0) {
    //     type = 2;
    //   } else if (armPos > (armMax - minClearance)) {
    //     type = 3;
    //   }  //get type
    //   Serial.println(delta);
    //   moveNegative(lidPos, armPos, delta, type);
    // }  //turn off light
    handleBtnRelease();

  } else {
    //   //MOVE POSITIVE
    //   //check positions
    //   lidPos = lidServo.read();
    //   armPos = armServo.read();
    //   //LID POSITION DRIVES THE ARM POSITION
    //   if (lidPos >= 0 && lidPos < minClearance) {
    //     type = 1;

    //     //lid only
    //     // moveLid(lidPos, minClearance + 1, delta);
    //     // lidPos = lidServo.read();
    //   } else if (lidPos >= minClearance && lidPos < lidMax) {
    //     type = 2;

    //     //both
    //     // moveLid(lidPos, lidMax, delta);
    //     // moveArm(armPos, armMax - minClearance, delta);

    //     // moveBoth(lidPos, lidMax, delta);

    //     // lidPos = lidServo.read();
    //     // armPos = armServo.read();
    //   } else if (lidPos == lidMax && armPos < armMax) {
    //     type = 3;

    //     //arm only
    //     // moveArm(armPos, armMax, delta);
    //     // lidPos = lidServo.read();
    //     // armPos = armServo.read();
    //   } else if (armPos == armMax) {
    //     type = 0;
    //   }  //if else for type

    //   //move arms

    //   Serial.println(delta);
    //   movePositive(lidPos, armPos, delta, type);
      handleBtnPush();
    }  //btn off/on

  }  //loop

  void handleBtnPush() {
    while (digitalRead(btnPin) == LOW) {
      //MOVE POSITIVE
      //check positions
      lidPos = lidServo.read();
      armPos = armServo.read();
      //LID POSITION DRIVES THE ARM POSITION
      if (lidPos >= 0 && lidPos < minClearance) {
        type = 1;
      } else if (lidPos >= minClearance && lidPos < lidMax) {
        type = 2;
      } else if (lidPos == lidMax && armPos < armMax) {
        type = 3;
      } else if (armPos == armMax) {
        type = 0;
      }  //if else for type

      //move arms
      Serial.println(delta);
      movePositive(lidPos, armPos, delta, type);
    }
  }

  void handleBtnRelease() {
    //MOVE NEGATIVE
    //check positions
    while (digitalRead(btnPin) == HIGH) {
      lidPos = lidServo.read();
      armPos = armServo.read();
      if (lidPos == 0 && armPos == 0) {
        digitalWrite(LED_BUILTIN, LOW);
      } else {
        if (armPos == 0 && lidPos > 0) {
          type = 1;
        } else if (armPos <= (armMax - minClearance) && armPos > 0) {
          type = 2;
        } else if (armPos > (armMax - minClearance)) {
          type = 3;
        }  //get type
        Serial.println(delta);
        moveNegative(lidPos, armPos, delta, type);
      }  //turn off light
    }
  }



  /**
* Move Servo Positive
* 
* @param pos An integer, the position variable for the lid servo
* @param armPos An integer, the position variable for the armServo
* @param delta An integer, the value by which to increment the angle of the servo. Max 10 
* @param type An integer representing the type of movement - lid/arm only or both 
* 
*/
  void movePositive(int pos, int armPos, int delta, int type) {
    // unsigned long openStart = 0;
    // unsigned long openStop = 0;



    //TODO - remove when finsihed
    digitalWrite(LED_BUILTIN, HIGH);  //here for testing&prototypes
    //thinking to just have this control arm mvmt depending on the position and "attitude" of the lid mvmt

    switch (type) {
      case 1:
        //lid < minClearance - move lid only
        pos += delta;
        lidServo.write(pos);
        delay(15);
        break;
      case 2:
        pos += delta;
        //maybe have to calculate this delta instead
        armPos += delta;
        // lid > minClearance - move both
        lidServo.write(pos);
        armServo.write(armPos);
        delay(15);
        break;
      case 3:
        // lid @ max, arm < armMax - arm only
        //maybe have to calculate this delta instead


        // //timer
        // openStart = millis();


        armPos += delta;
        armServo.write(armPos);
        delay(15);
        break;
      default:
        // openStop = millis();
        // unsigned long openTime = openStart - openStop;
        // // Serial.println("Open Time: " + openTime);
        // serialPrint(1, openTime);
        break;
    }  //switch stmt for type

  }  //movePositive

  /**
* Move Servo Negative
* 
* @param pos An integer, the position variable for the lid servo
* @param armPos An integer, the position variable for the armServo
* @param delta An integer, the value by which to increment the angle of the servo. Max 10 
* 
*/
  void moveNegative(int pos, int armPos, int delta, int type) {
    // unsigned long closeStart = 0;
    // unsigned long closeStop = 0;




    // pos -= delta;
    // armPos -= delta;
    // lidServo.write(pos);
    // armServo.write(armPos);
    // delay(15);
    switch (type) {
      case 1:
        //lid < minClearance - move lid only
        pos -= delta;
        lidServo.write(pos);
        delay(15);
        break;
      case 2:
        pos -= delta;
        //maybe have to calculate this delta instead
        armPos -= delta;
        // lid > minClearance - move both
        lidServo.write(pos);
        armServo.write(armPos);
        delay(15);
        break;
      case 3:
        // mid @ max, arm < armMax - arm only
        //maybe have to calculate this delta instead

        // //timer
        // closeStart = millis();


        armPos -= delta;
        armServo.write(armPos);
        delay(15);
        break;
      default:
        // closeStop = millis();
        // unsigned long closeTime = closeStart - closeStop;
        // // Serial.println("Close Time: " + closeTime);
        // serialPrint(2, closeTime);
        break;
    }  //switch stmt for type

  }  //movenegative


  void serialPrint(int type, unsigned long value) {
    switch (type) {
      case 1:
        //open
        Serial.println("Open");
        Serial.println(value);
        delay(10);
        break;
      case 2:
        //close
        Serial.println("Close");
        Serial.println(value);
        delay(10);
        break;
    }
  }
