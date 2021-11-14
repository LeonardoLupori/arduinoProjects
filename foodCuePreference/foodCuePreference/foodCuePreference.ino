/* FOOD CUE PREFERENCE
 * 
 * Arduino script for reproducing a 2 alternative forced choice test in mice 
 * testing the preference of mice towards food predictive stimuli while in a 
 * food deprived state.
 */



// LIBRARIES TO INCLUDE
#include <movingAvg.h>          // (library manager)
#include <TaskScheduler.h>      // (library manager)
                                // https://github.com/arkhipenko/TaskScheduler


// --------------------------------------------------------------
// PARAMETERS
// --------------------------------------------------------------

// TOUCH SENSOR FOR DETECTING LICKS
const int 
const int THRESHOLD = 20;


// --------------------------------------------------------------
// GLOBAL VARIABLES
// --------------------------------------------------------------

// GLOBAL VARIABLES
int sensorReading = 0;
int readingMovingAvg = 0;
const int THRESHOLD = 20;
bool touched = false;
int LED = 13;


movingAvg mySensor(5);    // use 10 data points for the moving average


//void sendSpeed_Clbk(){
//  Serial.println(millis());
//}




Scheduler taskManager;


void setup() {
  Serial.begin(115200);
  delay(1000); // give me time to bring up serial monitor
  Serial.println("ESP32 Touch Test");
  mySensor.begin();
  pinMode(LED,OUTPUT);

  Task task_SendSpeed(500, TASK_FOREVER, &sendSpeed_Clbk);
  Task task_SendLick(1, TASK_FOREVER, &sendLick_clbk);
  
  // Initialize the Task Manager  
  taskManager.init();
  // Add all the tasks to the Task Manager
  taskManager.addTask(task_SendSpeed);
  // Enable all the tasks
  task_SendSpeed.enable();
}


void loop() {
  checkForSerialInputs();
  taskManager.execute();
}


//  sensorReading = touchRead(T9);
//  readingMovingAvg = mySensor.reading(sensorReading);
//  if(readingMovingAvg<= THRESHOLD && !touched) {
//    String message = String("TOUCH") + millis();
//    Serial.println(message);
//    touched = true;
//  } else if(readingMovingAvg > THRESHOLD){
//    touched = false;
//  }
