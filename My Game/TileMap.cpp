#include "TileMap.h"

/// Constructor.
TileMap::TileMap(size_t n) {
	//mapTiles = nullptr;		// start with empty map.
	TileCell t;
	float w = 0.0f, h = 0.0f;

	printf("TileCell::GetTileSize(width, height) = %1.f %1.f\n", w, h);
	width = -1;				// arbitrary value/empty.
	height = -1;			// arbitrary value/empty.
	tileSize = ((float) n);		// set the tile size.
}

/// Destructor.
TileMap::~TileMap() {
	/*if (tileMap != nullptr) {
		//tileMap.clear();
		delete tileMap;
		delete mapTiles;
	}*/

	//mapTiles.clear();

	for (size_t i = 0; i < height; i++) {
		delete[] tileMap[i];
	}
	delete[] tileMap;
}

/// Load the map.
void TileMap::LoadMap(char* filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		fprintf(stderr, "map file not found\n");
		ABORT("map &s not found\n", filename);
	}

	std::vector<std::string> lines;
	std::string line;
	while (std::getline(file, line)) {
		lines.push_back(line);
	}
	file.close();

	if (lines.empty()) {
		fprintf(stderr, "map file is empty\n");
		ABORT("map file is empty\n");
		return;
	}

	height = lines.size();
	width = lines[0].size();

	//mapTiles = new std::map<Vector2, TileCell*>();
	//mapTiles = new TileCell* [height];
	

	tileMap = new TileCell* [height];
	for (size_t i = 0; i < height; i++) {
		tileMap[i] = new TileCell[width];
	}

	for (size_t i = 0; i < height; i++) {
		for (size_t j = 0; j < width; j++) {
			const char c = lines[i][j];
			//TileCell *tile = new TileCell;
			
			switch (c) {
			case 'F': {	// Add a tile to the map.
				TileCell tile = TileCell(eSprite::GrassTile, Vector2(j, i));
				//TileCell *tile = new TileCell();
				//tile->SetPosition(Vector2(j, i));
				//tileSize = tile->GetSize(eSprite::Tile, tile->GetTilePosition().x, tile->GetTilePosition().y);
				//mapTiles[i][j] = tile;
				//mapTiles.insert(std::make_pair(Vector2(j, i), tile));
				//tileMap[i][j] = new TileCell(eSprite::Tile, Vector2(j, i));
				tileMap[i][j] = tile;

				printf("new floor Tile at (%d, %d)\n", j, i);
				break;
			}
			case 'W': {	// Add a wall to the map.
				//TileCell* tile = new TileCell(Vector2(j, i));
				TileCell* tile = new TileCell(eSprite::GrassTile, Vector2(j, i));
				//mapTiles.insert(std::make_pair(Vector2(j, i), tile));
				printf("new wall Tile at (%d, %d)\n", j, i);
				break;
			}
			default: {
				break;
			}
			}
		}
	}
}

/// Make bounding boxes for walls.
void TileMap::MakeBoundingBoxes() {
	walls.clear();

	BoundingBox box;


	/*for (auto& tile : mapTiles) {
		if (tile.second->isSelected) {
			BoundingBox box;
			//box.m_vMin = Vector2(tile.second->position.x, tile.second->position.y);
			//box.m_vMax = Vector2(tile.second->position.x + 1, tile.second->position.y + 1);
			walls.push_back(box);
		}
	}*/
}

/// Draw the map.
void TileMap::DrawMap() {
	
}

/// Add to the map.
void TileMap::AddToMap() {
	
}

/// Update the map.
void TileMap::UpdateMap() {

}

/// Reset the map.
void TileMap::ResetMap(){

}