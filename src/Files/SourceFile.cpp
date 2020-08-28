#include "SourceFile.hpp"

SourceFile::SourceFile(std::string name, std::string hash) : name(std::move(name)), hash(std::move(hash)) {}


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

const std::string& SourceFile::getHash() const
{
    return hash;
}

void SourceFile::setName(const std::string& name)
{
    SourceFile::name = name;
}

void SourceFile::setDigest(const std::string& newHash)
{
    SourceFile::hash = newHash;
}

bool SourceFile::operator==(const SourceFile& rhs) const
{
    return name == rhs.name &&
           hash == rhs.hash;
}

bool SourceFile::operator!=(const SourceFile& rhs) const
{
    return !(rhs == *this);
}
