#include <iostream>
#include <cstdlib>
#include <unistd.h>

#include "MessageSelector.h"

int main(void)
{
    const std::vector<size_t> PINMAP = {10, 9, 11, 5, 6, 13, 19, 26, 12, 16, 20,
                                        21};
    MessageSelector messageSelector(PINMAP);

    std::cout << "Application started" << std::endl;

    for(;;)
    {
        messageSelector.Refresh();

        size_t message = messageSelector.GetCurrentMessage();
        if(message == 0)
            std::cout << "No message";
        else if(message == MessageSelector::INVALID)
            std::cout << "Invalid message";
        else
            std::cout << "Message " << message;
        std::cout << std::endl;

        usleep(10000); // 10ms
    }

    /*
    size_t lastValue = -1;
    for(;;)
    {
        bool found = false;
        for(size_t i = 0; i < PINMAP.size() && !found; ++i)
        {
            if(!readGPIO(gpio, PINMAP[i]))
            {
                found= true;
                if(i != lastValue)
                {
                    lastValue = i;

                    if(i == 0)
                        std::cout << "No message";
                    else
                        std::cout << "Message " << i;
                    std::cout << std::endl;
                }
            }
        }
        if(!found && lastValue != -1)
        {
            lastValue = -1;
            std::cout << "Invalid message" << std::endl;
        }
    }
    */

    return EXIT_SUCCESS;
}

