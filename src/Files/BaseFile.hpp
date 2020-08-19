#pragma once

#include <string>
#include "../Enums/FileType.hpp"

class BaseFile
{
public:
    virtual ~BaseFile() = default;

    virtual const std::string& getName() const = 0;
    virtual BaseFile* clone() const = 0;
    virtual FileType getType() const = 0;
};