/// \file Renderer.h 
/// \brief Interface for the renderer class CRenderer.

#pragma once

#include "GameDefines.h"
#include "SpriteRenderer.h"

/// \brief The renderer.
///
/// CRenderer handles the game-specific rendering tasks, relying on
/// the base class to do all of the actual API-specific rendering.

class CRenderer: public LSpriteRenderer {
  public:
    CRenderer(); ///< Constructor.

    void LoadImages(); ///< Load images.
    HWND GetWindow();
    LBaseCamera* GetCamera();
    int GetWindowWidth();
    int GetWindowHeight();
    float GetWindowScaleFromBase();

	void CalculateIso(int i, int j, float scale, float descSize, Vector2&); ///< Calculate isometric projection.

    

    void LoadScreenFont(char*);

  private:
      HWND currWindow;
    }; //CRenderer