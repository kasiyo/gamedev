#include "GameMaster.h"

GameMaster::GameMaster(const Vector2& p) : LBaseObject(eSprite::AnnoyingOrange, p) {
	desc.m_nCurrentFrame = 0;

}

/*void GameMaster::draw() {
	m_pRenderer->Draw(&desc); // Draw the unit.
}

void GameMaster::move() {
	// Move the unit.
	const float delta = m_pTimer->GetFrameTime();
}*/