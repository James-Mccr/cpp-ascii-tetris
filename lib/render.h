#pragma once

#include "console.h"
#include <string>
#include <vector>

class Render
{
public:
    Render(const Console& _console);

    void Draw(const std::vector<std::string>& map);

private:
    Console console{};
};