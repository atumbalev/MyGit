#include <cstring>
#include "Command.hpp"
#include "Parser.hpp"
#include "../Repository/Repository.hpp"

class CheckoutCommand : public Command
{
public:
    ~CheckoutCommand() override = default;

    bool execute(int argc, const char** argv) override
    {
        if (strcmp (argv[1], "checkout") == 0)
        {
            if (argc != 3 && argc != 4)
            {
                throw std::runtime_error ("CheckoutCommand: Invalid number of arguments!");
            }

            Repository repository;
            int id;
            if (argc == 4)
            {
                repository = Repository(argv[2]);
                repository.load();

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
            }
            else
            {
                repository.load();
                if (strcmp(argv[2], "HEAD") == 0)
                {
                    std::cout << "head";
                    id = repository.getHeadCommitId();
                }
                else
                {
                    try
                    {
                        id = std::stoi(argv[2]);
                    }
                    catch (const std::invalid_argument& e)
                    {
                        std::cerr << e.what() << std::endl;
                        return true;
                    }
                }
            }

            repository.checkout(id);
            repository.save();

            std::cout << "Repository content reverted to commit with id: " << repository.getHeadCommitId() << std::endl;

            return true;
        }
        else return false;
    }

private:
    static Registerer<CheckoutCommand> reg;
};

Registerer<CheckoutCommand> CheckoutCommand::reg;
