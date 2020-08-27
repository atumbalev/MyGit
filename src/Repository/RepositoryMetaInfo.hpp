#pragma once

#include <iostream>

struct RepositoryMetaInfo
{
    int nextCommitId;
    int currentCommitId;
    int headCommitId;
};

inline std::istream& operator>>(std::istream& in, RepositoryMetaInfo& metaInfo)
{
    in >> metaInfo.nextCommitId
       >> metaInfo.currentCommitId
       >> metaInfo.headCommitId;
    return in;
}

inline std::ostream & operator<<(std::ostream& out, const RepositoryMetaInfo& metaInfo)
{
    out << metaInfo.nextCommitId << ' '
        << metaInfo.currentCommitId << ' '
        << metaInfo.headCommitId;
    return out;
}