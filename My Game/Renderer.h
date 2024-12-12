/// \file Renderer.h 
/// \brief Interface for the renderer class CRenderer.

#pragma once

#include "GameDefines.h"
#include "SpriteRenderer.h"
#include "ComponentIncludes.h"
#include "Unit.h"
#include "UnitManager.h"
#include "GameMaster.h"
#include "Common.h"
#include "TileManager.h"

/// \brief The renderer.
///
/// CRenderer handles the game-specific rendering tasks, relying on
/// the base class to do all of the actual API-specific rendering.

class CRenderer : public LSpriteRenderer {
public:
	CRenderer(); ///< Constructor.

	void LoadImages(); ///< Load images.
	HWND GetWindow();

	LBaseCamera *GetCamera();
	int GetWindowWidth();
	int GetWindowHeight();
	float GetWindowScaleFromBase();

	void CalculateIso(int i, int j, float scale, float descSize, Vector2 &); ///< Calculate isometric projection.

	void DrawScaledText(const wchar_t *text, const Vector2 &p, float scale, XMVECTORF32 color); ///< Draw scaled text.

	void DrawSpriteOrder();
	void DrawGameOver(); ///< Draw game over.
	void DrawWin(); ///< Draw win.

	void LoadTextSprites();

	//void LoadScreenFont(const char*);

	void DrawScene(); ///< Draw the scene.

private:
	HWND currWindow;
}; //CRenderer