#include "lib/console.h"
#include "lib/frame.h"
#include "lib/grid.h"
#include "lib/input.h"
#include "lib/render.h"
#include <chrono>
#include <stdexcept>
#include <string>

static constexpr int FPS {60}; 

class Coordinate
{
public:
    Coordinate(int _x, int _y)
    {
        x = _x;
        y = _y;
    }

    int x{};
    int y{};
};

enum class Tetrominos
{
    Line,
    Z,    // top left to bottom right diagonal
    Z2,     // top right right to bottom left diagonal
    L,
    L2,
    Square,
    T
};

class Tetromino
{
public:
    Tetromino() : Tetromino(Tetrominos::Line) {}

    Tetromino(Tetrominos _type)
    {
        type = _type;
        switch(type)
        {
            case Tetrominos::Line:
                blocks = {{0,0},{0,1},{0,2},{0,3}};
                break;
            case Tetrominos::Z:
                blocks = {{1,0},{1,1},{0,1},{0,2}}; 
                break;
            case Tetrominos::Z2:
                blocks = {{0,0},{0,1},{1,1},{1,2}};
                break;
            case Tetrominos::L:
                blocks = {{1,0},{0,0},{0,1},{0,2}};
                break;
            case Tetrominos::L2:
                blocks = {{0,0},{1,0},{1,1},{1,2}};
                break;
            case Tetrominos::Square:
                blocks = {{0,0},{0,1},{1,1},{1,0}};
                break;
            case Tetrominos::T:
                blocks = {{0,0},{1,0},{1,1},{2,0}};
                break;
        }

        for (int i = 0; i < blocks.size(); i++)
        {
            blocks[i].x += grid.GetWidth()/2;
            if (grid.IsCollision(blocks[i].x, blocks[i].y, asciiIdle))
                spawnable = false;
            grid.SetTile(blocks[i].x, blocks[i].y, asciiActive);
        }
            

    }

    void Update()
    {
        if (idle) return;
        if (frames++ != 40) return;
        frames = 0;

        for (int i = 0; i < blocks.size(); i++)
        {
            if (!grid.IsOutOfBounds(blocks[i].x, blocks[i].y+1))
                if (!grid.IsCollision(blocks[i].x, blocks[i].y+1, asciiIdle)) 
                    continue;
            idle = true;
            for (int i = 0; i < blocks.size(); i++)
                grid.SetTile(blocks[i].x, blocks[i].y, asciiIdle);
            return;
        }    

        for (int i = 0; i < blocks.size(); i++)
            grid.SetTile(blocks[i].x, blocks[i].y++);

        for (int i = 0; i < blocks.size(); i++)
            grid.SetTile(blocks[i].x, blocks[i].y, asciiActive);
    }

    void Move(int xOffset, int yOffset)
    {
        if (idle) return;

        for (int i = 0; i < blocks.size(); i++)
        {
            if (grid.IsOutOfBounds(blocks[i].x+xOffset, blocks[i].y+yOffset)) return;
            if (grid.IsCollision(blocks[i].x+xOffset, blocks[i].y+yOffset, asciiIdle)) return;
        }

        if (yOffset == 1)
            frames = 0;

        for (int i = 0; i < blocks.size(); i++)
            grid.SetTile(blocks[i].x, blocks[i].y);

        for (int i = 0; i < blocks.size(); i++)
        {
            blocks[i].x += xOffset;
            blocks[i].y += yOffset;
            grid.SetTile(blocks[i].x, blocks[i].y, asciiActive);
        }
    }

    void Turn()
    {        
        if (idle) return;
        if (type == Tetrominos::Square) return;

        int xBase = blocks[1].x;
        int yBase = blocks[1].y;
        vector<Coordinate> newCoordinates{};

        for (int i = 0; i < blocks.size(); i++)
        {
            int xNorm = blocks[i].x - xBase;
            int yNorm = blocks[i].y - yBase;

            int xNew = xBase + yNorm * -1;
            int yNew = yBase + xNorm;

            if (grid.IsOutOfBounds(xNew, yNew))
                return;
            if (grid.IsCollision(xNew, yNew, asciiIdle))
                return;

            newCoordinates.emplace_back(xNew, yNew);
        }

        for (int i = 0; i < blocks.size(); i++)
            grid.SetTile(blocks[i].x, blocks[i].y);        

        for (int i = 0; i < blocks.size(); i++)
        {
            blocks[i].x = newCoordinates[i].x;
            blocks[i].y = newCoordinates[i].y;
            grid.SetTile(blocks[i].x, blocks[i].y, asciiActive);
        }
    }

    bool IsIdle() const { return idle; }
    bool IsSpawnable() const { return spawnable; }

    static constexpr char asciiIdle = '+';
    static constexpr char asciiActive = 'b';

private:
    bool spawnable{true};
    bool idle{};
    int frames{};
    vector<Coordinate> blocks{};
    Tetrominos type{};
};

class RowChecker
{
public:
    RowChecker()
    {
        fullRow = string(grid.GetWidth(), Tetromino::asciiIdle);
    }

    void Update()
    {
        clearedRowCount = 0;
        for (int row = grid.GetHeight()-1; row >= 0; row--)
        {
            if (grid.GetTiles()[row] != fullRow) continue;
            string emptyRow = string(grid.GetWidth(), Grid::empty);
            for (int irow = row; irow > 0; irow--)
                grid.SetRow(irow, grid.GetTiles()[irow-1]);
            row++;
            clearedRowCount++;
        }
    }

    int GetClearedRows() const { return clearedRowCount; }

private:
    string fullRow;
    int clearedRowCount{};
};

class Player
{
public:
    void Update()
    {
        if (userInput == UserInput::Left) tetromino.Move(-1, 0);
        else if (userInput == UserInput::Right) tetromino.Move(1, 0);
        else if (userInput == UserInput::Down) tetromino.Move(0, 1);
        else if (userInput == UserInput::Up) tetromino.Turn();

        if (tetromino.IsIdle())
        {
            tetromino = {static_cast<Tetrominos>(rand()%7)};
            if (!tetromino.IsSpawnable())
            {
                playable = false;
                return;
            }
            rowChecker.Update();
            score += rowChecker.GetClearedRows();
        }
            
        tetromino.Update();
    }

    bool IsPlayable() const { return playable; }
    int GetScore() const { return score; }

private:
    Tetromino tetromino{};
    RowChecker rowChecker{};
    int score{};
    bool playable{true};
};

class Game
{
public:
    void Update()
    {
        player.Update();
        if (!player.IsPlayable())
            over = true;
    }

    bool IsOver() { return over; }
    int GetScore() { return player.GetScore(); }

private:
    Player player;
    bool over{};
};

int main()
{
    srand(time(NULL));
    Console console{};
    Frame frame{FPS};
    Input input{};
    Render render{console};
    Game game{};

    while(1)
    {
        frame.limit();
        userInput = input.Read();
        if (userInput == UserInput::Quit) return 0;
        game.Update();
        if (game.IsOver())
        {
            console.moveCursor(console.height/2, console.width/2-5);
            console.print("Game Over");
            console.moveCursor(console.height/2+1, console.width/2-3);
            console.print("Score: " + to_string(game.GetScore()));
            break;
        }
        render.Draw(grid.GetTiles());
    }

    frame = {1};
    frame.limit();
    frame.limit();
    frame.limit();
    frame.limit();


    return 0;
}