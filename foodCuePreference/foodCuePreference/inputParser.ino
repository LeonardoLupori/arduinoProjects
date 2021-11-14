/*  INPUT PARSER
 *  Series of functions for parsing the serial messages that Python sends
 *  to the Arduino
 */


void checkForSerialInputs(){
  if(Serial.available() > 0){
    char message = Serial.read();
    parseMessage(message);
  }
}


// --------------------------------------------------------------
// Function for PARSING the serial inputs from Python
// --------------------------------------------------------------

void parseMessage(char msg){
  switch (msg) {
  case 'T':         // Start a new CONDITIONAL REWARD TRIAL
    trial_condReward();
    break;
  case 'P':         // Start a new PAVLOVIAN TRIAL
    break;
  case 'R':         // Deliver REWARD once
    break;
  case 'Q':         // Deliver AVERSIVE (quinine) once
    break;
  case 'L':         // Fast FORWARD of the syringe pump
    break;
  case 'J':         // Fast BACKWARD of the syringe pump
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
