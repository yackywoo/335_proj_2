#include "FileAVL.hpp"
#include "File.hpp"
#include "FileTrie.hpp"

#include <string>

// ALL YOUR CODE SHOULD BE IN THIS FILE. NO MODIFICATIONS SHOULD BE MADE TO FILEAVL / FILE CLASSES
// You are permitted to make helper functions (and most likely will need to)
// You must declare them "inline" | declare & implement them at the top of this file, before query()
// Below query(), implement and document all methods declared in FileTrie.hpp

/**
 * @brief Helper function that recursively traverses the tree while adding files if within range of [min,max]
 * 
 * @param subroot Current node to examine whether its in range or not
 * @param min The min value of the file size query range.
 * @param max The max value of the file size query range.
 * @param result std::vector<File*> storing pointers to all files in the tree within the given range.
 * @note english translation : for whatever node, if its in range then add those files to the result.
 *      else continue searching left / right 
 */
void FileAVL::search(Node*& subroot, size_t min, size_t max, std::vector<File*>& result) {
    //base case if null
    if (!subroot) {
        return;
    }

    //when min < max (as intended) find values >= min and <= max
    if (min <= max) {
        //if in range, add to result vector
        if (subroot->size_ >= min && subroot->size_ <= max) {
            for (const auto things : subroot->files_) {
                result.push_back(things);
            }
        }
    } 
    //when min > max (unintended) find values >= max and <= min
    else if (min >= max) {
        if (subroot->size_ >= max && subroot->size_ <= min) {
            for (const auto things : subroot->files_) {
                result.push_back(things);
            }
        }
    } 
    
    //search left/right
    search(subroot->right_,min,max,result);
    search(subroot->left_,min,max,result);
}

/**
 * @brief Retrieves all files in the FileAVL whose file sizes are within [min, max]
 * 
 * @param min The min value of the file size query range.
 * @param max The max value of the file size query range.
 * @return std::vector<File*> storing pointers to all files in the tree within the given range.
 * @note If the query interval is in descending order (ie. the given parameters min >= max), 
        the interval from [max, min] is searched (since max >= min)
 */
std::vector<File*> FileAVL::query(size_t min, size_t max) {
    std::vector<File*> result;

    search(root_, min, max, result);

    return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FileTrie implmentations below

/**
 * @brief Destroys the given FileTrieNodes and its children
 * 
 * @param sub_head The node to be deleted
 */
void FileTrie::deleteTrie(FileTrieNode* sub_head) {
    //base case if empty
    if (!sub_head) {
        return;
    }

    //for all pairs in unordered_map, continue traversing inside nested FileTrieNdoes
    for (auto pairs : sub_head->next) {
        //recursively call function until we make it to the end
        deleteTrie(sub_head->next[pairs.first]);
    }

    //if not empty, start deleting from bottom up
    delete sub_head;
} 

/**
 * @brief Traverses the FileTrie and copies the set of all matching files into the result, if the prefix is valid.
 *      If a single character in the prefix is not valid then the set is cleared and function ends.
 * 
 * @param prefix Prefix that is being searched for
 * @param subroot Current node that is being searched for a matching prefix character
 * @param result Set of files that contain a matching prefix, else empty set
 */
void FileTrie::searchHelper(const std::string& prefix, FileTrieNode* subroot, std::unordered_set<File*>& result) const {
    //if empty string dont bother searching
    if (prefix.empty()) {
        return;
    }

    FileTrieNode* traverse = subroot;
    auto char_ptr = prefix.begin();

    while (char_ptr != prefix.end()) {
        if (traverse->next.find(tolower(*char_ptr)) != traverse->next.end()) {
            //if the next char exists in the trie, nest deeper and copy to result
            traverse = traverse->next[tolower(*char_ptr)];
            result = traverse->matching;
        } else {
            //if the char isn't found then empty out result and return, nothing to see if char isn't found
            result = {};
            return;
        }
        ++char_ptr;
    }
}

/**
 * @brief Recursively traverses FileTrie if character is found while adding the file into each FileTrieNode's set.
 *      If character is not found then dynamically creates new FileTrieNodes for each new character and inserts file.
 * 
 * @param head FileTrieNode that is being traversed into
 * @param fileName Name of the file being added into the trie
 * @param f File that is being added to each FileTrieNode's set
 */
void FileTrie::addHelper(FileTrieNode* head, const std::string& fileName, File* f) {
    if (fileName.empty()) {
        return;
    }

    //insert file along the path
    head->matching.insert(f);

    //case insensitive -> store and search using lower case char
    char current_char = tolower(fileName[0]);

    //if find char, traverse deeper into trie
    if (head->next.find(current_char) != head->next.end()) {
        //recursively enter nested FileTrieNode's for the given char (from unordered_map)
        addHelper(head->next[current_char], fileName.substr(1), f);
    } 
    //if not found, create trie node for new char and continue adding chars
    else {
        FileTrieNode* new_character_node = new FileTrieNode(fileName[0], f);
        //adding the nested node into next for the given character
        head->next[current_char] = new_character_node;

        addHelper(new_character_node, fileName.substr(1), f );
    }
}

/**
 * @brief Default Constructor: Construct a new FileTrie object
 */
FileTrie::FileTrie() : head{nullptr} {}

/**
 * @brief Destroy the FileTrie, deallocating all necessary FileTrieNodes
 */
FileTrie:: ~FileTrie() {
    deleteTrie(head);
    head = nullptr;
}

/**
 * @brief Adds file into FileTrie object
 * 
 * @param f The file to be added
 */
void FileTrie::addFile(File* f) {
    if (!head) {
        //if empty head then create
        head = new FileTrieNode();
    }
    std::string filename = f->getName();

    addHelper(head, filename, f);
}

/**
 * @brief Searches the FileTrie for some prefix and returns a set of Files that begin with that prefix.
 *      If no match is found, then an empty set is returned.
 * 
 * @param prefix Prefix that is being searched for
 * @return Set of all Files with the same prefix, if found, else empty set.
 */
std::unordered_set<File*> FileTrie::getFilesWithPrefix(const std::string& prefix) const {
    std::unordered_set<File*> result;

    searchHelper(prefix, head, result);

    return result;
}