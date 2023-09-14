#include <Servo.h>

const double SERVO_OFFSET = 28; //For SN14 2x
//const double SERVO_OFFSET = 32; //For SNXX 20x

Servo filterWheel;

int RedLED = 5;
int OrangeLED = 4;
int GreenLED = 3;
int BlueLED = 2;
int UVLED = 6;
int ArmButton = 7;
int ResetButton = 8;
int ServoPin = 9;
int JumperPin = 10; //This is the jumper for determining whether we are in "Arming mode" or "arrow key mode"
int BlueDyeJumperPin = 11; //This is the jumper for determining whether blue dye mode is "on" or "off".

//For SN14 2x:
double emptyPos = 136 + SERVO_OFFSET;
double bluePos = 117 + SERVO_OFFSET;
double greenPos = 99 + SERVO_OFFSET;
double orangePos = 78 + SERVO_OFFSET;
double redPos = 58 + SERVO_OFFSET;

bool ArmState = false;
bool ResetState = true;

bool RedPrev = false;
bool OrangePrev = false;
bool GreenPrev = false;
bool BluePrev = false;
bool UVPrev = false;

bool PrevArmToggle = false;
bool PrevResetToggle = false;

bool ReadySecondBlue = false;

int number = 0;

int TargetRep = 1; //Changed from 2 to 1 for single image

int RepCounter = 1;

void setup() {
  Serial.begin(9600);
  pinMode(RedLED, INPUT);
  pinMode(OrangeLED, INPUT);
  pinMode(GreenLED, INPUT);
  pinMode(BlueLED, INPUT);
  pinMode(UVLED, INPUT);
  pinMode(ArmButton, INPUT);
  pinMode(ResetButton, INPUT);
  pinMode(JumperPin, INPUT);
  pinMode(BlueDyeJumperPin, INPUT);
  filterWheel.attach(ServoPin, 500, 2500);
  bool ArmState = false;
  bool ResetState = false;
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input pin:
  int RedState = digitalRead(RedLED);
  int OrangeState = digitalRead(OrangeLED);
  int GreenState = digitalRead(GreenLED);
  int BlueState = digitalRead(BlueLED);
  int UVState = digitalRead(UVLED);
  bool ArmToggle = digitalRead(ArmButton);
  bool ResetToggle = digitalRead(ResetButton);
  bool JumperState = digitalRead(JumperPin);
  bool BlueDyeJumperState = digitalRead(BlueDyeJumperPin);
  if (JumperState && !BlueDyeJumperState) {
    if (ArmToggle) {
      ArmState = true;
      ResetState = false;
      filterWheel.write(redPos);
    }
    if (ResetToggle) {
      ArmState = false;
      ResetState = true;
      filterWheel.write(emptyPos);
    }
    if (ResetState) {

    }
    if (ArmState) {
      if (RedState == true && RedPrev == false) {
        //Red turned on this cycle!
        RedPrev = true;
      } else if (RedState == true && RedPrev == true) {
        //Red is currently on, and has been on
      } else if (RedState == false && RedPrev == true && RepCounter != TargetRep) {
        //Red turned off on this cycle, but for the Xth time, which was not the last image to be taken
        RedPrev = false;
        RepCounter++;
      } else if(RedState == false && RedPrev == true && RepCounter == TargetRep){
        //Red turned off on this cycle, for the Xth time in a row!!!
        RedPrev = false;
        RepCounter = 1;
        filterWheel.write(orangePos);
      }else if (RedState == false && RedPrev == false) {
        //Red is off and has been off
        RedPrev = false;
      }

      if (OrangeState == true && OrangePrev == false) {
        //Orange turned on this cycle!
        OrangePrev = true;
      } else if (OrangeState == true && OrangePrev == true) {
        //Orange is currently on, and has been on
      } else if (OrangeState == false && OrangePrev == true && RepCounter != TargetRep) {
        //Orange turned off on this cycle, but for the Xth time, which was not the last image to be taken
        OrangePrev = false;
        RepCounter++;
      } else if (OrangeState == false && OrangePrev == true && RepCounter == TargetRep) {
        //Orange turned off on this cycle, for the Xth time in a row!!!
        OrangePrev = false;
        RepCounter = 1;
        filterWheel.write(greenPos);
      } else if (OrangeState == false && OrangePrev == false) {
        //Orange is off and has been off
        OrangePrev = false;
      }

      if (GreenState == true && GreenPrev == false) {
        //Green turned on this cycle!
        GreenPrev = true;
      } else if (GreenState == true && GreenPrev == true) {
        //Green is currently on, and has been on
      } else if (GreenState == false && GreenPrev == true && RepCounter != TargetRep) {
        //Green turned off on this cycle, but for the Xth time, which was not the last image to be taken
        GreenPrev = false;
        RepCounter++;
      } else if(GreenState == false && GreenPrev == true && RepCounter == TargetRep){
        //Green turned off on this cycle, for the Xth time in a row!!
        GreenPrev = false;
        RepCounter = 1;
        filterWheel.write(bluePos);
      } else if (GreenState == false && GreenPrev == false) {
        //Green is off and has been off
        GreenPrev = false;
      }

      if (BlueState == true && BluePrev == false) {
        //Blue turned on this cycle!
        BluePrev = true;
      } else if (BlueState == true && BluePrev == true) {
        //Blue is currently on, and has been on
      } else if (BlueState == false && BluePrev == true && RepCounter != TargetRep) {
        //Blue turned off on this cycle, but for the Xth time, which was not the last image to be taken
        BluePrev = false;
        RepCounter++;
      } else if(BlueState == false && BluePrev == true && RepCounter == TargetRep){
        //Blue turned off on this cycle, for the Xth time in a row!!
        BluePrev = false;
        RepCounter = 1;
        filterWheel.write(emptyPos);
        delay(6000);
        filterWheel.write(redPos);
      } else if (BlueState == false && BluePrev == false) {
        //Green is off and has been off
        BluePrev = false;
      }
    }
    // print out the state of the button:
    //Serial.println("Red State: " + String(RedState) + " Orange State: " + OrangeState + " Green State: " + GreenState + " Blue State: " + BlueState + " UV State: " + UVState + " ArmState: " + ArmState + " ResetState: " + ResetState);
    delay(1); // delay in between reads for stability

  }

  //BLUEDYEJUMPERSTATEBUSTEDWITHNEWMULTIIMAGELOGICDONOTUSE
  else if (JumperState && BlueDyeJumperState) {
    if (ArmToggle) {
      ArmState = true;
      ResetState = false;
      filterWheel.write(orangePos);
    }
    if (ResetToggle) {
      ArmState = false;
      ResetState = true;
      filterWheel.write(emptyPos);
    }
    if (ResetState) {

    }
    if (ArmState) {
      if (RedState == true && RedPrev == false) {
        //Red turned on this cycle!
        RedPrev = true;
      } else if (RedState == true && RedPrev == true) {
        //Red is currently on, and has been on
      } else if (RedState == false && RedPrev == true) {
        //Red turned off on this cycle!
        //Do stuff
        RedPrev = false;
        //filterWheel.write(orangePos);
      } else if (RedState == false && RedPrev == false) {
        //Red is off and has been off
        RedPrev = false;
      }

      if (OrangeState == true && OrangePrev == false) {
        //Red turned on this cycle!
        OrangePrev = true;
      } else if (OrangeState == true && OrangePrev == true) {
        //Red is currently on, and has been on
      } else if (OrangeState == false && OrangePrev == true) {
        //Red turned off on this cycle!
        //Do stuff
        OrangePrev = false;
        filterWheel.write(greenPos);
      } else if (OrangeState == false && OrangePrev == false) {
        //Red is off and has been off
        OrangePrev = false;
      }

      if (GreenState == true && GreenPrev == false) {
        //Red turned on this cycle!
        GreenPrev = true;
      } else if (GreenState == true && GreenPrev == true) {
        //Red is currently on, and has been on
      } else if (GreenState == false && GreenPrev == true) {
        //Red turned off on this cycle!
        //Do stuff
        GreenPrev = false;
        filterWheel.write(bluePos);
      } else if (GreenState == false && GreenPrev == false) {
        //Red is off and has been off
        GreenPrev = false;
      }

      if (BlueState == true && BluePrev == false && ReadySecondBlue == false) {
        //Red turned on this cycle!
        BluePrev = true;
      } else if (BlueState == true && BluePrev == true && ReadySecondBlue == false) {
        //Red is currently on, and has been on
      } else if (BlueState == false && BluePrev == true && ReadySecondBlue == false) {
        //Red turned off on this cycle!
        //Do stuff
        BluePrev = false;
        filterWheel.write(emptyPos);
        ReadySecondBlue = true;
      } else if (BlueState == false && BluePrev == false && ReadySecondBlue == false) {
        //Red is off and has been off
        BluePrev = false;
      }

      if (BlueState == true && BluePrev == false && ReadySecondBlue == true) {
        //Red turned on this cycle!
        BluePrev = true;
      } else if (BlueState == true && BluePrev == true && ReadySecondBlue == true) {
        //Red is currently on, and has been on
      } else if (BlueState == false && BluePrev == true && ReadySecondBlue == true) {
        //Red turned off on this cycle!
        //Do stuff
        BluePrev = false;
        filterWheel.write(bluePos);
        ReadySecondBlue = false;
        delay(5000);
        filterWheel.write(orangePos);
      } else if (BlueState == false && BluePrev == false && ReadySecondBlue == true) {
        //Red is off and has been off
        BluePrev = false;
      }

    }
    // print out the state of the button:
    Serial.println("Red State: " + String(RedState) + " Orange State: " + OrangeState + " Green State: " + GreenState + " Blue State: " + BlueState + " UV State: " + UVState + " ArmState: " + ArmState + " ResetState: " + ResetState + " ReadySecondBlue: " + String(ReadySecondBlue));
    delay(1); // delay in between reads for stability

  }
  else {
    if (ArmToggle) {
      PrevArmToggle = true;
    }
    if (ArmToggle == false && PrevArmToggle == true) {
      number = number + 1;
      PrevArmToggle = false;
    }
    if (ResetToggle) {
      PrevResetToggle = true;
    }
    if (ResetToggle == false && PrevResetToggle == true) {
      number = number - 1;
      PrevResetToggle = false;
    }

    if (number <= 0) {
      number = 0;
      filterWheel.write(emptyPos);
    }
    if (number == 1) {
      filterWheel.write(bluePos);
    }
    if (number == 2) {
      filterWheel.write(greenPos);
    }
    if (number == 3) {
      filterWheel.write(orangePos);
    }
    if (number == 4) {
      filterWheel.write(redPos);
    }
    if (number >= 4) {
      number = 4;
      filterWheel.write(redPos);
    }
    // print out the state of the button:
    //Serial.println("Red State: " + String(RedState) + " Orange State: " + OrangeState + " Green State: " + GreenState + " Blue State: " + BlueState + " UV State: " + UVState + " ArmState: " + ArmState + " ResetState: " + ResetState);
    delay(1); // delay in between reads for stability
  }
  //Serial.println(String(number));
}