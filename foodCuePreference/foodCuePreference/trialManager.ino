// MAIN TRIAL MANAGER FUNCTION
// ---------------------------------------------------
void trialManager() {
  if (trialRunning == false) {
    return;
  }


}





// CONDITIONAL REWARD TRIAL
// ---------------------------------------------------
void trial_condReward() {
  trialRunning = true;            // Set global variable
  trialStartTime = millis();
  Serial.println("Conditional reward trial.");
}

// PAVLOVIAN TRIAL
// ---------------------------------------------------
void trial_pavlovian() {
  trialRunning = true;            // Set global variable
  trialStartTime = millis();
  Serial.println("Pavlovian trial.");
}

// DELIVER ENSURE
// ---------------------------------------------------
void deliver_ensure() {
  mustDeliverEnsure = true;
  Serial.println("Ensure delivery.");
}

// DELIVER QUININE
// ---------------------------------------------------
void deliver_quinine() {
  Serial.println("Quinine delivery.");
}

// FAST FORWARD ENSURE
// ---------------------------------------------------
void ff_ensure() {
  Serial.println("Fast-forward Ensure.");
}

// FAST FORWARD QUININE
// ---------------------------------------------------
void ff_quinine() {
  Serial.println("Fast-forward Quinine.");
}

// FAST BACKWARD ENSURE
// ---------------------------------------------------
void fb_ensure() {
  Serial.println("Fast-backward Ensure.");
}

// FAST BACKWARD QUININE
// ---------------------------------------------------
void fb_quinine() {
  Serial.println("Fast-backward Quinine.");
}


void stepperManager(void * pvParameters) {
  for (;;) {
    if (mustDeliverEnsure) {
      giveEnsure(NUM_STEPS);
      mustDeliverEnsure = false;
    } else if (mustDeliverQuinine) {
      giveQuinine(NUM_STEPS);
      mustDeliverQuinine = false;
    }
    delay(10);
  }
  vTaskDelete( NULL );
}


// ----------------
void doStep(int pinStep, int stepLength_uS) {
  digitalWrite(pinStep, HIGH);
  delayMicroseconds(stepLength_uS);
  digitalWrite(pinStep, LOW);
  delayMicroseconds(stepLength_uS);
}

// ----------------
void giveEnsure(int stepsReward) {
  for (int j = 0; j < stepsReward; j++) {
    doStep(PIN_STEP_ENSURE, STEP_LENGHT_MICROSECONDS);
  }
}

// ----------------
void giveQuinine(int stepsReward) {
  for (int j = 0; j < stepsReward; j++) {
    doStep(PIN_STEP_QUININE, STEP_LENGHT_MICROSECONDS);
  }
}
