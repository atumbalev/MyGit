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
//    repository.commit("/home/atanas/Documents/sd/testingArea", "atanas", "second commit");
    repository.load();
    repository.log(true);
//    repository.checkout(1);
//    repository.save();
//    std::cout << fs::relative("/home/atanas/Documents/sd/Sort2dArray.c","/home/atanas/Documents/sd") << std::endl;
}