#include "SourceFile.hpp"

SourceFile::SourceFile(std::string name, std::vector<unsigned char> digest) : name(std::move(name)), digest(std::move(digest)) {}


const std::string& SourceFile::getName() const
{
    return name;
}

BaseFile* SourceFile::clone() const
{
    return new SourceFile(*this);
}

FileType SourceFile::getType() const
{
    return SOURCE_FILE;
}
