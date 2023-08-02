#include "Player.h"

Player* Player::create()
{
	Player* player = new Player();
	player->initWithFile("fighter.png");
	player->x = 0.0f;
	player->y = 0.0f;
	player->dx = 0.0f;
	player->dy = 0.0f;
	player->rotAngle = 0.0f;

	return player;
};
