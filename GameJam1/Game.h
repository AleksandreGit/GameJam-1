#pragma once
#include <SFML/Graphics.hpp>
#include "AnimableBuilder.h"
#include "Player.h"
#include "Map.h"

class Game
{
public:
	Game();

	void draw();
	void handleEvents(float deltaTime);
	void handleCamera();
	void update();

	const static float WORLD_UNIT;
	const static unsigned int W_WIDTH = 1920;
	const static unsigned int W_HEIGHT = 1080;

private:
	sf::RenderWindow m_window;
	sf::View m_playerView;
	sf::Clock m_clock;
	float m_currentZoom;
	Player m_player;
	Map m_map;

	const float ratio = AnimableBuilder::DEFAULT_WIDTH / AnimableBuilder::DEFAULT_HEIGHT;
	const unsigned int S_SIZE = 200;
	const unsigned int SCALE_FACTOR = 2;
};
