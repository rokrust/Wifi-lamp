#include <vector>

class Iot;
class NetworkModule;

class Message
{
    private:
        static std::vector<NetworkModule*> _attachedModules;

    public:
        void attach(NetworkModule* module)
        {
            _attachedModules.push_back(module);
        }
    protected:
        string name;
};

class NetworkModule
{
    private:
        IotDevice* _device;
        friend void IotDevice::addModule(NetworkModule* module);
    
    public:
        void setDevice(IotDevice* device);
        virtual void setup() = 0;
        virtual void loop() = 0;

        void unsubscribe(string message);
        void subscribe(string message);
        void send(Message message) { _device->broadCastMessage(message); }
        void receive(const Message& message);
};

class IotDevice
{

    private:
        std::vector<NetworkModule*> _modules;

    public:
        void setup()
        {
            for (int i = 0; i < _modules.size(); i++)
            {
                _modules[i]->setup();
            }
        }

        void loop()
        {
            for (int i = 0; i < _modules.size(); i++)
            {
                _modules[i]->loop();
            }
        }

        void addModule(NetworkModule* module)
        {
            module->_device = this;
            _modules.push_back(module);
        }


        void broadCastMessage(const Message& message)
        {
            for(int i = 0; i < _modules.size(); i++)
            {
                _modules[i]->receive(message);
            }
        }

};