/**
 * @file FileTrie.hpp
 * @brief Defines the interface for the FileTrie class & implementation of the TrieNode struct
 */


#pragma once
#include <unordered_map>
#include <unordered_set>
#include <cctype>
#include <string>
#include <iostream>
#include <functional>
#include "File.hpp"

struct FileTrieNode {   
    char stored;

    std::unordered_set<File*> matching;
    std::unordered_map<char, FileTrieNode*> next;

    FileTrieNode(const char& c = ' ', File* to_add = nullptr) : stored{c}, matching{}, next{} {
        if (to_add) { matching.insert(to_add); }
    }
};

class FileTrie {
    private:
        FileTrieNode* head;
        void deleteTrie(FileTrieNode* sub_head);
        void addHelper(FileTrieNode* head, const std::string& fileName, File* f);
        void searchHelper(const std::string& prefix, FileTrieNode* subroot, std::unordered_set<File*>& result) const;
    public:
        /**
         * @brief Default Constructor: Construct a new FileTrie object
         */
        FileTrie();

        /**
         * @brief Adds file into FileTrie object, case insensitive
         * 
         * @param f The file to be added
         */
        void addFile(File* f);

        /**
         * @brief Searches the FileTrie for some prefix and returns a set of Files that begin with that prefix.
         *      If no match is found, then an empty set is returned.
         * 
         * @param prefix Prefix that is being searched for
         * @return Set of all Files with the same prefix, if found, else empty set.
         */
        std::unordered_set<File*> getFilesWithPrefix(const std::string& prefix) const;

        /**
         * @brief Destroy the FileTrie, deallocating all necessary FileTrieNodes
         */
        ~FileTrie();
};