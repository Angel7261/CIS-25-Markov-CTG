#include "markov.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main() {
    srand(time(0));

    std::string filename;
    int order;
    int maxWords;

    std::cout << "Enter input filename: ";
    std::cin >> filename;

    std::cout << "Enter order (1, 2, or 3): ";
    std::cin >> order;

    std::cout << "Enter maximum number of words to generate: ";
    std::cin >> maxWords;

    if (order < 1 || order > 3 || maxWords < order) {
        std::cout << "Invalid order or word count.\n";
        return 0;
    }

    const int MAX_WORDS = 5000;
    std::string words[MAX_WORDS];
    std::string prefixes[MAX_WORDS];
    std::string suffixes[MAX_WORDS];

    int count = readWordsFromFile(filename, words, MAX_WORDS);

    if (count == -1) {
        std::cout << "Error: Could not open file.\n";
        return 0;
    }

    if (count <= order) {
        std::cout << "Not enough words in file to build chain.\n";
        return 0;
    }

    int chainSize = buildMarkovChain(words, count, order, prefixes, suffixes, MAX_WORDS);

    if (chainSize <= 0) {
        std::cout << "Chain could not be built.\n";
        return 0;
    }

    std::string output = generateText(prefixes, suffixes, chainSize, order, maxWords);

    std::cout << "\nGenerated Text:\n" << output << "\n";

    int actualCount = 1;
    for (char c : output) {
        if (c == ' ') actualCount++;
    }

    std::cout << "\nGenerated " << actualCount << " of at most " << maxWords << " words.\n";

    return 0;
}
