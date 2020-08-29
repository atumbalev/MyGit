#include <cstring>
#include "Command.hpp"
#include "Parser.hpp"
#include "../Repository/Repository.hpp"

class LogCommand : public Command
{
public:
    ~LogCommand() override = default;

    bool execute(int argc, const char** argv) override
    {
        if (strcmp (argv[1], "log") == 0)
        {
            if (argc != 2 && argc != 3)
            {
                throw std::runtime_error ("LogCommand: Invalid number of arguments!");
            }

            Repository repository;
            if (argc == 3)
            {
                repository = Repository(argv[2]);
            }

            repository.load();
            repository.log();
            repository.save();


            return true;
        }
        else return false;
    }

private:
    static Registerer<LogCommand> reg;
};

Registerer<LogCommand> LogCommand::reg;
