#include "iot.h"
#include "button.h"
#include <vector>

class ButtonModule : public iot::NetworkModule
{
    private:
        std::vector<Button> buttonList;

    public:
        void addButton(Button button);
        void setup();
        void loop();
};