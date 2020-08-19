#pragma once

template <typename T>
struct Node
{
    Node() = default;
    Node(const T& val) : data(val), next(nullptr) {}

    T data;
    Node* next;
};