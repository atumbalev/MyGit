#pragma once

#include <iostream>
#include "Command.hpp"

class Parser
{
public:
    Parser() : numberOfRegisteredCommands(0) {}
    void registerCommand(Command* command);
    void parseCommand(int argc, const char *argv[]);
private:
    static constexpr unsigned capacityOfCommands = 7;
    Command* commands[capacityOfCommands];
    unsigned numberOfRegisteredCommands;
};

class ParserSingleton
{
public:
    static Parser& getInstance();
    ParserSingleton() = delete;
    ParserSingleton(const ParserSingleton&) = delete;
    ParserSingleton& operator=(const ParserSingleton&) = delete;
};

template <typename Command>
class Registerer
{
public:
    Registerer () { ParserSingleton::getInstance().registerCommand(new Command); }
};
