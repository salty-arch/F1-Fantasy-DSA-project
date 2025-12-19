//
// AVL Tree Implementation
//

#include "AVLTree.h"
#include <iomanip>

// ============ CONSTRUCTOR & DESTRUCTOR ============

AVLTree::AVLTree() : root(nullptr) {}

AVLTree::~AVLTree() {
    destroyTree(root);
}

void AVLTree::destroyTree(AVLNode* node) {
    if (node == nullptr) return;
    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}

// ============ HEIGHT & BALANCE FUNCTIONS ============

int AVLTree::getHeight(AVLNode* node) {
    if (node == nullptr) return 0;
    return node->height;
}

int AVLTree::getBalanceFactor(AVLNode* node) {
    if (node == nullptr) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

AVLNode* AVLTree::updateHeight(AVLNode* node) {
    if (node == nullptr) return nullptr;

    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);
    node->height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);

    return node;
}

// ============ ROTATION FUNCTIONS ============

// Right Rotation (LL Case)
//       y                    x
//      / \                  / \
//     x   C    ----- >     A   y
//    / \                      / \
//   A   B                    B   C
AVLNode* AVLTree::rotateRight(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* B = x->right;

    // Perform rotation
    x->right = y;
    y->left = B;

    // Update heights
    updateHeight(y);
    updateHeight(x);

    return x;
}

// Left Rotation (RR Case)
//     x                      y
//    / \                    / \
//   A   y      ------>     x   C
//      / \                / \
//     B   C              A   B
AVLNode* AVLTree::rotateLeft(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* B = y->left;

    // Perform rotation
    y->left = x;
    x->right = B;

    // Update heights
    updateHeight(x);
    updateHeight(y);

    return y;
}

// ============ INSERT OPERATION ============

void AVLTree::insert(const Driver& driver) {
    root = insertNode(root, driver);
}

AVLNode* AVLTree::insertNode(AVLNode* node, const Driver& driver) {
    // Perform normal BST insertion
    if (node == nullptr) {
        return new AVLNode(driver);
    }

    // Insert based on points (higher points go to left for descending order)
    // For same points, use code as tiebreaker
    if (driver.getPoints() > node->driver.getPoints() ||
        (driver.getPoints() == node->driver.getPoints() &&
         driver.getCode() < node->driver.getCode())) {
        node->left = insertNode(node->left, driver);
    } else {
        node->right = insertNode(node->right, driver);
    }

    // 2. Update height of current node
    updateHeight(node);

    // 3. Get balance factor
    int balance = getBalanceFactor(node);

    // 4. Balance the tree if needed

    // Left-Left Case (LL)
    if (balance > 1 && (driver.getPoints() > node->left->driver.getPoints() ||
        (driver.getPoints() == node->left->driver.getPoints() &&
         driver.getCode() < node->left->driver.getCode()))) {
        return rotateRight(node);
    }

    // Right-Right Case (RR)
    if (balance < -1 && (driver.getPoints() < node->right->driver.getPoints() ||
        (driver.getPoints() == node->right->driver.getPoints() &&
         driver.getCode() > node->right->driver.getCode()))) {
        return rotateLeft(node);
    }

    // Left-Right Case (LR)
    if (balance > 1 && (driver.getPoints() < node->left->driver.getPoints() ||
        (driver.getPoints() == node->left->driver.getPoints() &&
         driver.getCode() > node->left->driver.getCode()))) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right-Left Case (RL)
    if (balance < -1 && (driver.getPoints() > node->right->driver.getPoints() ||
        (driver.getPoints() == node->right->driver.getPoints() &&
         driver.getCode() < node->right->driver.getCode()))) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

// ============ SEARCH OPERATION ============

Driver* AVLTree::search(const string& code) {
    AVLNode* result = searchNode(root, code);
    if (result == nullptr) return nullptr;
    return &(result->driver);
}

AVLNode* AVLTree::searchNode(AVLNode* node, const string& code) {
    if (node == nullptr) return nullptr;

    if (node->driver.getCode() == code) {
        return node;
    }

    // Search both subtrees since we're not organizing by code
    AVLNode* leftResult = searchNode(node->left, code);
    if (leftResult != nullptr) return leftResult;

    return searchNode(node->right, code);
}

// ============ TRAVERSAL OPERATIONS ============

void AVLTree::inOrderTraversal(AVLNode* node, vector<Driver>& result) {
    if (node == nullptr) return;

    inOrderTraversal(node->left, result);
    result.push_back(node->driver);
    inOrderTraversal(node->right, result);
}


vector<Driver> AVLTree::getDriversByPointsDescending() {
    vector<Driver> result;
    inOrderTraversal(root, result);
    return result;
}


vector<Driver> AVLTree::getTopNDrivers(int n) {
    vector<Driver> allDrivers = getDriversByPointsDescending();
    vector<Driver> topN;

    int count = n < allDrivers.size() ? n : allDrivers.size();
    for (int i = 0; i < count; i++) {
        topN.push_back(allDrivers[i]);
    }

    return topN;
}

// ============ UTILITY METHODS ============

int AVLTree::size() {
    return countNodes(root);
}

int AVLTree::countNodes(AVLNode* node) {
    if (node == nullptr) return 0;
    return 1 + countNodes(node->left) + countNodes(node->right);
}

bool AVLTree::isEmpty() {
    return root == nullptr;
}

void AVLTree::clear() {
    destroyTree(root);
    root = nullptr;
}

// ============ DISPLAY METHODS ============


void AVLTree::displayRankings() {
    if (isEmpty()) {
        cout << "No drivers in ranking!" << endl;
        return;
    }

    vector<Driver> drivers = getDriversByPointsDescending();

    cout << "\n=== DRIVER RANKINGS (AVL Tree) ===" << endl;
    cout << "Rank | Code | Name                | Team           | Points | Price" << endl;
    cout << "--------------------------------------------------------------------" << endl;

    for (int i = 0; i < drivers.size(); i++) {
        cout << setw(4) << (i + 1) << " | "
             << setw(4) << drivers[i].getCode() << " | "
             << setw(19) << left << drivers[i].getName() << " | "
             << setw(14) << drivers[i].getTeam() << " | "
             << setw(6) << right << drivers[i].getPoints() << " | "
             << setw(5) << drivers[i].getPrice() << "M" << endl;
    }
    cout << endl;
}