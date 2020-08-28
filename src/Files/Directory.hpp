#pragma once

#include <iostream>
#include <filesystem>
#include "BaseFile.hpp"
#include "DirectoryEntry.hpp"
#include "../HashMap/HashMap.hpp"

namespace fs = std::filesystem;

class Directory : public BaseFile
{
    friend std::istream& operator>>(std::istream& in, Directory& directory);
    friend std::ostream& operator<<(std::ostream& out, const Directory& directory);
public:
    Directory() = default;
    explicit Directory(std::string directoryName) : name(std::move(directoryName)) {}
    Directory(const Directory& other) : name(other.name), contents(other.contents) {}
    virtual ~Directory() = default;

    const std::string& getName() const override;
    HashMap<std::string, DirectoryEntry>& getContents();
    const HashMap<std::string, DirectoryEntry>& getContents() const;

    void addFileToContents(BaseFile* file);
    const BaseFile* findFile(const fs::path& filePath) const;
    Directory* compare(const Directory* other) const;

    void serialize(std::ostream& out) const;
    void deserialize(std::istream& in);

    void prettyPrint(std::ostream& out, int tabs = 0) const;

    BaseFile* clone() const override;
    FileType getType() const override;

private:

    std::string name;
    HashMap<std::string, DirectoryEntry> contents;

    void serializeRec(std::ostream& out, Directory* root) const;

    static void printTabs(std::ostream& out, int times);
};