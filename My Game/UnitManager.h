#pragma once
#include "Object.h"
#include "GameDefines.h"
#include "Renderer.h"
#include "Common.h"
#include "SpriteRenderer.h"
#include "ComponentIncludes.h"
#include "TileManager.h"
#include "BaseObject.h"

class Unit  : public CCommon, public LBaseObject {
	friend class CUnitManager;
	private:
		//Tile* tile = nullptr;
	public:
		Unit(eSprite t, const Vector2& p) : LBaseObject(t, p) {};
		UnitInfo info;
		Vector2 position;
		int health;
		int damage;
		int cost;

		void draw() {
			m_pRenderer->Draw(this); // Draw the unit.
		};

		LSpriteDesc2D desc;
		Unit& operator=(const Unit& rhs) {
			if (this != &rhs) {
				desc = rhs.desc;
			}
			return *this;
		}
};

class CUnitManager : public CCommon, public LBaseObjectManager<Unit> {
	private:
		size_t m_nWidth = 0;
		
		Unit** playerUnits = nullptr;
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
		void MoveUnit(int x, int y, int newX, int newY);
		void AttackUnit(int x, int y, int targetX, int targetY);
	};