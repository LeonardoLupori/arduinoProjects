//--------------------------------------------------------
// STEPPER MOTOR RELATED FUNCTIONS
//--------------------------------------------------------

// ----------------
void doStep(int pinStep, int stepLength_uS){
  digitalWrite(pinStep, HIGH);
  delayMicroseconds(stepLength_uS);         
  digitalWrite(pinStep, LOW);
  delayMicroseconds(stepLength_uS);   
}

// ----------------
void giveReward(int stepsReward){
  for(int j=0; j<stepsReward; j++){
    doStep(pinStep, stepLenghtMicroseconds);
  }
}

// ----------------
void doTrial(){
  wakeUp();
  changeDirection(forward);
  tone(pinSound, cueFrequency, cueDuration);
  cueTime = millis();
  waitingForReward = true;

  cueTTL_state = true;
  digitalWrite(pinTTL_cue,HIGH);
  lastCueTime = millis();
}

// ----------------
void rewardTrial(){
  if(waitingForReward == true && (millis()-cueTime) > delayCueReward){
    wakeUp();
    changeDirection(forward);
    giveReward(stepsReward);
    waitingForReward = false;
    String msg = millis() + rewardMsg;
    sendToMATLAB(msg);
  }
}

// ----------------
void wakeUp(){
  if(sleepState == true){
    digitalWrite(pinSleep, HIGH);   // exit from sleep
    sleepState = false;
  }
}

// ----------------
void changeDirection(bool direct){
  if(currDirection != direct){
    digitalWrite(pinDir, direct);
    currDirection = direct;
  }
}

// ----------------
void sleepMode(){   
  if(sleepState == false && (millis()- timeSinceLastCommand) > millisToStandby){
    digitalWrite(pinSleep, LOW);    // return to sleep
    sleepState = true;
    timeSinceLastCommand = millis();
  }
}

// ----------------
void cue(){
  tone(pinSound, cueFrequency, cueDuration);
}

//--------------------------------------------------------
// STEPPER MOTOR RELATED FUNCTIONS
//--------------------------------------------------------

void ttlManager(){
  if(cueTTL_state && (millis()-lastCueTime) > lickTttlLength){
    digitalWrite(pinTTL_cue, LOW);
    cueTTL_state = false;
  }

  if(onlyCueTTL_state && (millis()-lastOnlyCueTime) > lickTttlLength){
    digitalWrite(pinTTL_onlycue, LOW);
    onlyCueTTL_state = false;
  }
}

//--------------------------------------------------------
// SERIAL COMMUNICATIONS
//--------------------------------------------------------

// ----------------
int recieveFromMATLAB(){
  if(Serial.available()>0){

    if(waitingForReward == true){
      Serial.read();
    }
    
    String msg = Serial.readStringUntil('\n');
    if(msg == "handshakeFromMATLAB"){
      sendToMATLAB("handshakeFromArduino");
    }
    else if(msg=="forward"){
      wakeUp();
      changeDirection(forward);
      for(int i=0; i<300; i++){
        doStep(pinStep,stepLenghtMicroseconds);
      }
      timeSinceLastCommand = millis();
      sendToMATLAB("1");
    }
    else if(msg=="reverse"){
      wakeUp();
      changeDirection(reverse);
      for(int i=0; i<300; i++){
        doStep(pinStep,stepLenghtMicroseconds);
      }
      timeSinceLastCommand = millis();
      sendToMATLAB("1");
    }
    else if(msg=="trial"){
      doTrial();
      String msg = millis() + cueMsg;
      sendToMATLAB(msg);
    }
    else if(msg=="onlyCue"){
      cue();
      onlyCueTTL_state = true;
      digitalWrite(pinTTL_onlycue,HIGH);
      lastOnlyCueTime = millis();
      String msg = millis() + onlyCueMsg;
      sendToMATLAB(msg);
    }
    else if(msg=="cue"){
      cue();
      String msg = millis() + cueMsg;
      sendToMATLAB(msg);
    }
    
    else if(msg=="trigger"){
      String trigMsg = "_trigger";
      String msg = millis() + trigMsg;
      sendToMATLAB(msg);
    }
    
    else if(msg=="reward"){
      wakeUp();
      changeDirection(forward);
      giveReward(stepsReward);
      String msg = millis() + rewardMsg;
      sendToMATLAB(msg);

    }
  }
}
  
// ----------------
void sendToMATLAB(String msg){
  Serial.print(msg + '\n');
}
