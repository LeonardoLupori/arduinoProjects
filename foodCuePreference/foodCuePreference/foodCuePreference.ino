/* FOOD CUE PREFERENCE

   Arduino script for reproducing a 2 alternative forced choice test in mice
   testing the preference of mice towards food predictive stimuli while in a
   food deprived state.
*/


// LIBRARIES TO INCLUDE
#include <movingAvg.h>          // (install from library manager)
#include <TaskScheduler.h>      // (install from library manager)
                                // https://github.com/arkhipenko/TaskScheduler


// --------------------------------------------------------------
// PARAMETERS (CUSTOMIZABLE BY THE USER)
// --------------------------------------------------------------

// BEHAVIOR
// ---------------------------------
const int STIM_WINDOW_LENGHT = 2;
const int RESP_WINDOW_LENGHT = 2;

// TOUCH SENSOR FOR DETECTING LICKS
// ---------------------------------
const int PIN_LICKSENSOR = 32;
const bool AUTO_THRESHOLD_CALIBRATION = true;      // TO IMPLEMENT!!!!!
const int THRESHOLD = 20;

// SERIAL COMMUNICATION
// ---------------------------------
//  Sends back the serial messages that it recieves.
// (debugging purposes; default false)
const bool ECHO_FROM_ARDUINO = true;


// --------------------------------------------------------------
// GLOBAL VARIABLES
// --------------------------------------------------------------

// BEHAVIOR
// ---------------------------------
bool trialRunning = false;
String responseMode = "none";                     // Can be "FC","QC","NC","AL" or "none"
long trialStartTime = 0;
bool lickResponseDetected = false;

// BEHAVIOR
// ---------------------------------
long runningPosition = 0;

// --------------------------------------------------------------
// CALLBACK FUNCTIONS PROTOTYPES
// --------------------------------------------------------------
void sendSpeed_Clbk();
void sendLick_clbk();



int sensorReading = 0;
int readingMovingAvg = 0;
bool touched = false;
int LED = 13;


movingAvg touchMovingAvg(5);    // use 5 data points for the moving average

Scheduler taskManager;

Task task_SendSpeed(1000, TASK_FOREVER, &sendSpeed_Clbk);
Task task_SendLick(1, TASK_FOREVER, &sendLick_clbk);

// --------------------------------------------------------------
// SETUP FUNCTION
// --------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(1000); // give me time to bring up serial monitor


  Serial.println("ESP32 Touch Test");
  touchMovingAvg.begin();
  pinMode(LED, OUTPUT);

  // Initialize the Task Manager
  taskManager.init();
  // Add all the tasks to the Task Manager
  taskManager.addTask(task_SendSpeed);
  taskManager.addTask(task_SendLick);
  // Enable all the tasks
  task_SendSpeed.enable();
  task_SendLick.enable();
}


// --------------------------------------------------------------
// MAIN LOOP FUNCTION
// --------------------------------------------------------------
void loop() {
  checkForSerialInputs(ECHO_FROM_ARDUINO);
  taskManager.execute();
}
