#pragma once

#include <vector>
#include "BaseFile.hpp"

class SourceFile : public BaseFile
{
public:
    SourceFile() = default;
    SourceFile(std::string  name, std::string  hash);
    SourceFile(const SourceFile& other) = default;
    SourceFile(SourceFile&& other) = default;

    SourceFile& operator=(const SourceFile& other) = default;
    SourceFile& operator=(SourceFile&& other) = default;

    virtual ~SourceFile() = default;

    virtual const std::string& getName() const override;
    virtual BaseFile* clone() const override;
    virtual FileType getType() const override;

    const std::string& getHash() const;

    void setName(const std::string& name);
    void setDigest(const std::string& newHash);

private:
    std::string name;
    std::string hash;
};