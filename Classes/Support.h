#pragma once
#include "Headers.h"

class Character;

namespace support
{
    Vec2 getCoordInTileMap(TMXTiledMap* tiledmap, const Vec2& realpos);
	vector<Vec2> getListVec2(const shared_ptr<Character>& character);
}
