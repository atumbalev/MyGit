#pragma once

#include <iostream>
#include "BaseFile.hpp"
#include "SourceFile.hpp"

class DirectoryEntry
{
public:
    DirectoryEntry() = delete;
    DirectoryEntry(BaseFile* file) : file(file->clone()) {}
    DirectoryEntry(const DirectoryEntry& other) : file(other.file->clone()) {}
    DirectoryEntry(DirectoryEntry&& other) : file(other.file)
    {
        other.file = nullptr;
    }
    ~DirectoryEntry()
    {
        delete file;
    }

    DirectoryEntry& operator=(const DirectoryEntry& other)
    {
        if (this != &other)
        {
            delete file;
            file = other.file->clone();
        }
        return *this;
    }

    DirectoryEntry& operator=(DirectoryEntry&& other)
    {
        if (this != &other)
        {
            delete file;
            file = other.file;
            other.file = nullptr;
        }
        return *this;
    }

    bool operator==(const DirectoryEntry& rhs) const
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

    bool operator!=(const DirectoryEntry& rhs) const
    {
        return !(rhs == *this);
    }

    BaseFile* getFile()
    {
        return file;
    }

    const BaseFile* getFile() const
    {
        return file;
    }

    void setFile(BaseFile* newFile)
    {
        file = newFile;
    }

private:
    BaseFile* file;
};