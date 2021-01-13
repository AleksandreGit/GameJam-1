#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "AnimablePlayer.h"
#include "PlayerIdle.h"
#include "PlayerWalk.h"
#include <vector>
#include <iostream>

using namespace std;


class Player : public Entity
{
public:
	Player();

	virtual void move(float deltaTime);
	virtual void draw(sf::RenderWindow& window) const;
	virtual void attack(Entity& target) const;
	virtual void defend(Entity& attacker); 
	virtual void setState(State state);
	virtual void setDirection(Direction dir);

private:
	AnimablePlayer* m_currentAnimation;
};

