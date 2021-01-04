//--------------------------------------------------------
// HARDWARE FUNCTIONS
//--------------------------------------------------------

void doStep(int pinStep, int stepLength_uS){
  digitalWrite(pinStep, HIGH);
  delayMicroseconds(stepLength_uS);         
  digitalWrite(pinStep, LOW);
  delayMicroseconds(stepLength_uS);   
}


void giveReward(int stepsReward){
  for(int j=0; j<stepsReward; j++){
    doStep(pinStep, stepLenghtMicroseconds);
  }
}

void doTrial(){
  wakeUp();
  changeDirection(forward);
  tone(pinSound, cueFrequency, cueDuration);
  cueTime = millis();
  waitingForReward = true;
}

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

void wakeUp(){
  if(sleepState == true){
    digitalWrite(pinSleep, HIGH);   // exit from sleep
    sleepState = false;
  }
}

void changeDirection(bool direct){
  if(currDirection != direct){
    digitalWrite(pinDir, direct);
    currDirection = direct;
  }
}

void sleepMode(){   
  if(sleepState == false && (millis()- timeSinceLastCommand) > millisToStandby){
    digitalWrite(pinSleep, LOW);    // return to sleep
    sleepState = true;
    timeSinceLastCommand = millis();
  }
}

void debugBlink(int cycles, int del){
  for(int i=0; i<cycles; i++){
    digitalWrite(13,HIGH);
    delay(del);
    digitalWrite(13,LOW);
    delay(del);
  }
}

//--------------------------------------------------------
// SERIAL COMMUNICATIONS
//--------------------------------------------------------

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
      sendToMATLAB("1");
      delay(3);
      String msg = millis() + cueMsg;
      sendToMATLAB(msg);
    }
    else if(msg=="onlyCue"){
      tone(pinSound, cueFrequency, cueDuration);
      sendToMATLAB("1");
      delay(3);
      String msg = millis() + onlyCueMsg;
      sendToMATLAB(msg);
    }
    else if(msg=="reward"){
      wakeUp();
      changeDirection(forward);
      giveReward(stepsReward);
      sendToMATLAB("1");
      delay(3);
      String msg = millis() + rewardMsg;
      sendToMATLAB(msg);
    }
  }
}
  

void sendToMATLAB(String msg){
  Serial.print(msg + '\n');
}
