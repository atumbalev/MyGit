#include "CommandLineParser/Parser.hpp"
#include "HashMap/HashMap.hpp"

int main(int argc, const char* argv[])
{
    if (argc == 1)
    {
        std::cerr << "Input a command to execute" << std::endl;
        return -1;
    }

    ParserSingleton::getInstance().parseCommand(argc, argv);

}