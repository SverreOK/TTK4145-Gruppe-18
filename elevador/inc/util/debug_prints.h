#include <ncurses.h>
#include <chrono>
#include <boost/thread.hpp>

#include "super_container.h"
#include "event_queue.h"
#include "elevator_fsm.h"

class Debug_prints {
    private:
        Super_container *super_container;
        Elevator_id id;
        thread_safe_queue *event_queue;

        // Forward declaration
        std::string state_enum_to_string(state_enum state) {
            switch (state) {
                case state_enum::IDLE:
                    return "IDLE";
                case state_enum::MOVING_UP:
                    return "MOVING UP";
                case state_enum::MOVING_DOWN:
                    return "MOVING DOWN";
                case state_enum::DOOR_OPEN:
                    return "DOOR_OPEN";
            }
        }

        std::string event_enum_to_string(elevator_event event) {
            switch (event) {
                case elevator_event::ORDER_RECEIVED:
                    return "ORDER_RECEIVED";
                case elevator_event::ARRIVED_AT_FLOOR:
                    return "ARRIVED_AT_FLOOR";
                case elevator_event::DOOR_TIMEOUT:
                    return "DOOR_TIMEOUT";
            }
        }

    public:
        Debug_prints(Super_container *super_container, Elevator_id id, thread_safe_queue *event_queue) 
        : super_container(super_container), id(id), event_queue(event_queue) {}

        void debug_print_start(Super_container *super_container, Elevator_id id, thread_safe_queue *event_queue) {

            uint8_t floor;
            std::string current_state;
            int last_seen;
            std::string next_event;

            
            initscr();

            curs_set(0);

            while (DEBUG) {

                // update variables
                floor = super_container->get_elevator_by_id(id)->get_current_floor();
                current_state = state_enum_to_string(super_container->get_elevator_by_id(id)->get_current_state());
                last_seen = super_container->get_elevator_by_id(id)->get_last_seen();
                // i want to see the next event in the queue
                if (!event_queue->empty()) {
                    next_event = event_enum_to_string(event_queue->front());
                }
                else {
                    next_event = "NO EVENT IN QUEUE";
                }
                    



                clear();
                // create a nice printout of the variables above
                printw("Elevator ID: %s\n", id.id.c_str());
                printw("Current floor: %d\n", floor);
                printw("Current state: %s\n", current_state.c_str());
                printw("Last seen: %d\n", last_seen);
                printw("Next event: %s\n", next_event.c_str());

                refresh();

                boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
            }

            endwin();

        }
};
