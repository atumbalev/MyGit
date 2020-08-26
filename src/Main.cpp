#include <iostream>
#include <sstream>
#include "Commit/Commit.hpp"
#include "Hashing/HashingFunctions.hpp"
#include "Repository/Repository.hpp"

int main()
{
    std::stringstream ss;
    Directory dir("testDir");
    dir.addFile(new SourceFile("srcFile", hashString("src")));
    Commit commit("atanas", "initial commit", dir);
    ss << commit;
    std::cout << commit;
    std::cout << std::endl;

    Commit commit2;
    ss >> commit2;
    std::cout << commit2 << std::endl;

    Repository repository("/home/atanas/Documents/sd");
//    repository.init();
    BaseFile* res = repository.storeFile(repository.workTree);
    Directory* resDir = dynamic_cast<Directory*>(res);
    std::cout << *resDir << std::endl;

    auto make = dynamic_cast<Directory*>(resDir->findFile("make"));

    for (auto& it : make->getContents())
    {
        std::cout << it.first << std::endl;
    }

    std::cout << fs::relative("/home/atanas/Documents/sd/go6o/lo6o", repository.gitDir.parent_path()).string() << std::endl;
    std::cout << repository.workTree.relative_path().string() << std::endl;
}