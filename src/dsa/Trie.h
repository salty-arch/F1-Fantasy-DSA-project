//
// Trie for Driver Name Autocomplete Search
//

#ifndef F1_FANTASY_DSA_TRIE_H
#define F1_FANTASY_DSA_TRIE_H

#include "../Core/Driver.h"
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

struct TrieNode {
    unordered_map<char, TrieNode*> children;
    bool isEndOfWord;
    Driver* driver;  // Store pointer to driver at end of name

    TrieNode() : isEndOfWord(false), driver(nullptr) {}
};

class Trie {
private:
    TrieNode* root;

    // Helper functions
    void collectSuggestions(TrieNode* node, string prefix, vector<string>& suggestions);
    void collectDrivers(TrieNode* node, vector<Driver*>& drivers);
    void destroyTrie(TrieNode* node);
    string toLowerCase(const string& str);

public:
    Trie();
    ~Trie();

    // Main operations
    void insert(Driver* driver);
    vector<string> searchPrefix(const string& prefix);
    vector<Driver*> searchDriversByPrefix(const string& prefix);
    Driver* searchExact(const string& name);

    bool isEmpty();
    void clear();
};

#endif //F1_FANTASY_DSA_TRIE_H