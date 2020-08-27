#pragma once

#include <ctime>
#include <iostream>
#include <iomanip>
#include "../Files/SourceFile.hpp"
#include "../Files/Directory.hpp"

class Commit
{
    #define DELIMITER ','

    friend std::istream& operator>>(std::istream& in, Commit& commit)
    {
        commit.deserialize(in);
        return in;
    }

    friend std::ostream& operator<<(std::ostream& out, const Commit& commit)
    {
        commit.serialize(out);
        return out;
    }

public:
    Commit() = default;
    Commit(int id) : id(id), timestamp(std::time(nullptr)) {}
    Commit(int id, const std::string& author, const std::string& message, const Directory& workTree) : id(id), author(author), message(message), workTree(workTree)
    {
        timestamp = std::time(nullptr);
    }
    Commit(const Commit&) = default;
    ~Commit() = default;

    void serialize(std::ostream& out) const
    {
        out << id << DELIMITER
            << author << DELIMITER
            << message << DELIMITER
            << std::put_time(std::gmtime(&timestamp), "%T %F") << DELIMITER
            << workTree;
    }

    void deserialize(std::istream& in)
    {
        in >> id;
        in.ignore();
        std::getline(in, author, DELIMITER);
        std::getline(in, message, DELIMITER);
        std::tm tm{};
        std::string timeString;
        std::getline(in, timeString, ',');
        strptime(timeString.c_str(), "%T %F", &tm);
        timestamp = std::mktime(&tm);
        in >> workTree;
    }

    void prettyPrint(std::ostream& out, bool printDir = false) const
    {
        out << id << ": "
            << '\"' << message << '\"'
            << " from " << std::put_time(std::localtime(&timestamp), "%T %F")
            << " by " << author << std::endl;

        if (printDir)
        {
            workTree.prettyPrint(out, 1);
        }
    }

    Directory* getWorkTree()
    {
        return &workTree;
    }

    const Directory* getWorkTree() const
    {
        return &workTree;
    }

    int getId() const
    {
        return id;
    }

private:
    int id;
    std::string author;
    std::string message;
    std::time_t timestamp;
    Directory workTree;
};