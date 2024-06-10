#pragma once

#include <string>
#include <vector>

using namespace std;

class Grid
{
public:
    Grid(int _width, int _height);

    const vector<string>& GetTiles() const;

    void SetTile(int x, int y, char c = empty);
    void SetRow(int row, const string& rowChars);

    bool IsOutOfBounds(int x, int y);
    bool IsCollision(int x, int y, char c);

    int GetWidth() const;
    int GetHeight() const;

    static constexpr char empty = ' ';

private:
    int width{};
    int height{};
    vector<string> tiles;
};

extern Grid grid;