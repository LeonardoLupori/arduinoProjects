# Cued Pavlovian Conditioning

A setup for **pavlovian conditioning** in mice.  
Mice are trained to associate a short sound from a piezo to a positive reward consisting of a drop of sweetened water.

----

the setup comprises:
- **Automated reward delivery** - Arduino controls a stepper motor that precisely move a syringe containing the water reward (in our experiments lightly sweetened water). This setup is able to deliver small droplets of water as a reward after each trial
- **Auditory cue** - A piezo that emits an auditory cue of variable frequency (4kHz in our experiments). This cue signals the arrival of the reward (2s later in our experiments)
- **Capacitive lick sensor** - Records licking evemts
- **Serial communication** - Communication through USB to record events and their timestamps (in milliseconds since Arduino last reset). The recorded events are: 
    - *cue* - auditory cue was given (a reward will be following);
    - *reward* - drop of liquid reward was administered;
    - *only cue* - auditory cue, buth without a reward following
    - *lick* - a licking event;

- **Hardware triggers** - Arduino also sends out 5V pulses (2ms width) signalling the same events.


## Installation

## Contents
This folder contains both the arduino code to control the hardware