#pragma once
#include "Object.h"
#include <map>
#include <utility>

class GridTile : public CObject
{
public:
	GridTile(const Vector2& tile);	///< Constructor.
	~GridTile();	///< Destructor.

	void LoadTile(char*);	///< Load a tile.
	void AddToTile();		///< Add to the tile.
	void UpdateTile();		///< Update the tile.
	void ResetTile();		///< Reset the tile.

	Vector2 GetTilePosition();	///< Get the tile position.
	void SelectTile(GridTile* tile);		///< Select the tile.
	GridTile* GetSelectedTile();		///< Get the selected tile.
	void DeselectTile();		///< Deselect the tile.
private:
	const Vector2 tilePosition;
	GridTile* currentSelectedTile;

};

