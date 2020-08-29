//
// Created by atanas on 23.08.20 г..
//

#include "Repository.hpp"

Repository::Repository()
{
    configurePaths(fs::current_path());
}

Repository::Repository(const fs::path& repoPath)
{
    configurePaths(repoPath);
}

void Repository::configurePaths(const fs::path& repoPath)
{
    workTree = repoPath;
    gitDir = repoPath / gitDirSubdir;
    commitLog = gitDir / commitLogFile;
    commitLogMetadata = gitDir / commitLogMetadataFile;
    fileStorage = gitDir / fileStorageSubdir;
}

void Repository::init()
{
    if (fs::exists(gitDir))
    {
        throw std::runtime_error("Directory is already a MyGit Repository");
    }

    fs::create_directory(gitDir);

    metaInfo = RepositoryMetaInfo{0, -1, -1};

    Commit init(metaInfo.nextCommitId++);
    metaInfo.currentCommitId = init.getId();
    metaInfo.headCommitId = init.getId();
    commits.push_back(init);

    save();

    fs::create_directory(fileStorage);
}

void Repository::load()
{
    std::ifstream cLogMeta(commitLogMetadata);
    cLogMeta >> metaInfo;
    cLogMeta.close();

    std::ifstream cLog(commitLog);
    commits.clear();
    deserializeCommits(cLog);
    cLog.close();
}

void Repository::save() const
{
    std::ofstream cLogMeta(commitLogMetadata, std::ios::trunc);
    cLogMeta << metaInfo;
    cLogMeta.close();

    std::ofstream cLog(commitLog, std::ios::trunc);
    serializeCommits(cLog);
    cLog.close();
}

void Repository::commit(const std::string& author, const std::string& message)
{
    if (metaInfo.currentCommitId != metaInfo.headCommitId)
    {
        throw std::runtime_error("The Repository HEAD is detached. Cannot commit.");
    }

    Directory* tree = dynamic_cast<Directory*>(buildFileTree(workTree));
    commits.emplace_back(Commit(setPushCommitInfo(), author, message, *tree));
}

int Repository::setPushCommitInfo()
{
    metaInfo.currentCommitId = metaInfo.nextCommitId;
    metaInfo.headCommitId = metaInfo.nextCommitId;
    return metaInfo.nextCommitId++;
}

void Repository::log(bool printDir, std::ostream& out) const
{
    for (const Commit& commit : commits)
    {
        commit.prettyPrint(out, printDir);
    }
}

BaseFile* Repository::buildFileTree(const fs::path& filePath, Repository::Flag flag)
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
                dir->addFileToContents(buildFileTree(path, flag));
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

fs::path Repository::getStoredFile(const SourceFile* srcFile) const
{
    const std::string& hash = srcFile->getHash();
    return fileStorage / hash.substr(0,2) / hash.substr(2, hash.size() - 2);
}

const Directory* Repository::fetchCommitWorkTree(int commitId) const
{
    if (commitId > metaInfo.currentCommitId)
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

void Repository::revert(const fs::path& path, int commitId)
{
    const Directory* tree = fetchCommitWorkTree(commitId);
    const BaseFile* file = tree->findFile(path);
    if (file->getType() == SOURCE_FILE)
    {
        revertSourceFile(dynamic_cast<const SourceFile*>(file), workTree / path);
    }
    else
    {
        fs::remove_all(workTree / path);
        fs::create_directory(workTree / path);
        buildTreeRec(dynamic_cast<const Directory*>(file), workTree / path);
    }
}

void Repository::revertSourceFile(const SourceFile* file, const fs::path& dest) const
{
    fs::path stored = getStoredFile(file);
    fs::remove(dest);
    fs::copy_file(stored, dest);
}

void Repository::checkout(int commitId)
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
    metaInfo.headCommitId = commitId;
}

void Repository::buildTreeRec(const Directory* directory, const fs::path& path)
{
    const BaseFile* file;
    for (auto it = directory->getContents().cbegin(); it != directory->getContents().cend(); ++ it)
    {
        file = *it->second;

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

void Repository::serializeCommits(std::ofstream& out) const
{
    for (const Commit& commit : commits)
    {
        out << commit << std::endl;
    }
}

void Repository::deserializeCommits(std::istream& in)
{
    for (int i = 0; i < metaInfo.currentCommitId; ++i)
    {
        Commit c;
        in >> c;
        commits.emplace_back(c);
    }
}

void Repository::status(std::ostream& out)
{
    const Directory* tree = fetchCommitWorkTree(metaInfo.headCommitId);
    const BaseFile* current = buildFileTree(workTree, BUILD_ONLY);
    if (current->getType() != DIRECTORY)
    {
        throw std::runtime_error("Status: Invalid Working Directory");
    }
    const Directory* currentTree = dynamic_cast<const Directory*>(current);
    const Directory* diffTree = currentTree->compare(tree);

    if (diffTree->getContents().empty())
    {
        out << "The current worktree is even with head!" << std::endl;
    }
    else
    {
        out << "Not commited:" << std::endl;
        diffTree->prettyPrint(out, 1);
    }
}

int Repository::getCurrentCommitId() const
{
    return metaInfo.currentCommitId;
}

int Repository::getHeadCommitId() const
{
    return metaInfo.headCommitId;
}
