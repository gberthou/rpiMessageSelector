#include <sys/types.h>

#include "MessageSelector.h"
#include "gpio.h"

MessageSelector::MessageSelector(const std::vector<size_t> &pm):
    pinmap(pm)
{
    const unsigned int GPIO_INPUT = 0;

    for(size_t pin : pinmap)
        GPIO::SetFunction(pin, GPIO_INPUT);
    GPIO::SetPullUp(pinmap);
}

void MessageSelector::Refresh()
{
    bool found = false;
    for(size_t i = 0; i < pinmap.size() && !found; ++i)
    {
        if(!GPIO::Read(pinmap[i]))
        {
            found= true;
            buffer.AddValue(i);
        }
    }
    if(!found)
        buffer.AddValue(Buffer::INVALID);
}

size_t MessageSelector::GetCurrentMessage() const
{
    return buffer.GetCurrentMessage();
}

