#include "grid.h"

Grid::Grid(int _width, int _height)
{
    width = _width;
    height = _height;
    tiles = vector(height, string(width, empty));
}   

const vector<string>& Grid::GetTiles() const { return tiles; }

void Grid::SetTile(int x, int y, char c)
{
    tiles[y][x] = c;
}

void Grid::SetRow(int row, const string& rowChars)
{
    tiles[row] = rowChars;
}

bool Grid::IsOutOfBounds(int x, int y)
{
    return x < 0 || x >= width || y < 0 || y >= height;
}

bool Grid::IsCollision(int x, int y, char c)
{
    return tiles[y][x] == c;
}

int Grid::GetWidth() const { return width; }
int Grid::GetHeight() const { return height; }

Grid grid{0,0};