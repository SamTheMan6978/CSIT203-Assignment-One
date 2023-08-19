#include "Algorithm.h"

#include <iostream>

using namespace std;
// ---------------- Helper Functions ----------------

int max(int a, int b) {
    return (a > b) ? a : b;
}

int min(int a, int b) {
    return (a < b) ? a : b;
}

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

// AVLTree methods

// Constructor
AVLTree::AVLTree() : root(nullptr) {}

//Insertion
void AVLTree::insert(const Word& word) {
    root = insertRec(root, word, nullptr);
}


Node* AVLTree::insertRec(Node* node, const Word& word, Node* parent) {
    // 1. Regular BST-style insertion
    // If the current node is null, we create a new node for the word and return it.
    if (node == nullptr) {
        node = new Node(word);
        node->data = word;
        node->left = nullptr;
        node->right = nullptr;
        node->parent = parent;
        node->height = 1;  // Base height for a newly created node.
        return node;
    }

    // Compare the word with the current node's word to decide the direction of insertion.

    // If the word is less than current word, move to left subtree.
    if (strcmp(word.word, node->data.word) < 0) {
        node->left = insertRec(node->left, word, node);
    }
    // If the word is greater than current word, move to right subtree.
    else if (strcmp(word.word, node->data.word) > 0) {
        node->right = insertRec(node->right, word, node);
    }
    // If the word already exists in the tree, we increment its count.
    else {
        node->data.count++;
        return node;
    }

    // 2. Update height of the current node
    // We set the height to be one more than the max of left and right child heights.
    node->height = 1 + max(height(node->left), height(node->right));

    // 3. Get the balance factor to check if it became unbalanced after insertion.
    int balance = getBalance(node);

    // 4. If node became unbalanced, then there are 4 cases to be checked:

    // Left Left Case (Single rotation)
    if (balance > 1 && strcmp(word.word, node->left->data.word) < 0) {
        return rightRotate(node);
    }

    // Right Right Case (Single rotation)
    if (balance < -1 && strcmp(word.word, node->right->data.word) > 0) {
        return leftRotate(node);
    }

    // Left Right Case (Double rotation: Left child with right subtree)
    if (balance > 1 && strcmp(word.word, node->left->data.word) > 0) {
        node->left = leftRotate(node->left);  // First rotate the left child
        return rightRotate(node);  // Then rotate the unbalanced node
    }

    // Right Left Case (Double rotation: Right child with left subtree)
    if (balance < -1 && strcmp(word.word, node->right->data.word) < 0) {
        node->right = rightRotate(node->right);  // First rotate the right child
        return leftRotate(node);  // Then rotate the unbalanced node
    }

    return node;  // Return the potentially rotated node.
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
        // If the word is alphabetically smaller than the current node's word, search in the left subtree
        return search(node->left, word);
    }
    else {
        // If the word is alphabetically larger than the current node's word, search in the right subtree
        return search(node->right, word);
    }

}


Node* AVLTree::search(const char* word) const {

    // Start from the root of the tree
    Node* current = root;

    // Continue the loop until we've either found the word or reached a null node (end of a branch)
    while (current) {

        // Compare the word to be searched with the word in the current node
        if (strcmp(word, current->data.word) == 0) {

            // If they match, the word is found, and we return the node containing that word
            return current;

        }
        else if (strcmp(word, current->data.word) < 0) {

            // If the word to be searched is "less than" the current word (in alphabetical order),
            // we move to the left child of the current node
            current = current->left;

        }
        else {

            // If the word to be searched is "greater than" the current word,
            // we move to the right child of the current node
            current = current->right;
        }
    }

    // If we've exited the loop without returning, it means the word isn't in the tree.
    // Thus, we return a nullptr indicating the word was not found.
    return nullptr;
}


// This function is a public interface to start an inOrder traversal from the root of the AVL Tree.
void AVLTree::inOrder(const std::function<void(const Word&)>& process) const {
    // It calls the private, recursive version of inOrder, starting from the root.
    inOrder(root, process);
}

// This is the recursive implementation of the inOrder traversal for an AVL Tree.
void AVLTree::inOrder(Node* node, const std::function<void(const Word&)>& process) const {
    // Check if the current node is not nullptr.
    if (node) {
        // First, traverse the left subtree.
        inOrder(node->left, process);
        // Then, process the current node's data.
        process(node->data);
        // Finally, traverse the right subtree.
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
    // Instantiate a max heap that will be used to track the top 10 words by count
    MaxHeap heap;

    // Perform an in-order traversal of the AVL tree
    inOrder(root, [&heap](const Word& word) {
        // If the heap is already full and the current word's count is greater than the 
        // smallest count in the heap (the top in a max heap), 
        // then we remove the top of the heap and insert the current word.
        if (heap.isFull() && word.count > heap.getTop().count) {
            heap.removeTop();
            heap.insert(word);
        }
        // If the heap is not full, we simply insert the current word.
        else if (!heap.isFull()) {
            heap.insert(word);
        }
        });

    // I want to print the top 10 words in descending order of their counts. 
    // To do this, I will have empty the max heap, storing the words in an array.
    Word topWords[10];
    int heapSizeBeforeEmptying = heap.getSize(); // Get the number of words in the heap before emptying it
    for (int i = 0; i < 10 && i < heapSizeBeforeEmptying; i++) {
        topWords[i] = heap.getTop(); // Extract the word with the highest count from the heap
        heap.removeTop(); // Remove the word just extracted
    }

    // Now, print the top 10 (or fewer, if less than 10 words were processed) words 
    // from the array in descending order of count.
    for (int i = 0; i < 10 && i < heapSizeBeforeEmptying; i++) {
        cout << topWords[i].word << ": " << topWords[i].count << endl;
    }
}


void AVLTree::printLast10() {
    // Initialize an array to hold the last 10 words encountered and tracking variables
    Word last10Words[10];
    int currentIndex = 0;
    int totalWordsProcessed = 0;

    // Perform an in-order traversal, storing words in a circular buffer manner
    inOrder(root, [&last10Words, &currentIndex, &totalWordsProcessed](const Word& word) {
        last10Words[currentIndex] = word;
        currentIndex = (currentIndex + 1) % 10; // Ensure that index wraps around after 10
        totalWordsProcessed++;
        });

    // Determine the starting index for printing based on total words processed
    int start = totalWordsProcessed < 10 ? 0 : currentIndex;

    // Iterate and print the last 10 (or fewer) words in order
    for (int i = 0; i < min(10, totalWordsProcessed); i++) {
        cout << last10Words[start].word << ": " << last10Words[start].count << endl;
        start = (start + 1) % 10; // Ensure that index wraps around after 10
    }
}


Node* AVLTree::getMaxCountNode(Node* node) const {
    if (node == nullptr) return nullptr;

    Node* leftMax = getMaxCountNode(node->left);  // get the node with max count in the left subtree
    Node* rightMax = getMaxCountNode(node->right); // get the node with max count in the right subtree

    // Determine which of the three nodes (node, leftMax, rightMax) has the maximum count
    Node* tempMax = node;
    if (leftMax && leftMax->data.count > tempMax->data.count) tempMax = leftMax;
    if (rightMax && rightMax->data.count > tempMax->data.count) tempMax = rightMax;

    return tempMax; // Return the node with the max count

}

Node* AVLTree::getMinCountNode(Node* node) const {
    if (node == nullptr) return nullptr;

    Node* leftMin = getMinCountNode(node->left);  // get the node with min count in the left subtree
    Node* rightMin = getMinCountNode(node->right); // get the node with min count in the right subtree

    // Determine which of the three nodes (node, leftMin, rightMin) has the minimum count
    Node* tempMin = node;
    if (leftMin && leftMin->data.count < tempMin->data.count) tempMin = leftMin;
    if (rightMin && rightMin->data.count < tempMin->data.count) tempMin = rightMin;

    return tempMin; // Return the node with the min count

}

void AVLTree::displayUniqueWords(Node* node) const {

    // Base case: If the node is null, end the recursion for this branch
    if (!node) return;

    // If the word's count at the current node is 1 (indicating it's unique),
    // print the word to the console
    if (node->data.count == 1) {
        cout << node->data.word << endl;
    }

    // Recursive call to the left subtree
    displayUniqueWords(node->left);

    // Recursive call to the right subtree
    displayUniqueWords(node->right);
}

