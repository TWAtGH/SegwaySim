#pragma once

#include <SFML\Graphics.hpp>
#include <unordered_set>
#include <vector>

struct SGridMap
{
	sf::Clock mClock;
	unsigned int mWidth, mHeight;
	struct SBlock
	{
		SBlock()
			:mVisited(0)
		{};

		sf::Time mTime;
		unsigned int mVisited;
	};
	std::unordered_map<unsigned int, SBlock> mBlocked;
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
		auto res = mBlocked.find(idx);
		if (res != mBlocked.cend())
			return (res->second.mTime <= mClock.getElapsedTime());

		return true;
	}

	inline bool isExpired(unsigned int idx) const
	{
		auto res = mBlocked.find(idx);
		if (res != mBlocked.cend())
			return (res->second.mTime <= mClock.getElapsedTime());

		return false;
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


	std::vector<unsigned int> neighborIndices4(unsigned int idx) const
	{
		std::vector<unsigned int> results;
		const auto pos = getPosFromIdx(idx);
		const auto x = pos.x;
		const auto y = pos.y;

		if (x == 0)
		{
			results.push_back(idx + 1);
			if (y > 0)
				results.push_back(idx - mWidth);
			if (y < (mHeight - 1))
				results.push_back(idx + mWidth);
				
			return results;
		}
		else if (x == (mWidth - 1))
		{
			results.push_back(idx - 1);
			if (y > 0)
				results.push_back(idx - mWidth);
			if (y < (mHeight - 1))
				results.push_back(idx + mWidth);

			return results;
		}
		else
		{
			results.push_back(idx - 1);
			results.push_back(idx + 1);

			if (y > 0)
				results.push_back(idx - mWidth);
			if (y < (mHeight - 1))
				results.push_back(idx + mWidth);

			return results;
		}
	}

	void draw(sf::RenderWindow &window) const
	{
		sf::RectangleShape pixel(sf::Vector2f(1, 1));
		pixel.setFillColor(sf::Color::Black);
		for (auto blockedIdx : mBlockedToDraw)
		{
			if (!isExpired(blockedIdx))
			{
				const auto blockedPos = getPosFromIdx(blockedIdx);
				pixel.setPosition(blockedPos);
				window.draw(pixel);
			}
		}
	}
};