#include "markov.h"
#include <fstream>
#include <cstdlib>

std::string joinWords(const std::string words[], int startIndex, int count) {
    if (count <= 0) return "";
    std::string result = "";
    for (int i = 0; i < count; i++) {
        result += words[startIndex + i];
        if (i < count - 1) result += " ";
    }
    return result;
}

int readWordsFromFile(std::string filename, std::string words[], int maxWords) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) return -1;

    int counter = 0;
    while (counter < maxWords && inputFile >> words[counter]) {
        counter++;
    }

    inputFile.close();
    return counter;
}

int buildMarkovChain(const std::string words[], int numWords, int order,
                     std::string prefixes[], std::string suffixes[],
                     int maxChainSize) {

    if (order < 1 || order > 3 || numWords <= order || maxChainSize <= 0)
        return 0;

    int count = 0;

    for (int i = 0; i < numWords - order && count < maxChainSize; i++) {
        prefixes[count] = joinWords(words, i, order);
        suffixes[count] = words[i + order];
        count++;
    }

    return count;
}

std::string getRandomSuffix(const std::string prefixes[], const std::string suffixes[],
                            int chainSize, std::string currentPrefix) {

    if (chainSize <= 0) return "";

    int matchCount = 0;
    for (int i = 0; i < chainSize; i++) {
        if (prefixes[i] == currentPrefix)
            matchCount++;
    }

    if (matchCount == 0) return "";

    int pick = rand() % matchCount;

    int seen = 0;
    for (int i = 0; i < chainSize; i++) {
        if (prefixes[i] == currentPrefix) {
            if (seen == pick)
                return suffixes[i];
            seen++;
        }
    }

    return "";
}

std::string getRandomPrefix(const std::string prefixes[], int chainSize) {
    if (chainSize <= 0) return "";
    int index = rand() % chainSize;
    return prefixes[index];
}

std::string generateText(const std::string prefixes[], const std::string suffixes[],
                         int chainSize, int order, int numWords) {

    if (chainSize <= 0 || order < 1 || order > 3 || numWords < order)
        return "";

    std::string currentPrefix = getRandomPrefix(prefixes, chainSize);
    if (currentPrefix == "") return "";

    std::string result = currentPrefix;

    std::string currentWords[3];
    int wordIndex = 0;
    std::string temp = "";

    for (int i = 0; i < currentPrefix.length(); i++) {
        if (currentPrefix[i] == ' ') {
            currentWords[wordIndex] = temp;
            wordIndex++;
            temp = "";
        } else {
            temp += currentPrefix[i];
        }
    }
    currentWords[wordIndex] = temp;

    for (int i = 0; i < numWords - order; i++) {
        std::string newWord = getRandomSuffix(prefixes, suffixes, chainSize, currentPrefix);
        if (newWord == "") break;

        result += " " + newWord;

        for (int j = 0; j < order - 1; j++) {
            currentWords[j] = currentWords[j + 1];
        }
        currentWords[order - 1] = newWord;

        currentPrefix = joinWords(currentWords, 0, order);
    }

    return result;
}
