#ifndef BUTTON_LED_H
#define BUTTON_LED_H

#include <sys/types.h>

#include "../ToggleButton.h"
#include "../Buffer.h"

class ButtonLed
{
    public:
        ButtonLed(size_t pinButton, size_t pinLed);
        virtual ~ButtonLed() = default;

        void Refresh();

    protected:
        size_t pinButton;
        size_t pinLed;

        ToggleButton toggle;
        Buffer buffer;
};

#endif

