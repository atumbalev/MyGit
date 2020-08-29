#include <cstring>
#include "Command.hpp"
#include "Parser.hpp"
#include "../Repository/Repository.hpp"

class InitCommand : public Command
{
public:
    ~InitCommand() override = default;

    bool execute(int argc, const char** argv) override
    {
        if (strcmp (argv[1], "init") == 0)
        {
            if (argc > 3)
            {
                throw std::runtime_error ("InitCommand: Invalid number of arguments!");
            }
            else if (argc == 3)
            {
                Repository repository(argv[2]);
                repository.init();
                repository.save();

                std::cout << "Repository initialized!\n";
            }
            else
            {
                Repository repository;
                repository.init();
                repository.save();

                std::cout << "Repository initialized!\n";
            }

            return true;
        }
        else return false;
    }

private:
    static Registerer<InitCommand> reg;
};

Registerer<InitCommand> InitCommand::reg;