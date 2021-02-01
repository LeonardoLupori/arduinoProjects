#include <Bounce2.h>
#include <CapacitiveSensor.h>

// Experimental Constants
// -------------------------------------------------------
const int stepsReward = 12;
const int stepLenghtMicroseconds = 1000;
const int cueFrequency = 4000;
const int cueDuration = 500;
const int delayCueReward = 2000;

// PIN definitions
// -------------------------------------------------------
const uint8_t pinCapacSend = 10;    // Capacitive lick sensor (10M resistor between pins)
const uint8_t pinCapacRcv = 8;      // Capacitive lick sensor (connect sensor to this pin)

const uint8_t pinSound = 14;      // Controls the buzzer
const uint8_t pinStep = 33;       // Controls each step
const uint8_t pinDir = 34;        // Controls the direction of the rotation
const uint8_t pinSleep = 36;      // Sleep function of the stepper

const uint8_t pinTTL_cue = 17;
const uint8_t pinTTL_onlycue = 18;
const uint8_t pinTTL_lick = 19;

// -------------------------------------------------------
// INTERNAL VARIABLES
// -------------------------------------------------------
CapacitiveSensor   cSensor = CapacitiveSensor(pinCapacSend,pinCapacRcv);
// There is a bug for TEENSY 3.5 boards in CapacitiveSensor.cpp
// FIVE_VOLT_TOLERANCE_WORKAROUND was not defined, and only the #else code is getting run.
// The workaround is to add the following code to the top of CapacitiveSensor.cpp 
// (just below #include "CapacitiveSensor.h"):
//
//  #if defined(__MK64FX512__) // Teensy 3.5
//  #define FIVE_VOLT_TOLERANCE_WORKAROUND 1
//  #endif
//

// Stepper Sleep Mode
// -------------------------------------------------------
bool sleepState = true;
const int millisToStandby = 1000;       // The stepper driver goes to sleep (drastically reduces power)
unsigned long timeSinceLastCommand = 0;

// Lick sensor
// -------------------------------------------------------
const int lickTttlLength = 5;         // width in milliseconds of all the TTL signals
long lickReading = 0;
bool lickState = false;
long lastLick = 0;

// Stepper Motor
// -------------------------------------------------------
bool currDirection = false;
const bool forward = false;   // renaming for clarity
const bool reverse = true;    // renaming for clarity

// Serial Comms
// -------------------------------------------------------
String lickMsg = "_lick";
String rewardMsg = "_reward";
String cueMsg = "_cue";
String onlyCueMsg = "_onlyCue";

long cueTime = 0;
bool waitingForReward = false;

bool cueTTL_state = false ; 
bool onlyCueTTL_state = false;
long lastCueTime = 0;
long lastOnlyCueTime = 0;

//--------------------------------------------------------
// SETUP
//--------------------------------------------------------
void setup() {
  pinMode(pinTTL_lick,OUTPUT);
  pinMode(pinTTL_cue,OUTPUT);
  pinMode(pinTTL_onlycue,OUTPUT);
  digitalWrite(pinTTL_lick, LOW); 
  digitalWrite(pinTTL_cue, LOW); 
  digitalWrite(pinTTL_onlycue, LOW); 

  // CAPACITIVE sensor
  cSensor.set_CS_Timeout_Millis(1000);
  
  // MOTOR pins               
  pinMode(pinStep, OUTPUT);
  pinMode(pinDir, OUTPUT);
  pinMode(pinSleep, OUTPUT);
  
  digitalWrite(pinStep, LOW);   
  digitalWrite(pinSleep, LOW);  // put driver to sleep
  sleepState = true;
  digitalWrite(pinDir, forward); 
  
  Serial.begin(250000);
  timeSinceLastCommand = millis();       // update the time
}

//--------------------------------------------------------
// LOOP
//--------------------------------------------------------
void loop() {
  
  rewardTrial();          // If necessary gives a reward after the cue delay
  recieveFromMATLAB();    // Check for serial inputs from MATLAB
  ttlManager();           // Manages to switch on and off TTL signals
  
  //----------------------------------------------------
  // LICKING
  //----------------------------------------------------
  // Lick sensor update and TTL output
  if(lickState==false && millis()-lastLick > lickTttlLength){
    lickReading =  cSensor.capacitiveSensor(10);

    if(lickReading>40000){
      lickState = true;
      lastLick = millis();
      digitalWrite(pinTTL_lick,HIGH);
      String msg = lastLick + lickMsg;
      sendToMATLAB(msg); 
    }  
  } 
  else if(lickState==true && millis()-lastLick > lickTttlLength){
    lickReading =  cSensor.capacitiveSensor(10);
    if(lickReading<40000){
      lickState = false;
      digitalWrite(pinTTL_lick,LOW);
    }
  }

  // Puts the driver to sleep if some time has passed since the last command
  //----------------------------------------------------
  sleepMode();
}
