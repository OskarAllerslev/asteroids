#include <cstdlib>
#include <algorithm>
#include "game.h"
#include <conio.h>  // for _kbhit and _getch
#include <chrono>
#include <thread>

Game::Game() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    shipX = width / 2;
    shipY = height - 2;
    lastTime = GetTickCount();
}

void Game::run() {
    while (true) {
        processInput();
        update();
        render();
        std::this_thread::sleep_for(std::chrono::milliseconds(33)); // ~30 FPS
    }
}

void Game::processInput() {
	if (_kbhit()) {
		int key = _getch();
		if (key == 224) {
			key = _getch();
			if (key == 75 && shipX > 1) shipX--;
			else if (key == 77 && shipX < width - 2) shipX++;
		} 
		else if (key == 32) {
			bullets.emplace_back(shipX, shipY - 1);
		}
	}
}

void Game::update() {
	// Flyt eksisterende asteroider ned
	for (auto& a : asteroids) {
		a.y++;
	}

	// Fjern dem der rammer bunden
	asteroids.erase(std::remove_if(asteroids.begin(), asteroids.end(), [&](Asteroid& a) {
				return a.y >= height;
				}), asteroids.end());

	// Spawn ny asteroid hver ~20 frames
	asteroidSpawnTimer++;
	if (asteroidSpawnTimer >= 20) {
		int ax = rand() % width;
		asteroids.emplace_back(ax, 0);
		asteroidSpawnTimer = 0;
	}
}

void Game::render() {
    clearScreen();

    // Tegn skib
    screen[shipY * width + shipX].Char.AsciiChar = '^';
    screen[shipY * width + shipX].Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;


    for (auto& b : bullets) {
	    if (b.y >= 0 && b.y < height) {
		    screen[b.y * width + b.x].Char.AsciiChar = '|';
		    screen[b.y * width + b.x].Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
	    }
    }

    // Tegn asteroider
    for (auto& a : asteroids) {
	    if (a.y >= 0 && a.y < height) {
		    screen[a.y * width + a.x].Char.AsciiChar = 'O';
	    screen[a.y * width + a.x].Attributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	    }
    }
    // Tegn hele skærmen
    SMALL_RECT rect = { 0, 0, (SHORT)(width - 1), (SHORT)(height - 1) };
    COORD size = { (SHORT)width, (SHORT)height };
    COORD pos = { 0, 0 };
    WriteConsoleOutputA(hConsole, screen, size, pos, &rect);
}

void Game::clearScreen() {
    for (int i = 0; i < width * height; i++) {
        screen[i].Char.AsciiChar = ' ';
        screen[i].Attributes = 0;
    }
}
