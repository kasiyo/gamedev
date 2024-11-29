#pragma once
#include "Common.h"
#include "BaseObject.h"
#include "Renderer.h"
#include "GameDefines.h"
#include "TileManager.h"

class GameMaster : public CCommon, public LBaseObject {
public:
	GameMaster(const Vector2& p);
	GMInfo info;
	int x;
	int y;
	LSpriteDesc2D desc;

	void draw();

	void move();
	bool IsFacingPlayer;

	void GetSpawnPoint();
};

