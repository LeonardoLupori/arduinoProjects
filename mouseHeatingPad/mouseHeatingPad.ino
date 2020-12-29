#include <TCN75A.h>  // library for the temperature sensor

// USER PARAMETERS
uint8_t limitTemp = 41;

// SCRIPT VARIABLES
bool belowSetTemp = 0;                // whether the current temp is below the limit temperature
bool tempSensorWorking = 0;           // whether the temperature sensor is working or not
uint8_t alarmPin = 3;                 // PIN attached to the interrupt pin of the temp. sensor
uint8_t heatingPadPin = 6;            // PIN that controls the heating pad
uint8_t heatingLED = 2;               // PIN that controls the red LED showing when the heating pad is working
uint8_t alarmMode = 0;
uint8_t histeresisTemp = limitTemp-1; // temperature at which the heating pad is switched on
TCN75A tempSensor;

void setup() {
    pinMode(heatingLED,OUTPUT);
    pinMode(heatingPadPin,OUTPUT);
    pinMode(alarmPin,INPUT);
    Serial.begin(9600);
    // Initialize and setup the temperature sensor
    tempSensor.begin();                         // Initialization
    tempSensor.set_address(0);                  // Set the memory address to read
    tempSensor.set_resolution(3);               // Set the resolution to 12bits (9+3)
    tempSensor.set_templimit(limitTemp);        // Set the temp limit after which an alert is sent
    tempSensor.set_comp_int(alarmMode);         // Set the "comparator" mode
    tempSensor.set_hysteresis(histeresisTemp);  // set the Temperature Hysteresis register
}

void loop() {
  float t = tempSensor.read();                          // Read current temperature
  
  Serial.print("Temperature: ");                        // Displays temperature in the Serial Monitor
  Serial.print(t,4);                                    // Temperature is displayed with 4 decimal places
  Serial.print("    Temp Sensor: ");
  belowSetTemp = digitalRead(alarmPin);
  tempSensorWorking = tempSensor.read_register(1,1) > 0;  // Tries to read a register on the sensor to check whether it is connected and functioning
  
  if(tempSensorWorking && belowSetTemp){
    Serial.print("working.    Heating Pad: ON.");
    digitalWrite(heatingLED,HIGH);
    digitalWrite(heatingPadPin,HIGH);
    delay(40);
  }else{
    Serial.print("error.    Heating Pad: OFF.");
  }
  
  Serial.println("");
  digitalWrite(heatingPadPin,LOW);
  digitalWrite(heatingLED,LOW);
}
