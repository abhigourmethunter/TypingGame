#include "Game.h"
#include "Word.h"
#include "WordBank.h"
#include <raylib.h>
#include <cmath>

Game::Game(const int screen_width, const int screen_height) : timer(0), SCREEN_WIDTH(screen_width), SCREEN_HEIGHT(screen_height) {
    resetTimer();
}

void Game::update() {

    if (!timer) {
        resetTimer();
        std::string newWord = WordBank::getRandomWord();
        activeWords.push_back(Word{newWord, (float)(rand() % (SCREEN_WIDTH - MeasureText(newWord.c_str(), TEXT_SIZE))), 0.0f});
    }

    currentMatches = 0;
    for(auto it = activeWords.begin(); it != activeWords.end(); ) {
    if(typedString.size() <= it->text.size() && (it->text.compare(0, typedString.length(), typedString) == 0)) {
        it->isPotentialMatched = true;
        currentMatches++;
        if (it->text == typedString) {
            it = activeWords.erase(it);
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
    }
    else {
        ++it;
    }
}

    if(currentMatches == 0) {
        typedString.clear();
    }

    timer--;
    
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

    cursorBlinkTimer += GetFrameTime();

}

void Game::draw() {
    int typedTextWidth = MeasureText(typedString.c_str(), TEXT_SIZE);

    DrawRectangle(0, SCREEN_HEIGHT - TEXT_SIZE - 15, SCREEN_WIDTH, TEXT_SIZE + 15, GRAY);
    DrawText(typedString.c_str(), (SCREEN_WIDTH - typedTextWidth) / 2, SCREEN_HEIGHT - TEXT_SIZE - 5, TEXT_SIZE, GREEN);
    if (fmod(cursorBlinkTimer, 1.0f) < 0.5f) {
        DrawRectangle((SCREEN_WIDTH + typedTextWidth) / 2, SCREEN_HEIGHT - TEXT_SIZE - 5, 2, TEXT_SIZE, GREEN);
    }

    for (const Word& word : activeWords) {
        word.draw(typedString.length(), TEXT_SIZE);
    }
}

void Game::resetTimer() {
    timer = 80; //add level tuned timer later
}