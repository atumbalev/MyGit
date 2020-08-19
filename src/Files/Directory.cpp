//
// Created by atanas on 9.08.20 г..
//

#include "Directory.hpp"


const std::string& Directory::getName() const
{
    return name;
}

BaseFile* Directory::clone() const
{
    return new Directory(*this);
}

FileType Directory::getType() const
{
    return DIRECTORY;
}
