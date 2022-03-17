# PollutionSensor

For more details on sensors, wiring, UML diagram, other specifications, please have a look at our wiki here[https://github.com/Lilypads/PollutionSensor/blob/main/PollutionSensor.org]

## Pre-Requisite

### CMAKE

### BCM2835

Install latest version from BCM2835 from : http://www.airspayce.com/mikem/bcm2835/

Example script for Linux (1.71 was version number at the time of writing)

`
wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.71.tar.gz
`

`
tar -zxf bcm2835-1.71.tar.gz 
`

`
cd bcm2835-1.71
`

`
./configure
`

`
sudo make check
`

`
sudo make install
`

### Boost (optional: for unit tests)

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

4. Run the program

`./executableFileName`
