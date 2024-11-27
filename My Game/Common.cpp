/// \file Common.cpp
/// \brief Code for the class CCommon.
///
/// This file contains declarations and initial values
/// for CCommon's static member variables.

#include "Common.h"

CRenderer* CCommon::m_pRenderer = nullptr;
CObjectManager* CCommon::m_pObjectManager = nullptr;
LParticleEngine2D* CCommon::m_pParticleEngine = nullptr;
CTileManager* CCommon::m_pTileManager = nullptr;
CUnitManager* CCommon::m_pUnitManager = nullptr;

bool CCommon::m_bDrawAABBs = false;
bool CCommon::m_bGodMode = false;

Vector2 CCommon::m_vWorldSize = Vector2::Zero;
//CPlayer* CCommon::m_pPlayer = nullptr;
Unit* CCommon::playerUnit = nullptr;
Tile* CCommon::playerTile = nullptr;
