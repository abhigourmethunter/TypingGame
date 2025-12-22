#include "WordBank.h"
#include <vector>
#include <fstream>
#include <cstdlib>

namespace WordBank {
    static std::vector<std::string> words;

    void loadWords() {
        std::ifstream wordFile("assets/other/words.txt");
        std::string word;
        for(; std::getline(wordFile, word); ) {
            toLowerCase(word);
            words.push_back(word);
        }
    }

    std::string getRandomWord() {
        return words[rand() % words.size()];
    }

    void toLowerCase(std::string& str) {
        for (char& c : str) {
            c = std::tolower(c);
        }
    }
}
