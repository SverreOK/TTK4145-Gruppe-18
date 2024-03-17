## util module 🧰
### overview binoculars
- **config.h**: Defines configurations for elevator operation, door settings, network communication, debugging options and backup options.
- **debug_prints.h**: Contains the Debug_prints class, which utilizes ncurses for visual debugging output related to elevator state, events, and network status.
- **event_queue.h**: Defines a thread-safe queue for managing elevator events in a concurrent environment.

### config.h 🗒️
Contains the configurations for the system, see config.h for more documentation.

### debug_prints.h 🗒️
Module for refreshing the screen with new information. Refresh rate is decided in config.h.

**debug_print_start()** is used in the primary logic start thread, it runs a thread that continuously updates the debug display.

### event_queue.h 🗒️
Implements a thread-safe queue for managing elevator events. It supports:
- Safe pushing and popping of events in a multi-threaded environment.
- Condition variable-based signaling for event handling.

#### enumerations
- **elevator_event**: Defines possible elevator events (**ORDER_RECEIVED**, **ARRIVED_AT_FLOOR**, **DOOR_TIMEOUT**).

#### methods
- **push(elevator_event)**: Adds an event to the queue.
- **pop()**: Removes and returns the front event from the queue.
- **empty()**: Checks if the queue is empty.
- **front()**: Retrieves the front event from the queue without removing it.
- **get_last_popped_event()**: Retrieves the most recently popped event.
