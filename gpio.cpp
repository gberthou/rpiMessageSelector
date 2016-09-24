#include <stdexcept>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "gpio.h"

int GPIO::fd;
volatile u_int32_t *GPIO::gpio;

static void mapGPIO(int *pfd, volatile u_int32_t **peripherals)
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

void GPIO::Init()
{
    mapGPIO(&fd, &gpio);
}

void GPIO::Close()
{
    close(fd);
}

void GPIO::SetFunction(size_t pin, unsigned int mode)
{
    volatile u_int32_t *ptr = gpio + pin / 10;
    size_t shift = (pin % 10) * 3;
    mode &= 7;
    *ptr &= ~(7 << shift);
    *ptr |= (mode << shift);
}

static void wait(unsigned int cycles)
{
    for(volatile unsigned int x = cycles; x--;);
}

void GPIO::SetPullUp(const std::vector<size_t> &pins)
{
    const u_int32_t PULLUP = 2;

    volatile u_int32_t *gppud    = gpio + (0x94>>2);
    volatile u_int32_t *gppudclk = gppud + 1;

    *gppud = PULLUP;
    wait(150);
    for(size_t pin : pins)
        gppudclk[pin >> 5] |= (1 << (pin & 0x1f));
    wait(150);
    *gppud = 0;
    gppudclk[0] = 0;
    gppudclk[1] = 0;
}

u_int32_t GPIO::Read(size_t pin)
{
    volatile u_int32_t *gplev = gpio + (0x34>>2);
    return (gplev[pin >> 5] >> (pin & 0x1f)) & 1;
}

void GPIO::Write(size_t pin, u_int8_t value)
{
    value &= 1;

    size_t offset = (value ? 0x1c : 0x28)>>2;
    volatile u_int32_t *ptr = gpio + offset;
    ptr[pin >> 5] = (1 << (pin & 0x1f));
}

