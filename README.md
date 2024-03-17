# Peer-to-Peer Elevator System in C++ 🚀🤖🔗
This is a term project in [TTK4145](https://www.ntnu.no/studier/emner/TTK4145) developed by [endurs](https://github.com/endurs), [MSalthe](https://github.com/MSalthe), and [SverreOK](https://github.com/SverreOK). The project implements software for a distributed elevator system. This solution is utilzing a peer-to-peer architecture to enable communication between several elevators.

There are two main folders:
- **./Elevador**: Effectively the root folder for the distributed elevator system
- **./Excercices**: Contains multiple smaller excercices from the subject.

## Setup of project 🛠️
Documentation of code resides in each module folder in elevador\inc.
### Prerequisites 🧑‍💻
To build and run this project, you will need to have the following installed on your system:

#### Compiler ⚙️
**GCC (G++)**: The GNU C++ Compiler is required. The project uses features that are available in C++11 and later, so ensure your GCC version supports at least C++11.

#### Libraries 📚
This project depends on these external libraries,
- **jsoncpp**: For parsing and handling JSON data in the call assigner.
- **Boost**: Used for threading, networking and timing.
- **ncurses**: Used for the debug printing functionality.

#### Installing dependencies 🤠
Linux (Ubuntu) (😎):

``` bash
sudo apt-get update
sudo apt-get install g++ libjsoncpp-dev libboost-all-dev libncurses5-dev
```

### Configuration 📐
The configuration resides in elevador\inc\util\config.h. These are default values. 

The most important configs to check before compiling to run on your system are the following (shown with default setting):
``` cpp
#define NUM_ELEVATORS 3                     //must be equal or larger than amount of elevators expected on network
#define NUM_FLOORSS 4                       //must match your simulated/physical elevator
#define BROADCAST_ADDRESS "10.100.23.255"   //must match your network
#define ELEVATOR_ID "Ole"                   //must be unique between all elevators

//NOTE: you may also need to check the different ports used!
```


### Running the Project 🏃‍♂️
After you have configured correctly, you can build the project using:
``` bash
make
```
This will build a "main" file in the root folder you can run by using:
``` bash
./main
```
To build debug version, use:
``` bash
make debug
```
This will build a file called "main_debug". which can be ran using a debugger configuration/setup.
## Honorable Mentions 🎖️
Thank you Ola Vanni Flaata for networking knowledge.
