#include "CSegWay.hpp"


CSegWay::CSegWay()
	:mNavi(1024,768), 
	mPathIndex(0),
	mRemovePassedPath(true)
{
	mSegWayShape.setOrigin(8, 8);
	mSegWayShape.setFillColor(sf::Color::Red);
	mSegWayShape.setPosition(250, 500);
	mSegWayShape.setRadius(8.f);
	setTarget(sf::Vector2f(0,0));
}

void CSegWay::setTarget(const sf::Vector2f &newTarget)
{
	mNavi.mTarget = newTarget;
	mNavi.calculatePath(mSegWayShape.getPosition());
	mPathIndex = 0;
	if(!mNavi.mPath.empty())
		mPathIndex = static_cast<unsigned int>(mNavi.mPath.size() - 1);
}

void CSegWay::update(float deltaTime)
{
	while (mPathIndex>0)
	{
		mPathIndex--;

		auto pos = mNavi.mPath[mPathIndex];

		const auto idx = mNavi.mMap.getIdxFromPos(pos);
		if ((mBlocked.count(idx) == 0) || mNavi.mMap.isExpired(idx))
		{
			//no obstacle so move to next pixel
			mSegWayShape.setPosition(pos);

			if (mRemovePassedPath)
			{
				while((mNavi.mPath.size()-1)>mPathIndex)
					mNavi.mPath.pop_back();
			}

			break;
		}
		else if(mNavi.mMap.isExpired(idx))
		{
			//obstacle is expired so remove it and move

		}

		mNavi.onIdxNotPassable(idx);
		mNavi.calculatePath(mSegWayShape.getPosition());
		mPathIndex = static_cast<unsigned int>(mNavi.mPath.size() - 1);
	}
}

void CSegWay::draw(sf::RenderWindow &window)
{
	mNavi.drawPath(window);

	mNavi.drawTarget(window);

	window.draw(mSegWayShape);
}
