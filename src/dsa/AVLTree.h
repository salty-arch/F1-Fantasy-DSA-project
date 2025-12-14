//
// AVL Tree for Driver Rankings
// Maintains drivers sorted by points in a balanced binary search tree
//

#ifndef F1_FANTASY_DSA_AVLTREE_H
#define F1_FANTASY_DSA_AVLTREE_H

#include "../Core/Driver.h"
#include <vector>
#include <iostream>
using namespace std;

// AVL Tree Node structure
struct AVLNode {
    Driver driver;
    AVLNode* left;
    AVLNode* right;
    int height;

    // Constructor
    AVLNode(const Driver& d) : driver(d), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
private:
    AVLNode* root;

    // Helper functions
    int getHeight(AVLNode* node);
    int getBalanceFactor(AVLNode* node);
    AVLNode* updateHeight(AVLNode* node);

    // Rotation functions
    AVLNode* rotateRight(AVLNode* y);
    AVLNode* rotateLeft(AVLNode* x);

    // Insert helper
    AVLNode* insertNode(AVLNode* node, const Driver& driver);

    // Traversal helpers
    void inOrderTraversal(AVLNode* node, vector<Driver>& result);
    void reverseInOrderTraversal(AVLNode* node, vector<Driver>& result);

    // Search helpers
    AVLNode* searchNode(AVLNode* node, const string& code);

    // Utility
    void destroyTree(AVLNode* node);
    int countNodes(AVLNode* node);

    // Display helper
    void displayTree(AVLNode* node, int indent, bool isRight);

public:
    // Constructor and Destructor
    AVLTree();
    ~AVLTree();

    // Main operations
    void insert(const Driver& driver);
    Driver* search(const string& code);

    // Traversal methods
    vector<Driver> getDriversByPointsDescending();
    vector<Driver> getDriversByPointsAscending();
    vector<Driver> getTopNDrivers(int n);

    // Utility methods
    int size();
    bool isEmpty();
    void clear();

    // Display
    void displayTreeStructure();
    void displayRankings();
};

#endif //F1_FANTASY_DSA_AVLTREE_H