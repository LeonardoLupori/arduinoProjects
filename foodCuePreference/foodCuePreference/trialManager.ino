// AD-LIBITUM TRIAL
// ---------------------------------------------------
void trial_adLibitum() {
  trialStartTime = millis();
  responseMode = "AL";
  Serial.println(packageMessage("a", millis(), 0));
}

// CONDITIONAL REWARD TRIAL
// ---------------------------------------------------
void trial_condReward() {
  trialStartTime = millis();
  responseMode = "none";
  Serial.println(packageMessage("t", millis(), 0));
}

// PAVLOVIAN TRIAL
// ---------------------------------------------------
void trial_pavlovian() {
  trialStartTime = millis();
  responseMode = "none";
  Serial.println(packageMessage("p", millis(), 0));
}

// DELIVER ENSURE
// ---------------------------------------------------
void deliver_ensure() {
  mustDeliverEnsure = true;
}

// DELIVER QUININE
// ---------------------------------------------------
void deliver_quinine() {
  mustDeliverQuinine = true;
}

// FAST FORWARD ENSURE
// ---------------------------------------------------
void ff_ensure() {
  for (int j = 0; j < NUM_STEP_FAST_DELIVERY; j++) {
    doStep(PIN_STEP_ENSURE, STEP_LENGHT_MICROSECONDS);
  }
}

// FAST FORWARD QUININE
// ---------------------------------------------------
void ff_quinine() {
  for (int j = 0; j < NUM_STEP_FAST_DELIVERY; j++) {
    doStep(PIN_STEP_QUININE, STEP_LENGHT_MICROSECONDS);
  }
}

// FAST BACKWARD ENSURE
// ---------------------------------------------------
void fb_ensure() {
  digitalWrite(PIN_DIR_ENSURE, LOW);        // Switch the stepper direction to go backwards
  for (int j = 0; j < NUM_STEP_FAST_DELIVERY; j++) {
    doStep(PIN_STEP_ENSURE, STEP_LENGHT_MICROSECONDS);
  }
  digitalWrite(PIN_DIR_ENSURE, HIGH);       // Switch back to forward the stepper direction
}

// FAST BACKWARD QUININE
// ---------------------------------------------------
void fb_quinine() {
  digitalWrite(PIN_DIR_QUININE, LOW);        // Switch the stepper direction to go backwards
  for (int j = 0; j < NUM_STEP_FAST_DELIVERY; j++) {
    doStep(PIN_STEP_QUININE, STEP_LENGHT_MICROSECONDS);
  }
  digitalWrite(PIN_DIR_QUININE, HIGH);       // Switch back to forward the stepper direction
}



// STEPPER MOTORS MANAGER
// ---------------------------------------------------
/* This function is the main manager of stepper motors.
 * It runs on a parallel processor so that the delay in every step
 * does not interfere with the rest of the microcontroller's tasks.
 * It checks two global variables: mustDeliverEnsure and mustDeliverQuinine
 * and, in caseone of them is true, it delivers the appropriate liquid.
 */
void stepperManager(void * pvParameters) {
  for (;;) {
    if (mustDeliverEnsure) {
      Serial.println(packageMessage("e", millis(), 0));
      giveEnsure();
      mustDeliverEnsure = false;
    } else if (mustDeliverQuinine) {
      Serial.println(packageMessage("q", millis(), 0));
      giveQuinine();
      mustDeliverQuinine = false;
    }
    delay(10);
  }
  vTaskDelete( NULL );
}

// PERFORMS A SINGLE STEP
// ---------------------------------------------------
void doStep(int pinStep, int stepLength_uS) {
  digitalWrite(pinStep, HIGH);
  delayMicroseconds(stepLength_uS);
  digitalWrite(pinStep, LOW);
  delayMicroseconds(stepLength_uS);
}

// DELIVERS A PORTION OF ENSURE
// ---------------------------------------------------
void giveEnsure() {
  for (int j = 0; j < NUM_STEP_DELIVERY; j++) {
    doStep(PIN_STEP_ENSURE, STEP_LENGHT_MICROSECONDS);
  }
}

// DELIVERS A PORTION OF QUININE
// ---------------------------------------------------
void giveQuinine() {
  for (int j = 0; j < NUM_STEP_DELIVERY; j++) {
    doStep(PIN_STEP_QUININE, STEP_LENGHT_MICROSECONDS);
  }
}
