/*
#include<vector>
#include<functional>

namespace iot{
    using namespace std;

    template<class Msg_t>
    class Channel
    {
        using callback = function<Msg_t*>;

        protected:
            vector<callback> callbacks;

        public:
            void subscribe(callback receiveMsg) { callbacks.push_back(receiveMsg); }
            void send(Msg_t message) { for(auto f : callbacks) f(message); }


    };

    template<class ...messages>
    class Broadcaster : Channel<messages>...
    {
        using Channel<messages>::subscribe...;
        using Channel<messages>::send...;

        template<class message>
        Channel<message>& station() { return *this; }
    };
}

*/