#include "SourceFile.hpp"

SourceFile::SourceFile(std::string name, std::string digest) : name(std::move(name)), digest(std::move(digest)) {}


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

const std::string& SourceFile::getDigest() const
{
    return digest;
}

void SourceFile::setName(const std::string& name)
{
    SourceFile::name = name;
}

void SourceFile::setDigest(const std::string& digest)
{
    SourceFile::digest = digest;
}
