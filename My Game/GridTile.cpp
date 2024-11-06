#include "GridTile.h"

GridTile::GridTile(const Vector2& tile) : CObject(eSprite::Tile, tile)
{
	m_bIsTile = true;
}

GridTile::~GridTile()
{
	delete this;
}

void GridTile::LoadTile(char* tile)
{
	// Load the tile.
}

void GridTile::AddToTile()
{
	// Add to the tile.
}

void GridTile::UpdateTile()
{
	// Update the tile.
}

void GridTile::ResetTile()
{
	// Reset the tile.
}

Vector2 GridTile::GetTilePosition()
{
	return GridTile::tilePosition;
}

void GridTile::SelectTile(GridTile* tile)
{
	GridTile::currentSelectedTile = tile;
}

GridTile* GridTile::GetSelectedTile()
{
	return GridTile::currentSelectedTile;
}

void GridTile::DeselectTile()
{
	GridTile::currentSelectedTile = nullptr;
}