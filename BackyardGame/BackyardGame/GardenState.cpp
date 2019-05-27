#include "GardenState.h"



void GardenState::checkIfPlayerLeftGarden()
{
	if (player.getPosition().x < 5) 
	{
		music.StopPlayingSoundtrack();
		backToBackyard = true;
	}
}

GardenState::GardenState()
{
}


GardenState::GardenState(sf::RenderWindow * window) : GameState(window)
{
	map.LoadGardenMap(); // load map
	//entities settings
	fence_left.setSpritePosition(50, 50);
	fence_right.setSpritePosition(745, 50);
	fence_up.setSpritePosition(50, 50);
	fence_down1.setSpritePosition(50, 305);
	fence_down2.setSpritePosition(440, 305);
	flowers1.setSpritePosition(120, 130);
	flowers1.getSprite().setScale(4.f, 4.f);
	flowers2.setSpritePosition(465, 130);
	flowers2.getSprite().setScale(4.f, 4.f);
	chest.setSpritePosition(380, 75);
	chest.getSprite().setScale(3.f, 3.f);
	rock.setSpritePosition(600, 450);
	rock.getSprite().setScale(6.f, 6.f);
	dice_guy.setSpritePosition(380, 300);
	player.setSpritePosition(10, 400);

	fence_left.getSprite().setTexture(textures[9]);
	fence_right.getSprite().setTexture(textures[10]);
	fence_up.getSprite().setTexture(textures[7]);
	fence_down1.getSprite().setTexture(textures[8]);
	fence_down2.getSprite().setTexture(textures[8]);
	flowers1.getSprite().setTexture(textures[11]);
	flowers2.getSprite().setTexture(textures[11]);
	chest.getSprite().setTexture(textures[12]);
	rock.getSprite().setTexture(textures[13]);
	dice_guy.getSprite().setTexture(textures[14]);
	player.getSprite().setTexture(textures[1]);
}

GardenState::~GardenState()
{
}

void GardenState::checkMovementLimits(const double & dt)
{
}

void GardenState::colisionPreventEverything(const double & dt)
{
	colisionPreventing(player, fence_left, dt);
	colisionPreventing(player, fence_up, dt);
	colisionPreventing(player, fence_right, dt);
	colisionPreventing(player, fence_down1, dt);
	colisionPreventing(player, fence_down2, dt);
	colisionPreventing(player, chest, dt);
	colisionPreventing(player, rock, dt);
	colisionPreventing(player, dice_guy, dt);
}

void GardenState::update(const double & dt)
{
	this->updateKeybinds(dt); // works
	checkMovementLimits(dt); // works ///NEW
	this->map.update(dt); // ?
	this->player.update(dt); // works
	this->score.update(dt); 
	this->fence_left.update(dt);
	this->fence_up.update(dt);
	this->fence_right.update(dt);
	this->fence_down1.update(dt);
	this->fence_down2.update(dt);
	this->flowers1.update(dt);
	this->flowers2.update(dt);
	this->chest.update(dt);
	this->rock.update(dt);
	this->dice_guy.update(dt);
	rotatingPlayer(player, dt);
	checkDicesAction();
	colisionPreventEverything(dt); // <-- preventing collisions with all objects
	checkIfPlayerLeftGarden();
}

void GardenState::render(sf::RenderTarget * target)
{
	// the order matters
	this->map.render(this->window);
	this->fence_left.render(this->window);
	this->fence_up.render(this->window);
	this->fence_right.render(this->window);
	this->fence_down1.render(this->window);
	this->fence_down2.render(this->window);
	this->flowers1.render(this->window);
	this->flowers2.render(this->window);
	this->chest.render(this->window);
	this->rock.render(this->window);
	this->score.render(this->window);
	this->dice_guy.render(this->window);
	drawDicesSprites();
	this->window->draw(NPCMessage);
	this->window->draw(NPCResultText);
	this->player.render(this->window);
}




//Dices functions

void GardenState::moveCursor()
{
	if (this->player.getIsBlocked())
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			while (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				// to ignore long time press
			}
			GameDicesToDraw[1] = dice_guy.rightPressed();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			while (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				// to ignore long time press
			}
			GameDicesToDraw[1] = dice_guy.leftPressed();
		}
	}
}

void GardenState::activateDiceGuy()
{	
	this->GameDicesToDraw.push_back(this->dice_guy.getBoardSprite()); // board to draw						0
	this->GameDicesToDraw.push_back(this->dice_guy.getCursorSprite()); // cursor to draw					1
	this->GameDicesToDraw.push_back(this->dice_guy.getOponnentBoardSprite()); // oponnent board to draw		2
}


void GardenState::drawDiceGuyChoice()
{
	activateDiceGuy();
	this->dice_guy.drawNPCChoice(); // draw NPC's choice	
}

void GardenState::drawDicesSprites()
{
	std::vector<sf::Sprite>::iterator it = GameDicesToDraw.begin();
	for (it; it != GameDicesToDraw.end(); it++)
	{
		this->window->draw(*it);
	}
}
void GardenState::playDices()
{
	while (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {} // to ignore long time press
	music.PlayBattleSoundtrack();
	blockPlayer();
	if (score.getScore() >= 5)
	{
		blockPlayer();
		drawDiceGuyChoice();
	}
	else
	{
		dice_guy.notEnoughCoins();
		this->NPCMessage = dice_guy.getNPCMessage();
		finishedMiniGame = true;
		music.PlayOutsideSoundtrack();
	}
}

void GardenState::DicesResult()
{
	while (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {} // to ignore long time press
	this->GameDicesToDraw.erase(GameDicesToDraw.begin() + 1); // delete cursor
	this->GameDicesToDraw.push_back(dice_guy.getOponnentDice1().getDiceSprite()); // NPC choice to draw
	this->GameDicesToDraw.push_back(dice_guy.getOponnentDice2().getDiceSprite());
	this->GameDicesToDraw.push_back(dice_guy.getOponnentDice3().getDiceSprite());
	this->GameDicesToDraw.push_back(dice_guy.getOponnentDice4().getDiceSprite());
	this->GameDicesToDraw.push_back(dice_guy.getOponnentDice5().getDiceSprite());
	this->NPCMessage = dice_guy.getNPCMessage(); // NPC message to draw
	this->NPCResultText = dice_guy.getNPCResultText(); // result to draw
	if (this->NPCResultText.getString() == "YOU WON")
		score.add(10);
	else if (this->NPCResultText.getString() == "YOU LOST")
		score.subtract(10);
	finishedMiniGame = true;
}


void GardenState::finishDices()
{
	while (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {} // to ignore long time press
	this->GameDicesToDraw.clear();
	this->NPCMessage.setString("");
	this->NPCResultText.setString("");
	finishedMiniGame = false;
	unblockPlayer();
	music.PlayOutsideSoundtrack();
}

void GardenState::checkDicesAction()
{
	if (player.getIsBlocked() && finishedMiniGame)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
		{
			finishDices();
		}
	}
	if (player.getIsBlocked())
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
		{
			DicesResult();
		}
	}
	if (player.getIsBlocked() && !finishedMiniGame)
		moveCursor();
	if (player.getSprite().getGlobalBounds().intersects(dice_guy.getSprite().getGlobalBounds()))
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
		{
			playDices();
		}
	}
}
