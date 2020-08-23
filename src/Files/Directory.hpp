#pragma once

#include "BaseFile.hpp"
#include "../HashMap/HashMap.hpp"

class Directory : public BaseFile
{
public:
    virtual ~Directory() = default;

    const std::string& getName() const override;
    HashMap<std::string, BaseFile*> getContents()
    {
        return contents;
    }
    const HashMap<std::string, BaseFile*> getContents() const
    {
        return contents;
    }

    void addFile(const BaseFile* file)
    {
        contents.insert(file->getName(), file->clone());
    }

    BaseFile* findFile(const std::string& fileName)
    {
        auto entry = contents.find(fileName);
        if (entry == contents.end())
        {
            return nullptr;
        }
        return entry->second;
    }

    BaseFile* clone() const override;

    FileType getType() const override;

private:
    std::string name;
    HashMap<std::string, BaseFile*> contents;
};