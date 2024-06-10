#include "render.h"

Render::Render(const Console& _console)
{
    console = _console;
}

void Render::Draw(const std::vector<std::string>& map)
{
    for (int row = 0; row < map.size(); row++)
    {
        console.moveCursor(row, 0);
        console.print(map[row]);
    }
}