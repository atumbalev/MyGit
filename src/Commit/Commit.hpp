#pragma once

#include <ctime>
#include <iostream>
#include <iomanip>
#include "../Files/SourceFile.hpp"
#include "../Files/Directory.hpp"

class Commit
{
#define DELIMITER ','
public:
    Commit() = default;
    Commit(const std::string& author, const std::string& message, const Directory& workTree) : author(author), message(message), workTree(workTree)
    {
        timestamp = std::time(nullptr);
    }
    Commit(const Commit&) = default;
    ~Commit() = default;

    void serialize(std::ostream& out)
    {
        out << author << DELIMITER
            << message << DELIMITER
            << std::put_time(std::localtime(&timestamp), "%T %F") << DELIMITER
            << workTree;
    }

    void deserialize(std::istream& in)
    {
        std::getline(in, author, DELIMITER);
        std::getline(in, message, DELIMITER);
        std::tm tm{};
        std::string timeString;
        std::getline(in, timeString, ',');
        strptime(timeString.c_str(), "%T %F", &tm);
        timestamp = std::mktime(&tm);
        in >> workTree;
    }

private:
    std::string author;
    std::string message;
    std::time_t timestamp;
    Directory workTree;
};