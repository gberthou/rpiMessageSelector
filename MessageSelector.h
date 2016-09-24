#ifndef MESSAGE_SELECTOR_H
#define MESSAGE_SELECTOR_H

#include <vector>
#include <array>
#include <sys/types.h>

#include "Buffer.h"

class MessageSelector
{
    public:
        MessageSelector(const std::vector<size_t> &pinmap);
        virtual ~MessageSelector() = default;

        void Refresh();
        size_t GetCurrentMessage() const;

    protected:
        void addValue(size_t value);

        std::vector<size_t> pinmap;
        Buffer buffer;
};

#endif

