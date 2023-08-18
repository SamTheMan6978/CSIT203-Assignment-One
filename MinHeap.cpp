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
    while (i != 0 && (data[parent(i)].count > data[i].count ||
        (data[parent(i)].count == data[i].count && strcmp(data[parent(i)].word, data[i].word) < 0))) {
        swapWords(data[i], data[parent(i)]);
        i = parent(i);
    }
}

void MinHeap::bubbleDown(int i) {
    int l = left(i);
    int r = right(i);
    int smallest = i;

    if (l < size && (data[l].count < data[i].count ||
        (data[l].count == data[i].count && strcmp(data[l].word, data[i].word) > 0))) {
        smallest = l;
    }

    if (r < size && (data[r].count < data[smallest].count ||
        (data[r].count == data[smallest].count && strcmp(data[r].word, data[smallest].word) > 0))) {
        smallest = r;
    }

    if (smallest != i) {
        swapWords(data[i], data[smallest]);
        bubbleDown(smallest);
    }
}

// Public methods


bool MinHeap::isFull() const {
    return size == 10;
}

void MinHeap::insert(const Word& word) {
    if (size < 10) {
        data[size++] = word;
        bubbleUp(size - 1);
    }
    else {
        if (word.count < data[0].count ||
            (word.count == data[0].count && strcmp(word.word, data[0].word) > 0)) {
            data[0] = word;
            bubbleDown(0);
        }
    }
}

void MinHeap::trimToSize(int targetSize) {
    while (size > targetSize) {
        removeTop();
    }
}

Word MinHeap::getTop() {
    return size == 0 ? Word{ "", 0 } : data[0];
}

void MinHeap::removeTop() {
    if (size == 0) return;
    data[0] = data[--size];
    bubbleDown(0);
}

int MinHeap::getSize() const {
    return size;
}
