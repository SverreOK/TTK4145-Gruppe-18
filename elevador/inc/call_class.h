#include <string>
#include <vector>

//enum for direction of elevator
enum class button_type {
    UP_HALL = 0,
    DOWN_HALL = 1,
    CAB = -2
};

struct Elevator_id {
    std::string elevator_id;
};

class Call_id {
    private:
        Elevator_id elevator_id;
        int call_number;
    public:

        Call_id(Elevator_id elevator_id, int call_number);

        std::string get_call_id();
        std::string get_call_elevator_id();
        int get_call_number();
};

class Call {
    private:

        int floor;
        button_type call_type;
        Call_id ID;

    public:
        Call(int floor, int direction, int ID_num);
        bool get_reassignable();
        int get_floor();
        button_type get_call_type();
};

class Call_database {
    private:

    std::vector<Call> call_list;

    public:

    std::vector<Call> get_call_list();
    void add_call(Call);
    void change_call(Call call, std::string elevator_id);

};






















/*
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢨⣾⣿⣿⣷⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣷⣪⣽⣿⣿⣦⣄⣀⢀⣀⣀⣀⢀⢀⣀⣠⣴⣶⣿⣿⣿⣿⠏
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣯⣿⣿⣿⣿⣿⣿⣿⡿⣿⣿⣿⡿⠃⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣤⣤⣄⢠⢿⣽⣿⣿⣯⣿⣾⣶⣷⣿⣿⣿⣿⣿⣿⣿⣏⣻⣿⣿⣿⡟⠁⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣷⠀⠀⠀⠀⠈⣿⣿⣿⣿⣮⣿⣻⡯⣽⣿⣿⣾⣿⣿⣻⡿⢿⣫⣿⣾⣾⡗⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⠀⠀⠀⠀⠀⣯⡿⣿⣿⣿⣷⣾⣴⣽⣏⡄⢘⣿⣿⣿⣿⣿⣿⣿⠿⠋⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⣿⣿⣦⣍⠟⡛⠿⣿⣿⣿⣿⣿⣿⣿⠿⣿⣻⡏⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⡠⠤⠦⠤⠤⡇⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣤⡀⠀⠀⠉⠉⠀⣀⣴⣿⣿⣿⠅⠀⠀⠀⠀⠀⠀⠀
⠀⠀⢀⣀⣀⣀⡅⠀⠀⠀⠀⠈⠀⠀⠀⠀⠀⡿⠿⠟⠿⠿⣿⣿⣿⠀⢀⣠⣿⣿⣿⣿⢿⡿⠂⠀⠀⠀⠀⠀⠀⠀
⠀⣠⠊⠀⠀⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠈⠉⠛⣿⣿⣿⣿⣿⠿⠃⠀⠀⠀⠀⠀⠀⠀⠀
⣰⠓⠀⠀⠀⣻⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣙⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⢹⡁⠀⠀⠀⢟⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡾⠿⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⡜⢶⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣬⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠈⠹⢶⣆⡀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣰⡾⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠈⠛⠕⠲⠤⢤⢀⣀⣀⣀⣀⣀⣠⣤⡤⠼⠟⠛⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠁⠀⠀⠈⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
*/