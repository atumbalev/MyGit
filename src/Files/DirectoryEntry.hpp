#pragma once

#include <iostream>
#include "BaseFile.hpp"
#include "SourceFile.hpp"

class DirectoryEntry
{
public:
    DirectoryEntry() = default;
    DirectoryEntry(BaseFile* file);
    DirectoryEntry(const DirectoryEntry& other);
    DirectoryEntry(DirectoryEntry&& other);
    ~DirectoryEntry();

    DirectoryEntry& operator=(const DirectoryEntry& other);
    DirectoryEntry& operator=(DirectoryEntry&& other);

    bool operator==(const DirectoryEntry& rhs) const;
    bool operator!=(const DirectoryEntry& rhs) const;

    BaseFile* operator*();
    const BaseFile* operator*() const;

    BaseFile* operator->();
    const BaseFile* operator->() const;
private:
    BaseFile* file;
};