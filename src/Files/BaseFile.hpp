#pragma once

#include <string>
#include "../Enums/FileType.hpp"

class BaseFile
{
public:
    virtual ~BaseFile() = default;

    bool operator==(const BaseFile& other) const
    {
        return getName() == other.getName() && getType() == other.getType();
    }

    bool operator!=(const BaseFile& other) const
    {
        return !(*this == other);
    }

    virtual const std::string& getName() const = 0;
    virtual BaseFile* clone() const = 0;
    virtual FileType getType() const = 0;
};