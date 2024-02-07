class Call {
    public:
        Call(int floor, int direction, int ID, bool reassignable);
        bool get_reassignable();
        int get_floor();
        int get_direction();
    private:
        int floor;
        int direction;
        int ID;
        bool reassignable;
};