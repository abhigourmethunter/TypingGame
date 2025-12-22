#include <string>
#include "Word.h"
#include <raylib.h>

Word::Word(const std::string& t, int xpos, int ypos) : text(t), x(xpos), y(ypos) {
}

void Word::draw() const{
    DrawText(text.c_str(), x, y, 20, WHITE);
}