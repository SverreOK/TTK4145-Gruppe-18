
# data module 📖

This module contains three files related to the data types used.
- **call_class.h**: Contains a few smaller structs & classes for storing Call data.
- **elevator_state_class.h**: Contains the class used for storing state of an elevator
- **super_container.h**: The main object passed in the code, contains all of the above with methods for interacting.

## call_class.h 🔔

**Call** class is the main class contained inside this file, it is used to represent each and every call in the system. Calls are uniquely defined with an **Elevator_id** and contains further data to identify details of each call. It implements thread-safe functions using mutexes.

The call class has an accompanying struct called **call_message**, this is used when transmitting **Call** objects over the network.

The following are remaining simple structs & enum classes which are used in the Call class:
- **button_type**: enum class {UP_HALL, DOWN_HALL, CAB}
- **state_enum**: enum class {IDLE, MOVING_UP, MOVING_DOWN, DOOR_OPEN}
- **Elevator_id**: struct, contains a simple *std:string* "id"
- **Call_id**: struct, contains an *Elevator_id* "elevator_id" & *int* "call_number"


Lastly there are two functions which simplify comparing std::vector<Elevator_id> variables:
- **vector_elements_in_A_found_in_B** ( list1 , list2 )
- **vectors_are_equal** ( list1, list2 )

## elevator_state_class.h 🛗
This contains just one object and an accompanying struct.

The **Elevator_state** class contains a uniquie identifier (**Elevator_id**), and all data relevant to the position and status of an elevator. It implements thread-safe functions using mutexes.

The accompanying struct (**elevator_status_network**) is used for sending the **Elevator_state** over the network.

## super_container.h 📚📚

The **Super_container** Contains multiple *std::vectors* which stores calls, and elevator states for the entire networked elevator system. 

All functions are made thread-safe through mutexes. Most set or update functions contain logic to make them easy and safe to implement elsewhere in the code.
