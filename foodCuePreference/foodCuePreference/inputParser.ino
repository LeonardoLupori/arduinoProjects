/*  INPUT PARSER
 *  Series of functions for parsing the serial messages that Python sends
 *  to the Arduino
 */


char checkForSerialInputs(bool echoing){
  if(Serial.available() > 0){
    char message = Serial.read();
    response = parseMessage(message);
    if(echoing){
      return response;
    }
  }
}


// --------------------------------------------------------------
// Function for PARSING the serial inputs from Python
// --------------------------------------------------------------

char parseMessage(char msg){
  switch (msg) {
  case 'T':         // Start a new CONDITIONAL REWARD TRIAL
    trial_condReward();
    return 'T';
    break;
  case 'P':         // Start a new PAVLOVIAN TRIAL
    return 'P';
    break;
  case 'R':         // Deliver REWARD once
    return 'R';
    break;
  case 'Q':         // Deliver AVERSIVE (quinine) once
    return 'Q';
    break;
  case 'L':         // Fast FORWARD of the syringe pump
    return 'L';
    break;
  case 'J':         // Fast BACKWARD of the syringe pump
    return 'J';
    break;
  default:          // if nothing else matches, do the default
    break;
  }
}

// --------------------------------------------------------------
// Beginning of a new CONDITIONAL REWARD TRIAL
// --------------------------------------------------------------
void trial_condReward(){
  Serial.println("recieved.");
}
