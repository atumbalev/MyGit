#include "DirectoryEntry.hpp"

DirectoryEntry::DirectoryEntry(BaseFile* file) : file(file) {}

DirectoryEntry::DirectoryEntry(const DirectoryEntry& other) : file(other.file->clone()) {}

DirectoryEntry::DirectoryEntry(DirectoryEntry&& other) : file(other.file)
{
    other.file = nullptr;
}

DirectoryEntry::~DirectoryEntry()
{
    delete file;
}

DirectoryEntry& DirectoryEntry::operator=(const DirectoryEntry& other)
{
    if (this != &other)
    {
        delete file;
        file = other.file->clone();
    }
    return *this;
}

DirectoryEntry& DirectoryEntry::operator=(DirectoryEntry&& other)
{
    if (this != &other)
    {
        delete file;
        file = other.file;
        other.file = nullptr;
    }
    return *this;
}

bool DirectoryEntry::operator==(const DirectoryEntry& rhs) const
{
    if (file->getType() != rhs.file->getType())
    {
        return false;
    }

    if (file->getName() != rhs.file->getName())
    {
        return false;
    }

    if (file->getType() == SOURCE_FILE)
    {
        const SourceFile* lhsFile = reinterpret_cast<SourceFile*>(file);
        const SourceFile* rhsFile = reinterpret_cast<SourceFile*>(rhs.file);
        return lhsFile->getHash() == rhsFile->getHash();
    }

    return true;
}

bool DirectoryEntry::operator!=(const DirectoryEntry& rhs) const
{
    return !(rhs == *this);
}

BaseFile* DirectoryEntry::operator*()
{
    return file;
}

const BaseFile* DirectoryEntry::operator*() const
{
    return file;
}

BaseFile* DirectoryEntry::operator->()
{
    return file;
}

const BaseFile* DirectoryEntry::operator->() const
{
    return file;
}
