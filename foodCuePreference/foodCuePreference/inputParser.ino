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
    char response = parseMessage(message);
    if (echoing) {
      Serial.println(response);
    }
  }
}


// Function for parsing the serial inputs from Python
// ---------------------------------------------------
char parseMessage(char msg) {
  switch (msg) {
    case 'A':         // Start a AD-LIBITUM TRIAL
      responseMode = "AL";
      return 'A';
    case 'P':         // Start a PAVLOVIAN TRIAL
      return 'P';
    case 'T':         // Start a CONDITIONAL TRIAL
      return 'T';
    case 'Z':         // Response window for FOOC-CUE
      responseMode = "FC";
      return 'Z';
    case 'X':         // Response window for QUININE-CUE
      responseMode = "QC";
      return 'X';
    case 'C':         // Response window for NEUTRAL-CUE
      responseMode = "NC";
      return 'C';
    case 'I':         // Start of an INTERTRIAL
      return 'I';
    case 'R':         // Deliver a single dose of ENSURE
      return 'R';
    case 'V':         // Deliver a single dose of QUININE
      return 'V';
    case 'L':         // Fast FORWARD of the ENSURE syringe pump
      ff_quinine();
      return 'L';
    case 'K':         // Fast BACKWARD of the ENSURE syringe pump
      fb_quinine();
      return 'K';
    case 'M':         // Fast FORWARD of the quinine syringe pump
      ff_quinine();
      return 'M';
    case 'N':         // Fast BACKWARD of the quinine syringe pump
      fb_quinine();
      return 'N';
    default:          // if nothing else matches, do the default
      return msg;
  }
}
