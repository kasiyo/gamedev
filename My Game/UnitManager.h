#pragma once
#include "Object.h"
#include "GameDefines.h"

struct Unit {
	UnitInfo info;
	int x;
	int y;
	int health;
	int damage;
	int cost;
};

class CUnitManager {
private:
	std::vector<Unit> m_vecUnits;
public:
	CUnitManager();
	~CUnitManager();

	void AddUnit(UnitInfo info, int x, int y, int h, int d, int c);
	void RemoveUnit(int x, int y);
	void DrawUnits();
	void UpdateUnits();
	void MoveUnit(int x, int y, int newX, int newY);
	void AttackUnit(int x, int y, int targetX, int targetY);
};