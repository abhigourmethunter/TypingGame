#include <iostream>
#include <raylib.h>
#include "WordBank.h"
#include "Game.h"

int main () {

    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "TypingGame");
    SetTargetFPS(60);

    WordBank::loadWords();
    Game game = Game(SCREEN_WIDTH, SCREEN_HEIGHT);

    while (WindowShouldClose() == false){

        game.update();
        
        BeginDrawing();
            ClearBackground(BLACK);
            game.draw();
        EndDrawing();

    }

    CloseWindow();

}