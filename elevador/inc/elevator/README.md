# elevator module 🛗
### overview 🔭
- **elevator_driver.h**: Interface objects which implements functions to communicate with the local elevator server over TCP.
- **elevator_button_poller.h**: Implements threads that polls for *floor sensors*, *button presses*, and *obstruction switch* outputs through the elevator_driver. 
- **elevator_light_thread.h**: Implements threads that controls the button, and obstruction lights on the elevator through the elevator_driver.
- **elevator_algorithm.h**: Implements a set of functions which are used to determine actions the elevator, should take such as choosing direction to move.
- **elevator_fsm.h**: Implements the Finite State Machine which controls the behaviour and movement of the local elevator through the elevator_driver using the super_container.
