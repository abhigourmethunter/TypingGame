#include "Game.h"
#include "Word.h"
#include "WordBank.h"
#include <raylib.h>
#include <iostream>

Game::Game(const int screen_width, const int screen_height) : timer(0), level(1), SCREEN_WIDTH(screen_width), SCREEN_HEIGHT(screen_height) {
    resetTimer();

}

void Game::update() {
    std::cout << "timer: " << timer << std::endl;

    if (!timer) {
        resetTimer();
                    activeWords.push_back(Word{WordBank::getRandomWord(), rand() % SCREEN_WIDTH, 0  });
    }

    for(auto& word : activeWords) {
        word.y += 5;
    }

    timer--;
    
    // check for typing
}

void Game::draw() {
    std::cout << "Drawing game state" << std::endl;

    for (const Word& word : activeWords) {
        word.draw();
    }
}

void Game::resetTimer() {
    timer = 30 - level;
}