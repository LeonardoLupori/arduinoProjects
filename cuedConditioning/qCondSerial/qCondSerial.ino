#include <Bounce2.h>
#include <CapacitiveSensor.h>

// Experimental Constants
const int stepsReward = 12;
const int stepLenghtMicroseconds = 1000;
const int cueFrequency = 4000;
const int cueDuration = 500;
const int delayCueReward = 2000;

// PIN definitions
const uint8_t pinCapacSend = 5;    // Controls the capacitive lick sensor
const uint8_t pinCapacRcv = 8;     // Controls the capacitive lick sensor

const uint8_t pinOutLick = 6;     // Controls the capacitive lick sensor
const uint8_t pinOutCue = 7;     // Controls the capacitive lick sensor

const uint8_t pinSound = 13;     // Controls the buzzer
const uint8_t pinStep = 2;      // Controls each step
const uint8_t pinDir = 3;       // Controls the direction of the rotation
const uint8_t pinSleep = 4;     // Sleep function of the stepper
uint8_t pinButtonsUI[] = {9,10,11,12};    // UI buttons

// -------------------------------------------------------
CapacitiveSensor   cSensor = CapacitiveSensor(pinCapacSend,pinCapacRcv);

bool sleepState = true;
const int millisToStandby = 1000;     // The stepper driver goes to sleep (drastically reduces power)
unsigned long timeSinceLastCommand = 0;

long lickReading = 0;
bool lickState = false;
const int lickTttlLength = 2;
long lastLick = 0;

bool currDirection = false;
const bool forward = false;   // renaming for clarity
const bool reverse = true;    // renaming for clarity

// Initialize the debouncing for the UI buttons
int NUM_BUTTONS = sizeof(pinButtonsUI) / sizeof(pinButtonsUI[0]);
Bounce * buttons = new Bounce[NUM_BUTTONS];

String lickMsg = "_lick";
String rewardMsg = "_reward";
String cueMsg = "_cue";
String onlyCueMsg = "_onlyCue";

long cueTime = 0;
bool waitingForReward = false;

//--------------------------------------------------------
// SETUP
//--------------------------------------------------------
void setup() {
  
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

  // UI Buttons
  for(int i=0; i<NUM_BUTTONS; i++){
    buttons[i].attach(pinButtonsUI[i] , INPUT_PULLUP);
    buttons[i].interval(15); 
  }
  
  Serial.begin(9600);
  timeSinceLastCommand = millis();       // update the time
}

  

//--------------------------------------------------------
// LOOP
//--------------------------------------------------------
void loop() {
  
  rewardTrial();

  //----------------------------------------------------
  // SERIAL INPUTS FROM MATLAB
  //----------------------------------------------------
  recieveFromMATLAB();
  
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
      //Serial.println(msg);
      //Serial.write(13);
      //Serial.write(10);
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
  // BUTTONS
  //----------------------------------------------------

  // UPDATE ALL
  for (int i = 0; i < NUM_BUTTONS; i++)  {
    buttons[i].update();      // Update the Bounce instance
  }
  // Read REWARD button
  if(buttons[0].fell()){
    wakeUp();
    changeDirection(forward);
    tone(pinSound, cueFrequency, cueDuration);
    delay(delayCueReward);
    giveReward(stepsReward);
    timeSinceLastCommand = millis();
  }
  // Read FORWARD button
  if(buttons[1].fell()){
    wakeUp();
    changeDirection(forward);
    while(buttons[1].read()==LOW){
      doStep(pinStep,stepLenghtMicroseconds);
      buttons[1].update();
    }
    timeSinceLastCommand = millis();
  }
  // Read REVERSE button
  if(buttons[2].fell()){
    wakeUp();
    changeDirection(reverse);
    while(buttons[2].read()==LOW){
      doStep(pinStep,stepLenghtMicroseconds);
      buttons[2].update();
    }
    timeSinceLastCommand = millis();
  }
  // Read TRIAL button
  if(buttons[3].fell()){
    wakeUp();
    tone(pinSound, cueFrequency, cueDuration);
    timeSinceLastCommand = millis();
  }

  //----------------------------------------------------
  // SLEEPMODE
  // Puts the driver to sleep if some time has passed since the last command
  //----------------------------------------------------
  sleepMode();
}
