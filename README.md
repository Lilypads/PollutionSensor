# PollutionSensor

For more details on sensors, wiring, UML diagram, other specifications, please have a look at our wiki here[https://github.com/Lilypads/PollutionSensor/blob/main/PollutionSensor.org]

Our youtube channel contains video demonstrating device functionality here >> https://www.youtube.com/channel/UC7G0Ejtdh_g3ME1dvBaGvhw

Please also follow us on twitter to participate in the discussion here >> @PollutionSens

## Repository Content

* __.github/workflows__ contains script to auto-run tests during development cycles triggered by pull requests
* __ExampleCMakeLib__  contains square root library to demonstrate uses of CMake library structure for development purposes
* __Figures__ contains diagrams related to project documentations
* __Hardware__ contains hardware specification used in this project
* __pigpio__ contains GitHub submodule of pigpio library repository to build locally
* __src__ contains smaller modules of programs in the project
    * __GPS__ contains GPS library and demo script
    * __GitWrapper__ contains git key wrapper and demo script
    * __SPS30__ contains SPS30 library and demo script
    * __StateMachine__ contains state machine library and combined(with SPS30, GPS, StorageHandler) functionality demo script
    * __StorageHandler__ contains storage handler library and demo script
    * __UserInterface__ contains user-interface library and demo script
    * __referenceCPP__ contains other reference scripts used for development
* __sshAuth__ contains public ssh keys for developers to access pi remotely
* __test__ contains test scripts used in the workflow

## Pre-Requisite

### CMAKE

See CMake official installation procedure here https://cmake.org/install/ 

### pigpio

The library is included as a submodule on this repository, so it will be built along with the repository itself. You do not necessarily need to install this seperately.

### Boost

For Linux

`sudo apt install libboost-all-dev`

For MacOS using Homebrew

`brew install boost`

## How to get the program running

1. Clone the repository with submodules

`git clone --recurse-submodules https://github.com/Lilypads/PollutionSensor`

2. Get inside the directory

`cd PollutionSensor`

3. Compile here

`cmake .`

`make`

4. Run the main program with sudo to be able to access to gpios

`sudo ./executableFileName`

Replace executableFileName with the desired executable file name which was compiled by cmake

For example, to run main program

`sudo ./main`

5. To run unit tests (optional)

`ctest`
