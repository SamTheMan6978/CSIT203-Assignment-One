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
    while (i != 0 && (data[parent(i)].count < data[i].count ||
        (data[parent(i)].count == data[i].count && strcmp(data[parent(i)].word, data[i].word) > 0))) {
        swap(data[i], data[parent(i)]);
        i = parent(i);
    }
}

void MaxHeap::bubbleDown(int i) {
    int l = left(i);
    int r = right(i);
    int largest = i;

    if (l < size && (data[l].count > data[i].count ||
        (data[l].count == data[i].count && strcmp(data[l].word, data[i].word) < 0))) {
        largest = l;
    }

    if (r < size && (data[r].count > data[largest].count ||
        (data[r].count == data[largest].count && strcmp(data[r].word, data[largest].word) < 0))) {
        largest = r;
    }

    if (largest != i) {
        swap(data[i], data[largest]);
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
    if (size == 0) return;
    data[0] = data[size - 1];
    size--;
    bubbleDown(0);
}

Word MaxHeap::getTop() {
    if (size == 0) {
        Word emptyWord = { "", 0 };
        return emptyWord;
    }
    return data[0];
}
