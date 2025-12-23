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
    void drawHome();
    void drawPlay();
    void drawPause();
    void drawGameOver();
    
    void updateHome();
    void updatePlay();
    void updatePause();
    void updateGameOver();

    enum class GameState {
        HOME,
        PLAY,
        PAUSE,
        GAMEOVER
    } currentState = GameState::HOME;

    std::vector<Word> activeWords;
    int timer;
    std::string typedString;
    int currentMatches;
    const int SCREEN_WIDTH;
    const int SCREEN_HEIGHT;

    float cursorBlinkTimer = 0.0f;
    
    const int TEXT_SIZE = 50;
    const int TYPING_TEXT_SIZE = 35;
    const int WORD_FALLING_SPEED = 2.75;
};
