#pragma once
#include "GameState.h"
class Backyard : public GameState
{
private:
	Entity house, lake, red_tree;
	RPS old_man;
	std::vector <sf::Sprite> GameRPSToDraw;
	// RPS:
	void moveCursor();
	void activateOldMan();
	void drawOldManChoice();
	void drawRPSSprites();
	void playRPS();
	void RPSResult();
	void finishRPS();
	void checkRPSAction();
public:
	Backyard();
	Backyard(sf::RenderWindow* window);
	~Backyard();
	void checkDoor(Player & player, const double& dt);
	void colisionPreventEverything(const double &dt);
	void update(const double& dt);
	void render(sf::RenderTarget* target = nullptr);
};