#include "Buffer.h"

size_t Buffer::INVALID = -1;

Buffer::Buffer():
    cursor(data.begin()),
    currentMessage(INVALID)
{
    data.fill(INVALID);
}

void Buffer::AddValue(size_t value)
{
    // Write and shift iterator
    *cursor++ = value;
    if(cursor == data.end())
        cursor = data.begin();
    
    size_t i;
    for(i = 1; i < data.size() && data[i] == data[0]; ++i);
    if(i == data.size())
        currentMessage = data[0];
}

size_t Buffer::GetCurrentMessage() const
{
    return currentMessage;
}

