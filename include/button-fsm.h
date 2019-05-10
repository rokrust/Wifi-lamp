enum States { S, SS, L, LS, SL, LL };

class Fsm
{
    private:
        States currentState;

    public:
        void input();
};