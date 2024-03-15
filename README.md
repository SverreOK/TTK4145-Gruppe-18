# Peer-to-Peer Elevator System in C++ 🚀🤖🔗
This is a term project in [TTK4145](https://www.ntnu.no/studier/emner/TTK4145) developed by three students. The project implements software for a distributed elevator system. This solution is utilzing a peer-to-peer architecture to enable communication between several elevators.

## Setup of project 🛠️
Documentation resides in each module folder in elevador\inc.
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

For physical testing use,
``` cpp
#define NUM_FLOORSS 4
```

### Running the Project 🏃‍♂️
After you have configured correctly, you can build the project using:
``` bash
make
```
To build debug version, use:
``` bash
make debug
```

## Honorable Mentions 🎖️
Thank you Ola Vanni Flaata for networking knowledge.
