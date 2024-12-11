#pragma once
#include "BaseObject.h"
#include "GameDefines.h"
#include "Renderer.h"
#include "Common.h"
#include "Math.h"
#include "TileManager.h"

class Unit : public CCommon, public LBaseObject {
	friend class CUnitManager;
private:
public:
	Unit(eSprite t, const Vector2 &p) : LBaseObject(t, p) {};
	UnitInfo info;
	int cost;
	bool is_stationary = true;
	Tile *tile = nullptr;

	int x;
	int y;

	int z;
	int draw_index = -1;
	LSpriteDesc2D desc;
	Unit &operator=(const Unit &rhs) {
		if (this != &rhs) {
			desc = rhs.desc;
		}
		return *this;
	}

	void draw();	// Draw the unit.

	LerpInfo lerpInfo {
		Vector2(0.0f, 0.0f),		// starting position
		Vector2(0.0f, 0.0f),		// target position
		0.0f,		// current duration in ms
		(0.25f)	// max duration in ms
	};

	void move();


	void moveTo(Tile *t, const WPARAM k);


	/*void update() {
		printf("running update function\n");
		if (!is_stationary) {
			lerpInfo.currDuration += m_pTimer->GetFrameTime();
			float percentComplete = (std::min)(lerpInfo.currDuration / lerpInfo.maxDuration, 1.0f);

			if (lerpInfo.currDuration >= lerpInfo.maxDuration) {
				lerpInfo.currDuration = 0.0f;
				desc.m_vPos = lerpInfo.target;
				desc.m_vPos.y += 20.0f;	// for offset
				is_stationary = true;
				return;
			}
			else {
				desc.m_vPos.x = Math::lerp(desc.m_vPos.x, lerpInfo.target.x, percentComplete);
				desc.m_vPos.y = Math::lerp(desc.m_vPos.y, lerpInfo.target.y + 35.0f, percentComplete);
			}	// else < lerpInfo.maxDuration
		}	// if !is_stationary
		else { return; }
	};*/
};
