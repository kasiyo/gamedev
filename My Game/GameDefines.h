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

enum class eFont : UINT {
	Normal, Large, Small, Title, Notification,
	Size  //MUST BE LAST
};

/// \brief Sprite enumerated type.
///
/// An enumerated type for the sprites, which will be cast to an unsigned
/// integer and used for the index of the corresponding texture in graphics
/// memory. `Size` must be last.

enum class eSprite : UINT {
	Background, Bullet, Bullet2, Player, Smoke, Spark, Line,
	Tile, GrassTile, Unit, AnnoyingOrange,
	Block, Bridge, Fence, Water, Lagoon,
	Size  //MUST BE LAST
}; //eSprite

struct TileInfo {
	eSprite baseSprite;
	int frameIndex;
};

struct UnitInfo {
	eSprite baseSprite;
	int frameIndex;
};

static enum GameMasterPhase {
	ANNOYED, BACK_TURNED, GRIN,
};

struct GMInfo {
	eSprite baseSprite;
	int frameIndex;
	GameMasterPhase currentPhase;
};



static std::unordered_map<char, TileInfo> CHAR_TO_TILE = {
	{ 'F', { eSprite::GrassTile, 0 } },
	{ 'W', { eSprite::GrassTile, 1 } },
	{ 'T', { eSprite::GrassTile, 1 } },
	{ 'P', { eSprite::GrassTile, 1 } },
	{ '0', { eSprite::Tile, 0 } },		//	light brown paved tile
	{ '1', { eSprite::Tile, 1 } },		//	purple paved tile
	{ '2', { eSprite::Tile, 2 } },		//	dark brown paved tile
	{ '3', { eSprite::Tile, 3 } },		//	light brown unpaved tile
	{ '4', { eSprite::Tile, 4 } },		//	purple unpaved tile
	{ '5', { eSprite::Tile, 5 } },		//	dark brown unpaved tile
	{ '6', { eSprite::Tile, 6 } },		//	dead grass paved tile
	{ '7', { eSprite::Tile, 7 } },		//	snow tile
	{ '8', { eSprite::Tile, 8 } },		//	ice tile
	{ '9', { eSprite::Tile, 9 } },		//	lava tile
	{ 'A', { eSprite::Tile, 10 } },		//	dead grass tile
	{ 'B', { eSprite::Tile, 11 } },		//	dirt tile
	{ 'C', { eSprite::Tile, 12 } },		//	grass tile
	{ 'D', { eSprite::Tile, 13 } },		//	purple water tile
	{ 'E', { eSprite::Tile, 14 } },		//	orange water tile
	{ 'F', { eSprite::Tile, 15 } },		//	brick tile
};

static std::unordered_map<char*, UnitInfo> CHAR_TO_UNIT = {
	{ "FL", { eSprite::Unit, 0 } },	// front facing left
	{ "FR", { eSprite::Unit, 1 } }, // front facing right
	{ "BR", { eSprite::Unit, 2 } }, // back facing right
	{ "BL", { eSprite::Unit, 3 } }, // back facing left
};

static std::unordered_map<GameMasterPhase, GMInfo> CHAR_TO_GM = {
	{ ANNOYED, { eSprite::AnnoyingOrange, 0 } },
	{ BACK_TURNED, { eSprite::AnnoyingOrange, 1 } },
	{ GRIN, { eSprite::AnnoyingOrange, 2 } },
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
	Playing, Waiting, GameOver, GameWon
}; //eGameState

#endif //__L4RC_GAME_GAMEDEFINES_H__
