#include "TileMap.h"

/// Constructor.
TileMap::TileMap() {
	//mapTiles = nullptr;		// start with empty map.
	width = -1;				// arbitrary value/empty.
	height = -1;			// arbitrary value/empty.
}

/// Destructor.
TileMap::~TileMap() {
	/*if (mapTiles != nullptr) {
		mapTiles.clear();
		delete mapTiles;
	}*/
	mapTiles.clear();
}

/// Load the map.
void TileMap::MapTiles(char* filename) {
	/*if (mapTiles->empty()) {	// If the map is empty, load it.
		FILE* input;
		fopen_s(&input, filename, "r");

		if (input == NULL) {
			fprintf(stderr, "map file not found\n");
			ABORT("map &s not found\n", filename);
		}

		// Read the map.
		fseek(input, 0, SEEK_END);	// Seek to the end of the file.
		const size_t size = ftell(input);	// Get the size of the file.
		rewind(input);				// Rewind the file.

		char* buffer = new char[size + 1];
		if (buffer == nullptr) {
			fprintf(stderr, "memory allocation failed.\n");
			fclose(input);
			return;
		}

		fread(buffer, size, 1, input);
		fclose(input);

		width = 0;
		height = 0;
		int rowSize = 0;
		bool isFirstLine = true;

		for (int i = 0; i < size; i++) {
			if (buffer[i] != '\n') {
				rowSize++;
			}
			else {
				if (rowSize == 0) {
					ABORT("panic . . .\n");
				}
				if (rowSize != width && !isFirstLine && rowSize != 0) {
					ABORT("line %d of map is not the same length as the prev one.\n", height);
				}

				width = rowSize; rowSize = 0; height++;
				isFirstLine = false;
			}
		}
		size_t index = 0;

		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				const char c = buffer[index];

				switch (c) {
				case 'F': {// Add a tile to the map.
					TileCell* tile = new TileCell(Vector2(j, i));
					
					break;
				}
				case 'W': {	// Add a wall to the map.
					TileCell* tile = new TileCell(Vector2(j, i));
					
					break;
				}
				default: {
					break;
				}
					
				}

				index++;
			}
		}


		// Load the map.
	}
	else {
		mapTiles->clear();
	}*/
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
	

	for (size_t i = 0; i < height; i++) {
		for (size_t j = 0; j < width; j++) {
			const char c = lines[i][j];

			switch (c) {
			case 'F': {	// Add a tile to the map.
				TileCell* tile = new TileCell(Vector2(j, i));
				//mapTiles[i][j] = tile;
				mapTiles.insert(std::make_pair(Vector2(j, i), tile));
				printf("new floor Tile at (%d, %d)\n", j, i);
				break;
			}
			case 'W': {	// Add a wall to the map.
				TileCell* tile = new TileCell(Vector2(j, i));
				mapTiles.insert(std::make_pair(Vector2(j, i), tile));
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