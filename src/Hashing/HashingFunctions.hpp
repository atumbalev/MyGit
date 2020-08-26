#pragma once

#include <openssl/sha.h>
#include <filesystem>
#include <sstream>
#include <fstream>

namespace fs = std::filesystem;

inline std::string hashFile(const fs::path& filePath)
{
    unsigned char digest[SHA256_DIGEST_LENGTH];

    SHA256_CTX ctx;
    SHA256_Init(&ctx);

    std::ifstream file(filePath);
    if (!file)
    {
        throw std::runtime_error("Hash File: File not found!");
    }

    const size_t BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];

    while (file)
    {
        file.read (buffer, BUFFER_SIZE - 1);
        SHA256_Update(&ctx, buffer, file.gcount());
    }

    if (!file.eof ())
    {
        throw std::runtime_error("Hash File: Could not read whole file!");
    }

    SHA256_Final(digest, &ctx);

    // Write byte string to hex string
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
    }

    return ss.str();
}

inline std::string hashString(const std::string &str)
{
    unsigned char digest[SHA256_DIGEST_LENGTH];

    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, str.c_str(), str.size());
    SHA256_Final(digest, &ctx);

    // Write byte string to hex string
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
    }

    return ss.str();
}
