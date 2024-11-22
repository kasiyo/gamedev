#include "UnitManager.h"

CUnitManager::CUnitManager() {
}

CUnitManager::~CUnitManager() {
}

void CUnitManager::AddUnit(UnitInfo info, int x, int y, int h, int d, int c) {
	Unit unit;
	unit.info = info;
	unit.x = x;
	unit.y = y;
	unit.health = h;
	unit.damage = d;
	unit.cost = c;
	m_vecUnits.push_back(unit);
}

void CUnitManager::RemoveUnit(int x, int y) {
	for (int i = 0; i < m_vecUnits.size(); i++) {
		if (m_vecUnits[i].x == x && m_vecUnits[i].y == y) {
			m_vecUnits.erase(m_vecUnits.begin() + i);
		}
	}
}

void CUnitManager::DrawUnits() {
	for (int i = 0; i < m_vecUnits.size(); i++) {
		//draw unit
	}
}

void CUnitManager::UpdateUnits() {
	for (int i = 0; i < m_vecUnits.size(); i++) {
		//update unit
	}
}

void CUnitManager::MoveUnit(int x, int y, int newX, int newY) {
	for (int i = 0; i < m_vecUnits.size(); i++) {
		if (m_vecUnits[i].x == x && m_vecUnits[i].y == y) {
			m_vecUnits[i].x = newX;
			m_vecUnits[i].y = newY;
		}
	}
}

void CUnitManager::AttackUnit(int x, int y, int targetX, int targetY) {
	for (int i = 0; i < m_vecUnits.size(); i++) {
		if (m_vecUnits[i].x == x && m_vecUnits[i].y == y) {
			//attack unit
		}
	}
}