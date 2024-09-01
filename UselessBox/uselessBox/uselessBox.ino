#include <Servo.h>

//////////////////////////// typedefs 
struct LidStruct {
	Servo servo;
	int pin; 
	int lidMax; 
	int position; 
} lidStruct; 

struct ArmStruct {
	Servo servo;
	int pin; 
	int armMax; 
	int position; 
} armStruct; 

struct Timer {
  bool earlyFlip; // true if the user flipped the switch before cycle was completed
  int earlyCounter; // counts the number of times a user flips the switch early 
  int maxEarly; // number of early flips before the monster gets annoyed - when earlyCount == maxEarly, trigger annoyed sequence
  bool isAnnoyed; // true if the annoyed sequence is active, false if normal
  unsigned long annoyedStart; // time the annoyed sequence started
  unsigned long annoyedStop; // time the annoyed sequence gets the second switch flip
} timer;

const int btnPin = 3;
int btnVal = 0;

const unsigned long timeLimit = 10000; // time limit for reverting back to normal function from annoyed sequence

int delta = 2;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  //lid stuff
	lidStruct.lidMax = 90; 
	lidStruct.position = 0; 
	lidStruct.pin = 6;
	lidStruct.servo.attach(lidStruct.pin);
  lidStruct.servo.write(lidStruct.position);

  //arm stuff
  armStruct.armMax = 90; 
	armStruct.position = 0; 
	armStruct.pin = 5;
	armStruct.servo.attach(armStruct.pin);
  armStruct.servo.write(armStruct.position);

  //btn input
  pinMode(btnPin, INPUT_PULLUP);

  //LED as output
  pinMode (LED_BUILTIN, OUTPUT);

  //timer stuff
  timer.earlyFlip = false; 
  timer.earlyCounter = 0; 
  timer.maxEarly = 4; //annoyed on the 4th early flip
  timer.isAnnoyed = false;
  timer.annoyedStart = 0; 
  timer.annoyedStop = 0; 
}

void loop() {
  //get button status 
  btnVal = digitalRead(btnPin); 

  if (btnVal == HIGH) {
    //move negative - btn release // arm flipped switch off

    //if position is not max, increase early counter
    // if already annoyed, reset the annoyed & timer vals and use the slam open/close response
    // if early counter is at max, trigger annoyed sequence and start timer
    // otherwise, do a standard response

  } else {
    //move positive - btn press // user flipped switch on

    //if position is not min, increase early counter
    // if already annoyed, reset the annoyed & timer vals and use the slam open/close response
    // if early counter is at max, trigger annoyed sequence and start timer
    // otherwise, do a standard response

  }

}//loop


