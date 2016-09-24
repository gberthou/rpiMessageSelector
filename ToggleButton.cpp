#include "ToggleButton.h"

ToggleButton::ToggleButton():
    state(0)
{
}

void ToggleButton::Press()
{
    if(!(state & 1))
        ++state;
}

void ToggleButton::Release()
{
    if(state & 1)
        state = (state + 1) & 3;
}

bool ToggleButton::IsActivated() const
{
    return state != 0;
}

