#pragma once

#include <SFML\Graphics.hpp>
#include <unordered_set>
#include <vector>

struct SGridMap
{
	unsigned int mWidth, mHeight;

	std::unordered_set<unsigned int> mBlocked;
	std::vector<unsigned int> mBlockedToDraw;

	SGridMap(unsigned int width, unsigned int height)
		: mWidth(width), mHeight(height)
	{}

	inline unsigned int getIdxFromPos(const sf::Vector2f &pos) const
	{
		const auto row = static_cast<unsigned int>(pos.y);
		const auto col = static_cast<unsigned int>(pos.x);
		return (row*mWidth) + col;
	}

	inline sf::Vector2f getPosFromIdx(unsigned int idx) const
	{
		const auto row = static_cast<float>(idx) / mWidth;
		const auto col = static_cast<float>(idx % mWidth);
		return sf::Vector2f(col, row);
	}

	inline bool isPassable(unsigned int idx) const
	{
		return (mBlocked.count(idx) == 0);
	}

	std::vector<unsigned int> neighborIndices(unsigned int idx) const
	{
		std::vector<unsigned int> results;
		const auto pos = getPosFromIdx(idx);
		const auto x = pos.x;
		const auto y = pos.y;

		if (x == 0)
		{
			results.push_back(idx + 1);
			if (y == 0)
			{
				idx += mWidth;
				results.push_back(idx);
				results.push_back(idx + 1);
			}
			else if (y == (mHeight - 1))
			{
				idx -= mWidth;
				results.push_back(idx);
				results.push_back(idx + 1);
			}
			else
			{
				results.push_back(idx + mWidth);
				results.push_back(idx + mWidth + 1);
				idx -= mWidth;
				results.push_back(idx);
				results.push_back(idx + 1);
			}
			return results;
		}
		else if (x == (mWidth - 1))
		{
			results.push_back(idx - 1);
			if (y == 0)
			{
				idx += mWidth;
				results.push_back(idx - 1);
				results.push_back(idx);
			}
			else if (y == (mHeight - 1))
			{
				idx -= mWidth;
				results.push_back(idx - 1);
				results.push_back(idx);
			}
			else
			{
				results.push_back(idx + mWidth);
				results.push_back(idx + mWidth - 1);
				idx -= mWidth;
				results.push_back(idx);
				results.push_back(idx - 1);
			}
			return results;
		}
		else
		{
			results.push_back(idx - 1);
			results.push_back(idx + 1);

			if (y == 0)
			{
				idx += mWidth;
				results.push_back(idx - 1);
				results.push_back(idx);
				results.push_back(idx + 1);
			}
			else if (y == (mHeight - 1))
			{
				idx -= mWidth;
				results.push_back(idx - 1);
				results.push_back(idx);
				results.push_back(idx + 1);
			}
			else
			{
				results.push_back(idx + mWidth - 1);
				results.push_back(idx + mWidth);
				results.push_back(idx + mWidth + 1);
				idx -= mWidth;
				results.push_back(idx - 1);
				results.push_back(idx);
				results.push_back(idx + 1);
			}
			return results;
		}
	}

	void draw(sf::RenderWindow &window) const
	{
		sf::RectangleShape pixel(sf::Vector2f(1, 1));
		pixel.setFillColor(sf::Color::Black);
		for (auto blockedIdx : mBlockedToDraw)
		{
			const auto blockedPos = getPosFromIdx(blockedIdx);
			pixel.setPosition(blockedPos);
			window.draw(pixel);
		}
	}
};