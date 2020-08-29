#include <cstring>
#include "Command.hpp"
#include "Parser.hpp"
#include "../Repository/Repository.hpp"

class StatusCommand : public Command
{
public:
    ~StatusCommand() override = default;

    bool execute(int argc, const char** argv) override
    {
        if (strcmp (argv[1], "status") == 0)
        {
            if (argc > 3)
            {
                throw std::runtime_error ("LogCommand: Invalid number of arguments!");
            }

            Repository repository;
            if (argc == 3)
            {
                Repository repository(argv[2]);
            }

            repository.load();
            repository.status();
            repository.save();

            return true;
        }
        else return false;
    }

private:
    static Registerer<StatusCommand> reg;
};

Registerer<StatusCommand> StatusCommand::reg;