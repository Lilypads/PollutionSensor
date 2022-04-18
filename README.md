# PollutionSensor

For more details on sensors, wiring, UML diagram, other specifications, please have a look at our wiki here[https://github.com/Lilypads/PollutionSensor/blob/main/PollutionSensor.org]

Our youtube channel contains video demonstrating device functionality here >> https://www.youtube.com/channel/UC7G0Ejtdh_g3ME1dvBaGvhw

Please also follow us on twitter to participate in the discussion here >> @PollutionSens

## Pre-Requisite

### CMAKE

### pigpio

### Boost

For Linux

`sudo apt install libboost-all-dev`

For MacOS using Homebrew

`brew install boost`

## How to get the program running

1. Clone the repository

`git clone --recurse-submodules https://github.com/Lilypads/PollutionSensor`

2. Get inside the directory

`cd PollutionSensor`

3. Compile here

`cmake .`

`make`

4. Run the main program

`./executableFileName`
