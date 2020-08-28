#pragma once

template <typename T>
struct Node
{
    Node() : data(), next(nullptr) {}
    Node(const T& val) : data(val), next(nullptr) {}

    T data;
    Node* next;
};