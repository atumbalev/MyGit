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
    Repository()
    {
        configurePaths(fs::current_path());
    }

    explicit Repository(const fs::path& repoPath)
    {
        configurePaths(repoPath);
    }

    void configurePaths(const fs::path& repoPath)
    {
        workTree = repoPath;
        gitDir = repoPath / gitDirSubdir;
        commitLog = gitDir / commitLogFile;
        commitLogMetadata = gitDir / commitLogMetadataFile;
        fileStorage = gitDir / fileStorageSubdir;
    }

    void init()
    {
        if (fs::exists(gitDir))
        {
            throw std::runtime_error("Directory is already a MyGit Repository");
        }

        fs::create_directory(gitDir);

        info = RepositoryMetaInfo{0, -1, -1};

        Commit init(info.nextCommitId++);
        info.currentCommitId = init.getId();
        info.headCommitId = init.getId();
        commits.push_back(init);

        save();

        fs::create_directory(fileStorage);
    }

    void load()
    {
        std::ifstream cLogMeta(commitLogMetadata);
        cLogMeta >> info;
        cLogMeta.close();

        std::ifstream cLog(commitLog);
        commits.clear();
        deserializeCommits(cLog);
        cLog.close();
    }

    void save() const
    {
        std::ofstream cLogMeta(commitLogMetadata, std::ios::trunc);
        cLogMeta << info;
        cLogMeta.close();

        std::ofstream cLog(commitLog, std::ios::trunc);
        serializeCommits(cLog);
        cLog.close();
    }

    void commit(const fs::path& repoPath, const std::string& author, const std::string& message)
    {
        load();
        Directory* tree = dynamic_cast<Directory*>(buildFileTree(repoPath));
        commits.emplace_back(Commit(setPushCommitInfo(), author, message, *tree));
        save();
    }

    int setPushCommitInfo()
    {
        info.currentCommitId = info.nextCommitId;
        info.headCommitId = info.nextCommitId;
        return info.nextCommitId++;
    }

    void log(bool printDir = false, std::ostream& out = std::cout)
    {
        for (const Commit& commit : commits)
        {
            commit.prettyPrint(out, printDir);
        }
    }

    enum Flag {STORE, BUILD_ONLY};

    BaseFile* buildFileTree(const fs::path& filePath, Flag flag = STORE)
    {
        BaseFile* file;
        if (fs::is_directory(filePath))
        {
            file = new Directory(filePath.filename());
            Directory* dir = dynamic_cast<Directory*>(file);
            for (auto& path : fs::directory_iterator(filePath))
            {
                if (path != gitDir)
                {
                    dir->addFile(buildFileTree(path, flag));
                }
            }
        }
        else
        {
            std::string fileHash = hashFile(filePath);
            fs::path fileToStorePath = fileStorage / fileHash.substr(0, 2) / fileHash.substr(2, fileHash.size() - 2);

            if (flag == BUILD_ONLY || fs::exists(fileToStorePath))
            {
                return new SourceFile(filePath.filename(), fileHash);
            }

            if (!fs::exists(fileToStorePath.parent_path()))
            {
                fs::create_directory(fileToStorePath.parent_path());
            }

            fs::copy_file(filePath, fileToStorePath);

            file = new SourceFile(filePath.filename(), fileHash);
        }

        return file;
    }

    fs::path getStoredFile(const SourceFile* srcFile)
    {
        const std::string& hash = srcFile->getHash();
        return fileStorage / hash.substr(0,2) / hash.substr(2, hash.size() - 2);
    }

    const Directory* fetchCommitWorkTree(int commitId)
    {
        if (commitId > info.currentCommitId)
        {
            throw std::runtime_error("Repository: there is no commit with such id");
        }

        const Directory* tree;
        for (const Commit& c : commits)
        {
            if(c.getId() == commitId)
            {
                tree = c.getWorkTree();
            }
        }
        return tree;
    }

    void revertFile(const fs::path& path, int commitId)
    {
        const Directory* tree = fetchCommitWorkTree(commitId);
        const BaseFile* file = tree->findFile(path);
        assert(file->getType() == SOURCE_FILE);
        revertSourceFile(dynamic_cast<const SourceFile*>(file), workTree / path);
    }

    void revertSourceFile(const SourceFile* file, const fs::path& dest)
    {
        fs::path stored = getStoredFile(file);
        fs::remove(dest);
        fs::copy_file(stored, dest);
    }

    void checkout(int commitId)
    {
        const Directory* tree = fetchCommitWorkTree(commitId);
        for (const fs::path& path : fs::directory_iterator(workTree))
        {
            if (path != gitDir)
            {
                fs::remove_all(path);
            }
        }

        buildTreeRec(tree, workTree);
        info.headCommitId = commitId;
    }

    void buildTreeRec(const Directory* directory, const fs::path path)
    {
        const BaseFile* file;
        for (auto it = directory->getContents().cbegin(); it != directory->getContents().cend(); ++ it)
        {
            file = it->second.getFile();

            if (file->getType() == SOURCE_FILE)
            {
                const SourceFile* src = dynamic_cast<const SourceFile*>(file);
                revertSourceFile(src, path / src->getName());
            }
            else
            {
                const Directory* dir = dynamic_cast<const Directory*>(file);
                fs::create_directory(path / dir->getName());
                buildTreeRec(dir, path / dir->getName());
            }
        }
    }

    void serializeCommits(std::ofstream& out) const
    {
        for (const Commit& commit : commits)
        {
            out << commit << std::endl;
        }
    }

    void deserializeCommits(std::istream& in)
    {
        for (int i = 0; i < info.currentCommitId; ++i)
        {
            Commit c;
            in >> c;
            commits.emplace_back(c);
        }
    }

//private:
    RepositoryMetaInfo info;
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