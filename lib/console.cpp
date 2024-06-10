#include "console.h"
#include "grid.h"
#include <curses.h>
#include <ncurses.h>
#include <sys/ioctl.h>
#include <unistd.h>

Console::Console()
{
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    curs_set(0);

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    width = w.ws_col;
    height = w.ws_row;
    grid = {width, height};
}

Console::~Console()
{
    curs_set(1);
    endwin();
}

void Console::moveCursor(unsigned short row, unsigned short column)
{
    move(row, column);
}

void Console::print(const char c)
{
    addch(c);
    refresh();
}

void Console::print(const std::string& str)
{
    addstr(str.data());
    refresh();
}

int Console::read()
{
    return getch();
}
