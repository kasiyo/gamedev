#pragma once
#include "Common.h"
#include "BaseObject.h"
#include "Renderer.h"
#include "GameDefines.h"
#include "TileManager.h"
#include "ComponentIncludes.h"

class GameMaster : public CCommon, public LBaseObject {
public:
	GameMaster(const Vector2 &p);
	GMInfo info;
	int x;
	int y;
	int z;
	int draw_index = -1;
	LSpriteDesc2D desc;

	void draw();
	void move();
	bool IsFacingPlayer;

	void GetSpawnPoint();
	void SwitchPhases(int nextPhaseNum);
	LComponent *timer;
	bool FriendlyMode;	// start off as true
	void SetFriendlyMode(bool PlayerGodModeEnabled);
};

