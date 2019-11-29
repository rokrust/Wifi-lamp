#include "iot/iot.h"
#include "button.h"
#include <vector>

class ButtonModule : public iot::Module
{
    private:
        std::vector<Button> buttonList;

    public:
        void addButton(const Button& button);
        void setup();
        void loop();
        ButtonModule* clone() { return this; }
};