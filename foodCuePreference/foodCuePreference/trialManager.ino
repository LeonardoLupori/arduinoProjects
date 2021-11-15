// MAIN TRIAL MANAGER FUNCTION
// ---------------------------------------------------
void trialManager(){
  if(trialRunning == false){
    return;
  }

  
}





// CONDITIONAL REWARD TRIAL
// ---------------------------------------------------
void trial_condReward(){
  trialRunning = true;            // Set global variable
  trialStartTime = millis();
  Serial.println("Conditional reward trial.");
}

// PAVLOVIAN TRIAL
// ---------------------------------------------------
void trial_pavlovian(){
  trialRunning = true;            // Set global variable
  trialStartTime = millis();
  Serial.println("Pavlovian trial.");
}

// DELIVER ENSURE
// ---------------------------------------------------
void deliver_ensure(){
  Serial.println("Ensure delivery.");
}

// DELIVER QUININE
// ---------------------------------------------------
void deliver_quinine(){
  Serial.println("Quinine delivery.");
}

// FAST FORWARD ENSURE
// ---------------------------------------------------
void ff_ensure(){
  Serial.println("Fast-forward Ensure.");
}

// FAST FORWARD QUININE
// ---------------------------------------------------
void ff_quinine(){
  Serial.println("Fast-forward Quinine.");
}

// FAST BACKWARD ENSURE
// ---------------------------------------------------
void fb_ensure(){
  Serial.println("Fast-backward Ensure.");
}

// FAST BACKWARD QUININE
// ---------------------------------------------------
void fb_quinine(){
  Serial.println("Fast-backward Quinine.");
}
