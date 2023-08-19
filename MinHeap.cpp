#include "Algorithm.h"

// Private utility function to swap two Word elements
void swapWords(Word& a, Word& b) {
    Word temp = a;
    a = b;
    b = temp;
}

// Private methods

int MinHeap::parent(int i) {
    return (i - 1) / 2;
}

int MinHeap::left(int i) {
    return 2 * i + 1;
}

int MinHeap::right(int i) {
    return 2 * i + 2;
}

void MinHeap::bubbleUp(int i) {
    // Starting from the current node, move upward and compare with parent nodes.
    while (
        // Ensure we're not at the root. If we're at the root, there's no parent to compare with.
        i != 0 &&
        // The first condition checks if the parent's count is greater than the current node's count.
        // If true, this is a violation of the min-heap property, and a swap is required.
        (data[parent(i)].count > data[i].count ||
            // If counts are the same, words are compared alphabetically.
            (data[parent(i)].count == data[i].count && strcmp(data[parent(i)].word, data[i].word) < 0))
        ) {
        // If any of the conditions is met, swap the current node with its parent.
        // Restoring the min-heap property for the pair.
        swapWords(data[i], data[parent(i)]);

        // Move to the parent node to continue checking further up the tree.
        i = parent(i);
    }
}


void MinHeap::bubbleDown(int i) {
    int l = left(i);  // Get the left child's index.
    int r = right(i); // Get the right child's index.
    int smallest = i; // Starting by assuming the current node is the smallest.

    // If the left child exists and is smaller than the current node (or alphabetically follows with equal counts), 
    // mark the left child as the smallest.
    if (l < size && (data[l].count < data[i].count ||
        (data[l].count == data[i].count && strcmp(data[l].word, data[i].word) > 0))) {
        smallest = l;
    }

    // Similarly, if the right child exists and is smaller than the currently marked smallest node, 
    // mark the right child as the smallest.
    if (r < size && (data[r].count < data[smallest].count ||
        (data[r].count == data[smallest].count && strcmp(data[r].word, data[smallest].word) > 0))) {
        smallest = r;
    }

    // If either of the children was found to be smaller than the current node, swap with the smaller of them.
    if (smallest != i) {
        swapWords(data[i], data[smallest]);

        // Recursively continue the process from the swapped child to ensure the min-heap property is maintained.
        bubbleDown(smallest);
    }
}


// Public methods


bool MinHeap::isFull() const {
    return size == 10;
}

void MinHeap::insert(const Word& word) {

    // If the heap has less than 10 elements, we can directly insert the new Word
    if (size < 10) {
        data[size++] = word;          // Place the Word at the next available position
        bubbleUp(size - 1);           // Adjust its position if necessary using bubbleUp
    }
    else {
        // If the heap is full (has 10 elements), check if the new Word should replace the root
        // Replace the root if the new Word's count is less than the root's count
        // or if the counts are equal but the new Word is alphabetically greater than the root Word
        if (word.count < data[0].count ||
            (word.count == data[0].count && strcmp(word.word, data[0].word) > 0)) {

            data[0] = word;            // Replace the root with the new Word
            bubbleDown(0);             // Adjust the new root's position using bubbleDown
        }
    }
}

Word MinHeap::getTop() {

    // If the heap is empty, return a default Word with an empty string and 0 count
    return size == 0 ? Word{ "", 0 } : data[0];
}

void MinHeap::removeTop() {

    // If the heap is empty, there's nothing to remove, so return
    if (size == 0) return;

    // Replace the root with the last Word in the heap
    data[0] = data[--size];
    // Adjust the root's position using bubbleDown
    bubbleDown(0);
}

// Function to get the current size of the MinHeap
int MinHeap::getSize() const {
    return size;
}

