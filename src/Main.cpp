#include <iostream>
#include <sstream>
#include "Commit/Commit.hpp"
#include "Hashing/HashingFunctions.hpp"

int main()
{
    SourceFile* file1 = new SourceFile("file1", hashString("file1"));
    SourceFile* file2 = new SourceFile("file2", hashString("file2"));
    Directory root("root");
    Directory* subdir = new Directory("subdir");
    subdir->addFile(file1);
    root.addFile(subdir);

    root.addFile(file2);

    std::stringstream ss;
    Commit commit("atanas", "initial commit", root);
    commit.serialize(std::cout);
    commit.serialize(ss);
    std::cout << std::endl;

    HashMap<char, char> map((HashMap<char, char>()));

    Commit commit2;
    commit2.deserialize(ss);
    commit2.serialize(std::cout);
}