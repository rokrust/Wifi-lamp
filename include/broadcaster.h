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
            void subscribe(callback receiveMsg) { callbacks.push_back(callback); }
            void send(Msg_t message) { for(auto f : callbacks) f(message); }
    };

    template<class ...Channels_t>
    class BroadCaster : Channel<Channels_t>...
    {
        using Channel<Channels_t>::subscribe...;
        using Channel<Channels_t>::send...;

        private:

        public:
    };
}

