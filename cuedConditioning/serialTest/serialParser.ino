void serialParser(){
  if(Serial.available()>0){
    String msg = Serial.readStringUntil('\n');
    // Connection MATLAB<-->Arduino from the GUI
    // ---------------------------------------------
    if(msg == "handshakeFromMATLAB"){
      sendToMATLAB("handshakeFromArduino");
    }
    // 
    // ---------------------------------------------
    else if(msg == "forward"){
      for(int i=0; i<3; i++){
        digitalWrite(13,HIGH);
        delay(200);
        digitalWrite(13,LOW);
        delay(200);
      }
      sendToMATLAB("1");
      
    }
    //
    // ---------------------------------------------
    else if(msg == "reverse"){
      for(int i=0; i<2; i++){
        digitalWrite(13,HIGH);
        delay(200);
        digitalWrite(13,LOW);
        delay(200);
      }
      sendToMATLAB("melo");
    }
    else{
      sendToMATLAB(msg);
    }
  }
}


void sendToMATLAB(String msg){
  Serial.print(msg + '\n');
}
