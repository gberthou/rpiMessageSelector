#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <vector>

#include<SFML/Window.hpp>
#include<SFML/Graphics.hpp>

#include "MessageSelector.h"

sf::VideoMode getVideoMode()
{
    const std::vector<sf::VideoMode> &modes = sf::VideoMode::getFullscreenModes();
    if(!modes.size())
        throw std::runtime_error("No video mode detected");
    return modes[0];
}

int main(void)
{
    const std::vector<size_t> PINMAP = {10, 9, 11, 5, 6, 13, 19, 26, 12, 16, 20,
                                        21};
    /* gpio 10 -> no message
     * gpio 09 -> message 1
     * gpio 11 -> message 2
     * And so on
     */

    MessageSelector messageSelector(PINMAP);
    sf::RenderWindow window(getVideoMode(), "rpiDisplaySelect");
    window.setFramerateLimit(30);

    std::cout << "Application started" << std::endl;

    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed
            || (event.type == sf::Event::KeyPressed
                && event.key.code == sf::Keyboard::Escape))
            {
                window.close();
            }
        }

        messageSelector.Refresh();

        size_t message = messageSelector.GetCurrentMessage();
        if(message == 0)
            std::cout << "No message";
        else if(message == MessageSelector::INVALID)
            std::cout << "Invalid message";
        else
            std::cout << "Message " << message;
        std::cout << std::endl;

        window.clear();

        window.display();

        usleep(10000); // 10ms
    }

    return EXIT_SUCCESS;
}

