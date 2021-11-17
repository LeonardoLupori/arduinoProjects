# Food Cue Preference

Behavioral setup for *head-fixed* experiments testing the preference toward food-predictive visual cues. This is my personal implementation of the behavioral test found in (Burgess et. al. 2016[^1] , and in the following papers from the Andermann Lab.)

## Content

* **Microcontroller code**: The sub-folder *foodCuePreference* contains code (*.ino* files) that needs to be uploaded on the microcontroller. The main sketch to upload is *foodCuePreference.ino*, while the other files are addidional libraries;
* Python code for running experiments: TODO
* Python code for analyzing data: TODO

## Description

This apparatus and code allows to perform a closed-loop, Go/NoGo orientation discrimination task. In this task, mice are presented visual cues (square-wave drifting gratings) on an LCD monitor for 2 s, followed by a 2 s response window and a 6 s inter-trial interval. Mice can respond to the visual stimulation by licking the lick spout during the 2s response window. Licking during this window will trigger the delivery of a high-calorie liquid, a bitter solution of nothing, based on the visual stimulus presented. Licking during the stimulis window has no effect, and is not negatively reinforced.

Mice are trained to discriminate between:
* **Food Cue (FC)**: a stimulus associated with delivery of food (liquid meal replacement);
* **Aversive Cue (QC)**: a stimulus associated with the delivery of an aversive biter stimulus (a quinine solution),
* **Neutral Cue (NC)**: a stimulus that results in no delivery.

The apparatus is composed by two main parts:
1. **ESP32**. This microcontroller controls all the hardware parts of the setup and ensures good timing of the recorded responses.  
The ESP32 can be programmed like an arduino but has many advantages in term of raw performance. In particular, it runs on a 80MHz CPU (for comparison, Arduino UNO runs at 16MHz) and has integrated capacitive touch sensors.
The microcontroller detects licking and running activity of the mouse, control the stepper motors for delivering either Ensure or quinine and performs the logic for the closed loop experimental design.  
2. **Python Instance.** The ESP32 communicates (serial communication via USB) with a Python software that needs to run on a separate computer. This computer is responsible to:
   * present the experimenter with a GUI for inputing basic information on the experimental subject;
   * manage the trial orders in a pseudo-random way;
   * present visual stimuli on a monitor through Psychopy
   * redord events (licks, running activity etc) that are being sent from the ESP32
   * send manual instructions to the ESP32.


[^1]: Burgess CR, Ramesh RN, Sugden AU, Levandowski KM, Minnig MA, Fenselau H, Lowell BB, Andermann ML. *Hunger-Dependent Enhancement of Food Cue Responses in Mouse Postrhinal Cortex and Lateral Amygdala*. Neuron. 2016 Sep 7;91(5):1154-1169. doi: 10.1016/j.neuron.2016.07.032.


# Detailed description

## Communication protocol
The ESP32 and Python communicate through a serial USB connection.
The communication is bidirectional: 
* Python sends to the microcontroller messages containing a single byte
(a single character), and each character has a different meaning.
As a general rule, messages that Python sends to the Arduino are **uppercase**.
* On the other side, the microcontroller sends back messages that are composed of three, comma-separated values:  
   >"code,timeStamp,value"
   * *code* : a byte (a single character) containing info of a particular event. As a general rule, these codes are **lowercase**;
   * *timeStamp*: time (in milliseconds) of the event;
   * *value*: (optional) an optional value.

### Python -> ESP32 codes

|Code|Command|Description|
|:---:|:---|:---|
|A|Start an *Ad-libitum* trial|Unconditional delivery of Ensure upon the first detected lick|
|P|Start a *Pavlovian* Trial|Unconditional delivery of Ensure after a visual food cue (FC) stimulus|
|T|Start a *Conditional Trial*|Signal the beginning of a trial|
|Z|Response window for Food-Cue|Delivery of Ensure after the first lick|
|X|Response window for Quinine-cue|Delivery of quinine after the first lick|
|C|Response window for Neutral-cue|No response upon licking|
|I|Intertrial|Signals the beginning of an intertrial phase|
|R|Single dose Ensure|Manually deliver a single dose of Ensure|
|V|Single dose quinine|Manually deliver a single dose of quinine|
|L|Ensure Fast-Forward|Fast positive movement of the Ensure syringe pump|
|K|Ensure Fast-Backward|Fast negative movement of the Ensure syringe pump|
|M|Quinine Fast-Forward|Fast positive movement of the quinine syringe pump|
|N|Quinine Fast-Backward|Fast negative movement of the quinine syringe pump|
|Q|||

### ESP32 -> Python codes

the microcontroller sends back messages that are composed of three, comma-separated values:  
   >"code,timeStamp,value"
   * *code* : a byte (a single character) containing info of a particular event. As a general rule, these codes are **lowercase**;
   * *timeStamp*: time (in milliseconds) of the event;
   * *value*: (optional) an optional value.

|Code|Meaning|Details|
|:---:|:---|:---|
|a|Signals the beginning of an ***A**d-libitum* trial||
|p|Signals the beginning of a ***P**avlovian* Trial||
|t|Signals the beginning of a *Conditional **T**rial*||
|d|Contains the current **d**istance run by the mouse|The distance code is sent periodically with a timestamp and a value for the distance|
|l|Signals a **L**icking event||
|e|A dose of **E**nsure is delivered||
|q|A dose of **Q**uinine is delivered||