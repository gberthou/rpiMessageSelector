#ifndef MESSAGE_SELECTOR_H
#define MESSAGE_SELECTOR_H

#include <vector>
#include <array>
#include <sys/types.h>

#define MS_BUFFER_SIZE 16

class MessageSelector
{
    public:
        MessageSelector(const std::vector<size_t> &pinmap);
        virtual ~MessageSelector();

        void Refresh();
        size_t GetCurrentMessage() const;

        static size_t INVALID;

    protected:
        void addValue(size_t value);

        std::vector<size_t> pinmap;
        std::array<size_t, MS_BUFFER_SIZE> buffer;
        std::array<size_t, MS_BUFFER_SIZE>::iterator cursor;
        int fd;
        u_int32_t *gpio;
        size_t currentMessage;
};

#endif

