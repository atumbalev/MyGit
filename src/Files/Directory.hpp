#pragma once

#include "BaseFile.hpp"

class Directory : public BaseFile
{
public:
    virtual ~Directory() = default;

    const std::string& getName() const override;

    BaseFile* clone() const override;

    FileType getType() const override;

private:
    std::string name;
    // Set of BaseFile*w
};