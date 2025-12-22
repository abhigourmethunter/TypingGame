#pragma once
#include <string>

class Word {
    public:
        Word(const std::string& t, int xpos, int ypos);
        void draw() const;

        std::string text;
        int x;
        int y;
};