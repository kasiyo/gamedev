/// \file Game.h
/// \brief Interface for the game class CGame.

#ifndef __L4RC_GAME_GAME_H__
#define __L4RC_GAME_GAME_H__

#include "Component.h"
#include "Common.h"
#include "ObjectManager.h"
#include "Settings.h"
#include "Player.h"
#include "PlayerCamera.h"
#include "Math.h"
#include "TileManager.h"
#include "Notification.h"
#include "UnitManager.h"
#include "GameMaster.h"
#include <queue>

/// \brief The game class.
///
/// The game class is the object-oriented implementation of the game. This class
/// must contain the following public member functions. `Initialize()` does
/// initialization and will be run exactly once at the start of the game.
/// `ProcessFrame()` will be called once per frame to create and render the
/// next animation frame. `Release()` will be called at game exit but before
/// any destructors are run.

class CGame :
	public LComponent,
	public LSettings,
	public CCommon {

private:
	bool m_bDrawFrameRate = true; ///< Draw the frame rate.
	eGameState m_eGameState = eGameState::Playing; ///< Game state.
	int m_nNextLevel = 0; ///< Current level number.
	Tile *prevSelectedTile;
	Tile *prevHighlightedTile;
	std::vector<Tile *> tiles;
	std::vector<Unit *> units;
	std::vector<Vector2> unitPositions;

	unsigned int frameCount = 0;
	int currency = 0;

	//notification test
	std::vector<Notification> notifications;

	void HighlightTile(); ///< Highlight the tile under the cursor.
	void SelectTile(); ///< Select the tile under the cursor.
	void UpdateCurrency();
	void UpdateUnits();
	void UpdateNotifications();

	void UpdatePlayerUnit();

	void DrawNumFrames();
	void ProcessPlayerInput(const WPARAM k);
	void DetectPlayerInput();

	std::queue<WPARAM> inputBuffer;
	const int MAX_INPUTS_BUFFERED = 1;

	void LoadImages(); ///< Load images.
	void LoadSounds(); ///< Load sounds.
	void BeginGame(); ///< Begin playing the game.
	void MouseHandler(); ///< the mouse handler.
	void KeyboardHandler(); ///< The keyboard handler.
	void ControllerHandler(); ///< The controller handler.
	void RenderFrame(); ///< Render an animation frame.
	void DrawFrameRateText(); ///< Draw frame rate text to screen.
	void DrawGodModeText(); ///< Draw god mode text if in god mode.
	void DrawDebugMenu(); ///< Draw debug menu.

	void DrawSprites(); ///< Draw sprites.

	void CreateObjects(); ///< Create game objects.
	void FollowCamera(); ///< Make camera follow player character.
	void ProcessGameState(); ///< Process game state.

	PlayerCamera camera; ///< The camera.
	//TileMap tileMap; ///< The tile map.

	void GameOver(); ///< Game over.

	bool m_bDrawGameOver = false; ///< Player lost.
	bool m_bDrawWin = false; ///< Player won.

	bool m_bDrawDebugMenu = false; ///< Debug menu toggle.
	bool m_bGodMode = false; ///< God mode toggle.
	bool m_bPlayerIsMoving = false; ///< Player is moving.

	const int z_layers = 3; ///< Number of z layers.
public:
	~CGame(); ///< Destructor.

	void Initialize(); ///< Initialize the game.
	void ProcessFrame(); ///< Process an animation frame.
	void Release(); ///< Release the renderer.
}; //CGame

#endif //__L4RC_GAME_GAME_H__
