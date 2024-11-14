#include "File.hpp"
#include "FileAVL.hpp"
#include "FileTrie.hpp"
#include <iostream>
#include <vector>

int main () {
    File one("a","1");
    File two("Ab","12");
    File three("aB3","123");
    File four("Abd","1234");
    File five("ad","12345");

    File* one_1 = &one;
    File* two_1 = &two;
    File* three_1 = &three;
    File* four_1 = &four;
    File* five_1 = &five;

    FileAVL first;
    first.insert(one_1);
    first.insert(two_1);
    first.insert(three_1);
    first.insert(four_1);
    first.insert(five_1);
    
    //first.displayLevelOrder();
    //std::vector<File*> stuff = first.query(5,1);
    // for (const auto things : stuff) {
    //     std::cout << things->getName() << std::endl;
    // }

    FileTrie test;
    test.addFile(one_1);
    test.addFile(two_1);
    test.addFile(three_1);
    test.addFile(four_1);
    test.addFile(five_1);
    // for (const auto stuff : test.head->next) {
    //     std::cout << stuff.first  << std::endl;
    // }
    
    //traversing the nested unordered_map
    // FileTrieNode* trav = test.head->next['a']->next['b'];
    // for (const auto pairs : trav->next) {
    //     std::cout << pairs.first << " ";
    // }

    // std::cout << std::endl;

    // for (const auto files : trav->matching) {
    //     std::cout << files->getName() << " ";
    // }

    auto names = test.getFilesWithPrefix("as");
    for (auto things : names) {
        std::cout << things->getName() << " ";
    }
}