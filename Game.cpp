#include "Game.h"
#include "Word.h"
#include "WordBank.h"
#include <raylib.h>
#include <cmath>

Game::Game(const int screen_width, const int screen_height) : timer(0), SCREEN_WIDTH(screen_width), SCREEN_HEIGHT(screen_height) {
    heartTexture = LoadTexture("assets/art/Hearts/heart_animated_1.png");

    for(int i = 0; i < MAX_LIVES; i++) {
        heartAnims[i] = Animation(0, 4, 1, 0.1f, Animation::AnimationType::Once);
    }

    resetGame();
}

Game::~Game() {
    UnloadTexture(heartTexture);
}

void Game::update() {
    switch (currentState)
    {
    case GameState::HOME:
        updateHome();
        break;
    
    case GameState::PLAY:
        updatePlay();
        break;
    
    case GameState::PAUSE:
        updatePause();
        break;
    
    case GameState::GAMEOVER:
        updateGameOver();
        break;
    
    default:
        break;
    }
}

void Game::updateHome() {
    if(IsKeyPressed(KEY_ENTER)) {
        currentState = GameState::PLAY;
    }
}

void Game::updatePlay() {

    if(IsKeyPressed(KEY_SPACE)) {
        currentState = GameState::PAUSE;
        return;
    }

    if (!timer) {
        resetTimer();
        std::string newWord = WordBank::getRandomWord();
        activeWords.push_back(Word{newWord, (float)(rand() % (SCREEN_WIDTH - MeasureText(newWord.c_str(), TEXT_SIZE))), 0.0f});
    }
    
    int key = GetCharPressed();
    while (key > 0) {
        if ((key >= 32) && (key <= 125)) {
            typedString += std::tolower((char)key);
        }
        key = GetCharPressed();
    }

    if ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_BACKSPACE)) {
        typedString.clear();
    }

    else if (IsKeyPressed(KEY_BACKSPACE) && !typedString.empty()) {
        typedString.pop_back();
    }

    currentMatches = 0;
    for(auto it = activeWords.begin(); it != activeWords.end(); ) {
        if(typedString.size() <= it->text.size() && (it->text.compare(0, typedString.length(), typedString) == 0)) {
            it->isPotentialMatched = true;
            currentMatches++;
            if (it->text == typedString) {
                it = activeWords.erase(it);
                score++;
                currentMatches--;
                continue;
            }
        }
        else {
            it->isPotentialMatched = false;
        }
        
        it->y += WORD_FALLING_SPEED;
        
        if (it->y > SCREEN_HEIGHT - (TEXT_SIZE*2) - 15) {
            if(it->isPotentialMatched) {
                currentMatches--;
            }
            it = activeWords.erase(it);

            lives--;
            if(lives <= 0) {
                currentState = GameState::GAMEOVER;
            }
        }
        else {
            ++it;
        }
    }

    if(currentMatches == 0) {
        typedString.clear();
    }
        
    cursorBlinkTimer += GetFrameTime();

    if(frameCounter % GetFPS() == 0){
        seconds++;
        if(seconds % 8 == 0){
            wordRate--;  
            if(wordRate < 20) {
                wordRate = 20;
            }
        }
    }

    timer--;
    frameCounter++;

    for(int i = 0; i < MAX_LIVES; i++) {
        if (i >= lives) {
            heartAnims[i].update();
        }
    }
}

void Game::resetGame() {
    lives = MAX_LIVES;
    score = 0;
    seconds = 0;
    frameCounter = 0;
    wordRate = 90;
    activeWords.clear();
    typedString.clear();
    for(int i = 0; i < MAX_LIVES; i++) {
        heartAnims[i].cur = heartAnims[i].first;
    }
    resetTimer();
}

void Game::updatePause() {    
    if(IsKeyPressed(KEY_TAB)) {
        currentState = GameState::HOME;
        resetGame();
    }
    if(IsKeyPressed(KEY_SPACE)) {
        currentState = GameState::PLAY;        
    }
}

void Game::updateGameOver() {
    if(IsKeyPressed(KEY_ENTER)) {
        currentState = GameState::PLAY;
        resetGame();
    }

    if(IsKeyPressed(KEY_TAB)) {
        currentState = GameState::HOME;
        resetGame();
    }
}

void Game::draw() {
    switch (currentState)
    {
    case GameState::HOME:
        drawHome();
        break;
    
    case GameState::PLAY:
        drawPlay();
        break;
    
    case GameState::PAUSE:
        drawPause();
        break;
    
    case GameState::GAMEOVER:
        drawGameOver();
        break;
    
    default:
        break;
    }
}

void Game::drawHome() {
    DrawText("Press ENTER to Start", (SCREEN_WIDTH - MeasureText("Press ENTER to Start", TEXT_SIZE)) / 2, SCREEN_HEIGHT / 2 - TEXT_SIZE, TEXT_SIZE, LIGHTGRAY);
    DrawText("Press ESC to Exit", (SCREEN_WIDTH - MeasureText("Press ESC to Exit", TEXT_SIZE)) / 2, SCREEN_HEIGHT / 2 + TEXT_SIZE, TEXT_SIZE, LIGHTGRAY);
}

void Game::drawPlay() {
    
    int typedTextWidth = MeasureText(typedString.c_str(), TEXT_SIZE);

    DrawRectangle(0, SCREEN_HEIGHT - TEXT_SIZE - 15, SCREEN_WIDTH, TEXT_SIZE + 15, GRAY);
    DrawText(typedString.c_str(), (SCREEN_WIDTH - typedTextWidth) / 2, SCREEN_HEIGHT - TEXT_SIZE - 5, TEXT_SIZE, GREEN);
    if (fmod(cursorBlinkTimer, 1.0f) < 0.5f) {
        DrawRectangle((SCREEN_WIDTH + typedTextWidth) / 2, SCREEN_HEIGHT - TEXT_SIZE - 5, 2, TEXT_SIZE, GREEN);
    }

    for (const Word& word : activeWords) {
        word.draw(typedString.length(), TEXT_SIZE);
    }

    const int BAR_HEIGHT = 40;
    DrawRectangle(0, 0, SCREEN_WIDTH, BAR_HEIGHT, BLUE);
    
    std::string scoreText = "Score: " + std::to_string(score);
    DrawText(scoreText.c_str(), 15, (BAR_HEIGHT - TEXT_SIZE + 10) / 2, TEXT_SIZE - 10, WHITE);
    
    /*std::string livesText = "Lives: " + std::to_string(lives);
    int livesTextWidth = MeasureText(livesText.c_str(), TEXT_SIZE);
    DrawText(livesText.c_str(), SCREEN_WIDTH - livesTextWidth - 15, (BAR_HEIGHT - TEXT_SIZE + 10) / 2, TEXT_SIZE - 10, WHITE);*/
    
    const int HEART_SIZE = 30;
    const int HEART_SPACING = 5;
    for(int i = 0; i < MAX_LIVES; i++) {
        float heartX = SCREEN_WIDTH - (MAX_LIVES - i) * (HEART_SIZE + HEART_SPACING) - 10;
        float heartY = (BAR_HEIGHT - HEART_SIZE) / 2;
        DrawTexturePro(heartTexture, heartAnims[i].frame(5), 
                       {heartX, heartY, HEART_SIZE, HEART_SIZE}, 
                       {0, 0}, 0.0f, WHITE);
    }
}

void Game::drawPause() {
    drawPlay();
    
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.9f));
    
    DrawText("GAME PAUSED", (SCREEN_WIDTH - MeasureText("GAME PAUSED", TEXT_SIZE)) / 2, SCREEN_HEIGHT / 2 - TEXT_SIZE, TEXT_SIZE, LIGHTGRAY);
    DrawText("Press SPACE to Resume", (SCREEN_WIDTH - MeasureText("Press SPACE to Resume", TEXT_SIZE)) / 2, SCREEN_HEIGHT / 2 + TEXT_SIZE, TEXT_SIZE, LIGHTGRAY);
    DrawText("Press TAB for Home Menu", (SCREEN_WIDTH - MeasureText("Press TAB for Home Menu", TEXT_SIZE)) / 2, SCREEN_HEIGHT / 2 + (TEXT_SIZE * 3), TEXT_SIZE, LIGHTGRAY);
}

void Game::drawGameOver() {
    DrawText("Your Score: ", (SCREEN_WIDTH - MeasureText("Your Score: ", TEXT_SIZE)) / 2, SCREEN_HEIGHT / 2 - (TEXT_SIZE*2), TEXT_SIZE, SKYBLUE);
    DrawText(std::to_string(score).c_str(), (SCREEN_WIDTH - MeasureText(std::to_string(score).c_str(), TEXT_SIZE)) / 2, SCREEN_HEIGHT / 2 - (TEXT_SIZE*2), TEXT_SIZE, SKYBLUE); 
    DrawText("GAME OVER", (SCREEN_WIDTH - MeasureText("GAME OVER", TEXT_SIZE)) / 2, SCREEN_HEIGHT / 2 - TEXT_SIZE, TEXT_SIZE, LIGHTGRAY);
    DrawText("Press ENTER to Restart", (SCREEN_WIDTH - MeasureText("Press ENTER to Restart", TEXT_SIZE)) / 2, SCREEN_HEIGHT / 2 + TEXT_SIZE, TEXT_SIZE, LIGHTGRAY);
    DrawText("Press TAB for Home Menu", (SCREEN_WIDTH - MeasureText("Press TAB for Home Menu", TEXT_SIZE)) / 2, SCREEN_HEIGHT / 2 + (TEXT_SIZE * 3), TEXT_SIZE, LIGHTGRAY);
}

void Game::resetTimer() {
    timer = wordRate;
}