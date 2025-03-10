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
	float alpha = DEFAULT_TILE_ALPHA;
	bool viewableByGameMaster = false;
	bool isOccupied = false;
	bool isWalkable = true;
	bool isWinTile = false;
	bool isNull = true;
	bool isStartTile = false;

	int x;
	int y;
	int z;

	int draw_index = -1;
	Vector2 pos;
	float y_offset = 0;
	float x_offset = 0;
	LSpriteDesc2D desc;
} Tile;

class CTileManager :
	public CCommon,
	public LSettings {
private:
	size_t m_nWidth = 0; ///< Number of tiles wide.
	size_t m_nHeight = 0; ///< Number of tiles high.

	float m_fTileSize = 0.0f; ///< Tile width and height.

	Tile **m_chMap = nullptr; ///< The tile map.

	std::vector<BoundingBox> m_vecWalls; ///< AABBs for the walls.
	std::vector<Vector2> m_vecTurrets; ///< Turret positions.
	Vector2 m_vPlayer; ///< Player location.

	std::vector<Vector2> m_vecUnits; ///< Unit positions.

	float mapWidth = 0.0f;
	float mapHeight = 0.0f;
	const int numRows = 30;
	const int numCols = 30;
	const int numLayers = 1;
public:
	CTileManager(size_t); ///< Constructor.
	~CTileManager(); ///< Destructor.

	std::vector<Tile *> WorldMap; ///< World map.
	std::vector<Tile *> StartTiles; ///< Start tiles.

	void LoadMap(char *); ///< Load a map.
	void Draw(eSprite); ///< Draw the map with a given tile.
	void DrawBoundingBoxes(eSprite); ///< Draw the bounding boxes.
	void GetObjects(std::vector<Vector2> &, Vector2 &); ///< Get objects.
	bool GetTile(int x, int y, Tile **refval); ///< Get a tile.
	bool GetIndex(int x, int y, int z, Tile **refval); ///< Get an index.

	float GetTileSize();

	Tile **GetMap(); ///< Get the map.

	const size_t GetWidth(); ///< Get width.
	const size_t GetHeight(); ///< Get height.

	Tile *GetGMSpawnPoint(); ///< Get the game master spawn point (centermost tile of first few rows).

	void SetTileVisibility(bool);

	/// <summary>
	///	--- TODO: build a layer system for z-levels --- ///
	/// </summary>
	//void AddLayer(); ///< Add a layer.
	//void LoadLayer(char*); ///< Load a layer.
	//void DrawLayer(eSprite); ///< Draw a layer.
	//void EditLayer(int layerToEdit); ///< Edit a layer.

	const bool Visible(const Vector2 &, const Vector2 &, float) const; ///< Check visibility.
	const bool CollideWithWall(BoundingSphere, Vector2 &, float &) const; ///< Object-wall collision test.
}; //CTileManager

#endif //__L4RC_GAME_TILEMANAGER_H__