#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
#include <vector>
#include <cstring>

#include "MessageSelector.h"
#include "Buffer.h"
#include "gpio.h"

#include "peripherals/ButtonLed.h"

inline bool fileExists(const char *filename)
{
    std::ifstream file(filename);
    return file.good();
}

static void displayImage(const char *filename, pid_t *child)
{
    if(*child > 0)
    {
        if(kill(*child, SIGTERM) == 0)
            waitpid(*child, NULL, WEXITED);
    }
    *child = fork();
    if(*child == 0)
    {
        const char *argv[] = {"/usr/bin/fbi", "-noverbose", filename, NULL};
        execv(argv[0], const_cast<char**>(argv));
        exit(EXIT_FAILURE);
    }
}

static void getAnimationFrames(const char *dirname,
                               std::vector<std::string> &frames)
{
    size_t i = 0;
    for(;;)
    {
        std::ostringstream oss;
        oss << dirname << i << ".png";
        if(!fileExists(oss.str().c_str()))
            break;
        frames.push_back(oss.str());
        ++i;
    }
}

static void displayAnimation(const char *dirname, pid_t *child)
{
    if(*child > 0)
    {
        if(kill(*child, SIGTERM) == 0)
            waitpid(*child, NULL, WEXITED);
    }
    *child = fork();
    if(*child == 0)
    {
        std::vector<const char*> argv{"/usr/bin/fbi", "-noverbose", "-t", "1"};
        std::vector<std::string> frames;
        getAnimationFrames(dirname, frames);

        std::vector<char *> tmp;
        tmp.reserve(frames.size());
        for(std::string name : frames)
        {
            char *s = new char[name.size()+1];
            strcpy(s, name.c_str());
            tmp.push_back(s);
            argv.push_back(s);
        }
        argv.push_back(NULL);
        for(auto i : argv) std::cout << i << std::endl;

        execv(argv[0], const_cast<char**>(argv.data()));
        for(char *s : tmp)
            delete [] s;
        exit(EXIT_FAILURE);
    }
}

int main(void)
{
    const std::vector<size_t> PINMAP = {6, 5, 11, 8, 7, 12, 16, 20, 21, 26, 19,
                                        13};
    /* gpio 06 -> message 1
     * gpio 05 -> message 2
     * gpio 11 -> message 3
     * And so on */

    const char EMPTY_FILENAME[] = "/mnt/empty.png";

    GPIO::Init();

    ButtonLed buttonLed(15, 14);
    MessageSelector messageSelector(PINMAP);
    std::cout << "Application started" << std::endl;
    size_t lastMessage = Buffer::INVALID;
    bool lastButtonState = false;
    
    pid_t child = 0;
    displayImage(EMPTY_FILENAME, &child);
    for(;;)
    {
        buttonLed.Refresh();
        messageSelector.Refresh();

        if(buttonLed.IsActivated())
        {
            size_t newMessage = messageSelector.GetCurrentMessage();
            if(lastMessage != newMessage)
            {
                lastMessage = newMessage;
                if(newMessage != Buffer::INVALID)
                {
                    std::ostringstream oss;
                    oss << "/mnt/" << newMessage << "/";
                    displayAnimation(oss.str().c_str(), &child);
                }
            }
            lastButtonState = true;
        }
        else if(lastButtonState)
        {
            displayImage(EMPTY_FILENAME, &child);
            lastMessage = Buffer::INVALID;
            lastButtonState = false;
        }
        usleep(10000); // 10ms
    }

    GPIO::Close();

    return EXIT_SUCCESS;
}

