#pragma once
#include "asteroid.h"
#include <windows.h>
#include <vector> 
#include "bullet.h"
class Game {
	public:
		Game();
		void run();

	private:
		void processInput();
		void update();
		void render();
		void clearScreen();

		HANDLE hConsole;
		CHAR_INFO screen[80*30];
		int width = 80;
		int height = 30;

		int shipX;
		int shipY;

		DWORD lastTime;

		std::vector<Bullet> bullets;
		std::vector<Asteroid> asteroids;
		int asteroidSpawnTimer = 0;
};
