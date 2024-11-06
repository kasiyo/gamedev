#pragma once
#include "BaseObject.h"
#include "BaseObjectManager.h"
#include "SpriteRenderer.h"
#include "GameDefines.h"
#include "GridTile.h"
#include "Object.h"
#include "Renderer.h"
#include <map>
#include <utility>
#include <vector>
#include <fstream>
#include <ParticleEngine.h>
#include "Common.cpp"
#include "Common.h"
#include "Sprite.h"


struct TileCell {
	LSpriteDesc2D desc;
	TileCell& operator=(const TileCell& rhs) {
		if (this != &rhs) {
			desc = rhs.desc;
		}
		return *this;
	}
	TileCell(eSprite t, const Vector2& pos) {
		desc = LSpriteDesc2D(UINT(t), pos);
	};
	TileCell() {
		desc = LSpriteDesc2D(UINT(eSprite::Tile), Vector2(0, 0));
	};
	
	//TileCell() : LBaseObject(eSprite::Tile, Vector2(0, 0)) {};

	const Vector2 position;
	bool isSelected = false;
	bool isWall = false;
	int zIndex = -1;
	size_t width;
	size_t height;

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
};

class TileMap /* : public LBaseObjectManager<TileCell>*/ {
	public:
		TileMap(size_t);				///< Constructor.
		~TileMap();				///< Destructor.

		void LoadMap(char*);	///< Map the tiles
		void MakeBoundingBoxes();	///< Make bounding boxes for walls.
		void DrawMap();			///< Draw the map.
		void AddToMap();		///< Add to the map.
		void UpdateMap();		///< Update the map.
		void ResetMap();		///< Reset the map.

		void SelectTile(TileCell* tile) {
			tile->SelectTile();
		};

		size_t GetTileSize() {
			return tileSize;
		}
	private:
		size_t width = 0;			///< # of tiles wide.
		size_t height = 0;			 ///< # of tiles tall.
		//float tileMap[height][width];	///< Map of tiles.
		std::map<Vector2, TileCell*> mapTiles;	///< Map of tiles.
		std::vector<BoundingBox> walls;	///< AABBs for the walls.
		TileCell** tileMap = nullptr;		///< Map of tiles.

		float tileSize = 0.0f;		///< Tile width and height.

		std::vector<TileCell*> selectedTiles;
};

