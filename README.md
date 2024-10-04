# Model-Railway-Automatic-Level-Crossing-and-signals
Arduino based Automatic Level Crossing Gate Controller for double track line on a Model Railway (UK original style gates)
with CMRI reporting to JMRI

This project runs on an Arduino Nano with eight LDR-based sensors distributed along a double-track railway with the old style Level Crossing gates and a station at the centre.  The sensors detect a train entering the section from either side with at least 1 metre of track (n-gauge - more needed for OO or O gauge) to the level crossing gates - this allows time for the gates to close in sequence (exit gates last) to road traffic and open to Train traffic. The sensor for leaving the section is also >1metre away so that long trains will be clear of the gates before opening to road traffic.  Trains entering simultaneously on each track will not trigger the gates to open for road traffic until both have exited the section. Additionally, there is a two-aspect signal at the exit to the station which changes to green when the gates are open for train traffic and changes to red again immediately after being triggered by the train leaving the platform. All the sensor data is sent to a computer running JMRI using a CMRI USB connection - the serial.print lines are commented out when CMRI is running - and are active only when debugging the hardware.

Traffic is only permitted in one direction for each line so that the software is only triggered in the correct sequence—this reduces the chance of spurious triggering and "switch bounce."  Pins are now used for warning signals to road traffic — this is operating now after I designed some for 3D printing using a Resin printer.  Ironically, having spent time designing prototypical hoods for the traffic warning lights, I have had to remove the hoods to make it obvious that the lights are working because they were only viewable at the same height as the road, and most viewers are taller and don't see the lights flashing - i.e. the hoods worked too well!

The version uploaded 3rd October 2024 is used on Maun Valley Junction layout where the station stopping sensors are not used, hence only 6 sensors are used.

3D printing files are available at https://www.thingiverse.com/thing:6786859

