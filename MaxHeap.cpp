#include "Algorithm.h"

// Private helper methods

void MaxHeap::swap(Word& a, Word& b) {
    Word temp = a;
    a = b;
    b = temp;
}

int MaxHeap::parent(int i) {
    return (i - 1) / 2;
}

int MaxHeap::left(int i) {
    return 2 * i + 1;
}

int MaxHeap::right(int i) {
    return 2 * i + 2;
}


void MaxHeap::bubbleUp(int i) {
    // Starting from the given node (usually the last inserted node in the heap), check against its parent.
    while (
        // Ensure we aren't at the root. If we're at the root, there's no parent to compare with.
        i != 0 &&
        // The first part of this condition checks if the parent's count is less than the current node's count.
        // If true, it indicates a violation of the max-heap property and a swap is needed.
        (data[parent(i)].count < data[i].count ||
            // If the counts are the same, words are compared lexicographically.
            (data[parent(i)].count == data[i].count && strcmp(data[parent(i)].word, data[i].word) > 0))
        ) {
        // If either condition is met, swap the current node with its parent.
        // This is done to restore the max-heap property.
        swap(data[i], data[parent(i)]);

        // Move to the parent node and repeat the process.
        // This ensures the heap property is maintained for all nodes above the current one.
        i = parent(i);
    }
}



void MaxHeap::bubbleDown(int i) {
    int l = left(i);  // Get the left child's index.
    int r = right(i); // Get the right child's index.
    int largest = i;  // Assume the current node is the largest.

    // If the left child exists and has a larger count (or alphabetically comes before with equal counts), 
    // update the 'largest' variable.
    if (l < size && (data[l].count > data[i].count ||
        (data[l].count == data[i].count && strcmp(data[l].word, data[i].word) < 0))) {
        largest = l;
    }

    // If the right child exists and has a larger count than the 'largest' node till now, 
    // update the 'largest' variable.
    if (r < size && (data[r].count > data[largest].count ||
        (data[r].count == data[largest].count && strcmp(data[r].word, data[largest].word) < 0))) {
        largest = r;
    }

    // If the current node isn't the largest, swap it with the largest of its two children
    // to restore the max-heap property.
    if (largest != i) {
        swap(data[i], data[largest]);

        // Recursively bubble down from the child node.
        bubbleDown(largest);
    }
}


// Public methods

bool MaxHeap::isFull() const {
    return size == 10;
}

void MaxHeap::insert(const Word& word) {
    if (!isFull()) {
        data[size++] = word;
        bubbleUp(size - 1);
    }
    else {
        if (word.count > data[0].count) {
            data[0] = word;
            bubbleDown(0);
        }
    }
}

int MaxHeap::getSize() const {
    return size;
}

void MaxHeap::removeTop() {

    // If the heap is empty, there's nothing to remove, so return
    if (size == 0) return;

    // Replace the root with the last Word in the heap
    data[0] = data[size - 1];
    // Decrease the size of the heap by 1, effectively removing the last Word
    size--;
    // Adjust the position of the new root to maintain max-heap property using bubbleDown
    bubbleDown(0);
}

Word MaxHeap::getTop() {

    // If the heap is empty, return a default Word with an empty string and 0 count
    if (size == 0) {
        Word emptyWord = { "", 0 };
        return emptyWord;
    }

    // Otherwise, return the root Word which is the maximum in the MaxHeap
    return data[0];
}
