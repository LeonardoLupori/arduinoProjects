/*  INPUT PARSER
    Series of functions for parsing the serial messages that Python sends
    to the Arduino
*/

// Main function for recieving serial communication from Python.
// The echoing argument determines whether or not the arduino sends
// back the messages it recieves (for debugging purposes)
// ---------------------------------------------------
void checkForSerialInputs(bool echoing) {
  if (Serial.available() > 0) {
    char message = Serial.read();
    char answerBack = parseMessage(message);
    if (echoing) {
      Serial.println(answerBack);
    }
  }
}


// Function for parsing the serial inputs from Python
// ---------------------------------------------------
char parseMessage(char msg) {
  switch (msg) {
    case 'A':         // Start a AD-LIBITUM TRIAL
      trial_adLibitum();
      break;
    case 'P':         // Start a PAVLOVIAN TRIAL
      trial_pavlovian();
      break;
    case 'T':         // Start a CONDITIONAL TRIAL
      trial_condReward();
      break;
    case 'Z':         // Response window for FOOC-CUE
      responseMode = "FC";
      break;
    case 'X':         // Response window for QUININE-CUE
      responseMode = "QC";
      break;
    case 'C':         // Response window for NEUTRAL-CUE
      responseMode = "NC";
      break;
    case 'I':         // Start of an INTERTRIAL
      responseMode = "none";
      break;
    case 'R':         // Deliver a single dose of ENSURE
      deliver_ensure();
      break;
    case 'V':         // Deliver a single dose of QUININE
      deliver_quinine();
      break;
    case 'L':         // Fast FORWARD of the ENSURE syringe pump
      ff_quinine();
      break;
    case 'K':         // Fast BACKWARD of the ENSURE syringe pump
      fb_quinine();
      break;
    case 'M':         // Fast FORWARD of the quinine syringe pump
      ff_quinine();
      break;
    case 'N':         // Fast BACKWARD of the quinine syringe pump
      fb_quinine();
      break;
    default:          // if nothing else matches, do the default
      break;
  }
  return msg;
}
