/// \file GameDefines.h
/// \brief Game specific defines.

#ifndef __L4RC_GAME_GAMEDEFINES_H__
#define __L4RC_GAME_GAMEDEFINES_H__

#include "Defines.h"
#include <unordered_map>

static const XMFLOAT4 DEFAULT_TILE_TINT = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.f);
static const XMFLOAT4 HIGHLIGHT_TILE_TINT = XMFLOAT4(0.9f, 1.f, 0.9f, 1.f);
static const XMFLOAT4 SELECTED_TILE_TINT = XMFLOAT4(1.f, 1.2f, 1.f, 1.f);
static const XMFLOAT4 DEFAULT_UNIT_TINT = XMFLOAT4(10.f, 1.f, 1.f, 1.f);

static const float UPDATE_UNITS_IN_SECONDS = 0.5f;
static const float UPDATE_CURRENCY_IN_SECONDS = 0.5f;

static const int CURRENCY_GAINED_ON_UPDATE = 10;
static const int CURRENCY_MAX = 100;

static const float NOTIFICATION_DURATION = 1.0f;

static const float WALK_DURATION = 0.025f;

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
	Block, Bridge, Fence, Water, Lagoon, Nature,
	Size  //MUST BE LAST
}; //eSprite

struct TileInfo {
	eSprite baseSprite;
	int frameIndex;
	bool isFullHeight;
};

struct BlockInfo {
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

struct LerpInfo {
	Vector2 source;
	Vector2 target;
	float currDuration;
	float maxDuration;
};


static std::unordered_map<char, TileInfo> CHAR_TO_TILE = {
	{ '`', { eSprite::GrassTile, 0 } },
	//{ 'W', { eSprite::GrassTile, 1 } },
	//{ 'T', { eSprite::GrassTile, 1 } },
	//{ 'P', { eSprite::GrassTile, 1 } },
	{ '0', { eSprite::Tile, 0, false } },		//	light brown paved tile
	{ '1', { eSprite::Tile, 1, false } },		//	purple paved tile
	{ '2', { eSprite::Tile, 2, false } },		//	dark brown paved tile
	{ '3', { eSprite::Tile, 3, false } },		//	light brown unpaved tile
	{ '4', { eSprite::Tile, 4, false } },		//	purple unpaved tile
	{ '5', { eSprite::Tile, 5, false } },		//	dark brown unpaved tile
	{ '6', { eSprite::Tile, 6, false } },		//	dead grass paved tile
	{ '7', { eSprite::Tile, 7, false } },		//	snow tile
	{ '8', { eSprite::Tile, 8, false } },		//	ice tile
	{ '9', { eSprite::Tile, 9, false } },		//	lava tile
	{ 'A', { eSprite::Tile, 10, false } },		//	dead grass tile
	{ 'B', { eSprite::Tile, 11, false } },		//	dirt tile
	{ 'C', { eSprite::Tile, 12, false } },		//	grass tile
	{ 'D', { eSprite::Tile, 13, false } },		//	purple water tile
	{ 'E', { eSprite::Tile, 14, false } },		//	orange water tile
	{ 'F', { eSprite::Tile, 15, false } },		//	brick tile

	{ 'G', { eSprite::Block, 0, true } },		//	clay brick block
	{ 'H', { eSprite::Block, 1, true } },		//	wooden block
	{ 'I', { eSprite::Block, 2, true } },		//	stone brick block
	{ 'J', { eSprite::Block, 3, true } },		//	red brick block
	{ 'K', { eSprite::Block, 4, true } },		//	unpaved clay block
	{ 'L', { eSprite::Block, 5, true } },		//	unpaved stone block
	{ 'M', { eSprite::Block, 6, true } },		//	dirt block
	{ 'N', { eSprite::Block, 7, true } },		//	light grass block
	{ 'O', { eSprite::Block, 8, true } },		//	dark grass block
	{ 'P', { eSprite::Block, 9, true } },		//	light paved road block
	{ 'Q', { eSprite::Block, 10, true } },		//	dark paved road block
	{ 'R', { eSprite::Block, 11, true } },		//	snow block
	{ 'S', { eSprite::Block, 12, true } },		//	ice block
	{ 'T', { eSprite::Block, 13, true } },		//	orange cream soda block
	{ 'U', { eSprite::Block, 14, true } },		//	lava block
	{ 'V', { eSprite::Block, 15, true } },		//	purple rock block

	{ 'W', { eSprite::Water, 0, true } },		//	water full corner block
	{ 'X', { eSprite::Water, 1, true } },		//	water left corner block
	{ 'Y', { eSprite::Water, 2, true } },		//	water right corner block
	{ 'Z', { eSprite::Water, 3, true } },		//	tall water tile
	{ 'a', { eSprite::Water, 4, false } },		//	water full corner tile
	{ 'b', { eSprite::Water, 5, false } },		//	water left corner tile
	{ 'c', { eSprite::Water, 6, false } },		//	water right corner tile
	{ 'd', { eSprite::Water, 7, false } },		//	short water tile

	{ 'e', { eSprite::Lagoon, 0, true } },		//	lagoon full corner block
	{ 'f', { eSprite::Lagoon, 1, true } },		//	lagoon left corner block
	{ 'g', { eSprite::Lagoon, 2, true } },		//	lagoon right corner block
	{ 'h', { eSprite::Lagoon, 3, true } },		//	tall lagoon tile
	{ 'i', { eSprite::Lagoon, 4, false } },		//	lagoon full corner tile
	{ 'j', { eSprite::Lagoon, 5, false } },		//	lagoon left corner tile
	{ 'k', { eSprite::Lagoon, 6, false } },		//	lagoon right corner tile
	{ 'l', { eSprite::Lagoon, 7, false } },		//	short lagoon tile

	{ 'm', { eSprite::Fence, 0, true } },		//	north/south fence
	{ 'n', { eSprite::Fence, 1, true } },		//	east/west fence
	{ 'o', { eSprite::Bridge, 0, true } },		//	north/south dark bridge
	{ 'p', { eSprite::Bridge, 1, true } },		//	east/west dark bridge
	{ 'q', { eSprite::Bridge, 2, true } },		//	north/south light bridge
	{ 'r', { eSprite::Bridge, 3, true } },		//	east/west light bridge

	{ 's', { eSprite::Nature, 0, true } },		//	tree stump
	{ 't', { eSprite::Nature, 1, true } },		//	tree trunk
	{ 'u', { eSprite::Nature, 2, true } },		//	big bush
	{ 'v', { eSprite::Nature, 3, true } },		//	cactus
	{ 'w', { eSprite::Nature, 4, true } },		//	icicles
	{ 'x', { eSprite::Nature, 5, true } },		//	purple spike
	{ 'y', { eSprite::Nature, 6, false } },		//	small rock cluster
	{ 'z', { eSprite::Nature, 7, false } },		//	small bush
	{ '!', { eSprite::Nature, 8, true } },		//	big rock cluster
	{ '@', { eSprite::Nature, 9, false } },		//	dead bush
	{ '#', { eSprite::Nature, 10, true } },		//	cat tail bush
	{ '$', { eSprite::Nature, 11, true } },		//	fire
};

static std::unordered_map<char, BlockInfo> CHAR_TO_BLOCK = {
	{ 'B', { eSprite::Block, 0 } },		//	brick block
	{ 'C', { eSprite::Block, 1 } },		//	crate block
	{ 'D', { eSprite::Block, 2 } },		//	dirt block
	{ 'G', { eSprite::Block, 3 } },		//	grass block
	{ 'L', { eSprite::Block, 4 } },		//	lava block
	{ 'M', { eSprite::Block, 5 } },		//	metal block
	{ 'P', { eSprite::Block, 6 } },		//	purple block
	{ 'S', { eSprite::Block, 7 } },		//	snow block
	{ 'W', { eSprite::Block, 8 } },		//	wood block
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
	BGM,
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
