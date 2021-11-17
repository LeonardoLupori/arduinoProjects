// Task callback for sending the current threadmill position
// ---------------------------------------------------
void sendDistance_Clbk() {
  String message = packageMessage("d", millis(), runningPosition);
  Serial.println(message);
}


// Task callback for sending licking events
// ---------------------------------------------------
void sendLick_clbk() {
  if (detectLick()) {
    // First, Sent the lick event to Python through serial
    String message = packageMessage("l", millis(), 0);
    Serial.println(message);
    // Second, check if we are currently in a response window.
    // If we are, deliver the appropriate solution
    if (responseMode == "FC") {
      deliver_ensure();
    } else if (responseMode == "QC"){
      deliver_quinine();
    } else if (responseMode == "AL"){
      deliver_ensure();
    }
    responseMode = "none";      // Resets the response mode until next trial
  }
}


// Function to detect single licking events 
// ---------------------------------------------------
bool detectLick() {
  sensorReading = touchRead(PIN_LICKSENSOR);                  // Read current value
  readingMovingAvg = touchMovingAvg.reading(sensorReading);   // Add it to the running average object and get the current AVG
  if (readingMovingAvg <= THRESHOLD && !touched) {
    touched = true;
    return true;
  } else if (readingMovingAvg > THRESHOLD && touched) {
    touched = false;
    return false;
  } else{
    return false;
  }
}

// Utility function for creating comma separated strings
// to send by serial communication to Python
// ---------------------------------------------------
String packageMessage(String code, unsigned long timeStamp, long value) {
  String messageOne = code + "," + timeStamp + "," + value;
  return messageOne;
}
