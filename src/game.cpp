#include "game.hpp"
#include <cstdlib>
#include <ctime>

BirdGame::BirdGame() {
	std::srand(std::time(nullptr));
	initGame();
}

void BirdGame::initGame() {
	std::lock_guard<std::mutex> lock(game_mutex);

	bird_x = 3.0;
	bird_y = height / 2;    
	velocity = 0.0;
	
	bar_x = width / 2;       
	bar_y_up = height / 3;   
	bar_y_down = height / 2; 
	
	if (score > high_score) high_score = score;
	score = 0;
	is_game_over = false;
	score_counted = false;
}

void BirdGame::checkInput() {
	std::lock_guard<std::mutex> lock(game_mutex);
	if (is_game_over) return;
	velocity = -1.1; 
}

void BirdGame::updatePhysics() {
	std::lock_guard<std::mutex> lock(game_mutex);
	if (is_game_over) return;

	const double gravity = 0.15; 
	bird_y += velocity;
	velocity += gravity;

	bar_x--;

	is_game_over = checkStatus();
	
	rebuildBar();
}

bool BirdGame::checkStatus() {
	if (bird_y >= height || bird_y < 0) return true;
	
	if (int(bird_x) == bar_x) {
		if (int(bird_y) >= bar_y_up && int(bird_y) <= bar_y_down) {
			if (!score_counted) {
				score++;
				score_counted = true; 
			}
		} else {
			return true; 
		}
	} else {
		if (bar_x < int(bird_x)) {
			score_counted = false;
		}
	}
	return false;
}

void BirdGame::rebuildBar() {
	if (bar_x <= 0) {
		int temp = std::rand() % (int)(height * 0.8);
		bar_x = width;
		bar_y_up = temp - height / 10;
		bar_y_down = temp + height / 10;
		score_counted = false;
	}
}
