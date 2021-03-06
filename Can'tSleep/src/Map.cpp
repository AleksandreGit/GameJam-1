#include "./../header/Map.h"

Map::Map() {
	for (int i = 0; i < MAP_SIZE; i++) {
		m_tiles.push_back(GRASS);
		m_elements.push_back(nullptr);
	}
	m_tilesTexture = TextureLoader::GetInstance()->getTexture("Floor");
	generateRandom();

}

void Map::draw(sf::RenderWindow& window, int currentPos) const {
	int bound = BLOC_TO_SHOW / 2;
	int lowerBound = currentPos - bound + 1;
	int upperBound = currentPos + bound;
	float lowerF = lowerBound * TILE_WIDTH;
	float upperF = upperBound * TILE_WIDTH;

	if (lowerBound < 0) {
		lowerBound = 0;
	}
	if (upperBound > MAP_SIZE) {
		upperBound = MAP_SIZE;
	}

	int firstValue = lowerBound;
	for (int i = lowerBound; i < upperBound; i++) {
		if (m_elements[i]) {
			m_elements[i]->draw(window);
		}
	}

	for (int i = lowerBound; i < upperBound; i++) {
		sf::Sprite sprite;
		sprite.setTexture(m_tilesTexture);
		switch (m_tiles[i]) {
		case GRASS:
			sprite.setTextureRect(sf::IntRect(0, 0, TILE_WIDTH, TILE_HEIGHT));
			break;
		case FOREST:
			sprite.setTextureRect(sf::IntRect(TILE_WIDTH, 0, TILE_WIDTH, TILE_HEIGHT));
			break;
		case ROCK:
			sprite.setTextureRect(sf::IntRect(2 * TILE_WIDTH, 0, TILE_WIDTH, TILE_HEIGHT));
			break;
		}
		sprite.setPosition(TILE_WIDTH * firstValue-1, 0);
		firstValue++;
		window.draw(sprite);
	}

	for (DropedItem item : m_dropedItems) {
		if (lowerF < item.getCurrentPosition() < upperF) {
			item.draw(window);
		}
	}
}

std::vector<EnvironmentElement*> Map::getCurrentElements(int currentPos) {
	int bound = BLOC_TO_SHOW / 2;
	int lowerBound = currentPos - bound + 1;
	int upperBound = currentPos + bound;

	if (lowerBound < 0) {
		lowerBound = 0;
	}
	if (upperBound > MAP_SIZE) {
		upperBound = MAP_SIZE;
	}

	int firstValue = lowerBound;
	std::vector<EnvironmentElement*> result;
	for (int i = lowerBound; i < upperBound; i++) {
		if (m_elements[i]) {
			if (m_elements[i]->getHealth() <= 0) {
				std::vector<DropedItem*> items = m_elements[i]->getDropedItems();
				dropItems(items);
				if (!dynamic_cast<Constructible*>(m_elements[i])) {
					m_elements[i]->remove();
				}
				else {
					m_elements[i]->setHealth(100);
				}
				m_elements[i] = nullptr;
			}
			else {
				result.push_back(m_elements[i]);
			}
		}
	}
	return result;
}

void Map::generateRandom() {
	int j = 0;
	float proba = 0.0f;
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_real_distribution<float> distribution(0.0, 1.0);
	std::uniform_real_distribution<float> seed(-10000.0f, 10000.0f);

	// -30
	PerlinNoise perlin(seed(generator), 0.015f, 1.0f, 5);

	for (int i = 0; i < MAP_SIZE; i++) {
		proba = distribution(generator);
		if (perlin.noise1D(i) <= -0.4f) {
			m_tiles[j] = ROCK;
			if (proba <= 0.6f) {
				m_elements[j] = new Mineral(STONE, j);
			}

		}
		else if (perlin.noise1D(i) > -0.4f && perlin.noise1D(i) <= 0.2f) {
			m_tiles[j] = FOREST;
			if (proba <= 0.7f) {
				m_elements[j] = new Tree(OAK, j);
			}

		}
		else {
			m_tiles[j] = GRASS;
			if (proba <= 0.10f) {
				m_elements[j] = new Mineral(STONE, j);
			}
			else if (proba > 0.40f && proba <= 0.50f) {
				m_elements[j] = new Tree(OAK, j);
			}
		}
		j++;
	}
	int pos = MAP_SIZE / 2;
	m_elements[pos] = new Trap(pos);
}

void Map::dropItems(std::vector<DropedItem*> items) {
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_real_distribution<float> distribution(items[0]->getCurrentPosition() - 0.5f * TILE_WIDTH, items[0]->getCurrentPosition() + 0.5f * TILE_WIDTH);
	std::uniform_real_distribution<float> seed(-10000.0f, 10000.0f);
	float itemPos;
	for (DropedItem* item : items) {
		itemPos = distribution(generator);
		item->setCurrentPosition(itemPos);
		m_dropedItems.push_back(*item);
	}

}

void Map::dropItem(Item* item, int position) {
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_real_distribution<float> distribution((position - 0.5f) * TILE_WIDTH, (position + 0.5f) * TILE_WIDTH);
	std::uniform_real_distribution<float> seed(-10000.0f, 10000.0f);

	float itemPos = distribution(generator);
	m_dropedItems.push_back(DropedItem(item, itemPos));
}

bool Map::putDownConstructible(Constructible* constructible, int position) {
	if (!m_elements[position]) {
		m_elements[position] = constructible;
		return true;
	}
	return false;
}

void Map::checkInteraction(Player& player) {
	int i = 0;
	for (DropedItem item : m_dropedItems) {
		if (player.canPickUpItem(&item)) {
			std::cout << "ON PICKUP LA!" << std::endl;
			player.m_inventory.addItem(item.getItem());
			m_dropedItems.erase(m_dropedItems.begin() + i);
			break;
		}
		i++;
	}
}