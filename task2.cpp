#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Word {
	string word;
	int count;
	int pages[100];
};

int main() {
	ifstream input("input.txt");

	const string stopWords[] = {
		"the", "to", "of", "and", "her", "a", "in", "was", "i", "she", "that", "not", "he", "his", "be", "as",
		"had", "it", "you", "with", "for", "but", "have", "is", "at", "on", "my", "by", "they", "were", "",
		"all", "so", "could", "him", "which", "been", "from", "very", "would", "their", "no", "your", "what"
    };
    const int countOfStopWords = 44;
    const int SIZE_OF_PAGE = 45;

    int countOfWords = 0;
    int sizeOfArray = 10;
	Word *words = new Word[sizeOfArray];
	int currentLineNumber = 0;

    readText:
        string line;

	    if (getline(input, line)) {
		    currentLineNumber++;

            if (line == "") { // i.e line is empty
                goto readText;
            }

		    int start = 0, end = 0;

	        processWords:

            increaseEnd:
                if (line[end] != ' ' && line[end] != '\0') {
                    end++;
                    goto increaseEnd;
                }

            string word;
            int length = 0;
            makeWord:
                if (start < end) {
                    word += line[start];
                    length++;
                    start++;
                    goto makeWord;
                }

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
						// cout << "found" << endl;

						if (line[end] == '\0') {
			                goto readText;
			            }
                        end++;
						start = end;
                        goto processWords; // i.e. skip all the other part
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
                        if (words[j].count < 100) {
                            words[j].count++;
                            words[j].pages[words[j].count - 1] = (currentLineNumber + SIZE_OF_PAGE) / SIZE_OF_PAGE;
                        }
                    }
                    j++;
                    goto findWord;
                }

            if (countOfWords + 1 >= sizeOfArray && !isFound) {
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
            }

            if (!isFound && length > 2) {
                words[countOfWords].word = word;
                words[countOfWords].count = 1;
                words[countOfWords].pages[words[countOfWords].count - 1] = (currentLineNumber + SIZE_OF_PAGE)
																			/ SIZE_OF_PAGE;
                countOfWords++;
            }

            if (line[end] == '\0') {
                goto readText;
            }

            end++;
			start = end;
            goto processWords;
        }

    int i = 0;
    outerLoop:
        if (i < countOfWords) {
            int j = 0;
            innerLoop:
                if (j < countOfWords - 1) {
                    if (words[j].word > words[j + 1].word) {
                        Word tmpWord = words[j];
                        words[j] = words[j + 1];
                        words[j + 1] = tmpWord;
                    }
                    j++;
                    goto innerLoop;
                }
            i++;
            goto outerLoop;
        }

    i = 0;
    outerPrintLoop:
        if (i < countOfWords) {
            cout << words[i].word << " -->";
            int j = 0;
            innerPrintLoop:
                if (j < words[i].count) {
                    cout << " " << words[i].pages[j];
                    j++;
                    goto innerPrintLoop;
                }
            cout << endl;
            i++;
            goto outerPrintLoop;
        }
	input.close();
}
