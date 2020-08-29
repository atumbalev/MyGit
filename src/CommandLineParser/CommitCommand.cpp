#include <cstring>
#include "Command.hpp"
#include "Parser.hpp"
#include "../Repository/Repository.hpp"

class CommitCommand : public Command
{
public:
    ~CommitCommand() override = default;

    bool execute(int argc, const char** argv) override
    {
        if (strcmp (argv[1], "commit") == 0)
        {
            if (argc != 4 && argc != 5)
            {
                throw std::runtime_error ("CommitCommand: invalid number of arguments!");
            }

            Repository repository;

            if (argc == 5)
            {
                repository = Repository(argv[2]);

                repository.load();

                repository.commit(argv[3], argv[4]);
                repository.save();
            }
            else
            {
                repository.load();

                repository.commit(argv[2], argv[3]);
                repository.save();
            }

            std::cout << "Changes commited with id: " << repository.getCurrentCommitId() << "\n";
            return true;
        }
        else return false;
    }

private:
    static Registerer<CommitCommand> reg;
};

Registerer<CommitCommand> CommitCommand::reg;