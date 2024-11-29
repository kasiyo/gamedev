#include "GameMaster.h"

GameMaster::GameMaster(const Vector2& p) : LBaseObject(eSprite::AnnoyingOrange, p) {
	desc.m_nCurrentFrame = 0;

	if (m_pTileManager != nullptr) {
		//spawnTile = m_pTileManager->GetGMSpawnPoint();
		//m_vPos = spawnTile->pos;
		GMTile = m_pTileManager->GetGMSpawnPoint();
		desc.m_vPos = GMTile->pos;
		m_vPos = GMTile->pos;

	}

}

void GameMaster::draw() {
	this->desc.m_vPos = GMTile->pos;
	this->m_vPos = GMTile->pos;
	m_pRenderer->Draw(this); // Draw the unit.
}

void GameMaster::move() {
	// Move the unit.
	const float delta = m_pTimer->GetFrameTime();
}