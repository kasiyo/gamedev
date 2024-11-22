/// \file GameDefines.h
/// \brief Game specific defines.

#ifndef __L4RC_GAME_GAMEDEFINES_H__
#define __L4RC_GAME_GAMEDEFINES_H__

#include "Defines.h"
#include <unordered_map>

static const XMFLOAT4 DEFAULT_TILE_TINT = XMFLOAT4(0.8, 0.8, 0.8, 1);
static const XMFLOAT4 HIGHLIGHT_TILE_TINT = XMFLOAT4(0.9, 1, 0.9, 1);
static const XMFLOAT4 SELECTED_TILE_TINT = XMFLOAT4(1, 1.2, 1, 1);
static const XMFLOAT4 DEFAULT_UNIT_TINT = XMFLOAT4(10, 1, 1, 1);

static const float UPDATE_UNITS_IN_SECONDS = 0.5f;
static const float UPDATE_CURRENCY_IN_SECONDS = 0.5f;

static const int CURRENCY_GAINED_ON_UPDATE = 10;
static const int CURRENCY_MAX = 100;

static const float NOTIFICATION_DURATION = 1.0f;

/// \brief Sprite enumerated type.
///
/// An enumerated type for the sprites, which will be cast to an unsigned
/// integer and used for the index of the corresponding texture in graphics
/// memory. `Size` must be last.

enum class eSprite : UINT {
	Background, Bullet, Bullet2, Player, Smoke, Tile, Spark, Line,
	Wall, GrassTile, Appliance,
	Size  //MUST BE LAST
}; //eSprite

struct TileInfo {
	eSprite baseSprite;
	int frameIndex;
};

static std::unordered_map<char, TileInfo> CHAR_TO_TILE = {
	{ 'F', { eSprite::GrassTile, 0 } },
	{ 'W', { eSprite::GrassTile, 1 } },
	{ 'T', { eSprite::GrassTile, 1 } },
	{ 'P', { eSprite::GrassTile, 1 } },
	{ '0', { eSprite::Tile, 0 } },
	{ '1', { eSprite::Tile, 1 } },
	{ '2', { eSprite::Tile, 2 } },
	{ '3', { eSprite::Tile, 3 } },
	{ '4', { eSprite::Tile, 4 } },
	{ '5', { eSprite::Tile, 5 } },
	{ '6', { eSprite::Tile, 6 } },
	{ '7', { eSprite::Tile, 7 } },
	{ '8', { eSprite::Tile, 8 } },
	{ '9', { eSprite::Tile, 9 } },
	{ 'A', { eSprite::Tile, 10 } },
	{ 'B', { eSprite::Tile, 11 } },
	{ 'C', { eSprite::Tile, 12 } },
	{ 'D', { eSprite::Tile, 13 } },
	{ 'E', { eSprite::Tile, 14 } },
};

/// \brief Sound enumerated type.
///
/// An enumerated type for the sounds, which will be cast to an unsigned
/// integer and used for the index of the corresponding sample. `Size` must 
/// be last.

enum class eSound : UINT {
	Start, Boom, Clang, Grunt, Gun, Ricochet,
	Size  //MUST BE LAST
}; //eSound

/// \brief Game state enumerated type.
///
/// An enumerated type for the game state, which can be either playing or
/// waiting a short interval for the level to end after winning or losing to
/// let sounds and particle effects play to the end without being cut short.

enum class eGameState {
	MainMenu, Playing, Paused, Waiting
}; //eGameState

#endif //__L4RC_GAME_GAMEDEFINES_H__
