#pragma once

#include <iostream>
#include <filesystem>
#include "BaseFile.hpp"
#include "DirectoryEntry.hpp"
#include "../HashMap/HashMap.hpp"

namespace fs = std::filesystem;

class Directory : public BaseFile
{
    friend std::istream& operator>>(std::istream& in, Directory& directory)
    {
        directory.deserialize(in);
        return in;
    }

    friend std::ostream& operator<<(std::ostream& out, const Directory& directory)
    {
        directory.serialize(out);
        return out;
    }
public:
    Directory() = default;
    explicit Directory(std::string directoryName) : name(std::move(directoryName)) {}
    Directory(const Directory& other) : name(other.name), contents(other.contents) {}
    virtual ~Directory() = default;

    const std::string& getName() const override;
    HashMap<std::string, DirectoryEntry>& getContents()
    {
        return contents;
    }
    const HashMap<std::string, DirectoryEntry>& getContents() const
    {
        return contents;
    }

    void addFile(BaseFile* file)
    {
        contents.insert(file->getName(), DirectoryEntry(file));
    }

    const BaseFile* findFile(const fs::path& filePath) const
    {

        HashMap<std::string, DirectoryEntry>::ConstIterator entry = contents.cbegin();
        const auto* currentContents = &contents;
        for (const fs::path& fileName : filePath.parent_path())
        {
            entry = currentContents->find(fileName.string());
            if (entry == contents.cend())
            {
                throw std::runtime_error("Directory: Find file: File not found!");
            }

            currentContents = &dynamic_cast<const Directory*>(entry->second.getFile())->contents;
        }

        entry = currentContents->find(filePath.filename().string());
        if (entry == contents.cend())
        {
            throw std::runtime_error("Directory: Find file: File not found!");
        }
        return entry->second.getFile();
    }

    void serialize(std::ostream& out) const
    {
        serializeRec(out, const_cast<Directory*>(this));
    }

    void prettyPrint(std::ostream& out, int tabs = 0) const
    {
        for (HashMap<std::string, DirectoryEntry>::ConstIterator entry = contents.cbegin(); entry != contents.cend(); ++entry)
        {
            const BaseFile* base = entry->second.getFile();
            printTabs(out, tabs);
            out << base->getName() << std::endl;

            if (base->getType() == DIRECTORY)
            {
                const Directory* dir = dynamic_cast<const Directory*>(base);
                dir->prettyPrint(out, tabs + 1);
            }
        }
    }

    void printTabs(std::ostream& out, int times) const
    {
        for (int i = 0; i < times; ++i)
        {
            out << '\t';
        }
    }

    void deserialize(std::istream& in)
    {
        char next;
        in >> next;
        if (next != '(')
        {
            throw std::runtime_error("Directory Serialization: Malformed File!");
        }

        std::getline(in, name, ')');

        in >> next;
        if (next != '{')
        {
            throw std::runtime_error("Directory Serialization: Malformed File!");
        }

        next = in.peek();
        while (next != '}' && in.good())
        {
            if (in.peek () == '(')
            {
                Directory* subdir = new Directory();
                subdir->deserialize(in);
                contents.insert(subdir->getName(), subdir);
                in >> next;
                if (next != '}' && next != ',')
                {
                    throw std::runtime_error("Directory Serialization: Malformed File!");
                }
            }
            else
            {
                std::string filename;
                std::string hash;
                std::getline(in, filename, ':');

                hash.resize(64);
                in.read(&hash[0], 64);
                in >> next;

                if (next != ',' && next != '}')
                {
                    throw std::runtime_error("Directory Serialization: Malformed File!");
                }

                contents.insert(filename, new SourceFile(filename, std::move(hash)));

            }
        }

        if (in.bad())
        {
            throw std::runtime_error("Directory Serialization: Malformed File!");
        }
    }

    BaseFile* clone() const override;

    FileType getType() const override;

private:
    std::string name;
    HashMap<std::string, DirectoryEntry> contents;

    void serializeRec(std::ostream& out, Directory* root) const
    {
        out << '(' << root->name << ')';
        out << '{';
        BaseFile* cur;
        HashMap<std::string, DirectoryEntry>::Iterator next = root->contents.begin();
        for (HashMap<std::string, DirectoryEntry>::Iterator it = root->contents.begin(); it != root->contents.end(); ++it)
        {
            cur = it->second.getFile();
            if (cur->getType() == DIRECTORY)
            {
                serializeRec(out, dynamic_cast<Directory*>(cur));
            }
            else
            {
                assert(cur->getType() == SOURCE_FILE);
                SourceFile* src = reinterpret_cast<SourceFile*>(cur);

                out << src->getName() << ':' << src->getHash();
            }

            if (++next != root->contents.end())
            {
                out << ',';
            }
        }
        out << '}' << std::flush;
    }
};