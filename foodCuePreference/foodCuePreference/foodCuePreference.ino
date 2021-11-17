/* FOOD CUE PREFERENCE
   --------------------------------------------------------------
   Arduino script for reproducing a 2 alternative forced choice test in mice
   testing the preference of mice towards food predictive stimuli while in a
   food deprived state.
*/

// --------------------------------------------------------------
// LIBRARIES TO INCLUDE
// --------------------------------------------------------------
#include <movingAvg.h>          // (install from library manager)
#include <TaskScheduler.h>      // (install from library manager)
// https://github.com/arkhipenko/TaskScheduler


// --------------------------------------------------------------
// PARAMETERS (CUSTOMIZABLE BY THE USER)
// --------------------------------------------------------------

// TOUCH SENSOR FOR DETECTING LICKS
// ---------------------------------
const int PIN_LICKSENSOR = 32;                  // Pin Number of the lick sensor
const bool AUTO_THRESHOLD_CALIBRATION = true;   // Automatically sets the threshold to half the baseline
int THRESHOLD = 20;                             // Threshold value below which a lick is detected
// This value is overwritten if AUTO_THRESHOLD_CALIBRATION==true

// SERIAL COMMUNICATION
// ---------------------------------
const bool ECHO_FROM_ARDUINO = true;            // Sends back the serial messages that it recieves. (debugging purposes; default false)

// STEPPER MOTORS
// ---------------------------------
const int NUM_STEPS = 1000;                 // Number of steps for each delivery
const int PIN_STEP_ENSURE = 14;             // Pin Number controlling the Ensure stepper motor
const int PIN_STEP_QUININE = 27;            // Pin Number controlling the Quinine stepper motor
const int PIN_DIR_ENSURE = 3;               // Pin Number controlling the direction of the Ensure stepper
const int PIN_DIR_QUININE = 4;              // Pin Number controlling the direction of the Quinine stepper
const int NUM_STEP_DELIVERY = 100;          // Number of steps for delivering Ensure or Quinine
const int NUM_STEP_FAST_DELIVERY = 1000;    // Number of steps for Fast-Forward or -Backward movements
const int STEP_LENGHT_MICROSECONDS = 500;   // Length of each step pulse in micro seconds



// --------------------------------------------------------------
// GLOBAL VARIABLES (do not edit!)
// --------------------------------------------------------------

// BEHAVIOR
// ---------------------------------
String responseMode = "none";         // Can be "FC","QC","NC","AL" or "none"
long trialStartTime = 0;              // Time (ms) since the last trial start
long runningPosition = 0;             // Stores the current distance run by the mouse

// LICKING
// ---------------------------------
int sensorReading = 0;                // Stores the last reading of the lick sensor
int readingMovingAvg = 0;             // Stores the last updated running average reading of the lick sensor(less noisy)
bool touched = false;                 // Whether the lick sensor is currently being touched or not
bool lickResponseDetected = false;    //

// STEPPER MOTORS
// ---------------------------------
bool mustDeliverEnsure = false;       // Whether to deliver Ensure at the next available moment
bool mustDeliverQuinine = false;      // Whether to deliver Quinine at the next available moment


// --------------------------------------------------------------
// CALLBACK FUNCTIONS PROTOTYPES
// initialize "blank" functions to be better defined later.
// --------------------------------------------------------------
void sendDistance_Clbk();
void sendLick_clbk();

// INITIALIZE MOVING AVERAGE (for lick sensor)
movingAvg touchMovingAvg(5);    // use 5 data points for the moving average

/* *CORE: 0*  INITIALIZE DUAL CORE TASK
   --------------------------------------------------------------
   This initializes a task that will be run on CORE 0, while the loop()
   function is run in parallel on CORE 1. This allows true parallel processing
   so that generating the pulses for the stepper motor in the syringes
   that usually requires delay() functions, does not imair the precision of
   lick sensing and serial communication.
*/
TaskHandle_t stepManager;


/* *CORE: 1*  MULTITASK SCHEDULING
   --------------------------------------------------------------
   This initializes a scheduler (included as <TaskScheduler.h>) that allows to run
   many scheduled tasks at the right time inside the loop() function.
   In particular, two tasks are defined:
   - The first tasks sends periodically the current distance run through Serial Communication
   - The second task checks periodically the lick sensor and, in the event of a lick
      it sends the event by serial and delivers Ensure or Quinine if appropriate
*/
Scheduler taskManager;

// FIRST TASK
Task task_SendDistance(5000, TASK_FOREVER, &sendDistance_Clbk);
// SECOND TASK
Task task_SendLick(1, TASK_FOREVER, &sendLick_clbk);


// --------------------------------------------------------------
// SETUP FUNCTION
// --------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(100);                           // Short delay to allow the serial communication to complete
  welcomeMesage();                      // Welcome message

  // PIN MODES
  // ---------------------------------
  pinMode(PIN_STEP_ENSURE, OUTPUT);
  pinMode(PIN_STEP_QUININE, OUTPUT);
  pinMode(PIN_DIR_ENSURE, OUTPUT);
  pinMode(PIN_DIR_QUININE, OUTPUT);
  digitalWrite(PIN_DIR_ENSURE, HIGH);   // Default the direction to high
  digitalWrite(PIN_DIR_QUININE, HIGH);  // Default the direction to high

  touchMovingAvg.begin();               // Start the moving average object
  if (AUTO_THRESHOLD_CALIBRATION) {     // Perform auto calibration, if requested
    lickAutoCalibration();
  }

  // TASK MANAGER (CORE 1)
  // ---------------------------------
  taskManager.init();                     // Initialize the Task Manager
  taskManager.addTask(task_SendDistance); // Add task to the Task Manager
  taskManager.addTask(task_SendLick);     // Add task to the Task Manager
  task_SendDistance.enable();             // Enable the task
  task_SendLick.enable();                 // Enable the task

  // DEFINE TASK RUNNING IN PARALLEL ON CORE 0
  // ---------------------------------
  xTaskCreatePinnedToCore(
    stepperManager,    // Task function
    "stepManager",     // name of task
    100000,            // Stack size of task
    NULL,              // parameter of the task
    0,                 // priority of the task
    &stepManager,      // Task handle to keep track of created task
    0);                // pin task to core 0
}


// --------------------------------------------------------------
// MAIN LOOP FUNCTION
// --------------------------------------------------------------
void loop() {
  checkForSerialInputs(ECHO_FROM_ARDUINO);
  taskManager.execute();
}


// --------------------------------------------------------------
// ADDITIONAL FUNCTIONS
// --------------------------------------------------------------

void welcomeMesage() {
  for (int i = 0; i < 29; i++) {
    Serial.print("*");
  }
  Serial.println("*");
  Serial.println("ESP32 is booting up...");
  for (int i = 0; i < 29; i++) {
    Serial.print("*");
  }
  Serial.println("*");
  Serial.println("A simple setup for head-fixed behavioral Go/NoGo tasks in mice.");
}


void lickAutoCalibration() {
  int numSampledReadings = 500;       // How many samples to read for the baseline
  int totalReading = 0;               // Stores the sum of all readings

  // Sample the lick sensor many times and take the average baseline value
  Serial.println("Auto-calibration of lick sensor...");
  for (int i = 0; i < numSampledReadings; i++) {
    totalReading += touchRead(PIN_LICKSENSOR);
    delay(10);
  }
  int avgValue = int(totalReading / numSampledReadings);  // Average baseline value
  int newThresh = int(avgValue / 2);                      // New threshold
  Serial.print("\t- Average Baseline:");                  // Print info about the calibration
  Serial.println(avgValue, DEC);
  Serial.print("\t- Threshold set:");
  Serial.println(newThresh, DEC);
  Serial.println("Calibration done!");
  THRESHOLD = newThresh;                                  // Set the new threshold
}
