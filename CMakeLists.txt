cmake_minimum_required(VERSION 3.17)
project(MyGit)

find_package(OpenSSL REQUIRED)

set(CMAKE_CXX_STANDARD 20)

add_executable(MyGit src/Main.cpp src/Files/BaseFile.hpp src/Enums/FileType.hpp src/Hashing/HashingFunctions.hpp src/Files/SourceFile.cpp src/Files/SourceFile.hpp src/Files/Directory.cpp src/Files/Directory.hpp src/List/List.hpp src/List/List.inl src/List/ListIterator.hpp src/List/ListIterator.inl src/List/ListNode.hpp src/List/ListConstIterator.hpp src/List/ListConstIterator.inl)

target_link_libraries(MyGit stdc++fs)
target_link_libraries(MyGit OpenSSL::SSL)