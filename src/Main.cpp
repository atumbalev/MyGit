#include <iostream>
#include <memory>
#include "List/List.hpp"
#include "HashMap/HashMap.hpp"
#include "Files/BaseFile.hpp"
#include "Files/SourceFile.hpp"

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

    List<int> l2(std::move(list));

    List<int> l3(l2);
    l3.push_back(5);
//    l3.remove(2);
    l3.remove(++(++l3.cbegin()));

    for (auto a : l3)
    {
        std::cout << a << std::endl;
    }

    HashMap<std::string, int> map;
    map.insert("pe6o", 1);
    map.insert("pe6o", 6);
    map.insert("de6o", 6);
    map.insert("pe6a", 6);
    map.insert("pe64o", 6);
    map.insert("pe6as", 6);
//    map.erase(map.find("pe6o"));
    auto entry = map.find("atanas");

    if (entry == map.end())
    {
        std::cout << "not found\n";
    }

    for (auto& el : map)
    {
        std::cout << el.first << " " << el.second << std::endl;
    }

    HashMap<std::string, BaseFile*> dirCont;

    SourceFile* file = new SourceFile("file", "hash");
    List<BaseFile*> flist;
    flist.push_front(file);
    flist.pop_front();

    std::cout << file->getName() << std::endl;

    dirCont.insert(file->getName(), file);
    dirCont.clear();

}