# Food Cue Preference

Behavioral setup for *head-fixed* experiments testing the preference toward food-predictive visual cues. This is my personal implementation of the behavioral test found in (Burgess et. al. 2016, and following papers from the Andermann Lab.)

## Content

* Microcontroller code: The sub-folder *foodCuePreference* contains code (*.ino* files) that needs to be uploaded on the microcontroller. The main sketch to upload is *foodCuePreference.ino*, while the other files are addidional libraries;
* Python code for running experiments: TODO
* Python code for analyzing data: TODO

## Description

This apparatus and code allows to perform a closed-loop, Go/NoGo orientation discrimination task. In this task, mice are presented visual cues (square-wave drifting gratings) on an LCD monitor for 2 s, followed by a 2 s response window and a 6 s inter-trial interval. Mice can respond to the visual stimulation by licking the lick spout during the 2s response window. Licking during this window will trigger the delivery of a high-calorie liquid, a bitter solution of nothing, based on the visual stimulus presented. Licking during the stimulis window has no effect, and is not negatively reinforced.

Mice are trained to discriminate between:
* **Food Cue (FC)**: a stimulus associated with delivery of food (liquid meal replacement);
* **Aversive Cue (QC)**: a stimulus associated with the delivery of an aversive biter stimulus (a quinine solution),
* **Neutral Cue (NC)**: a stimulus that results in no delivery.

The apparatus is composed by two main parts:
1. **ESP32**. This microcontroller controls all the Hardware parts of the setup and ensures good timing of the recorded responses.  
The ESP32 can be programmed like an arduino but has many advantages in term of raw performance. In particular, it runs on a 80MHz CPU (for comparison, Arduino UNO runs at 16MHz) and has integrated capacitive touch sensors.
The microcontroller detects licking and running activity of the mouse, control the stepper motors for delivering either Ensure or quinine and performs the logic for the closed loop experimental design.  
2. **Python Instance.** The ESP32 communicates (serial communication via USB) with a Python software that needs to run on a separate computer. This computer is responsible to:
   * present the experimenter with a GUI for inputing basic information on the experimental subject;
   * manage the trial orders in a pseudo-random way;
   * present visual stimuli on a monitor through Psychopy
   * redord events (licks, running activity etc) that are being sent from the ESP32
   * send manual instructions to the ESP32.

## References

* Burgess CR, Ramesh RN, Sugden AU, Levandowski KM, Minnig MA, Fenselau H, Lowell BB, Andermann ML. *Hunger-Dependent Enhancement of Food Cue Responses in Mouse Postrhinal Cortex and Lateral Amygdala*. Neuron. 2016 Sep 7;91(5):1154-1169. doi: 10.1016/j.neuron.2016.07.032.
