#pragma once

#include <cstring>
#include <functional>

struct Word {
    char word[120];
    int count;
};

struct Node {
    Word data;
    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent = nullptr;
    int height = 0;

    // Example constructor for Node
    Node(const Word& w) : data(w), left(nullptr), right(nullptr), parent(nullptr), height(0) {}
};

class MinHeap {
private:
    Word data[10];
    int size = 0;

    int parent(int i);
    int left(int i);
    int right(int i);
    void bubbleUp(int i);
    void bubbleDown(int i);

public:
    bool isFull() const;
    void insert(const Word& word);
    void trimToSize(int targetSize);
    Word getTop();
    void removeTop();
    int getSize() const;
};

class MaxHeap {
private:
    Word data[10];
    int size = 0;

    int parent(int i);
    int left(int i);
    int right(int i);
    void bubbleUp(int i);
    void bubbleDown(int i);

public:
    MaxHeap() : size(0) {
        memset(data, 0, sizeof(data));
    }

    void swap(Word& a, Word& b);
    bool isFull() const;
    void insert(const Word& word);
    int getSize() const;
    void removeTop();
    Word getTop();
};

class AVLTree {
private:
    Node* root;

    Node* insertRec(Node* node, const Word& word, Node* parent);
    Node* search(Node* node, const char* word) const;
    void inOrder(Node* node, const std::function<void(const Word&)>& process) const;
    Node* getMaxCountNode(Node* node) const;
    Node* getMinCountNode(Node* node) const;
    void displayUniqueWords(Node* node) const;

public:
    AVLTree();

    void insert(const Word& word);
    Node* search(const char* word) const;
    void inOrder(const std::function<void(const Word&)>& process) const;

    Word getTopWord() const;
    Word getLastWord() const;
    void printUniqueWords() const;

    void printTop10();
    void printLast10();
};
