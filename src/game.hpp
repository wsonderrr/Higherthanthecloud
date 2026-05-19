#ifndef GAME_HPP
#define GAME_HPP

#include <mutex>

class BirdGame {
public:
	const int width = 50;
	const int height = 20;
	
	double bird_x = 3.0;
	double bird_y = 10.0;
	double velocity = 0.0;
	
	int bar_x = 25;
	int bar_y_up = 6;      
	int bar_y_down = 10;   
	
	int score = 0;
	int high_score = 0;
	bool is_game_over = false;
	bool score_counted = false; 
	
	std::mutex game_mutex;
	
	BirdGame();
	void initGame();       
	void checkInput();     
	void updatePhysics();  
	bool checkStatus();    
	void rebuildBar();     
};

#endif
