#include "GameMaster.h"

GameMaster::GameMaster(const Vector2& p) : LBaseObject(eSprite::AnnoyingOrange, p) {
	desc.m_nCurrentFrame = 0;

	if (m_pTileManager != nullptr) {
		//spawnTile = m_pTileManager->GetGMSpawnPoint();
		//m_vPos = spawnTile->pos;
		GMTile = m_pTileManager->GetGMSpawnPoint();
		desc.m_vPos = GMTile->pos;
		m_vPos = GMTile->pos;
		m_nCurrentFrame = 0;
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

void GameMaster::GetSpawnPoint() {
	if (m_pTileManager != nullptr) {
		GMTile = m_pTileManager->GetGMSpawnPoint();
	}
}

void GameMaster::SwitchPhases(int nextPhaseNum) {
	const int numStates = 3;

	GameMasterPhase nextPhase = static_cast<GameMasterPhase>(nextPhaseNum);
	std::string phaseName;
	switch (nextPhase) {
	case BACK_TURNED: {
		this->info.currentPhase = BACK_TURNED;
		this->m_nCurrentFrame = nextPhaseNum;
		this->desc.m_nCurrentFrame = nextPhaseNum;
		phaseName = "BACK_TURNED";
		IsFacingPlayer = false;
		m_pTileManager->SetTileVisibility(false);
		break;
	};
	case ANNOYED: {
		info.currentPhase = ANNOYED;
		this->m_nCurrentFrame = nextPhaseNum;
		IsFacingPlayer = true;
		this->desc.m_nCurrentFrame = nextPhaseNum;
		phaseName = "ANNOYED";
		m_pTileManager->SetTileVisibility(true);
		break;
	};
	case GRIN: {
		info.currentPhase = GRIN;
		this->m_nCurrentFrame = nextPhaseNum;
		IsFacingPlayer = true;
		this->desc.m_nCurrentFrame = nextPhaseNum;
		phaseName = "GRIN";
		m_pTileManager->SetTileVisibility(true);
		break;
	};
	};
	this->m_nCurrentFrame = nextPhaseNum;
	printf("Switched to phase %d: %s\n", nextPhaseNum, phaseName.c_str());
	//m_pRenderer->Draw(this);
}