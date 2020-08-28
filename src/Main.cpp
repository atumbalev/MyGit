#include <iostream>
#include <sstream>
#include "Commit/Commit.hpp"
#include "Hashing/HashingFunctions.hpp"
#include "Repository/Repository.hpp"

int main()
{
    Repository repository("/home/atanas/Documents/sd/testingArea");
//    fs::remove_all(repository.gitDir);
//    repository.init();
    repository.load();
    repository.log();
//    repository.checkout(1);
//    repository.commit("/home/atanas/Documents/sd/testingArea", "atanas", "second commit");
    repository.status();
    repository.save();
}