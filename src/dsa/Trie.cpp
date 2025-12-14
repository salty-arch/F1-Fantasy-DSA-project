//
// Trie Implementation for Driver Search
//

#include "Trie.h"
#include <algorithm>
#include <cctype>
#include <iostream>

// ============ CONSTRUCTOR & DESTRUCTOR ============

Trie::Trie() {
    root = new TrieNode();
}

Trie::~Trie() {
    destroyTrie(root);
}

void Trie::destroyTrie(TrieNode* node) {
    if (node == nullptr) return;

    for (auto& pair : node->children) {
        destroyTrie(pair.second);
    }
    delete node;
}

// ============ UTILITY FUNCTIONS ============

string Trie::toLowerCase(const string& str) {
    string lower = str;
    transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
}

// ============ INSERT OPERATION ============

void Trie::insert(Driver* driver) {
    string name = toLowerCase(driver->getName());
    TrieNode* current = root;

    // Insert full name
    for (char c : name) {
        if (c == ' ') continue;  // Skip spaces

        if (current->children.find(c) == current->children.end()) {
            current->children[c] = new TrieNode();
        }
        current = current->children[c];
    }

    current->isEndOfWord = true;
    current->driver = driver;

    // Also insert by driver code for quick access
    string code = toLowerCase(driver->getCode());
    current = root;

    for (char c : code) {
        if (current->children.find(c) == current->children.end()) {
            current->children[c] = new TrieNode();
        }
        current = current->children[c];
    }

    current->isEndOfWord = true;
    current->driver = driver;
}

// ============ SEARCH OPERATIONS ============

vector<string> Trie::searchPrefix(const string& prefix) {
    vector<string> suggestions;
    string lowerPrefix = toLowerCase(prefix);

    // Navigate to prefix node
    TrieNode* current = root;
    for (char c : lowerPrefix) {
        if (c == ' ') continue;

        if (current->children.find(c) == current->children.end()) {
            return suggestions;  // Prefix not found
        }
        current = current->children[c];
    }

    // Collect all suggestions from this point
    collectSuggestions(current, lowerPrefix, suggestions);

    return suggestions;
}

vector<Driver*> Trie::searchDriversByPrefix(const string& prefix) {
    vector<Driver*> drivers;
    string lowerPrefix = toLowerCase(prefix);

    // Navigate to prefix node
    TrieNode* current = root;
    for (char c : lowerPrefix) {
        if (c == ' ') continue;

        if (current->children.find(c) == current->children.end()) {
            return drivers;  // Prefix not found
        }
        current = current->children[c];
    }

    // Collect all drivers from this point
    collectDrivers(current, drivers);

    return drivers;
}

Driver* Trie::searchExact(const string& name) {
    string lowerName = toLowerCase(name);
    TrieNode* current = root;

    for (char c : lowerName) {
        if (c == ' ') continue;

        if (current->children.find(c) == current->children.end()) {
            return nullptr;
        }
        current = current->children[c];
    }

    if (current->isEndOfWord) {
        return current->driver;
    }

    return nullptr;
}

// ============ HELPER FUNCTIONS ============

void Trie::collectSuggestions(TrieNode* node, string prefix, vector<string>& suggestions) {
    if (node == nullptr) return;

    if (node->isEndOfWord && node->driver != nullptr) {
        suggestions.push_back(node->driver->getName() + " (" +
                            node->driver->getCode() + ")");
    }

    for (auto& pair : node->children) {
        collectSuggestions(pair.second, prefix + pair.first, suggestions);
    }
}

void Trie::collectDrivers(TrieNode* node, vector<Driver*>& drivers) {
    if (node == nullptr) return;

    if (node->isEndOfWord && node->driver != nullptr) {
        drivers.push_back(node->driver);
    }

    for (auto& pair : node->children) {
        collectDrivers(pair.second, drivers);
    }
}

// ============ UTILITY METHODS ============

bool Trie::isEmpty() {
    return root->children.empty();
}

void Trie::clear() {
    destroyTrie(root);
    root = new TrieNode();
}