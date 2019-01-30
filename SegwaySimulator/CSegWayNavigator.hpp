#pragma once

#include <unordered_map>
#include <vector>

#include <SFML\Graphics.hpp>

#include "SGridMap.hpp"

class CSegWayNavigator
{
private:

	void aStarSearch(const unsigned int startIdx, const unsigned int targetIdx, std::unordered_map<unsigned int, unsigned int>& pathMap);

public:
	SGridMap mMap;
	sf::Vector2f mTarget;
	std::vector<sf::Vector2f> mPath;

	CSegWayNavigator(unsigned int w, unsigned int h);

	void onIdxNotPassable(unsigned int idx);

	void calculatePath(const sf::Vector2f &startPos);

	void drawTarget(sf::RenderWindow &window) const;
	void drawPath(sf::RenderWindow &window) const;
	void drawMap(sf::RenderWindow &window) const;
};