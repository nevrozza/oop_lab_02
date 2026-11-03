#include "str/string_class.h"
#include <iostream>

using namespace std;

int main() {
    cout << "Введите строку: ";
    String text;
    char ch;
    while (cin.get(ch) && ch != '\n') {
        text.push_back(ch);
    }


    const size_t capacity = text.size() / 2 + text.size() % 2;
    String *words = new String[capacity];
    size_t *counts = new size_t[capacity];
    size_t count;
    text.word_histogram(words, counts, count);
    for (size_t i = 0; i < count; ++i) {
        cout << words[i].c_str() << ": " << counts[i] << '\n';
    }

    delete[] counts;
    delete[] words;

    return 0;
}
