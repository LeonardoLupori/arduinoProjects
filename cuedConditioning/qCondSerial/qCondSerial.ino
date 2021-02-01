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
const uint8_t pinCapacSend = 12;    // Controls the capacitive lick sensor
const uint8_t pinCapacRcv = 10;     // Controls the capacitive lick sensor

const uint8_t pinOutLick = 6;     // Controls the capacitive lick sensor
const uint8_t pinOutCue = 7;     // Controls the capacitive lick sensor

const uint8_t pinSound = 14;     // Controls the buzzer
const uint8_t pinStep = 33;      // Controls each step
const uint8_t pinDir = 34;       // Controls the direction of the rotation
const uint8_t pinSleep = 36;     // Sleep function of the stepper

const uint8_t pinTTL_cue = 17;
const uint8_t pinTTL_onlycue = 18;
const uint8_t pinTTL_lick = 19;

// -------------------------------------------------------
// INTERNAL VARIABLES
// -------------------------------------------------------
CapacitiveSensor   cSensor = CapacitiveSensor(pinCapacSend,pinCapacRcv);

bool sleepState = true;
const int millisToStandby = 1000;       // The stepper driver goes to sleep (drastically reduces power)
unsigned long timeSinceLastCommand = 0;

// Lick sensor
// -------------------------------------------------------
long lickReading = 0;
bool lickState = false;
const int lickTttlLength = 200;
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
  pinMode(pinTTL_cue,OUTPUT);
  pinMode(pinTTL_onlycue,OUTPUT);

  // CAPACITIVE sensor
  cSensor.set_CS_Timeout_Millis(1000);

  pinMode(pinOutLick,OUTPUT);
  pinMode(pinOutCue,OUTPUT);
  digitalWrite(pinOutLick, LOW); 
  digitalWrite(pinOutCue, LOW); 
  
  // MOTOR pins               
  pinMode(pinStep, OUTPUT);
  pinMode(pinDir, OUTPUT);
  pinMode(pinSleep, OUTPUT);
  
  digitalWrite(pinStep, LOW);   
  digitalWrite(pinSleep, LOW);  // put driver to sleep
  sleepState = true;
  digitalWrite(pinDir, forward); 
  
  Serial.begin(9600);
  timeSinceLastCommand = millis();       // update the time
}

  

//--------------------------------------------------------
// LOOP
//--------------------------------------------------------
void loop() {
  rewardTrial();          // If necessary gives a reward after the cue delay
  
  recieveFromMATLAB();    // Check for serial inputs from MATLAB
  
  ttlManager();
  
  //----------------------------------------------------
  // LICKING
  //----------------------------------------------------
  // Lick sensor update and TTL output
  if(lickState==false && millis()-lastLick > lickTttlLength){
    lickReading =  cSensor.capacitiveSensor(10);
    if(lickReading>2000){
      lickState = true;
      lastLick = millis();
      digitalWrite(pinOutLick,HIGH);
      String msg = lastLick + lickMsg;
      sendToMATLAB(msg); 
    }  
  } 
  else if(lickState==true && millis()-lastLick > lickTttlLength){
    lickReading =  cSensor.capacitiveSensor(10
    );
    if(lickReading<2000){
      lickState = false;
      digitalWrite(pinOutLick,LOW);
    }
  }

  //----------------------------------------------------
  // SLEEPMODE
  // Puts the driver to sleep if some time has passed since the last command
  //----------------------------------------------------
  sleepMode();
}
