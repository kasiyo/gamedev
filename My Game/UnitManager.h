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

	//float moveDuration = 300000.0f;//24.609375f * 2500.0f;
	//float moveDuration = 24609375.0f;//5000000.0f;
	float moveDuration = 10.75f;

	Vector2 currentVelocity = Vector2(-20000.f, -20000.f);
	float smoothTime = 0.1f;
	float maxSpeed = 0.000001f;
	float unitSpeed = 1.0f;
	Vector2 currentAcceleration;

	LerpInfo lerpInfo{
		Vector2(0.0f, 0.0f),		// starting position
		Vector2(0.0f, 0.0f),		// target position
		0.0f,		// current duration in ms
		(200.0f)	// max duration in ms
	};


	void move() {
		// Move the unit.

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


		Tile* destTile = nullptr;

		float currTime = m_pTimer->GetFrameTime();

		float percentComplete = lerpInfo.currDuration / lerpInfo.maxDuration;
		//float currProgress = Math::lerp(lerpInfo.source, lerpInfo.target, percentComplete);

	};

	void MoveToTile(int x, int y) {
		// Move the unit to the tile.
		float startTime = m_pTimer->GetFrameTime();
		float endTime = startTime + 2.0f;
		float smoothTime = 1.0f;
		float progress = (startTime - m_pTimer->GetFrameTime()) / (endTime - startTime);
		float maxSpeed = 50.0f;
		float velocityX = Math::SmoothDamp(currentVelocity.x, x, &currentAcceleration.x, smoothTime, maxSpeed, m_pTimer->GetFrameTime());
		float velocityY = Math::SmoothDamp(currentVelocity.y, y, &currentAcceleration.y, smoothTime, maxSpeed, m_pTimer->GetFrameTime());
		currentVelocity = Vector2(velocityX, velocityY);


		desc.m_vPos += currentVelocity * m_pTimer->GetFrameTime();	// moves unit by velocity
	};

	void moveTo(const Vector2 endPos, float deltaTime) {
		// Move the unit to the tile.
		lerpInfo.source = desc.m_vPos;
		lerpInfo.target = endPos;



		playerUnit->lerpInfo.currDuration += WALK_DURATION;

		float progress = 1.0f - (float)pow(deltaTime, 0.1);
		//float progress = currTime / moveDuration;
		printf("deltaTime: %f\n", deltaTime);

		float velocityX = Math::SmoothDamp(currentVelocity.x, endPos.x / 2, &currentAcceleration.x, smoothTime, maxSpeed, progress);
		float velocityY = Math::SmoothDamp(currentVelocity.x, endPos.y / 2, &currentAcceleration.y, smoothTime, maxSpeed, progress);
		currentVelocity = Vector2(velocityX, velocityY);

		//desc.m_vPos += currentVelocity/* * deltaTime*/;
		float percentComplete = playerUnit->lerpInfo.currDuration / playerUnit->lerpInfo.maxDuration;

		printf("playerUnit->lerpInfo.currDuration: %f\n", playerUnit->lerpInfo.currDuration);
		printf("playerUnit->lerpInfo.maxDuration: %f\n", playerUnit->lerpInfo.maxDuration);

		printf("percentComplete: %f\n", percentComplete);


		//desc.m_vPos.x = Math::lerp(desc.m_vPos.x, endPos.x, 1.0f - deltaTime);
		//desc.m_vPos.y = Math::lerp(desc.m_vPos.y, endPos.y + 20.0f, 1.0f - deltaTime);


		desc.m_vPos.x = Math::lerp(desc.m_vPos.x, endPos.x, percentComplete);
		desc.m_vPos.y = Math::lerp(desc.m_vPos.y, endPos.y, percentComplete);

		playerUnit->lerpInfo.currDuration += deltaTime;
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