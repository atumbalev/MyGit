#include "Parser.hpp"

void Parser::registerCommand(Command* command)
{
    commands[numberOfRegisteredCommands++] = command;
}

void Parser::parseCommand(int argc, const char* argv[])
{
    bool executed = false;
    for (int i = 0; i < numberOfRegisteredCommands && !executed; ++i)
    {
        try
        {
            executed = commands[i]->execute(argc, argv);
        }
        catch (const std::exception& er)
        {
            std::cerr << er.what();
            return;
        }
    }
    if (!executed)
    {
        std::cout << "Invalid Command!" << std::endl;
    }
}

Parser& ParserSingleton::getInstance()
{
    static Parser instance;
    return instance;
}
