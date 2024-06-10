#include "input.h"
#include <curses.h>
#include <ncurses.h>

UserInput Input::Read()
{
    int i = getch();
    switch (i)
    {
        case 'q': return UserInput::Quit;
        case KEY_LEFT: return UserInput::Left;
        case KEY_RIGHT: return UserInput::Right;
        case KEY_UP: return UserInput::Up;
        case KEY_DOWN: return UserInput::Down;
        case KEY_SPACE: return UserInput::Space;
        default: return UserInput::None;
    }
}

UserInput userInput{};