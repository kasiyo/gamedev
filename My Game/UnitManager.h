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
	void move() {
		// Move the unit.
		x += 1;
		const float delta = m_pTimer->GetFrameTime();
	};
	//float moveDuration = 300000.0f;//24.609375f * 2500.0f;
	float moveDuration = 24609375.0f;//5000000.0f;
	LTimer timerFunction;
	//static uint64_t moveDurationInTicks = timerFunction.SecondsToTicks((double)moveDuration);
	void moveTo(const Vector2 endPos, float deltaTime) {
		// Move the unit to the tile.
		//desc.m_vPos = endPos;
		//m_pTimer->SetFrameTime(moveDuration);
		float endTime = moveDuration + deltaTime;
		float currTime = deltaTime; //
		static double moveDurationInTicks;
		//m_pTimer->SetFixedTimeStep(true);
		//m_pTimer->SetFrameTime(0.25);
		float movesToGo = 0.0f;
		float moveTicker = 0.0f;
		//float progress = 1.0f - pow(0.00000099, deltaTime);

		float progress = 1.0f - pow(0.00000001, deltaTime);
		//float progress = currTime / moveDuration;
		printf("deltaTime: %f\n", deltaTime);

		printf("currTime: %f\n", currTime);
		printf("endTime: %f\n", endTime);
		//printf("progress: %f\n", progress);
		Vector2 currPos(desc.m_vPos.x + progress, desc.m_vPos.y + progress);
		while (movesToGo < endTime) {

			//float progress = moveTicker / (moveDuration);
			//float progress = (currTime - deltaTime) / (endTime - deltaTime);

			if (progress > 1.0f) {
				progress = 1.0f;
			}
			if (currPos.x > endPos.x) {
				currPos.x = endPos.x;
			}
			desc.m_vPos.x = Math::lerp(desc.m_vPos.x, endPos.x, progress);
			desc.m_vPos.y = Math::lerp(desc.m_vPos.y, endPos.y + 20.0f, progress);
			movesToGo += progress * 500000.0f;
			currTime += (progress);
			currPos.x += progress * 2;
			currPos.y += progress;

			moveTicker += progress;
		}
		//desc.m_vPos.x = Math::lerp(desc.m_vPos.x, endPos.x, endTime);
		//desc.m_vPos.y = Math::lerp(desc.m_vPos.y, endPos.y + 20.0f, endTime);


		//desc.m_vPos.x = Math::lerp(desc.m_vPos.x, endPos.x, deltaTime * 0.25f);
		//printf("deltaTime * 2.5f: %f\n", deltaTime * 2.5f);
		//desc.m_vPos.y = Math::lerp(desc.m_vPos.y, endPos.y + 20.0f, deltaTime * 0.25f);
		//desc.m_vPos.y += 20.0f;	// hardcode y-offset.
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
	//Unit* gameMaster = nullptr;
	//Unit* playerUnit = nullptr;

	void AddUnit(struct Tile*);
	void EditUnit(Unit u);
	void RemoveUnit(int x, int y);
	std::vector<Unit*> GetUnits();
	bool GetUnit(int x, int y, Unit** refval);

	void Draw();
	void UpdateUnits();
	void MoveUnit(Vector2 moveDirection, float deltaTime);
	void AttackUnit(int x, int y, int targetX, int targetY);
};