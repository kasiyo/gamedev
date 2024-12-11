/// \file TileManager.cpp
/// \brief Code for the tile manager CTileManager.

#include "TileManager.h"
#include "SpriteRenderer.h"
#include "Abort.h"
#define STBI_ASSERT(x)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Renderer.h"


/// Construct a tile manager using square tiles, given the width and height
/// of each tile.
/// \param n Width and height of square tile in pixels.

CTileManager::CTileManager(size_t n) :
	m_fTileSize((float)n * 1.25f) {
	/// --- TODO: sprite-sizing default from game.cpp --- ///
	//m_fTileSize((float)n) {
	printf("TileManager::TileManager(%1.f)\n", m_fTileSize);
	//Tile* CTileManager::GMSpawnPoint = nullptr;

} //constructor

/// Delete the memory used for storing the map.

CTileManager::~CTileManager() {
	for (size_t i = 0; i < m_nHeight; i++)
		delete[] m_chMap[i];

	delete[] m_chMap;
} //destructor


/// Delete the old map (if any), allocate the right sized chunk of memory for
/// the new map, and read it from a text file.
/// \param filename Name of the map file.

void CTileManager::LoadMap(char *filename) {
	if (m_chMap != nullptr) { //unload any previous maps
		for (size_t i = 0; i < m_nHeight; i++)
			delete[] m_chMap[i];

		delete[] m_chMap;
	} //if

	m_vecTurrets.clear(); //clear out the turret list

	FILE *input; //input file handle

	fopen_s(&input, filename, "rb"); //open the map file
	if (input == nullptr) {//abort if it's missing
		fprintf(stderr, "Map file not found\n");
		ABORT("Map &s not found.", filename); //panic
	}
	//read map file into a character buffer 

	fseek(input, 0, SEEK_END); //seek to end of map file
	const size_t n = ftell(input); //get file size in bytes
	rewind(input); //seek to start of file

	char *buffer = new char[n + 1]; //temporary character buffer
	if (buffer == nullptr) {
		fprintf(stderr, "Memory allocation failed\n");
		fclose(input);
		return;
	}

	fread(buffer, n, 1, input); //read the whole thing in a chunk
	fclose(input); //close the map file, we're done with it

	//get map width and height into m_nWidth and m_nHeight

	m_nWidth = 0;
	m_nHeight = 0;
	size_t w = 0; //width of current row
	bool bFirstLine = true;

	for (size_t i = 0; i < n; i++) {
		if (buffer[i] != '\n')
			w++; //skip characters until the end of line
		else {
			if (w == 0)ABORT("Panic!"); //should never happen

			if (w != m_nWidth && !bFirstLine && w != 0) //not the same length as the previous one
				ABORT("Line %d of map is not the same length as the previous one.", m_nHeight);

			m_nWidth = w; w = 0; m_nHeight++; //next line
			bFirstLine = false; //the next line is not the first
		} //else
	} //for

	//allocate space for the map 

	m_chMap = new Tile * [m_nHeight];

	for (size_t i = 0; i < m_nHeight; i++) {
		m_chMap[i] = new Tile[m_nWidth];
	}

	//load the map information from the buffer to the map

	size_t index = 0; //index into character buffer

	for (size_t i = 0; i < m_nHeight; i++) {
		for (size_t j = 0; j < m_nWidth; j++) {
			const char c = buffer[index];

			if (CHAR_TO_TILE.count(c) > 0) {
				m_chMap[i][j].info = CHAR_TO_TILE.at(c);
			} else {
				printf("char_to_tile does not contain %c\n", c);
				printf("m_chMap[%zu][%zu].info.baseSprite = eSprite::GrassTile\n", i, j);
				m_chMap[i][j].info.baseSprite = eSprite::GrassTile;
				m_chMap[i][j].info.frameIndex = 0;
				m_chMap[i][j].alpha = 0.0f;
			}

			if (m_chMap[i][j].info.isFullHeight) {
				m_chMap[i][j].y_offset = 50.0f;
				printf("m_chMap[%zu][%zu].y_offset = %f\n", i, j, m_chMap[i][j].y_offset);
				m_chMap[i][j].x_offset = m_chMap[i][j].desc.m_vPos.x;
				//m_chMap[i][j].x_offset = (m_pRenderer->GetHeight(eSprite::GrassTile) * 0.55f) / 2.0f;
				//printf("m_chMap[%zu][%zu].x_offset = %f\n", i, j, m_chMap[i][j].x_offset);
			} else {
				m_chMap[i][j].y_offset = 20.0f;
			}

			m_chMap[i][j].x = j; //x coordinate
			m_chMap[i][j].y = i; //y coordinate
			m_chMap[i][j].draw_index = 0; //draw index

			index++; //next index
		} //for

		index += 1; //skip end of line character (assume single linefeed now)
	} //for

	m_vWorldSize = Vector2((float)m_nWidth, (float)m_nHeight) * m_fTileSize;

	delete[] buffer; //clean up
} //LoadMap

/// Get positions of objects listed on map.
/// \param turrets [out] Vector of turret positions
/// \param player [out] Player position.

void CTileManager::GetObjects(std::vector<Vector2> &walls, Vector2 &player) {
	//turrets = m_vecTurrets;
	walls = m_vecTurrets;
	player = m_vPlayer;
} //GetObjects

/// Get the tile at a given position.
bool CTileManager::GetTile(int x, int y, Tile **refval) {
	if (x < 0 || x >= m_nWidth || y < 0 || y >= m_nHeight) {
		return false;
	}
	*refval = &m_chMap[y][x];
	return true;
}

/// Get the map.
Tile **CTileManager::GetMap() {
	return m_chMap;
}

/// This is for debug purposes so that you can verify that
/// the collision shapes are in the right places.
/// \param t Line sprite to be stretched to draw the line.

void CTileManager::DrawBoundingBoxes(eSprite t) {
	for (auto &p : m_vecWalls)
		m_pRenderer->DrawBoundingBox(t, p);
} //DrawBoundingBoxes

/// Draw order is top-down, left-to-right so that the image
/// agrees with the map text file viewed in NotePad.
/// \param t Sprite type for a 3-frame sprite: 0 is floor, 1 is wall, 2 is an error tile.

void CTileManager::Draw(eSprite t) {
	LSpriteDesc2D desc; //sprite descriptor for tile

	desc.m_nSpriteIndex = (UINT)t; //sprite index for tile

	//desc.m_fXScale = 1.3333333f; //scale of sprite
	//desc.m_fYScale = 1.3333333f; //scale of sprite

	desc.m_fXScale = 1.35f; //scale of sprite
	desc.m_fYScale = 1.35f; //scale of sprite

	/// --- TODO: sprite-sizing default from game.cpp
	//desc.m_fXScale = 1.0f;
	//desc.m_fYScale = 1.0f;

	const int w = (int)ceil(m_nWinWidth / m_fTileSize) + 2; //width of window in tiles, with 2 extra
	const int h = (int)ceil(m_nWinHeight / m_fTileSize) + 2; //height of window in tiles, with 2 extra

	const Vector2 campos = m_pRenderer->GetCameraPos(); //camera position
	const Vector2 origin = campos + 0.5f * m_nWinWidth * Vector2(-1.0f, 1.0f); //position of top left corner of window

	for (int i = 0; i < m_nHeight; i += 1) {
		for (int j = 0; j < m_nWidth; j += 1) {

			// make sure indices are within bounds.
			if (i >= m_nHeight || j >= m_nWidth || i < 0 || j < 0) {
				continue;
			}
			// calculate isometric coordinates for a tile based on grid position
			// (j - i): shift tile positions diagonally
			// * 0.5f: scale down the difference to fit the isometric grid
			// * (m_fTileSize * 1.5f): scale up the difference by 1.5x to fit the isometric grid
			// - (m_fTileSize * 0.25f): shift the x position to the left by 0.25x the tile size
			float isoX = ((j - i) * 0.5f) * (m_fTileSize * 1.5f) - (m_fTileSize * 0.25f);
			// (j + i): shift tile positions diagonally
			// * 0.5f: scale down the difference to fit the isometric grid
			// * (m_fTileSize * 0.75f): scale down the difference to fit the isometric grid
			// + (m_fTileSize * 0.25f): shift the y position up by 0.25x the tile size
			float isoY = ((j + i) * 0.5f) * (m_fTileSize * .75f) + (m_fTileSize * 0.25f);

			float scale = 1.5f;
			//float scale = 0.625f/*1.5f*/; //scale of the tile

			//multiply the isometric coordinates by the scale
			//desc.m_vPos.x = isoX * scale;
			//desc.m_vPos.y = (m_nWinHeight / 2) + (-isoY * scale);

			m_pRenderer->CalculateIso(i, j, scale, m_fTileSize, desc.m_vPos);
			desc.m_f4Tint = m_chMap[i][j].tint;
			desc.m_fAlpha = m_chMap[i][j].alpha;


			Tile &t = m_chMap[i][j];

			desc.m_nSpriteIndex = (UINT)t.info.baseSprite;
			desc.m_nCurrentFrame = t.info.frameIndex;

			m_chMap[i][j].pos = desc.m_vPos; //store the position of the tile

			LSpriteDesc3D desc3D(desc, 1.0f);

			m_pRenderer->Draw(&desc); //finally we can draw a tile
		} //for
	} //for
} //Draw

float CTileManager::GetMapWidth() {
	return m_vWorldSize.x;
	//return m_nWidth;
}

float CTileManager::GetMapHeight() {
	return m_vWorldSize.y;
	//return m_nHeight;
}

float CTileManager::GetTileSize() {
	return m_fTileSize;
}

const size_t CTileManager::GetWidth() {
	return m_nWidth;
} //GetWidth

const size_t CTileManager::GetHeight() {
	return m_nHeight;
} //GetHeight

Tile *CTileManager::GetGMSpawnPoint() {
	size_t centerX = m_nWidth / 2;
	size_t centerY = 2;
	Tile *t = nullptr;
	if (GetTile(centerX - (size_t)1, 1, &t)) {
		return t;
	}

	return &m_chMap[centerY][centerX];
}

/// Set the visibility of the tiles.
void CTileManager::SetTileVisibility(bool b) {
	if (b == true) {
		for (size_t i = 0; i < m_nHeight; i++) {
			for (size_t j = 0; j < m_nWidth; j++) {
				m_chMap[i][j].viewableByGameMaster = true;
			}
		}
	} else {
		for (size_t i = 0; i < m_nHeight; i++) {
			for (size_t j = 0; j < m_nWidth; j++) {
				m_chMap[i][j].viewableByGameMaster = false;
			}
		}
	}
} //SetTileVisibility

/// Check whether a circle is visible from a point, that is, either the left
/// or the right side of the object (from the perspective of the point)
/// has no walls between it and the point. This gives some weird behavior
/// when the circle is partially hidden by a block, but it doesn't seem
/// particularly unnatural in practice. It'll do.
/// \param p0 A point.
/// \param p1 Center of circle.
/// \param r Radius of circle.
/// \return true If the circle is visible from the point.

const bool CTileManager::Visible(const Vector2 &p0, const Vector2 &p1, float r) const {
	bool visible = true;

	for (auto i = m_vecWalls.begin(); i != m_vecWalls.end() && visible; i++) {
		Vector2 direction = p0 - p1;
		direction.Normalize();
		const Vector2 norm = Vector2(-direction.y, direction.x);

		const float delta = std::min(r, 16.0f);

		//left-hand triangle
		const Vector3 v0(p0);
		const Vector3 v1(p1 + r * norm);
		const Vector3 v2(p1 + (r - delta) * norm);

		//right-hand triangle
		const Vector3 v3(p1 - r * norm);
		const Vector3 v4(p1 - (r - delta) * norm);

		visible = !(*i).Intersects(v0, v1, v2) || !(*i).Intersects(v0, v3, v4);
	} //for

	return visible;
} //Visible

/// Check whether a bounding sphere collides with one of the wall bounding boxes.
/// If so, compute the collision normal and the overlap distance. 
/// \param s Bounding sphere of object.
/// \param norm [out] Collision normal.
/// \param d [out] Overlap distance.
/// \return true if the bounding sphere overlaps a wall.

const bool CTileManager::CollideWithWall(
	BoundingSphere s, Vector2 &norm, float &d) const {
	bool hit = false; //return result, true if there is a collision with a wall

	for (auto i = m_vecWalls.begin(); i != m_vecWalls.end() && !hit; i++) {
		const BoundingBox &aabb = *i; //shorthand

		Vector3 corner[8]; //for corners of aabb
		aabb.GetCorners(corner);  //get corners of aabb
		s.Center.z = corner[0].z; //make sure they are at the same depth

		//the first 4 corners of aabb are the same as the last 4 but with different z

		hit = s.Intersects(aabb); //includes when they are touching

		if (hit) { //collision with either a point or an edge
			bool bPointCollide = false; //true if colliding with corner of bounding box

			for (UINT i = 0; i < 4 && !bPointCollide; i++) //check first 4 corners
				if (s.Contains(corner[i])) { //collision of bounding sphere with corner
					bPointCollide = true;
					Vector3 norm3 = s.Center - corner[i]; //vector from corner to sphere center
					norm = (Vector2)norm3; //cast to 2D
					d = s.Radius - norm.Length(); //overlap distance
					norm.Normalize(); //norm needs to be a unit vector
				} //if

			if (!bPointCollide) { //edge collide
				const float fLeft = corner[0].x; //left of wall
				const float fRight = corner[1].x; //right of wall
				const float fBottom = corner[1].y; //bottom of wall
				const float fTop = corner[2].y; //top of wall

				const float epsilon = 0.01f; //small amount of separation

				if (s.Center.x <= fLeft) { //collide with left edge
					norm = -Vector2::UnitX; //normal
					d = s.Center.x - fLeft + s.Radius + epsilon; //overlap
				} //if

				else if (fRight <= s.Center.x) { //collide with right edge
					norm = Vector2::UnitX; //normal
					d = fRight - s.Center.x + s.Radius + epsilon; //overlap
				} //if

				else if (s.Center.y <= fBottom) { //collide with bottom edge
					norm = -Vector2::UnitY; //normal
					d = s.Center.y - fBottom + s.Radius + epsilon; //overlap
				} //if

				else if (fTop <= s.Center.y) { //collide with top edge
					norm = Vector2::UnitY; //normal
					d = fTop - s.Center.y + s.Radius + epsilon; //overlap
				} //if 
			} //if
		} //if
	} //for

	return hit;
} //CollideWithWall