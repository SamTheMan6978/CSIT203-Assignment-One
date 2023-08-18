#include <iostream>
#include <fstream>
#include <cstring>

#include "Algorithm.h"

using namespace std;



// Functions
void toLowerCase(char* s) {
    while (*s) {
        if (static_cast<unsigned char>(*s) >= 0 && static_cast<unsigned char>(*s) <= 255) {
            *s = tolower(static_cast<unsigned char>(*s));
        }
        s++;
    }
}

void stripPunctuation(char* word) {
    int j = 0;
    for (int i = 0; word[i]; i++) {
        unsigned char current = static_cast<unsigned char>(word[i]);
        unsigned char prev = i > 0 ? static_cast<unsigned char>(word[i - 1]) : 0;
        unsigned char next = word[i + 1] ? static_cast<unsigned char>(word[i + 1]) : 0;

        if (isalpha(current) || (i > 0 && isalpha(prev) && isalpha(next))) {
            word[j++] = word[i];
        }
    }
    word[j] = '\0'; // String termination
}

int main() {
    AVLTree tree;

    // Variable declaration for file reading
    char filename[200];
    ifstream file;

    // File opening
    cout << "Please enter your file name: " << endl;
    cin >> filename;
    file.open(filename);

    // File handling
    if (file.is_open()) {
        cout << "File: " << filename << " has been opened successfully!" << endl;
    }
    else {
        cout << "ERROR: File not found!" << endl;
        return 1;
    }

    // Word reading
    char word[120];

    while (file >> word) {
        if (file.fail()) {
            cout << "ERROR: Reading Failed!" << endl;
            return 1;
        }
        else {
            stripPunctuation(word);
            toLowerCase(word);


            Word newWord;
            strcpy_s(newWord.word, word);
            newWord.count = 1;

            Node* existingWordNode = tree.search(word);
            if (!existingWordNode) {
                tree.insert(newWord);
            }
            else {
                existingWordNode->data.count++;
            }
        }
    }
    file.close();

    cout << "\nTop 10 Words:\n";
    tree.printTop10();

    cout << "\nLast 10 Words:\n";
    tree.printLast10();

    // Display unique words
    tree.printUniqueWords();

    return 0;
}
