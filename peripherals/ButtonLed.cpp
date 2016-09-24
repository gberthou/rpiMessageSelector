#include "ButtonLed.h"
#include "../gpio.h"


ButtonLed::ButtonLed(size_t pButton, size_t pLed):
    pinButton(pButton),
    pinLed(pLed)
{
    const size_t GPIO_INPUT = 0;
    const size_t GPIO_OUTPUT = 1;

    GPIO::SetFunction(pinButton, GPIO_INPUT);
    GPIO::SetFunction(pinLed, GPIO_OUTPUT);

    GPIO::SetPullUp({pinButton});
    GPIO::Write(pinLed, 0);
}

void ButtonLed::Refresh()
{
    buffer.AddValue(GPIO::Read(pinButton));

    size_t msg = buffer.GetCurrentMessage();
    if(msg)
        toggle.Release();
    else
        toggle.Press();

    GPIO::Write(pinLed, toggle.IsActivated() ? 1 : 0);
}

