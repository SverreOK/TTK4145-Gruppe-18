#include "elevator_driver.h"


int main(int argc, char *argv[]) {
    ElevatorDriver elevator;

    elevator.connect();

    elevator.get_button_signal(0,0);


    return 0;
}