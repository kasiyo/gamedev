/// \file TileManager.h
/// \brief Interface for the tile manager CTileManager.

#ifndef __L4RC_GAME_TILEMANAGER_H__
#define __L4RC_GAME_TILEMANAGER_H__

#include <vector>

#include "Common.h"
#include "Settings.h"
#include "Sprite.h"
#include "GameDefines.h"
#include "UnitManager.h"

/// \brief The tile manager.
///
/// The tile manager is responsible for the tile-based background.

typedef struct Tile {
	TileInfo info;
	XMFLOAT4 tint = DEFAULT_TILE_TINT;
	bool viewableByGameMaster = false;
	bool isOccupied = false;
	int x;
	int y;
	Vector2 pos;
} Tile;

class CTileManager :
	public CCommon,
	public LSettings
{
private:
	size_t m_nWidth = 0; ///< Number of tiles wide.
	size_t m_nHeight = 0; ///< Number of tiles high.

	float m_fTileSize = 0.0f; ///< Tile width and height.

	Tile** m_chMap = nullptr; ///< The tile map.

	std::vector<BoundingBox> m_vecWalls; ///< AABBs for the walls.
	std::vector<Vector2> m_vecTurrets; ///< Turret positions.
	Vector2 m_vPlayer; ///< Player location.

	std::vector<Vector2> m_vecUnits; ///< Unit positions.

	float mapWidth = 0.0f;
	float mapHeight = 0.0f;
	std::vector<Vector2> TilesInScreenSpace; ///< Tiles in screen space.
public:
	CTileManager(size_t); ///< Constructor.
	~CTileManager(); ///< Destructor.

	void LoadMap(char*); ///< Load a map.
	void Draw(eSprite); ///< Draw the map with a given tile.
	void DrawBoundingBoxes(eSprite); ///< Draw the bounding boxes.
	void GetObjects(std::vector<Vector2>&, Vector2&); ///< Get objects.
	bool GetTile(int x, int y, Tile** refval); ///< Get a tile.

	float GetMapWidth();
	float GetMapHeight();
	float GetTileSize();

	Tile** GetMap(); ///< Get the map.

	const size_t GetWidth(); ///< Get width.
	const size_t GetHeight(); ///< Get height.
	//static Tile* GMSpawnPoint;
	Tile* GetGMSpawnPoint(); ///< Get the game master spawn point (centermost tile of first few rows).

	const bool Visible(const Vector2&, const Vector2&, float) const; ///< Check visibility.
	const bool CollideWithWall(BoundingSphere, Vector2&, float&) const; ///< Object-wall collision test.
}; //CTileManager

#endif //__L4RC_GAME_TILEMANAGER_H__