#pragma once

#include "BaseFile.hpp"
#include "SourceFile.hpp"
#include "Directory.hpp"

class DirectoryEntry
{
public:
    DirectoryEntry() = default;
    DirectoryEntry(BaseFile* file) : file(file) {}
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
    }

    DirectoryEntry& operator=(DirectoryEntry&& other)
    {
        if (this != &other)
        {
            delete file;
            file = other.file;
            other.file = nullptr;
        }
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
            return lhsFile->getDigest() == rhsFile->getDigest();
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