#pragma once

#include <ncurses.h>
#include <chrono>
#include <vector>
#include <boost/thread.hpp>

#include "super_container.h"
#include "event_queue.h"
#include "elevator_fsm.h"

class Debug_prints {
    private:

        // Debug information from the system
        Super_container *super_container;
        Elevator_id id;
        thread_safe_queue *event_queue;

        // ART
        std::vector<std::string> door_open_art = {
                " ___________________ ",
                "|  _______________  |",
                "| |               | |",
                "| |               | |",
                "| |       O       | |", // Stick figure inside
                "| |      -|-      | |", // Representing the body and arms
                "| |      / \\      | |", // Representing the legs
                "| |_______________| |", // Representing the emptiness of my soul and the elevator and the door and the stick figure and the world and the universe and the void
                "|___________________|"
        };

        // PRINT ART
        void print_door_open_art(int start) {
            for (int i = 0; i < static_cast<int>(door_open_art.size()); i++) {
                mvprintw(start+i, 1, door_open_art.at(i).c_str(), "\n");
            }
        }

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
                
                default:
                    return "";
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
                case elevator_event::DOOR_CLOSED:
                    return "DOOR_CLOSED";

                default:
                    return "";
            }
        }

        std::string call_type_enum_to_string(button_type call_type) {
            switch (call_type) {
                case button_type::CAB:
                    return "CAB";
                case button_type::UP_HALL:
                    return "UP";
                case button_type::DOWN_HALL:
                    return "DOWN";

                default:
                    return "";
            }
        }

    public:
        Debug_prints(Super_container *super_container, Elevator_id id, thread_safe_queue *event_queue) 
        : super_container(super_container), id(id), event_queue(event_queue) {}

        void debug_print_start(Super_container *super_container, Elevator_id id, thread_safe_queue *event_queue) {

            //Physical
            uint8_t floor;
            std::string current_state;
            std::string last_popped_event;

            //Iteration stuff
            int i = 1;
            int j = 0;

            // init screen, and set color
            initscr();
            start_color();
            init_pair(1, COLOR_CYAN, COLOR_BLACK);

            // hide cursor
            curs_set(0);

            // box for the window
            box(stdscr, 0, 0);
            attron(COLOR_PAIR(1));
            clear();

            // Updates screen if debug is enabled
            while (true) {
                
                // This snippet clears the screen every 10 iterations and not every 1 iteration to avoid glitching
                if (j == 10) {
                    j = 0;
                    clear();
                }
                j++;

                // UPDATE VARIABLES
                floor = super_container->get_elevator_by_id(id)->get_current_floor();
                current_state = state_enum_to_string(super_container->get_elevator_by_id(id)->get_current_state());
                last_popped_event = event_enum_to_string(event_queue->get_last_popped_event());
                
                // ----------------------------------------------------------------------------------------

                // CREATE DEBUG PRINTS
                mvprintw(0, 1, " ------------------ DEBUG PRINTS -----------------------\n");
                mvprintw(1, 1, "\n ------------------ PHYSICAL ELEVATOR ------------------\n");
                mvprintw(2, 1, "Elevator ID: %s\n", id.id.c_str());
                mvprintw(3, 1, "Current floor: %d\n", floor);
                mvprintw(4, 1, "Current state: %s\n", current_state.c_str());
                mvprintw(5, 1, "Last popped event: %s\n", last_popped_event.c_str());
                mvprintw(6, 1, "\n ------------------ CALL REQUESTS ----------------------\n");
                i = 0;
                for (auto call : super_container->get_locally_assigned_calls()) {
                    mvprintw(8+i, 1, "Call from floor: %d type: %s\n", call->get_floor(), call_type_enum_to_string(call->get_call_type()).c_str());
                    i++;
                }


                mvprintw(20, 1, "\n ------------------ NETWORK ----------------------------\n");
                i = 0;
                for (auto &elevator : super_container->get_alive_elevators()) {
                    mvprintw(22+i, 1, "Elevator with ID: %s is alive\n", elevator.id.c_str()); //NOTE changed this fellar a bit for warnings -A
                    i++;
                }

                mvprintw(30, 1, "\n ------------------ ART ----------------------------\n");
                print_door_open_art(32);

                // Updates variables
                refresh();

                boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
            }
            // Turn off color
            attroff(COLOR_PAIR(1)); 

            // End ncurses
            endwin();

        };
};
