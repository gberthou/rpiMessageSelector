#include <iostream>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <vector>

#include<SFML/Window.hpp>
#include<SFML/Graphics.hpp>

#include "MessageSelector.h"
#include "Buffer.h"
#include "gpio.h"

#include "peripherals/ButtonLed.h"

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
    const std::vector<sf::Color> COLORS = {
        sf::Color(0,0,255), sf::Color(255,0,0), sf::Color(0,255,0)};

    /* gpio 10 -> no message
     * gpio 09 -> message 1
     * gpio 11 -> message 2
     * And so on */

    GPIO::Init();

    ButtonLed buttonLed(15, 14);
    MessageSelector messageSelector(PINMAP);
    sf::RenderWindow window(getVideoMode(), "rpiDisplaySelect",
                            sf::Style::Fullscreen);
    window.setFramerateLimit(30);

    std::cout << window.getSize().x << 'x' << window.getSize().y << std::endl;
    std::cout << "Application started" << std::endl;

    size_t lastMessage = Buffer::INVALID;

    window.clear();
    window.display();

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

        buttonLed.Refresh();
        messageSelector.Refresh();

        size_t message = messageSelector.GetCurrentMessage();
        if(message != Buffer::INVALID && message < COLORS.size()
        && message != lastMessage)
        {
            sf::Vector2f size;
            size.x = window.getSize().x;
            size.y = window.getSize().y;

            sf::RectangleShape rectangle(size);
            rectangle.setFillColor(COLORS[message]);

            window.draw(rectangle);
            window.display();
            
            lastMessage = message;
        }
        usleep(10000); // 10ms

    }

    GPIO::Close();

    return EXIT_SUCCESS;
}

