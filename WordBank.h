#pragma once
#include <string>

namespace WordBank {
    std::string getRandomWord();
    void loadWords();
    void toLowerCase(std::string& str);
}
