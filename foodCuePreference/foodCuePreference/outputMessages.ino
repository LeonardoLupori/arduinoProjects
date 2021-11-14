// Task callback for sending the current threadmill position
void sendSpeed_Clbk(){
  Serial.println(millis());
}




// Task callback for sending licking events
void sendLick_clbk(){
  // check for touch sensor, if there's a lick send it  
  Serial.println('l');


  sensorReading = touchRead(T9);
  readingMovingAvg = mySensor.reading(sensorReading);
  if(readingMovingAvg<= THRESHOLD && !touched) {
    String message = String("TOUCH") + millis();
    Serial.println(message);
    touched = true;
  } else if(readingMovingAvg > THRESHOLD){
    touched = false;
  }
}
