#include "UnitManager.h"

CUnitManager::CUnitManager(size_t n) :
	m_fUnitSize((float)n) {

	printf("CUnitManager::CUnitManager(%1.f)\n", m_fUnitSize);
}

CUnitManager::~CUnitManager() {
	for (size_t i = 0; i < m_vecUnits.size(); i++) {
		delete m_vecUnits[i];
	}
}

void CUnitManager::AddUnit(struct Tile *t) {
	Unit *unit = nullptr;

	unit = new Unit(eSprite::Unit, t->pos);
	unit->desc.m_fXScale = 1.0f;
	unit->desc.m_fYScale = 1.0f;
	unit->desc.m_nSpriteIndex = UINT(eSprite::Unit);
	unit->desc.m_nCurrentFrame = 2;
	unit->desc.m_vPos = t->pos;

	unit->desc.m_vPos.y += t->y_offset;	// hardcode y-offset.
	//printf("unit->desc.m_nSpriteIndex: %d\n", unit->desc.m_nSpriteIndex);
	//printf("unit->desc.m_nCurrentFrame: %d\n", unit->desc.m_nCurrentFrame);
	//printf("tile->pos: %f, %f\n", t->pos.x, t->pos.y);
	//printf("unit->desc.m_vPos: %f, %f\n", unit->desc.m_vPos.x, unit->desc.m_vPos.y);
	unit->x = t->x;
	unit->y = t->y;
	unit->m_pTimer->GetFrameTime();
	unit->tile = t;
	unit->draw_index = t->draw_index + 1;
	m_vecUnits.push_back(unit);

	playerUnit = unit;
}

void CUnitManager::EditUnit(Unit u) {
	for (int i = 0; i < m_vecUnits.size(); i++) {
		//if (m_vecUnits[i].x == u.x && m_vecUnits[i].y == u.y) {
		//	m_vecUnits[i] = u;
		//}
	}
}

void CUnitManager::RemoveUnit(int x, int y) {
	for (int i = 0; i < m_vecUnits.size(); i++) {
		//if (m_vecUnits[i].x == x && m_vecUnits[i].y == y) {
		//	m_vecUnits.erase(m_vecUnits.begin() + i);
		//}
	}
}

std::vector<Unit *> CUnitManager::GetUnits() {
	return m_vecUnits;
}

bool CUnitManager::GetUnit(int x, int y, Unit **refval) {
	if (x < 0 || x >= m_pTileManager->GetWidth() || y < 0 || y >= m_pTileManager->GetHeight()) {
		return false;
	}

	*refval = &m_vecUnits[y][x];

	for (int i = 0; i < m_vecUnits.size(); i++) {
		//if (m_vecUnits[i].x == x && m_vecUnits[i].y == y) {
		//	*refval = &m_vecUnits[i];
		//	return true;
		//}
	}
	return false;
}

void CUnitManager::Draw() {
	/// --- TODO: render units by tile position; if unit's position is < another unit's
	/// position, render that one first. --- ///
	for (int i = 0; i < m_vecUnits.size(); i++) {
		m_vecUnits[i]->desc.m_fXScale = 2.0f;
		m_vecUnits[i]->desc.m_fYScale = 2.0f;
		//m_vecUnits[i]->desc.m_vPos.y += 20.0f;	// make her fly up.
		m_vecUnits[i]->draw();
	}

}

void CUnitManager::UpdateUnits() {
	for (int i = 0; i < m_vecUnits.size(); i++) {
		//update unit
	}
}

void CUnitManager::MoveUnit(Tile *destTile) {
	Vector2 endPos = destTile->pos;
	endPos.y += 20.0f;	// for offset

	if (!playerUnit->is_stationary) {
		playerUnit->lerpInfo.currDuration += m_pTimer->GetFrameTime();
		float percentComplete = (std::min)(playerUnit->lerpInfo.currDuration / playerUnit->lerpInfo.maxDuration, 1.0f);
		playerUnit->desc.m_vPos = Math::lerp(playerUnit->desc.m_vPos, endPos, percentComplete);

		if (playerUnit->lerpInfo.currDuration >= playerUnit->lerpInfo.maxDuration) {

			playerUnit->lerpInfo.currDuration = 0.0f;
			playerUnit->is_stationary = true;
		}
	}
}

void CUnitManager::AttackUnit(int x, int y, int targetX, int targetY) {
	for (int i = 0; i < m_vecUnits.size(); i++) {
		//if (m_vecUnits[i].x == x && m_vecUnits[i].y == y) {
			//attack unit
		//}
	}
}