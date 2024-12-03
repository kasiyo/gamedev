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

	//float moveDuration = 300000.0f;//24.609375f * 2500.0f;
	//float moveDuration = 24609375.0f;//5000000.0f;
	float moveDuration = 10.75f;

	Vector2 currentVelocity = Vector2(-20000.f, -20000.f);
	float smoothTime = 0.1f;
	float maxSpeed = 0.000001f;
	float unitSpeed = 1.0f;
	Vector2 currentAcceleration;

	void move() {
		// Move the unit.

		const float delta = m_pTimer->GetFrameTime();
		Vector2 currPos = Vector2(x, y);
		Vector2 moveDirection = Vector2(0.0f, 0.0f);
		Vector2 upVector = Vector2(0.0f, -1.0f);
		Vector2 downVector = Vector2(0.0f, 1.0f);
		Vector2 leftVector = Vector2(-1.0f, 0.0f);
		Vector2 rightVector = Vector2(1.0f, 0.0f);
		if (m_pKeyboard->Down(VK_LEFT)) {
			moveDirection += leftVector;
		}
		if (m_pKeyboard->Down(VK_RIGHT)) {
			moveDirection += rightVector;
		}
		if (m_pKeyboard->Down(VK_UP)) {
			moveDirection += upVector;
		}
		if (m_pKeyboard->Down(VK_DOWN)) {
			moveDirection += downVector;
		}
		Vector2 offset = -(moveDirection - currPos);

		Tile* destTile = nullptr;

		while (offset.x != 0.0f || offset.y != 0.0f) {
			//moveTo(Vector2(x + offset.x, y + offset.y), delta);
			float amount = delta * unitSpeed;
			float deltaOffset = sqrt(offset.x * offset.x + offset.y * offset.y);
			float total = (std::min)(amount, deltaOffset);
			//offset.x = offset.x - (total * offset.x / deltaOffset);
			//offset.y = offset.y - (total * offset.y / deltaOffset);
			--offset.x;
			--offset.y;
			desc.m_vPos.x += offset.x;
			desc.m_vPos.y += offset.y;
		}


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

		float endTime = moveDuration + deltaTime;
		float currTime = deltaTime; //

		float movesToGo = 0.0f;
		float amount = deltaTime * unitSpeed;


		//Vector2 offset(desc.m_vPos.x - endPos.x, desc.m_vPos.y - endPos.y);
		printf("currPos (x, y) : %f, %f\n", desc.m_vPos.x, desc.m_vPos.y);
		Vector2 offset = -(endPos - desc.m_vPos);
		printf("offset (x, y) : %f, %f\n", offset.x, offset.y);
		float progress = 1.0f - (float)pow(deltaTime, 0.1);
		//float progress = currTime / moveDuration;
		printf("deltaTime: %f\n", deltaTime);

		printf("currTime: %f\n", currTime);
		printf("endTime: %f\n", endTime);

		float velocityX = Math::SmoothDamp(currentVelocity.x, endPos.x / 2, &currentAcceleration.x, smoothTime, maxSpeed, progress);
		float velocityY = Math::SmoothDamp(currentVelocity.x, endPos.y / 2, &currentAcceleration.y, smoothTime, maxSpeed, progress);
		currentVelocity = Vector2(velocityX, velocityY);

		//desc.m_vPos += currentVelocity/* * deltaTime*/;

		printf("m_pTimer->m_nDelta: %f\n", m_pTimer->GetFrameTime());
		int framesUntilCompletion = 0;

		/*while (framesUntilCompletion < 2) {
			++framesUntilCompletion;
			desc.m_vPos.x = Math::lerp(desc.m_vPos.x, endPos.x, 1.0f - (float)framesUntilCompletion / 60.0f);
			desc.m_vPos.y = Math::lerp(desc.m_vPos.y, endPos.y + 20.f, 1.0f - (float)framesUntilCompletion / 60.0f);
			offset.x += progress;
			offset.y += progress;
			progress += deltaTime;
			++framesUntilCompletion;
		}*/

		desc.m_vPos.x = Math::lerp(desc.m_vPos.x, endPos.x, 1.0f - deltaTime);
		desc.m_vPos.y = Math::lerp(desc.m_vPos.y, endPos.y + 20.0f, 1.0f - deltaTime);
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