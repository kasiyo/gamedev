#pragma once
#include "Object.h"
#include "GameDefines.h"
#include "Renderer.h"
#include "Common.h"
#include "SpriteRenderer.h"
#include "ComponentIncludes.h"
#include "TileManager.h"
#include "BaseObject.h"
#include "Math.h"

class Unit : public CCommon, public LBaseObject {
	friend class CUnitManager;
private:
public:
	Unit(eSprite t, const Vector2& p) : LBaseObject(t, p) {};
	UnitInfo info;
	int health;
	int damage;
	int cost;
	bool is_stationary = true;
	Tile* tile = nullptr;

	int x;
	int y;
	LSpriteDesc2D desc;
	Unit& operator=(const Unit& rhs) {
		if (this != &rhs) {
			desc = rhs.desc;
		}
		return *this;
	}
	void draw() {
		m_pRenderer->Draw(&desc); // Draw the unit.
	};

	LerpInfo lerpInfo{
		Vector2(0.0f, 0.0f),		// starting position
		Vector2(0.0f, 0.0f),		// target position
		0.0f,		// current duration in ms
		(0.25f)	// max duration in ms
	};

	void move() {// Move the unit.
		const float delta = m_pTimer->GetFrameTime();

		int dest_x = this->x;	// start at current tile position
		int dest_y = this->y;	// start at current tile position

		if (m_pKeyboard->Down(VK_LEFT)) {
			dest_x -= 1;
		}
		if (m_pKeyboard->Down(VK_RIGHT)) {
			dest_x += 1;
		}
		if (m_pKeyboard->Down(VK_UP)) {
			dest_y += 1;
		}
		if (m_pKeyboard->Down(VK_DOWN)) {
			dest_y -= 1;
		}

		float percentComplete = lerpInfo.currDuration / lerpInfo.maxDuration;
	};

	/// --- Move the unit to the tile. --- ///
	void moveTo(const Vector2 endPos, const WPARAM k) {
		printf("running moveTo function\n");

		float percentComplete = (std::min)(lerpInfo.currDuration / lerpInfo.maxDuration, 1.0f);
		float progress = 1.0f - (float)pow(percentComplete, m_pTimer->GetFrameTime());

		lerpInfo.target = endPos;

		printf("percentComplete: %f %\n", percentComplete);
		printf("lerpInfo.target: %f %f\n", lerpInfo.target.x, lerpInfo.target.y);
		switch (k) {
			case VK_LEFT: {
				desc.m_nCurrentFrame = 3;
				break;
			}
			case VK_RIGHT: {
				desc.m_nCurrentFrame = 1;
				break;
			}
			case VK_UP: {
				desc.m_nCurrentFrame = 2;
				break;
			}
			case VK_DOWN: {
				desc.m_nCurrentFrame = 0;
				break;
			}
		}
		//desc.m_vPos.x = Math::lerp(desc.m_vPos.x, endPos.x, progress);
		//desc.m_vPos.y = Math::lerp(desc.m_vPos.y, endPos.y + 40.0f, progress);

		//desc.m_vPos = Math::lerp(desc.m_vPos, endPos, progress);
		//desc.m_vPos.y += 35.0f;	// "lift up" unit
	};


	void update() {
		printf("running update function\n");
		if (!is_stationary) {
			lerpInfo.currDuration += m_pTimer->GetFrameTime();
			float percentComplete = (std::min)(lerpInfo.currDuration / lerpInfo.maxDuration, 1.0f);

			printf("lerpInfo.source: %f %f\n", lerpInfo.source.x, lerpInfo.source.y);
			printf("lerpInfo.target: %f %f\n", lerpInfo.target.x, lerpInfo.target.y);
			printf("lerpInfo.currDuration: %f ", lerpInfo.currDuration);

			if (lerpInfo.currDuration >= lerpInfo.maxDuration) {

				printf("is >= lerpInfo.maxDuration: %f\n", lerpInfo.maxDuration);
				printf("**current position (x, y): (%f, %f)**\n", desc.m_vPos.x, desc.m_vPos.y);
				lerpInfo.currDuration = 0.0f;
				desc.m_vPos = lerpInfo.target;
				printf("**updated to new position (x, y): (%f, %f)**\n", desc.m_vPos.x, desc.m_vPos.y);
				desc.m_vPos.y += 20.0f;	// for offset
				is_stationary = true;
				return;
			}
			else {
				printf("is < lerpInfo.maxDuration: %f - ", lerpInfo.maxDuration);
				printf("current position (x, y): (%f, %f)\n", desc.m_vPos.x, desc.m_vPos.y);

				lerpInfo.source = desc.m_vPos;


				printf("%f %% complete\n", percentComplete);
				float progress = 1.0f - (float)pow(percentComplete, m_pTimer->GetFrameTime());
				//desc.m_vPos = Math::lerp(desc.m_vPos, lerpInfo.target, percentComplete);

				//desc.m_vPos.x = Math::lerp(desc.m_vPos.x, lerpInfo.target.x, progress);
				//desc.m_vPos.y = Math::lerp(desc.m_vPos.y, lerpInfo.target.y + 40.0f, progress);

				desc.m_vPos.x = Math::lerp(desc.m_vPos.x, lerpInfo.target.x, percentComplete);
				desc.m_vPos.y = Math::lerp(desc.m_vPos.y, lerpInfo.target.y + 35.0f, percentComplete);


				printf("updated to new position (x, y): (%f, %f)\n", desc.m_vPos.x, desc.m_vPos.y);
				//lerpInfo.currDuration += m_pTimer->GetFrameTime();
			}	// else < lerpInfo.maxDuration
		}	// if !is_stationary
		else { return; }
	};
};

class CUnitManager : public CCommon, public LBaseObjectManager<Unit> {
private:
	size_t m_nWidth = 0;

	std::vector<Unit> enemyUnits;
	int numPlayerUnits = 0;

	float m_fUnitSize = 0.0f;
public:
	CUnitManager(size_t);
	~CUnitManager();

	std::vector<Unit*> m_vecUnits;

	void AddUnit(struct Tile*);
	void EditUnit(Unit u);
	void RemoveUnit(int x, int y);
	std::vector<Unit*> GetUnits();
	bool GetUnit(int x, int y, Unit** refval);

	void Draw();
	void UpdateUnits();
	void MoveUnit(Tile* destTile);
	void AttackUnit(int x, int y, int targetX, int targetY);
};