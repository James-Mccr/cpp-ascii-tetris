#pragma once

class Frame
{
public:
    Frame(long fps);
    
    void limit();

private:
    long milliseconds_per_frame{};
};