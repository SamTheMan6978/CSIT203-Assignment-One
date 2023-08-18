#include "Algorithm.h"

#include <iostream>

using namespace std;
// ---------------- Helper Functions ----------------

int height(Node* node) {
    if (node == nullptr) return 0;
    return node->height;
}

int getBalance(Node* node) {
    if (node == nullptr) return 0;
    return height(node->left) - height(node->right);
}

Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T3 = x->right;

    x->right = y;
    y->left = T3;

    y->height = 1 + max(height(y->left), height(y->right));
    x->height = 1 + max(height(x->left), height(x->right));

    return x;
}

Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = 1 + max(height(x->left), height(x->right));
    y->height = 1 + max(height(y->left), height(y->right));

    return y;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

// AVLTree methods

AVLTree::AVLTree() : root(nullptr) {}

void AVLTree::insert(const Word& word) {
    root = insertRec(root, word, nullptr);
}


Node* AVLTree::insertRec(Node* node, const Word& word, Node* parent) {
    // 1. Insertion as in BST
    if (node == nullptr) {
        node = new Node(word);
        node->data = word;
        node->left = nullptr;
        node->right = nullptr;
        node->parent = parent;
        node->height = 1;
        return node;
    }

    if (strcmp(word.word, node->data.word) < 0) {
        node->left = insertRec(node->left, word, node);
    }
    else if (strcmp(word.word, node->data.word) > 0) {
        node->right = insertRec(node->right, word, node);
    }
    else {
        node->data.count++;  // Increment the count for duplicate word
        return node;
    }

    // 2. Update height of current node
    node->height = 1 + max(height(node->left), height(node->right));

    // 3. Get balance factor to check if it became unbalanced
    int balance = getBalance(node);

    // 4. If the node becomes unbalanced, there are 4 cases:

    // Left Left Case
    if (balance > 1 && strcmp(word.word, node->left->data.word) < 0) {
        return rightRotate(node);
    }

    // Right Right Case
    if (balance < -1 && strcmp(word.word, node->right->data.word) > 0) {
        return leftRotate(node);
    }

    // Left Right Case
    if (balance > 1 && strcmp(word.word, node->left->data.word) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && strcmp(word.word, node->right->data.word) < 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

Node* AVLTree::search(Node* node, const char* word) const {
    if (!node) {
        // If the node is nullptr (we've reached a leaf), the word is not in the tree
        return nullptr;
    }
    if (strcmp(word, node->data.word) == 0) {
        // If the word matches the current node's word, return the current node
        return node;
    }
    else if (strcmp(word, node->data.word) < 0) {
        // If the word is lexicographically smaller than the current node's word, search in the left subtree
        return search(node->left, word);
    }
    else {
        // If the word is lexicographically larger than the current node's word, search in the right subtree
        return search(node->right, word);
    }

}

Node* AVLTree::search(const char* word) const {
    Node* current = root;
    while (current) {
        if (strcmp(word, current->data.word) == 0) {
            return current;
        }
        else if (strcmp(word, current->data.word) < 0) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }
    return nullptr;
}

void AVLTree::inOrder(const std::function<void(const Word&)>& process) const {
    inOrder(root, process);
}

void AVLTree::inOrder(Node* node, const std::function<void(const Word&)>& process) const {
    if (node) {
        inOrder(node->left, process);
        process(node->data);
        inOrder(node->right, process);
    }
}


Word AVLTree::getTopWord() const {
    Node* maxNode = getMaxCountNode(root);
    if (maxNode) return maxNode->data;
    else return { "", 0 };
}

Word AVLTree::getLastWord() const {
    Node* minNode = getMinCountNode(root);
    if (minNode) return minNode->data;
    else return { "", 0 };
}

void AVLTree::printUniqueWords() const {
    std::cout << "\nUnique Words:" << std::endl;
    displayUniqueWords(root);
}

void AVLTree::printTop10() {
    MaxHeap heap;

    inOrder(root, [&heap](const Word& word) {
        if (heap.isFull() && word.count > heap.getTop().count) {
            heap.removeTop();
            heap.insert(word);
        }
        else if (!heap.isFull()) {
            heap.insert(word);
        }
        });

    // Sorting the heap to print in descending order of count
    Word topWords[10];
    int heapSizeBeforeEmptying = heap.getSize(); // Get the heap size before emptying
    for (int i = 0; i < 10 && i < heapSizeBeforeEmptying; i++) {
        topWords[i] = heap.getTop();
        heap.removeTop();
    }

    for (int i = 0; i < 10 && i < heapSizeBeforeEmptying; i++) {
        cout << topWords[i].word << ": " << topWords[i].count << endl;
    }
}

int min(int a, int b) {
    return (a < b) ? a : b;
}

void AVLTree::printLast10() {
    Word last10Words[10];
    int currentIndex = 0;
    int totalWordsProcessed = 0;

    inOrder(root, [&last10Words, &currentIndex, &totalWordsProcessed](const Word& word) {
        last10Words[currentIndex] = word;
        currentIndex = (currentIndex + 1) % 10; // This ensures circularity
        totalWordsProcessed++;
        });

    // Determine where to start printing from
    int start = totalWordsProcessed < 10 ? 0 : currentIndex;

    // Print the last 10 words
    for (int i = 0; i < min(10, totalWordsProcessed); i++) {
        cout << last10Words[start].word << ": " << last10Words[start].count << endl;
        start = (start + 1) % 10; // This ensures circularity
    }
}

Node* AVLTree::getMaxCountNode(Node* node) const {
    if (node == nullptr) return nullptr;

    Node* leftMax = getMaxCountNode(node->left);
    Node* rightMax = getMaxCountNode(node->right);

    Node* tempMax = node;
    if (leftMax && leftMax->data.count > tempMax->data.count) tempMax = leftMax;
    if (rightMax && rightMax->data.count > tempMax->data.count) tempMax = rightMax;

    return tempMax;
}

Node* AVLTree::getMinCountNode(Node* node) const {
    if (node == nullptr) return nullptr;

    Node* leftMin = getMinCountNode(node->left);
    Node* rightMin = getMinCountNode(node->right);

    Node* tempMin = node;
    if (leftMin && leftMin->data.count < tempMin->data.count) tempMin = leftMin;
    if (rightMin && rightMin->data.count < tempMin->data.count) tempMin = rightMin;

    return tempMin;
}

void AVLTree::displayUniqueWords(Node* node) const {
    if (!node) return;

    if (node->data.count == 1) {
        cout << node->data.word << endl;
    }

    displayUniqueWords(node->left);
    displayUniqueWords(node->right);
}
