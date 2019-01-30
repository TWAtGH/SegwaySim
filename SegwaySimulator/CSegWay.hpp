#pragma once

#include <unordered_set>

#include <SFML\Graphics.hpp>

#include "CSegWayNavigator.hpp"

class CSegWay
{
private:
	sf::CircleShape mSegWayShape;
	unsigned int mPathIndex;

public:
	bool mRemovePassedPath;


	CSegWayNavigator mNavi;
	std::unordered_set<unsigned int> mBlocked;

public:
	CSegWay();

	void setTarget(const sf::Vector2f &newTarget);

	void update(float deltaTime);
	void draw(sf::RenderWindow &window);

};