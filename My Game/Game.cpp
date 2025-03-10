/// \file Game.cpp
/// \brief Code for the game class CGame.

#include "Game.h"

#include "GameDefines.h"
#include "SpriteRenderer.h"
#include "ComponentIncludes.h"
#include "ParticleEngine.h"
#include "TileManager.h"
#include "Renderer.h"
#include "shellapi.h"

/// Delete the renderer, the object manager, and the tile manager. The renderer
/// needs to be deleted before this destructor runs so it will be done elsewhere.

int sound = 0;
static size_t spriteSize = 0;
static const float SPRITE_SCALE = 0.55f;

static float inputTimer = 0.0f;
static float currDelay = 0.0f;
CGame::~CGame() {
	delete m_pParticleEngine;
	delete m_pObjectManager;
	delete m_pUnitManager;
	delete m_pTileManager;
	delete m_pGameMaster;
} //destructor

/// Initialize the renderer, the tile manager and the object manager, load 
/// images and sounds, and begin the game.

void CGame::Initialize() {
	m_pRenderer = new CRenderer();
	m_pRenderer->Initialize(eSprite::Size);
	LoadImages(); //load images from xml file list

	m_pTileManager = new CTileManager((size_t)m_pRenderer->GetWidth(eSprite::GrassTile) * SPRITE_SCALE);
	m_pUnitManager = new CUnitManager((size_t)m_pRenderer->GetWidth(eSprite::Unit));

	/// --- TODO: Change the tile size to match the new sprite size --- ///
	//m_pTileManager = new CTileManager((size_t)m_pRenderer->GetWidth(eSprite::GrassTile));

	m_pObjectManager = new CObjectManager; //set up the object manager 
	LoadSounds(); //load the sounds for this game

	spriteSize = (size_t)m_pRenderer->GetWidth(eSprite::GrassTile);

	m_pParticleEngine = new LParticleEngine2D(m_pRenderer);

	float x = 0.0f, y = 0.0f;
	camera = PlayerCamera();

	HWND hwnd = m_pRenderer->GetWindow();
	SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) & ~WS_SIZEBOX); // no resize!

	BeginGame();
} //Initialize

void CGame::HighlightTile() {
	POINT p = {};//  get mouse position

	GetCursorPos(&p);
	ScreenToClient(m_pRenderer->GetWindow(), &p);

	Vector2 mousePos((float)p.x - 500.0f, (float)p.y + 8.0f); //hardcode L
	//Vector2 mousePos((float)p.x - 500.0f, (float)p.y + 10.0f);
	mousePos.x += camera.GetPos().x;
	mousePos.y -= camera.GetPos().y;

	float tileSizeX = (float)(int)((float)spriteSize * 1.5f);
	float tileSizeY = (float)(int)((float)spriteSize * 0.75f);

	/// --- TODO: Change the tile size to match the new sprite size --- ///
	//float tileSizeX = (float)(int)((float)spriteSize * 0.625f);
	//float tileSizeY = (float)(int)((float)spriteSize * 0.625f);

	Vector2 selected = Vector2(
		(mousePos.y / tileSizeY) + (mousePos.x / tileSizeX),
		(mousePos.y / tileSizeY) - (mousePos.x / tileSizeX)
	);

	int selectedX = (int)selected.x;
	int selectedY = (int)selected.y;

	/// --- TODO: debug statements for tile selection mouse coordinates --- ///
	/*if (m_pKeyboard->TriggerDown(VK_LBUTTON)) {
		printf("p.x: %d p.y: %d\n", p.x, p.y);
		printf("mousePos: %1.f %1.f\n", mousePos.x, mousePos.y);
		printf("cameraPos: %1.f %1.f\n", camera.GetPos().x, camera.GetPos().y);
		printf("tileX: %d tileY: %d\n", selectedX, selectedY);
	}*/


	Tile *highlightedTile = 0;
	if (m_pTileManager->GetTile(selectedX, selectedY, &highlightedTile)) {
		if (prevHighlightedTile && prevHighlightedTile != prevSelectedTile) {
			prevHighlightedTile->tint = DEFAULT_TILE_TINT;
		}

		if (highlightedTile != prevSelectedTile) {
			highlightedTile->tint = HIGHLIGHT_TILE_TINT;
		}

		prevHighlightedTile = highlightedTile;
	}
}

void CGame::SelectTile() {
	POINT p = {};//  get mouse position

	GetCursorPos(&p);
	ScreenToClient(m_pRenderer->GetWindow(), &p);

	Vector2 mousePos((float)p.x - 500.0f, (float)p.y + 8.0f); // hardcode L
	mousePos.x += camera.GetPos().x;
	mousePos.y -= camera.GetPos().y;

	float tileSizeX = (float)(int)((float)spriteSize * 1.5f);
	float tileSizeY = (float)(int)((float)spriteSize * 0.75f);

	Vector2 selected = Vector2(
		(mousePos.y / tileSizeY) + (mousePos.x / tileSizeX),
		(mousePos.y / tileSizeY) - (mousePos.x / tileSizeX)
	);

	int selectedX = (int)selected.x;
	int selectedY = (int)selected.y;

	//spawning (delete later)
	if (/*currency >= 10*/m_pUnitManager->canSpawn) {
		Tile *selectedTile = nullptr;

		if (m_pTileManager->GetTile(selectedX, selectedY, &selectedTile)) {
			selectedTile->tint = DEFAULT_UNIT_TINT;
			if (!selectedTile->isOccupied) {    // if tile is NOT occupied

				if (m_bGodMode) {	// can spawn anywhere in god mode
					selectedTile->isOccupied = true;
					m_pUnitManager->AddUnit(selectedTile);
					printf("playerUnit created at playerUnit->tile->x: %d playerUnit->tile->y: %d\n", playerUnit->tile->x, playerUnit->tile->y);

				} else {
					if (selectedTile->y == m_pTileManager->GetHeight() - 1) {	// can only spawn on start tile in normal mode

						selectedTile->isOccupied = true;
						m_pUnitManager->AddUnit(selectedTile);
						printf("playerUnit created at playerUnit->tile->x: %d playerUnit->tile->y: %d\n", playerUnit->tile->x, playerUnit->tile->y);
					} else {
						Notification errNotification = {};
						char text[64];
						sprintf_s(text, "you can't spawn there!");
						errNotification.text = std::string(text);
						errNotification.endTime = m_pTimer->GetTime() + NOTIFICATION_DURATION;
						errNotification.startTime = m_pTimer->GetTime();
						notifications.push_back(errNotification);
					}	// else not start tile
				}	// else not god mode
			}	// if tile is NOT occupied
			tiles.push_back(selectedTile);

			units.push_back(playerUnit);
		}
		currency -= 10;
	} else {
		Notification errNotification = {};
		char text[64];
		//sprintf_s(text, "Not enough currency to buy unit!");
		sprintf_s(text, "player has already been spawned!");
		errNotification.text = std::string(text);
		errNotification.endTime = m_pTimer->GetTime() + NOTIFICATION_DURATION;
		errNotification.startTime = m_pTimer->GetTime();
		notifications.push_back(errNotification);
	}

	printf("tilemanager width: %zu tilemanager height: %zu\n", m_pTileManager->GetWidth(), m_pTileManager->GetHeight());
}

void CGame::UpdateCurrency() {
	if (currency < CURRENCY_MAX)
		currency = currency + CURRENCY_GAINED_ON_UPDATE;
	//printf("Currency: %d\n", currency);
}

void CGame::UpdateUnits() {
	std::vector<int> removeIndices = std::vector<int>();
	for (int i = 0; i < tiles.size(); i++) {
		int nextY = tiles[i]->y - 1;

		Tile *nextTile = nullptr;
		if (m_pTileManager->GetTile(tiles[i]->x, nextY, &nextTile)) {
			tiles[i]->tint = DEFAULT_TILE_TINT;
			nextTile->tint = DEFAULT_UNIT_TINT;
			tiles[i] = nextTile;
		} else {
			tiles[i]->tint = DEFAULT_TILE_TINT;
			tiles.erase(tiles.begin() + i);
			i -= 1;
		}
	}
}

void CGame::UpdateNotifications() {
	for (int i = 0; i < notifications.size(); i++) {
		Notification &notif = notifications[i];
		float currentTime = m_pTimer->GetTime();
		float progress = (currentTime - notif.startTime) / (notif.endTime - notif.startTime);

		notif.progress = progress;

		if (currentTime >= notif.endTime) {
			notifications.erase(notifications.begin() + i);
			i--;
		}
	}
}

void CGame::UpdatePlayerUnit() {
	if (playerUnit == nullptr) { return; }
	playerUnit->move();
}



/// Load the specific images needed for this game. This is where `eSprite`
/// values from `GameDefines.h` get tied to the names of sprite tags in
/// `gamesettings.xml`. Those sprite tags contain the name of the corresponding
/// image file. If the image tag or the image file are missing, then the game
/// should abort from deeper in the Engine code leaving you with an error
/// message in a dialog box.

void CGame::LoadImages() {
	m_pRenderer->BeginResourceUpload();
	m_pRenderer->Load(eSprite::GrassTile, "grasstile");
	m_pRenderer->Load(eSprite::Tile, "tile");
	m_pRenderer->Load(eSprite::Unit, "unit");
	m_pRenderer->Load(eSprite::AnnoyingOrange, "annoyingorange");
	m_pRenderer->Load(eSprite::Block, "block");
	m_pRenderer->Load(eSprite::Bridge, "bridge");
	m_pRenderer->Load(eSprite::Fence, "fence");
	m_pRenderer->Load(eSprite::Water, "water");
	m_pRenderer->Load(eSprite::Lagoon, "lagoon");
	m_pRenderer->Load(eSprite::Nature, "nature");

	m_pRenderer->Load(eSprite::Player, "player");
	m_pRenderer->Load(eSprite::Bullet, "bullet");
	m_pRenderer->Load(eSprite::Bullet2, "bullet2");
	m_pRenderer->Load(eSprite::Smoke, "smoke");
	m_pRenderer->Load(eSprite::Spark, "spark");
	m_pRenderer->Load(eSprite::Black, "black");
	m_pRenderer->Load(eSprite::Orange, "orange");
	m_pRenderer->Load(eSprite::Line, "greenline");

	m_pRenderer->EndResourceUpload();
} //LoadImages

/// Initialize the audio player and load game sounds.

void CGame::LoadSounds() {
	m_pAudio->Initialize(eSound::Size);

	m_pAudio->Load(eSound::Grunt, "grunt");
	m_pAudio->Load(eSound::Clang, "clang");
	m_pAudio->Load(eSound::Gun, "gun");
	m_pAudio->Load(eSound::Ricochet, "ricochet");
	m_pAudio->Load(eSound::Start, "start");
	m_pAudio->Load(eSound::Boom, "boom");

	m_pAudio->Load(eSound::BGM, "bgm");
} //LoadSounds

/// Release all of the DirectX12 objects by deleting the renderer.

void CGame::Release() {
	delete m_pRenderer;
	m_pRenderer = nullptr; //for safety
} //Release

/// Ask the object manager to create a player object and turrets specified by
/// the tile manager.

void CGame::CreateObjects() {
	std::vector<Vector2> turretpos; //vector of turret positions
	Vector2 playerpos; //player positions
	Vector2 cameraPos = camera.GetPos();

	m_pTileManager->GetObjects(turretpos, playerpos); //get positions

	//m_pPlayer = (CPlayer*)m_pObjectManager->create(eSprite::Player, playerpos);
} //CreateObjects

/// Call this function to start a new game. This should be re-entrant so that
/// you can restart a new game without having to shut down and restart the
/// program. Clear the particle engine to get rid of any existing particles,
/// delete any old objects out of the object manager and create some new ones.

void CGame::BeginGame() {
	m_pParticleEngine->clear(); //clear old particles

	/*switch (m_nNextLevel) {
	  case 0: m_pTileManager->LoadMap("Media\\Maps\\tiny.txt"); break;
	  case 1: m_pTileManager->LoadMap("Media\\Maps\\small.txt"); break;
	  case 2: m_pTileManager->LoadMap("Media\\Maps\\map.txt"); break;
	  case 3: m_pTileManager->LoadMapFromImageFile("Media\\Maps\\maze.png");
		break;
	} //switch*/

	//m_pTileManager->LoadMap("Media\\Maps\\small.txt");

	m_pTileManager->LoadMap("Media\\Maps\\betamap03.txt");
	m_pObjectManager->clear(); //clear old objects
	CreateObjects(); //create new objects (must be after map is loaded) 
	if (m_pTileManager->GetMap() != nullptr) {
		GMTile = m_pTileManager->GetGMSpawnPoint();
		m_pGameMaster = new GameMaster(GMTile->pos);
	}

	m_pAudio->stop(); //stop all  currently playing sounds

	/*if (sound < 1) {
		m_pAudio->play(eSound::Start); //play start-of-game sound
		sound++;
	}*/
	m_eGameState = eGameState::Playing; //now playing

	/// --- TODO: update bgm.mp3 into .wav filetype. --- ///
	m_pAudio->loop(eSound::BGM, camera.GetPos() - Vector2(1200.0f, 1200.0f)); //play background music
	m_pAudio->SetScale(0.00000000000025f); //set volume scale
} //BeginGame

void CGame::MouseHandler() {

}

/// Poll the keyboard state and respond to the key presses that happened since
/// the last frame.

void CGame::KeyboardHandler() {
	m_pKeyboard->GetState(); //get current keyboard state

	if (m_pKeyboard->TriggerDown(VK_TAB)) { //if tab key is triggered, toggle debug menu
		if (m_bDrawDebugMenu) {
			m_bDrawDebugMenu = false;
		} else {
			m_bDrawDebugMenu = true;
		}
	} // if tab key

	if (m_pKeyboard->TriggerDown('C')) {	//if 'C' key is triggered, toggle free camera
		if (m_bFreeCamera) {
			m_bFreeCamera = false;
		} else {
			m_bFreeCamera = true;
		}
	} // if 'C' key

	if (m_pKeyboard->TriggerDown(VK_RETURN)) {
		m_nNextLevel = (m_nNextLevel + 1) % 4;
		BeginGame();
	} // if enter key

	if (m_pKeyboard->TriggerDown(VK_F1)) {	//help
		ShellExecute(0, 0, "https://larc.unt.edu/code/topdown/", 0, 0, SW_SHOW);
	} // if F1 key

	if (m_pKeyboard->TriggerDown(VK_F2)) {		// toggle frame rate
		m_bDrawFrameRate = !m_bDrawFrameRate;
	} // if F2 key

	if (m_pKeyboard->TriggerDown(VK_F3)) {		// toggle AABB drawing
		m_bDrawAABBs = !m_bDrawAABBs;
	} // if F3 key

	if (m_pKeyboard->TriggerDown(VK_F4)) {		// move to next level
		m_nNextLevel = (m_nNextLevel + 1) % 3;
		BeginGame();
	} // if F4 key

	if (m_pKeyboard->TriggerDown(VK_BACK)) {	// start game
		BeginGame();
	} // if backspace key

	if (m_pKeyboard->TriggerDown('G')) {		// toggle god mode
		if (m_bGodMode) {
			m_bGodMode = false;
			if (m_pGameMaster != nullptr) {
				m_pGameMaster->SetFriendlyMode(m_bGodMode);
			}
		} else {
			m_bGodMode = true;
			if (m_pGameMaster != nullptr) {
				m_pGameMaster->SetFriendlyMode(m_bGodMode);
			}
		}
	} // if 'G' key

	if (m_pKeyboard->TriggerDown(VK_LBUTTON)) { // select tile for player spawn point
		SelectTile();
	}; //if left click

	HighlightTile();	// highlight tile under cursor on default

	Vector2 moveDirection;

	/// camera movement
	Vector2 downVector(0, 1);      //  pan down
	if (m_pKeyboard->Down('W') /* || m_pKeyboard->Down(VK_UP)*/) {
		m_bDrawCameraPrompt = false;
		moveDirection += downVector;
	}

	Vector2 rightVector(-1, 0);     //  pan right
	if (m_pKeyboard->Down('A') /* || m_pKeyboard->Down(VK_LEFT)*/) {
		m_bDrawCameraPrompt = false;
		moveDirection += rightVector;
	}

	Vector2 leftVector(1, 0);   //  pan left
	if (m_pKeyboard->Down('D') /* || m_pKeyboard->Down(VK_RIGHT)*/) {
		m_bDrawCameraPrompt = false;
		moveDirection += leftVector;
	}

	Vector2 upVector(0, -1);     //  pan up
	if (m_pKeyboard->Down('S') /* || m_pKeyboard->Down(VK_DOWN)*/) {
		m_bDrawCameraPrompt = false;
		moveDirection += upVector;
	}

	camera.MoveCamera(moveDirection * 50.f, m_pTimer->GetFrameTime());

} //KeyboardHandler

/// Poll the XBox controller state and respond to the controls there.

void CGame::ControllerHandler() {
	if (!m_pController->IsConnected())return;

	m_pController->GetState(); //get state of controller's controls 
} //ControllerHandler

void CGame::DrawNumFrames() {
	char text[64];
	sprintf_s(text, "Frame: %d", frameCount);
	m_pRenderer->DrawScreenText(text, Vector2(10.0f, 30.0f));
}

/// Draw the current frame rate to a hard-coded position in the window.
/// The frame rate will be drawn in a hard-coded position using the font
/// specified in `gamesettings.xml`.

void CGame::DrawFrameRateText() {
	const std::string s = std::to_string(m_pTimer->GetFPS()) + " fps"; //frame rate
	const Vector2 pos(m_nWinWidth - 128.0f, 30.0f); //hard-coded position
	m_pRenderer->DrawScreenText(s.c_str(), pos); //draw to screen
} //DrawFrameRateText

/// Draw the god mode text to a hard-coded position in the window using the
/// font specified in `gamesettings.xml`.

void CGame::DrawGodModeText() {
	const Vector2 pos(64.0f, 30.0f); //hard-coded position
	m_pRenderer->DrawScreenText("God Mode", pos); //draw to screen
} //DrawGodModeText


/// Draw the debug menu to the screen.
void CGame::DrawDebugMenu() {
	XMVECTORF32 redColor = { 1.0f, 0.0f, 0.0f, 1.0f };
	const Vector2 pos(64.0f, 96.0f); //hard-coded position
	m_pRenderer->DrawScreenText("Debug Menu", pos, redColor); //draw to screen
	m_pRenderer->DrawScreenText("- WASD to move camera", pos + Vector2(0.0f, 32.0f), redColor); //draw to screen
	m_pRenderer->DrawScreenText("- arrow keys to move player", pos + Vector2(0.0f, 64.0f), redColor); //draw to screen
	m_pRenderer->DrawScreenText("- 'G' for god mode.", pos + Vector2(0.0f, 96.0f), redColor); //draw to screen
} //DrawDebugMenu

/// Draw the start prompt to the screen.
void CGame::DrawStartPrompt() {
	if (playerUnit == nullptr) {
		const Vector2 pos(64.0f, 64.0f); //hard-coded position
		const Vector2 center(m_pRenderer->GetWindowWidth() / 2.0f, m_pRenderer->GetWindowHeight() / 2.0f); //center of screen
		const Vector2 pos2(256.0f, 350.0f);
		const wchar_t *text0 = L"make it to the finish line without the orange seeing u.";
		const wchar_t *text1 = L"designate any tile in the first row as your spawn point.";
		XMVECTORF32 orangeColor = { 1.0f, 0.5f, 0.25f, 1.0f };
		//m_pRenderer->DrawScreenText("make it to the finish line without the orange seeing u.", pos + Vector2(0.0f, 20.0f), orangeColor);
		//m_pRenderer->DrawScreenText("designate any tile in the first row as your spawn point.", pos, orangeColor); //draw to screen

		m_pRenderer->DrawScaledText(text0, Vector2((m_pRenderer->GetWindowWidth() - wcslen(text0)) / 5.25f, 350.0f), 0.875f, orangeColor);
		m_pRenderer->DrawScaledText(text1, Vector2((m_pRenderer->GetWindowWidth() - wcslen(text0)) / 5.25f, 375.0f), 0.875f, orangeColor);

	}

} //DrawStartPrompt

/// Draw the camera prompt to the screen.
void CGame::DrawCameraPrompt() {
	const Vector2 pos(650.0f, 730.0f); //hard-coded position
	XMVECTORF32 orangeColor = { 1.0f, 0.5f, 0.0f, 1.0f };
	XMVECTORF32 red = { 1.0f, 0.0f, 0.0f, 1.0f };
	const wchar_t *text = L"WASD to pan the camera around.";

	m_pRenderer->DrawScaledText(text, pos, 0.75f, red); //draw to screen
} //DrawCameraPrompt

/// Draw the sprites to the screen.

void CGame::DrawSprites() {
	for (int i = 0; i < z_layers; i++) {
		for (int j = 0; j < m_pTileManager->GetHeight(); j++) {
			for (int k = 0; k < m_pTileManager->GetWidth(); k++) {
				Tile *tile;
				if (m_pTileManager->GetTile(k, j, &tile)) {
					m_pRenderer->Draw(&tile->desc);

					if (playerUnit != nullptr) {
						if (playerUnit->tile->x == k && playerUnit->tile->y == j) {
							m_pUnitManager->Draw();
						}	// if player unit is on this tile
					}	// if player unit isn't nullptr
				}	// if valid tile
			}	// for k
		}	// for j
	}	// for i
} //DrawSprites

/// Move player unit according to user input.

void CGame::ProcessPlayerInput(const WPARAM k) {
	int dest_x = playerUnit->tile->x;		// destination coordinates start at player's current position
	int dest_y = playerUnit->tile->y;

	Tile *destTile = nullptr;		// destination tile to navigate to + update playerTile to
	printf("processing player input\n");

	switch (k) {
		case VK_LEFT: {		// move left one tile
			dest_x -= 1;
			break;
		}
		case VK_RIGHT: {	// move right one tile
			dest_x += 1;
			break;
		}
		case VK_UP: {		// move up one tile
			dest_y -= 1;
			break;
		}
		case VK_DOWN: {		// move down one tile
			dest_y += 1;
			break;
		}
	}	// switch

	/// --- check if movement is possible before moving the player unit --- ///
	if (m_pTileManager->GetTile(dest_x, dest_y, &destTile)) {

		playerUnit->is_stationary = false;
		playerUnit->lerpInfo.source = playerUnit->desc.m_vPos;

		if (destTile->isWalkable) {
			playerUnit->lerpInfo.target = destTile->pos;

			if (destTile->viewableByGameMaster) {
				if (!m_bGodMode) {		//game over
					m_bDrawGameOver = true;
					m_pUnitManager->m_vecUnits.clear();
				}	// if not in god mode
				else {
					playerUnit->lerpInfo.currDuration = 0;
					playerUnit->moveTo(destTile, k);
					printf("playerUnit->tile->x: %d playerUnit->tile->y: %d\n", playerUnit->tile->x, playerUnit->tile->y);
					playerUnit->tile = destTile;

					if (destTile->isWinTile) {
						m_bDrawWin = true;
					}	// if win tile
				}	// else in god mode
			}	// if viewable by game master
			else {
				playerUnit->lerpInfo.currDuration = 0;
				playerUnit->moveTo(destTile, k);
				printf("playerUnit->tile->x: %d playerUnit->tile->y: %d\n", playerUnit->tile->x, playerUnit->tile->y);
				playerUnit->tile = destTile;

				if (destTile->isWinTile) {
					m_bDrawWin = true;
				}	// if win tile
			}	// else not viewable by game master
		}	// if walkable
	}	// if valid destination tile

	currDelay = WALK_DURATION;
}

/// Detect the player input and either process or push to input buffer.

void CGame::DetectPlayerInput() {
	if (playerUnit == nullptr) {
		return;
	}

	if (m_pKeyboard->TriggerDown(VK_LEFT)) {	// process player input for left key
		printf("left key detected");
		if (currDelay <= 0.f) {
			printf("; now processing\n");
			ProcessPlayerInput(VK_LEFT);
			currDelay = WALK_DURATION;

		} else if (inputBuffer.size() < MAX_INPUTS_BUFFERED) {
			printf("; now buffering\n");
			inputBuffer.push(VK_LEFT);
		}
	}
	if (m_pKeyboard->TriggerDown(VK_RIGHT)) {	// process player input for right key
		printf("right key detected");
		if (currDelay <= 0.f) {
			printf("; now processing\n");
			ProcessPlayerInput(VK_RIGHT);
			currDelay = WALK_DURATION;
		} else if (inputBuffer.size() < MAX_INPUTS_BUFFERED) {
			printf("; now buffering\n");
			inputBuffer.push(VK_RIGHT);
		}
	}
	if (m_pKeyboard->TriggerDown(VK_UP)) {		// process player input for up key
		printf("up key detected");
		if (currDelay <= 0.f) {
			printf("; now processing\n");
			ProcessPlayerInput(VK_UP);
			currDelay = WALK_DURATION;
		} else if (inputBuffer.size() < MAX_INPUTS_BUFFERED) {
			printf("; now buffering\n");
			inputBuffer.push(VK_UP);
		}
	}
	if (m_pKeyboard->TriggerDown(VK_DOWN)) {	// process player input for down key
		printf("down key detected");
		if (currDelay <= 0.f) {
			printf("; now processing\n");
			ProcessPlayerInput(VK_DOWN);
			currDelay = WALK_DURATION;
		} else if (inputBuffer.size() < MAX_INPUTS_BUFFERED) {
			printf("; now buffering\n");
			inputBuffer.push(VK_DOWN);
		}
	}

}

/// Ask the object manager to draw the game objects. The renderer is notified of
/// the start and end of the frame so that it can let Direct3D do its
/// pipelining jiggery-pokery.

void CGame::RenderFrame() {

	m_pRenderer->BeginFrame(); //required before rendering

	/// get window size for zoom
	RECT windowRect;

	//m_pTileManager->Draw(eSprite::GrassTile); //draw tiles
	m_pObjectManager->draw(); //draw objects
	DrawSprites(); //draw sprites
	m_pGameMaster->draw();
	//m_pUnitManager->Draw(); //draw units

	m_pParticleEngine->Draw(); //draw particles

	//draw currency
	char text[256];
	//sprintf_s(text, "Currency: %d", currency);
	sprintf_s(text, "u will die if u don't make it past this ornage. don't let him see u.");
	m_pRenderer->DrawScreenText(text, Vector2(10.0f, 10.0f), XMVECTORF32({ 1.f, 0.843137324f, 0.f, 1.f }));
	if (m_bDrawFrameRate)DrawFrameRateText(); //draw frame rate, if required
	if (m_bGodMode)DrawGodModeText(); //draw god mode text, if required
	if (m_bDrawDebugMenu)DrawDebugMenu(); //draw debug menu, if required
	if (m_bDrawCameraPrompt)DrawCameraPrompt(); //draw camera prompt at start
	DrawStartPrompt(); //draw start prompt

	//DrawNumFrames(); //draw frame number

	//draw notifications
	for (int i = 0; i < notifications.size(); i++) {
		Notification &notif = notifications[i];

		float progress = notif.progress;
		Vector2 startPos = Vector2(10.0f, 600.0f);
		Vector2 endPos = Vector2(10.0f, 450.0f);

		Vector2 currentPos = Math::lerp(startPos, endPos, progress);
		float alpha = Math::lerp(1.0f, 0.0f, progress);

		m_pRenderer->DrawScreenText(
			notif.text.c_str(),
			currentPos,
			XMVECTORF32({ 1.f, 0.843137324f, 0.f, alpha })
		);
	}

	if (m_bDrawGameOver) {
		m_pRenderer->DrawGameOver();
	}
	if (m_bDrawWin) {
		m_pRenderer->DrawWin();
	}
	m_pRenderer->EndFrame(); //required after rendering
} //RenderFrame

/// Make the camera follow the player, but don't let it get too close to the
/// edge unless the world is smaller than the window, in which case we just
/// center everything.

void CGame::FollowCamera() {
	Vector3 newPos(camera.GetPos());
	m_pRenderer->SetCameraPos(newPos); //camera to player
} //FollowCamera

/// This function will be called regularly to process and render a frame
/// of animation, which involves the following. Handle keyboard input.
/// Notify the audio player at the start of each frame so that it can prevent
/// multiple copies of a sound from starting on the same frame.  
/// Move the game objects. Render a frame of animation. 

static float accumulatorOfTime = 0.0f;

void CGame::ProcessFrame() {
	KeyboardHandler(); //handle keyboard input
	ControllerHandler(); //handle controller input
	m_pAudio->BeginFrame(); //notify audio player that frame has begun

	m_pTimer->Tick([&]() { //all time-dependent function calls should go here
		frameCount++;
		FollowCamera(); //make camera follow player
		m_pObjectManager->move(); //move all objects
		m_pParticleEngine->step(); //advance particle animation

		accumulatorOfTime += m_pTimer->GetFrameTime();
		while (accumulatorOfTime >= 1.0f / 60.0f) {
			//update units
			if (frameCount % (int)(60.0f * UPDATE_UNITS_IN_SECONDS) == 0) {
				UpdateUnits();
			}
			//update currency
			if (frameCount % (int)(60.0f * UPDATE_CURRENCY_IN_SECONDS) == 0) {
				UpdateCurrency();
			}

			accumulatorOfTime -= 1.0f / 60.0f;
		}

		/// --- process player input --- ///
		currDelay = currDelay - m_pTimer->GetFrameTime();
		currDelay = (std::max)(currDelay, 0.0f);
		if (playerUnit != nullptr) {

			if (currDelay <= 0.0f && !inputBuffer.empty()) {
				ProcessPlayerInput(inputBuffer.front());
				inputBuffer.pop();
			}

			DetectPlayerInput();

			if (!playerUnit->is_stationary) {
				playerUnit->move();
			}

			int nextPhase = Math::RandomizePhase(2, currDelay);

			if (frameCount % (int)(60.0f * 1.5f) == 0) {
				m_pGameMaster->SwitchPhases(nextPhase);
			} else {
				m_pGameMaster->desc.m_nCurrentFrame = m_pGameMaster->m_nCurrentFrame;
			}
		}

		//update notifications
		UpdateNotifications();
		});		// m_pTimer->Tick
	if (m_bDrawGameOver) {
		if (m_pKeyboard->TriggerDown(VK_RETURN)) {
			BeginGame();
			m_eGameState = eGameState::Playing;
			m_bDrawGameOver = false;
		}
	}

	if (m_bDrawWin) {
		if (m_pKeyboard->TriggerDown(VK_RETURN)) {
			m_pUnitManager->m_vecUnits.clear();
			BeginGame();
			m_eGameState = eGameState::Playing;
			m_bDrawWin = false;
		}
	}
	RenderFrame(); //render a frame of animation
	ProcessGameState(); //check for end of game
} //ProcessFrame

void CGame::GameOver() {
	XMVECTORF32 black = { 0.0f, 0.0f, 0.0f, 1.0f };
	XMVECTORF32 white = { 1.0f, 1.0f, 1.0f, 1.0f };
	while (!m_pKeyboard->TriggerDown(VK_RETURN)) {
		BeginGame();
		m_eGameState = eGameState::Playing;
		m_pRenderer->SetBgColor(black); //black background
	}

}

/// Take action appropriate to the current game state. If the game is currently
/// playing, then if the player has been killed or all turrets have been
/// killed, then enter the wait state. If the game has been in the wait
/// state for longer than 3 seconds, then restart the game.

void CGame::ProcessGameState() {
	static float t = 0; //time at start of game

	switch (m_eGameState) {
		case eGameState::Playing:
			break;

		case eGameState::Waiting:
			break;

		case eGameState::GameOver: {
			//GameOver();
			break;
		}

		case eGameState::GameWon:
			break;
	} //switch
} //CheckForEndOfGame