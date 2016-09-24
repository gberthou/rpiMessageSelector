#ifndef GPIO_H
#define GPIO_H

#include <cstdlib>
#include <vector>

class GPIO
{
    public:
        static void Init();
        static void Close();
        static void SetFunction(size_t pin, unsigned int mode);
        static void SetPullUp(const std::vector<size_t> &pins);
        static u_int32_t Read(size_t pin);
        static void Write(size_t pin, u_int8_t value);

    private:
        static int fd;
        static volatile u_int32_t *gpio;
};

#endif

