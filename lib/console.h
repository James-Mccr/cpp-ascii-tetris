#pragma once

#include <string>

class Console
{
public:
    Console();
    ~Console();

    int width{};
    int height{};

    void moveCursor(unsigned short row, unsigned short column);
    void print(const char c);
    void print(const std::string& str);
    int read();
};