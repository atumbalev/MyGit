#include <iostream>
#include <array>
#include "List/List.hpp"

int main()
{
    List<int> list{1, 2, 3};
    for (auto a : list)
    {
        std::cout << a << std::endl;
    }

    int i = 0;
    ListConstIterator<int> it(nullptr);
    for (it = list.begin(); it != list.end(); ++it)
    {
        std::cout << "here\n";
        ++i;
        if (i == 2)
        {
            break;
        }
    }

    list.insert_after(it, 5, 6, 7, 8);

    for (auto a : list)
    {
        std::cout << a << std::endl;
    }

    std::cout << "new list\n";

    List<int> l2(list);
    l2.splice(list);

    std::cout << "size: " << list.size() << std::endl;

    for (auto a : l2)
    {
        std::cout << a << std::endl;
    }

}