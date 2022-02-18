#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
using namespace std;

struct Word {
    string word;
    int count;
};

int main() {
    ifstream input("input.txt");

    string stopWords[] = {
        "the", "to", "of", "and", "her", "a", "in", "was", "i", "she", "that", "not", "he", "his", "be", "as",
        "had", "it", "you", "with", "for", "but", "have", "is", "at", "on", "my", "by", "they", "were",
        "all", "so", "could", "him", "which", "been", "from", "very", "would", "their", "no", "your", "what"
    };
    const int countOfStopWords = 43;

    const int COUNT_OF_WORDS_TO_SHOW = 25;
    int countOfWords = 0;
    int sizeOfArray = 10;
    Word *words = new Word[sizeOfArray];

    string word;
    readText:
        if (input >> word) {

            int i = 0;

            string tmpWord;
            processWord:
                if (word[i] != '\0') {
                    if (word[i] >= 'A' && word[i] <= 'Z') {
                        word[i] += 32;
                    }

                    if (word[i] >= 'a' && word[i] <= 'z') {
                        tmpWord += word[i];
                    }
                    i++;
                    goto processWord;
                }
            word = tmpWord;

            int stopWord = 0;
            checkIsStopWord:
                if (stopWord < countOfStopWords) {
                    if (word == stopWords[stopWord]) {
                        goto readText; // i.e. skip all the other part
                    }
                    stopWord++;
                    goto checkIsStopWord;
                }

            int j = 0;
            bool isFound = false;
            findWord:
                if (j < countOfWords && !isFound) {
                    if (words[j].word == word) {
                        isFound = true;
                        words[j].count++;
                    }
                    j++;
                    goto findWord;
                }

                if (countOfWords + 1 > sizeOfArray && !isFound) {
                    sizeOfArray *= 2;
                    Word *tmpWords = new Word[sizeOfArray];

                    int currentWord = 0;
                    copyWords:
                        tmpWords[currentWord] = words[currentWord];
                        currentWord++;
                        if (currentWord < countOfWords) {
                            goto copyWords;
                        }

                    delete[] words;
                    words = tmpWords;

                    words[countOfWords].word = word;
                    words[countOfWords].count = 1;
                    countOfWords++;
                }
                if (!isFound) {
                    words[countOfWords].word = word;
                    words[countOfWords].count = 1;
                    countOfWords++;
                }

            goto readText;
        }

    int i = 0;
    outerCycle:
        if (i + 1 < countOfWords) {
            int j = 0;
            innerCycle:
                if (j + 1 + i < countOfWords) {
                    if (words[j].count < words[j + 1].count){
                        Word tmpWord = words[j];
                        words[j] = words[j + 1];
                        words[j + 1] = tmpWord;
                    }
                    j++;
                    goto innerCycle;
                }

            i++;
            goto outerCycle;
        }

    i = 0;
    showWords:
        if (i < countOfWords && i < COUNT_OF_WORDS_TO_SHOW) {
            cout << words[i].word << " – " << words[i].count << endl;
            i++;
            goto showWords;
        }
}
