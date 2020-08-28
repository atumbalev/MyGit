#include "Directory.hpp"


const std::string& Directory::getName() const
{
    return name;
}

BaseFile* Directory::clone() const
{
    return new Directory(*this);
}

FileType Directory::getType() const
{
    return DIRECTORY;
}

HashMap<std::string, DirectoryEntry>& Directory::getContents()
{
    return contents;
}

const HashMap<std::string, DirectoryEntry>& Directory::getContents() const
{
    return contents;
}

void Directory::addFileToContents(BaseFile* file)
{
    contents.insert(file->getName(), DirectoryEntry(file));
}

const BaseFile* Directory::findFile(const fs::path& filePath) const
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

        currentContents = &dynamic_cast<const Directory*>(*(entry->second))->contents;
    }

    entry = currentContents->find(filePath.filename().string());
    if (entry == contents.cend())
    {
        throw std::runtime_error("Directory: Find file: File not found!");
    }
    return *entry->second;
}

void Directory::serialize(std::ostream& out) const
{
    serializeRec(out, const_cast<Directory*>(this));
}

void Directory::deserialize(std::istream& in)
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

void Directory::prettyPrint(std::ostream& out, int tabs) const
{
    for (HashMap<std::string, DirectoryEntry>::ConstIterator entry = contents.cbegin(); entry != contents.cend(); ++entry)
    {
        const BaseFile* base = *entry->second;
        printTabs(out, tabs);
        out << base->getName() << std::endl;

        if (base->getType() == DIRECTORY)
        {
            const Directory* dir = dynamic_cast<const Directory*>(base);
            dir->prettyPrint(out, tabs + 1);
        }
    }
}

void Directory::serializeRec(std::ostream& out, Directory* root) const
{
    out << '(' << root->name << ')';
    out << '{';
    BaseFile* cur;
    HashMap<std::string, DirectoryEntry>::Iterator next = root->contents.begin();
    for (HashMap<std::string, DirectoryEntry>::Iterator it = root->contents.begin(); it != root->contents.end(); ++it)
    {
        cur = *it->second;
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

void Directory::printTabs(std::ostream& out, int times)
{
    for (int i = 0; i < times; ++i)
    {
        out << '\t';
    }
}

Directory* Directory::compare(const Directory* other) const
{
    Directory* treeDiff = new Directory(name);

    for (auto it = contents.cbegin(); it != contents.cend(); ++ it)
    {
        const BaseFile* current = *it->second;
        if (other->contents.find(current->getName()) == other->contents.cend())
        {
            treeDiff->addFileToContents(current->clone());
        }
        else if (current->getType() == DIRECTORY)
        {
            const Directory* currentDir = dynamic_cast<const Directory*>(current);
            const Directory* otherDir = dynamic_cast<const Directory*>(other->findFile(currentDir->getName()));

            Directory* res = currentDir->compare(otherDir);
            if (!res->getContents().empty())
            {
                treeDiff->addFileToContents(res);
            }
        }
        else // SourceFile
        {
            const SourceFile* currentSrc = dynamic_cast<const SourceFile*>(current);
            const SourceFile* otherSrc = dynamic_cast<const SourceFile*>(other->findFile(currentSrc->getName()));
            if (*currentSrc != *otherSrc)
            {
                treeDiff->addFileToContents(otherSrc->clone());
            }
        }
    }

    return treeDiff;
}

std::istream& operator>>(std::istream& in, Directory& directory)
{
    directory.deserialize(in);
    return in;
}

std::ostream& operator<<(std::ostream& out, const Directory& directory)
{
    directory.serialize(out);
    return out;
}
