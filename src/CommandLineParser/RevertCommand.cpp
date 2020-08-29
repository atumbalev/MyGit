#include <cstring>
#include "Command.hpp"
#include "Parser.hpp"
#include "../Repository/Repository.hpp"

class RevertCommand : public Command
{
public:
    ~RevertCommand() override = default;

    bool execute(int argc, const char** argv) override
    {
        if (strcmp (argv[1], "revert") == 0)
        {
            if (argc != 5 && argc != 4)
            {
                throw std::runtime_error ("RevertCommand: Invalid number of argument!");
            }

            Repository repository;
            int id;
            const char* filename;
            if (argc == 5)
            {
                repository = Repository(argv[2]);

                if (strcmp(argv[3], "HEAD") == 0)
                {
                    id = repository.getHeadCommitId();
                } else
                {
                    try
                    {
                        id = std::stoi(argv[3]);
                    }
                    catch (const std::invalid_argument& e)
                    {
                        std::cerr << e.what() << std::endl;
                        return true;
                    }
                }

                filename = argv[3];
            }
            else
            {
                if (strcmp(argv[3], "HEAD") == 0)
                {
                    id = repository.getCurrentCommitId();;
                }
                else
                {
                    try
                    {
                        id = std::stoi(argv[3]);
                    }
                    catch (const std::invalid_argument& e)
                    {
                        std::cerr << "Input a valid commit id or HEAD" << std::endl;
                        return true;
                    }
                    filename = argv[2];
                }
            }

            repository.load();
            repository.revert(filename, id);
            repository.save();

            std::cout << "File: " << filename << " reverted to its contents in commit with id: " << id << std::endl;
            return true;
        }
        else return false;

    }

private:
    static Registerer<RevertCommand> reg;
};

Registerer<RevertCommand> RevertCommand::reg;