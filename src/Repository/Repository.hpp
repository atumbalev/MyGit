#pragma once

#include <filesystem>
#include <iostream>
#include <fstream>
#include "../Commit/Commit.hpp"
#include "../Hashing/HashingFunctions.hpp"

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

        std::ofstream clog(commitLog);
        clog.close();

        std::ofstream cLogMeta(commitLogMetadata);
        cLogMeta.close();

        fs::create_directory(fileStorage);
    }

    BaseFile* storeFile(const fs::path& filePath)
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
                    dir->addFile(storeFile(path));
                }
            }
        }
        else
        {
            std::string fileHash = hashFile(filePath);
            fs::path fileToStorePath = fileStorage / fileHash.substr(0, 2) / fileHash.substr(2, fileHash.size() - 2);

            if (fs::exists(fileToStorePath))
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

    void revertFile(BaseFile* file, fs::path path)
    {
        fs::path relative = fs::relative(path, workTree);
        if (file->getType() == SOURCE_FILE)
        {
            SourceFile* src = dynamic_cast<SourceFile*>(file);
            fs::path storedPath = getStoredFile(src);
            fs::remove(relative);
            fs::copy_file(storedPath, path);
        }
        else // DIRECTORY
        {
            Directory* dir = dynamic_cast<Directory*>(file);
            for (auto& it : dir->getContents())
            {
                revertFile(it.second.getFile(), relative);
            }
        }
    }

//private:
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