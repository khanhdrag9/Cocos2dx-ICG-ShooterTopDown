//
//  Support.cpp
//  Shooter
//
//  Created by Khanh on 1/24/19.
//

#include "Support.h"
#include "Player.h"

namespace support
{
    Vec2 getCoordInTileMap(TMXTiledMap* tiledmap ,const Vec2& realpos)
    {
        Size tileSize = tiledmap->getTileSize();
        Size mapSize = tiledmap->getMapSize();
        
        int x = realpos.x / tileSize.width;
        int y = (mapSize.height * tileSize.height - realpos.y) / tileSize.height;
        
        return Vec2(x, y);
    }

	vector<Vec2> getListVec2(const shared_ptr<Character>& character, const Vec2& position)
	{
		vector<Vec2> result;

		Size size = character->sprite->getContentSize();

		if (character->getTypeplayer() == Character::typeplayer::SQUARE)
		{

		}
		else if (character->getTypeplayer() == Character::typeplayer::CIRCLE)
		{

		}
		else if (character->getTypeplayer() == Character::typeplayer::RECT)
		{

		}		

		return result;
	}
}
