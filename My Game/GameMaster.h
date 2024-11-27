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
	void draw() {
		m_pRenderer->Draw(&desc); // Draw the unit.
	};

	void move() {
		// Move the unit.
		const float delta = m_pTimer->GetFrameTime();
	};
	bool IsFacingPlayer;
};

