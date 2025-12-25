#pragma once
#include "Animation.h"
#include "Word.h"
#include <vector>

class Game {
public:
    Game(const int screen_width, const int screen_height);
    ~Game();
    void update();
    void draw();

private:

    const int TEXT_SIZE = 50;
    const int TYPING_TEXT_SIZE = 35;
    const int WORD_FALLING_SPEED = 2.75;
    static constexpr int MAX_LIVES = 5;

    void resetTimer();
    void drawHome();
    void drawPlay();
    void drawPause();
    void drawGameOver();
    
    void updateHome();
    void updatePlay();
    void updatePause();
    void updateGameOver();

    void resetGame();

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
    int score = 0;
    int seconds = 0;
    int frameCounter = 0;
    int wordRate = 90;
    int lives = MAX_LIVES;

    float cursorBlinkTimer = 0.0f;

    Texture2D heartTexture;
    Animation heartAnims[MAX_LIVES];

};
