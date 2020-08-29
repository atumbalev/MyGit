#pragma once

#include <filesystem>
#include <iostream>
#include <fstream>
#include "../Commit/Commit.hpp"
#include "../Hashing/HashingFunctions.hpp"
#include "RepositoryMetaInfo.hpp"

namespace fs = std::filesystem;

class Repository
{
public:
    Repository();
    explicit Repository(const fs::path& repoPath);
    Repository(const Repository&) = default;
    Repository& operator=(const Repository&) = default;
    ~Repository() = default;

    void load();
    void save() const;

    void init();
    void commit(const std::string& author, const std::string& message);
    void checkout(int commitId);
    void log(bool printDir = false, std::ostream& out = std::cout) const;
    void revert(const fs::path& path, int commitId);
    void status(std::ostream& out = std::cout);

    int getCurrentCommitId() const;
    int getHeadCommitId() const;

private:
    void configurePaths(const fs::path& repoPath);
    int setPushCommitInfo();

    void serializeCommits(std::ofstream& out) const;
    void deserializeCommits(std::istream& in);
    void buildTreeRec(const Directory* directory, const fs::path& path);

    enum Flag {STORE, BUILD_ONLY};
    BaseFile* buildFileTree(const fs::path& filePath, Flag flag = STORE);
    void revertSourceFile(const SourceFile* file, const fs::path& dest) const;
    fs::path getStoredFile(const SourceFile* srcFile) const;
    const Directory* fetchCommitWorkTree(int commitId) const;

private:
    RepositoryMetaInfo metaInfo;
    std::vector<Commit> commits;

    fs::path workTree;
    fs::path gitDir;
    fs::path commitLog;
    fs::path commitLogMetadata;
    fs::path fileStorage;

    static constexpr const char* gitDirSubdir = ".MyGit";
    static constexpr const char* commitLogFile = "Commit.log";
    static constexpr const char* commitLogMetadataFile = "Commit.metadata";
    static constexpr const char* fileStorageSubdir = "FileStorage";
};