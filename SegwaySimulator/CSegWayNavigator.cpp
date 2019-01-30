#pragma once

#include <functional>
#include <queue>

#include "CSegWayNavigator.hpp"
#include "SGridMap.hpp"

void CSegWayNavigator::aStarSearch(const unsigned int startIdx, const unsigned int targetIdx, std::unordered_map<unsigned int, unsigned int>& pathMap)
{
	typedef std::pair<float, unsigned int> PRIOENT;
	std::priority_queue<PRIOENT, std::vector<PRIOENT>, std::greater<PRIOENT>> frontier;
	std::unordered_map<unsigned int, int> costs;

	frontier.emplace(std::make_pair(0.f, startIdx));

	pathMap[startIdx] = startIdx;
	costs[startIdx] = 0;

	const auto targetPos = mMap.getPosFromIdx(targetIdx);
	while (!frontier.empty())
	{
		const unsigned int curIdx = frontier.top().second;
		frontier.pop();

		const int newCost = costs[curIdx] + 1;

		if (curIdx == targetIdx)
			break;
		
		for (auto nextPosIdx : mMap.neighborIndices4(curIdx))
		{
			if (mMap.isPassable(nextPosIdx))
			{
				const auto costsOfNeighbor = costs.find(nextPosIdx);
				if ( (costsOfNeighbor==costs.cend()) || (newCost < costsOfNeighbor->second) )
				{
					const auto diff = mMap.getPosFromIdx(nextPosIdx) - targetPos;
					const float h = fabs(diff.x) + fabs(diff.y);

					frontier.emplace(std::make_pair(newCost + h, nextPosIdx));

					costs[nextPosIdx] = newCost;
					pathMap[nextPosIdx] = curIdx;
				}
			}
		}
	}
}

CSegWayNavigator::CSegWayNavigator(unsigned int w, unsigned int h)
	:mMap(w, h)
{}

void CSegWayNavigator::onIdxNotPassable(unsigned int idx)
{
	mMap.mBlocked[idx].mTime = mMap.mClock.getElapsedTime()+sf::seconds(10+(rand()%20));
	mMap.mBlocked[idx].mVisited += 1;

	mMap.mBlockedToDraw.push_back(idx);
}

void CSegWayNavigator::calculatePath(const sf::Vector2f &startPos)
{
	const unsigned int startIdx = mMap.getIdxFromPos(startPos);
	const unsigned int targetIdx = mMap.getIdxFromPos(mTarget);

	if (startIdx != targetIdx)
	{
		std::unordered_map<unsigned int, unsigned int> pathMap;
		aStarSearch(startIdx, targetIdx, pathMap);

		mPath.clear();
		unsigned int curPosIdx = targetIdx;
		while (curPosIdx != startIdx)
		{
			mPath.push_back(mMap.getPosFromIdx(curPosIdx));
			curPosIdx = pathMap[curPosIdx];
		}
	}
}

void CSegWayNavigator::drawTarget(sf::RenderWindow &window) const
{
	sf::RectangleShape target(sf::Vector2f(8, 8));
	target.setOrigin(4, 4);
	target.setPosition(mTarget);
	target.setFillColor(sf::Color::Magenta);

	window.draw(target);
}

void CSegWayNavigator::drawPath(sf::RenderWindow &window) const
{
	sf::RectangleShape pixel(sf::Vector2f(1, 1));
	pixel.setFillColor(sf::Color::Cyan);


	for (auto pathPos : mPath)
	{
		pixel.setPosition(pathPos);
		window.draw(pixel);
	}
}

void CSegWayNavigator::drawMap(sf::RenderWindow &window) const
{
	mMap.draw(window);
}
