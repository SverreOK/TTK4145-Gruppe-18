//enum for direction

enum Direction {
    UP = 1,
    DOWN = -1,
    STOP = 0
};


class Call {
    public:
        Call(int floor, int direction, int ID, bool reassignable);
        bool get_reassignable();
        int get_floor();
        int get_direction();
    private:
        int floor;
        Direction direction;
        int ID;
        bool reassignable;
};