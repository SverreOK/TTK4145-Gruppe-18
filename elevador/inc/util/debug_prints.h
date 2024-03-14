#include <ncurses.h>
#include <chrono>
#include <boost/thread.hpp>

#include "super_container.h"

class Debug_prints {
    private:
        Super_container *super_container;
        Elevator_id id;

    public:
        Debug_prints(Super_container *super_container, Elevator_id id) 
        : super_container(super_container), id(id) {}

        void debug_print_start(Super_container *super_container, Elevator_id id) {

            uint8_t floor;
            
            initscr();

            curs_set(0);

            while (DEBUG) {

                // update variables
                floor = super_container->get_elevator_by_id(id)->get_current_floor();

                clear();
                //floor is int
                mvprintw(0, 0, "Elevator floor: %d", floor);

                refresh();

                boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
            }

            endwin();

        }

};
