#pragma once
#include "GridTile.h"
#include "Object.h"
#include <map>
#include <utility>
#include <vector>
#include <fstream>


typedef struct TileCell : public CObject {
	TileCell(const Vector2& pos) : CObject(eSprite::Tile, pos) {}
	const Vector2 position;
	bool isSelected = false;

	void AddToTile() {
		// Add to the tile.
	}

	void UpdateTile() {
		// Update the tile.
	}

	void ResetTile() {
		// Reset the tile.
	}
	
	void SelectTile() {
		isSelected = true;
	}

	Vector2 GetTilePosition() {
		return position;
	}

	void DeselectTile() {
		isSelected = false;
	}
} TileCell;

class TileMap
{
public:
	TileMap();				///< Constructor.
	~TileMap();				///< Destructor.

	void MapTiles(char*);	///< Map the tiles
	void DrawMap();			///< Draw the map.
	void AddToMap();		///< Add to the map.
	void UpdateMap();		///< Update the map.
	void ResetMap();		///< Reset the map.
	
	void SelectTile(TileCell* tile) {
		tile->SelectTile();
	};
private:
	int width = 0;			///< # of tiles wide.
	int height = 0;			 ///< # of tiles tall.
	//float tileMap[height][width];	///< Map of tiles.
	std::map<Vector2, TileCell*> mapTiles;	///< Map of tiles.



	std::vector<TileCell*> selectedTiles;
};

