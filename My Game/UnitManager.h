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
#include "Unit.h"

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