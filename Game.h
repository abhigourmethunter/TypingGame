#pragma once
#include <vector>
#include "Word.h"

class Game {
public:
    Game(const int screen_width, const int screen_height);
    void update();
    void draw();

private:
    void resetTimer();
    void keyPressed();

    std::vector<Word> activeWords;
    int timer;
    int level;
    const int SCREEN_WIDTH;
    const int SCREEN_HEIGHT;
};
