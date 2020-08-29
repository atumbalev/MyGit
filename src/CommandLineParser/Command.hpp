#pragma once

class Command
{
public:
    virtual ~Command() = default;
    virtual bool execute(int argc, const char** argv) = 0;
};