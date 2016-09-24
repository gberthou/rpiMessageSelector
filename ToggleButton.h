#ifndef TOGGLE_BUTTON_H
#define TOGGLE_BUTTON_H

#include <sys/types.h>

class ToggleButton
{
    public:
        ToggleButton();
        virtual ~ToggleButton() = default;

        void Press();
        void Release();

        bool IsActivated() const;

    protected:
        u_int8_t state;

};

#endif

