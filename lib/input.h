#pragma once

static constexpr int KEY_SPACE {' '};

enum class UserInput { None, Left, Right, Up, Down, Quit, Space };

class Input
{
public:
    UserInput Read();
};

extern UserInput userInput;