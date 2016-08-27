#include <stdexcept>

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "MessageSelector.h"

size_t MessageSelector::INVALID = -1;

static void mapGPIO(int *pfd, u_int32_t **peripherals)
{
    *pfd = open("/dev/mem", O_RDWR | O_SYNC);
    if(*pfd < 0)
        throw std::runtime_error("Cannot open /dev/mem");
    *peripherals = static_cast<u_int32_t*>(
            mmap(0, 0xff, PROT_READ | PROT_WRITE, MAP_SHARED, *pfd,
                        0x20200000));
    if(*peripherals == MAP_FAILED)
        throw std::runtime_error("Cannot map GPIO");

}

static void setGPIOFunction(u_int32_t *gpio, size_t pin, unsigned int mode)
{
    u_int32_t *ptr = gpio + pin / 10;
    size_t shift = (pin % 10) * 3;
    mode &= 7;
    *ptr &= ~(7 << shift);
    *ptr |= (mode << shift);
}

static void wait(unsigned int cycles)
{
    for(volatile unsigned int x = cycles; x--;);
}

static void setGPIOPullUp(u_int32_t *gpio, const std::vector<size_t> &pins)
{
    const u_int32_t PULLUP = 2;

    u_int32_t *gppud    = gpio + (0x94>>2);
    u_int32_t *gppudclk = gppud + 1;

    *gppud = PULLUP;
    wait(150);
    for(size_t pin : pins)
        gppudclk[pin >> 5] |= (1 << (pin & 0x1f));
    wait(150);
    *gppud = 0;
    gppudclk[0] = 0;
    gppudclk[1] = 0;
}

static void initGPIO(u_int32_t *gpio, const std::vector<size_t> &pinmap)
{
    const unsigned int GPIO_INPUT = 0;

    for(size_t pin : pinmap)
        setGPIOFunction(gpio, pin, GPIO_INPUT);
    setGPIOPullUp(gpio, pinmap);
}

static u_int32_t readGPIO(u_int32_t *gpio, size_t pin)
{
    u_int32_t *gplev = gpio + (0x34>>2);
    return (gplev[pin >> 5] >> (pin & 0x1f)) & 1;
}

MessageSelector::MessageSelector(const std::vector<size_t> &pm):
    pinmap(pm),
    cursor(buffer.begin()),
    currentMessage(INVALID)
{
    mapGPIO(&fd, &gpio);
    initGPIO(gpio, pm);

    for(size_t i = 0; i < buffer.size(); ++i)
        buffer[i] = INVALID;
}

MessageSelector::~MessageSelector()
{
    close(fd);
}

void MessageSelector::Refresh()
{
    bool found = false;
    for(size_t i = 0; i < pinmap.size() && !found; ++i)
    {
        if(!readGPIO(gpio, pinmap[i]))
        {
            found= true;
            addValue(i);
        }
    }
    if(!found)
        addValue(INVALID);

    size_t i;
    for(i = 1; i < buffer.size() && buffer[i] == buffer[0]; ++i);
    if(i == buffer.size())
        currentMessage = buffer[0];
}

size_t MessageSelector::GetCurrentMessage() const
{
    return currentMessage;
}

void MessageSelector::addValue(size_t value)
{
    // Write and shift iterator
    *cursor++ = value;
    if(cursor == buffer.end())
        cursor = buffer.begin();
}

