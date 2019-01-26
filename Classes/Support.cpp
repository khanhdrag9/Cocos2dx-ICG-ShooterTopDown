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
        //int y = realpos.y / tileSize.height;
        
        return Vec2(x, y);
    }

	vector<Vec2> getListVec2(const shared_ptr<Character>& character, const Vec2& position)
	{
		vector<Vec2> result;
        Vec2 increpos = position - character->sprite->getPosition();

		auto box = character->sprite->getBoundingBox();

		if (character->getTypeplayer() == Character::typeplayer::SQUARE)
		{
            result = vector<Vec2>
            {
                Vec2(box.getMinX(), box.getMinY()),
                Vec2(box.getMaxX(), box.getMinY()),
                Vec2(box.getMaxX(), box.getMaxY()),
                Vec2(box.getMinX(), box.getMaxY())
            };
		}
		else if (character->getTypeplayer() == Character::typeplayer::CIRCLE)
		{
            
		}
		else if (character->getTypeplayer() == Character::typeplayer::RECT)
		{

		}
        
        for(auto& p : result)
            p += increpos;

		return result;
	}
}
