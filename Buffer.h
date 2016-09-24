#ifndef BUFFER_H
#define BUFFER_H

#include <array>

#define BUFFER_SIZE 8

class Buffer
{
    public:
        Buffer();
        virtual ~Buffer() = default;

        void AddValue(size_t value);
        size_t GetCurrentMessage() const;

        static size_t INVALID;

    protected:
        std::array<size_t, BUFFER_SIZE> data;
        std::array<size_t, BUFFER_SIZE>::iterator cursor;
        size_t currentMessage;
};

#endif

