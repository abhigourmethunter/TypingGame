#include <string>
#include "Word.h"
#include <raylib.h>

Word::Word(const std::string& t, float xpos, float ypos) : text(t), x(xpos), y(ypos) {
}

void Word::draw(int matchedChars, int textSize) const{
    if(isPotentialMatched) {
        DrawText(text.substr(0, matchedChars).c_str(), x, y, textSize, SKYBLUE);
        DrawText(text.substr(matchedChars).c_str(), x + MeasureText(text.substr(0, matchedChars).c_str(), textSize), y, textSize, WHITE);
    }
    else{
        DrawText(text.c_str(), x, y, textSize, WHITE);
    }
}