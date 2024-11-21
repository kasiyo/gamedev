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


CGame::~CGame(){
  delete m_pParticleEngine;
  delete m_pObjectManager;
  delete m_pTileManager;
} //destructor

/// Initialize the renderer, the tile manager and the object manager, load 
/// images and sounds, and begin the game.

void CGame::Initialize(){
  m_pRenderer = new CRenderer();
  m_pRenderer->Initialize(eSprite::Size); 
  LoadImages(); //load images from xml file list
  
  m_pTileManager = new CTileManager((size_t)m_pRenderer->GetWidth(eSprite::GrassTile) * SPRITE_SCALE);
  
  /// --- TODO: Change the tile size to match the new sprite size --- ///
  //m_pTileManager = new CTileManager((size_t)m_pRenderer->GetWidth(eSprite::GrassTile));

  m_pObjectManager = new CObjectManager; //set up the object manager 
  LoadSounds(); //load the sounds for this game

  spriteSize = m_pRenderer->GetWidth(eSprite::GrassTile);

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
    

    Tile* highlightedTile = 0;
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

	/*Tile* selectedTile = 0;
    if (m_pTileManager->GetTile(selectedX, selectedY, &selectedTile)) {
        if (prevSelectedTile != nullptr) {
            prevSelectedTile->tint = DEFAULT_TILE_TINT;
        }
		selectedTile->tint = SELECTED_TILE_TINT;
		prevSelectedTile = selectedTile;
    }
    else {
		if (prevSelectedTile != nullptr) {
			prevSelectedTile->tint = DEFAULT_TILE_TINT;
			prevSelectedTile = nullptr;
		}
    }*/

    //spawning (delete later)
    if (currency >= 10) {
        Tile* selectedTile = nullptr;
        if (m_pTileManager->GetTile(selectedX, selectedY, &selectedTile)) {
            selectedTile->tint = DEFAULT_UNIT_TINT;
            units.push_back(selectedTile);
        }
        currency -= 10;
    }
    else {
        Notification errNotification = {};
        char text[64];
        sprintf_s(text, "Not enough currency to buy unit!");
        errNotification.text = std::string(text);
        errNotification.endTime = m_pTimer->GetTime() + NOTIFICATION_DURATION;
        errNotification.startTime = m_pTimer->GetTime();
        notifications.push_back(errNotification);
    }
	
}

void CGame::UpdateCurrency() {
    if (currency < CURRENCY_MAX)
        currency = currency + CURRENCY_GAINED_ON_UPDATE;
    printf("Currency: %d\n", currency);
}

void CGame::UpdateUnits() {
    std::vector<int> removeIndices = std::vector<int>();
    for (int i = 0; i < units.size(); i++) {
        int nextY = units[i]->y - 1;

        Tile* nextTile = nullptr;
        if (m_pTileManager->GetTile(units[i]->x, nextY, &nextTile)) {
            units[i]->tint = DEFAULT_TILE_TINT;
            nextTile->tint = DEFAULT_UNIT_TINT;
            units[i] = nextTile;
        }
        else {
            units[i]->tint = DEFAULT_TILE_TINT;
            units.erase(units.begin() + i);
            i -= 1;
        }
    }
}

void CGame::UpdateNotifications() {
    for (int i = 0; i < notifications.size(); i++) {
        Notification& notif = notifications[i];
        float currentTime = m_pTimer->GetTime();
        float progress = (currentTime - notif.startTime) / (notif.endTime - notif.startTime);

        notif.progress = progress;

        if (currentTime >= notif.endTime) {
            notifications.erase(notifications.begin() + i);
            i--;
        }
    }
}

/// Load the specific images needed for this game. This is where `eSprite`
/// values from `GameDefines.h` get tied to the names of sprite tags in
/// `gamesettings.xml`. Those sprite tags contain the name of the corresponding
/// image file. If the image tag or the image file are missing, then the game
/// should abort from deeper in the Engine code leaving you with an error
/// message in a dialog box.

void CGame::LoadImages(){  
  m_pRenderer->BeginResourceUpload();
  m_pRenderer->Load(eSprite::GrassTile, "grasstile");
  m_pRenderer->Load(eSprite::Appliance, "appliance");
  m_pRenderer->Load(eSprite::Tile, "tile");
  m_pRenderer->Load(eSprite::Unit, "unit");
  m_pRenderer->Load(eSprite::Player,  "player");
  m_pRenderer->Load(eSprite::Bullet,  "bullet");
  m_pRenderer->Load(eSprite::Bullet2, "bullet2");
  m_pRenderer->Load(eSprite::Smoke,   "smoke");
  m_pRenderer->Load(eSprite::Spark,   "spark");
  
  m_pRenderer->Load(eSprite::Line,    "greenline"); 

  m_pRenderer->EndResourceUpload();
} //LoadImages

/// Initialize the audio player and load game sounds.

void CGame::LoadSounds(){
  m_pAudio->Initialize(eSound::Size);

  m_pAudio->Load(eSound::Grunt, "grunt");
  m_pAudio->Load(eSound::Clang, "clang");
  m_pAudio->Load(eSound::Gun, "gun");
  m_pAudio->Load(eSound::Ricochet, "ricochet");
  m_pAudio->Load(eSound::Start, "start");
  m_pAudio->Load(eSound::Boom, "boom");
} //LoadSounds

/// Release all of the DirectX12 objects by deleting the renderer.

void CGame::Release(){
  delete m_pRenderer;
  m_pRenderer = nullptr; //for safety
} //Release

/// Ask the object manager to create a player object and turrets specified by
/// the tile manager.

void CGame::CreateObjects(){
  std::vector<Vector2> turretpos; //vector of turret positions
  Vector2 playerpos; //player positions
  Vector2 cameraPos = camera.GetPos();
  Vector2 basePos((m_nWinWidth / 4), ((m_nWinHeight / 4) + 50));

  m_pTileManager->GetObjects(turretpos, playerpos); //get positions
  
  //m_pPlayer = (CPlayer*)m_pObjectManager->create(eSprite::Player, playerpos);

} //CreateObjects

/// Call this function to start a new game. This should be re-entrant so that
/// you can restart a new game without having to shut down and restart the
/// program. Clear the particle engine to get rid of any existing particles,
/// delete any old objects out of the object manager and create some new ones.

void CGame::BeginGame(){  
  m_pParticleEngine->clear(); //clear old particles
  
  /*switch (m_nNextLevel) {
    case 0: m_pTileManager->LoadMap("Media\\Maps\\tiny.txt"); break;
    case 1: m_pTileManager->LoadMap("Media\\Maps\\small.txt"); break;
    case 2: m_pTileManager->LoadMap("Media\\Maps\\map.txt"); break;
    case 3: m_pTileManager->LoadMapFromImageFile("Media\\Maps\\maze.png");
      break;
  } //switch*/

  //m_pTileManager->LoadMap("Media\\Maps\\small.txt");
  //m_pTileManager->LoadMap("Media\\Maps\\basefloor.txt");
  //m_pTileManager->LoadMap("Media\\Maps\\bwfloor.txt");
  m_pTileManager->LoadMap("Media\\Maps\\tilefloor.txt");
  m_pObjectManager->clear(); //clear old objects
  CreateObjects(); //create new objects (must be after map is loaded) 

  
  m_pAudio->stop(); //stop all  currently playing sounds
  if (sound < 1) {
	  m_pAudio->play(eSound::Start); //play start-of-game sound
	  sound++;
  }
  m_eGameState = eGameState::Playing; //now playing
} //BeginGame

void CGame::MouseHandler() {

}

/// Poll the keyboard state and respond to the key presses that happened since
/// the last frame.

void CGame::KeyboardHandler(){
  m_pKeyboard->GetState(); //get current keyboard state
  
  if (m_pKeyboard->TriggerDown(VK_RETURN)) {
      m_nNextLevel = (m_nNextLevel + 1) % 4;
      BeginGame();
  } //if

  if(m_pKeyboard->TriggerDown(VK_F1)) //help
    ShellExecute(0, 0, "https://larc.unt.edu/code/topdown/", 0, 0, SW_SHOW);
  
  if(m_pKeyboard->TriggerDown(VK_F2)) //toggle frame rate
    m_bDrawFrameRate = !m_bDrawFrameRate;
  
  if(m_pKeyboard->TriggerDown(VK_F3)) //toggle AABB drawing
    m_bDrawAABBs = !m_bDrawAABBs; 

  if (m_pKeyboard->TriggerDown(VK_F4)) // move to next level
  {
      m_nNextLevel = (m_nNextLevel + 1) % 3;
      BeginGame();
  }
  if(m_pKeyboard->TriggerDown(VK_BACK)) //start game
    BeginGame();

  if (m_pKeyboard->TriggerDown(VK_LBUTTON)) { //left click
      SelectTile();
  };

  HighlightTile();

  Vector2 moveDirection;


    /// camera movement
  Vector2 downVector(0, 1);      //  pan down
  if (m_pKeyboard->Down('W') || m_pKeyboard->Down(VK_UP)) {
      moveDirection += downVector;
  }

  Vector2 rightVector(-1, 0);     //  pan right
  if (m_pKeyboard->Down('A') || m_pKeyboard->Down(VK_LEFT)) {
      moveDirection += rightVector;
  }

  Vector2 leftVector(1, 0);   //  pan left
  if (m_pKeyboard->Down('D') || m_pKeyboard->Down(VK_RIGHT)) {
      moveDirection += leftVector;
  }

  Vector2 upVector(0, -1);     //  pan up
  if (m_pKeyboard->Down('S') || m_pKeyboard->Down(VK_DOWN)) {
      moveDirection += upVector;
  }

  camera.MoveCamera(moveDirection * 50.f, m_pTimer->GetFrameTime());

  //if (m_pKeyboard->Down('Q'))


} //KeyboardHandler

/// Poll the XBox controller state and respond to the controls there.

void CGame::ControllerHandler(){
  if(!m_pController->IsConnected())return;

  m_pController->GetState(); //get state of controller's controls 
  
  /*if (m_pPlayer) { //safety
    m_pPlayer->SetSpeed(100*m_pController->GetRTrigger());
    m_pPlayer->SetRotSpeed(-2.0f*m_pController->GetRThumb().x);

    if(m_pController->GetButtonRSToggle()) //fire gun
      m_pObjectManager->FireGun(m_pPlayer, eSprite::Bullet);

    if(m_pController->GetDPadRight()) //strafe right
      m_pPlayer->StrafeRight();
  
    if(m_pController->GetDPadLeft()) //strafe left
      m_pPlayer->StrafeLeft();

    if(m_pController->GetDPadDown()) //strafe back
      m_pPlayer->StrafeBack();
  } //if*/
} //ControllerHandler

/// Draw the current frame rate to a hard-coded position in the window.
/// The frame rate will be drawn in a hard-coded position using the font
/// specified in `gamesettings.xml`.

void CGame::DrawFrameRateText(){
  const std::string s = std::to_string(m_pTimer->GetFPS()) + " fps"; //frame rate
  const Vector2 pos(m_nWinWidth - 128.0f, 30.0f); //hard-coded position
  m_pRenderer->DrawScreenText(s.c_str(), pos); //draw to screen
} //DrawFrameRateText

/// Draw the god mode text to a hard-coded position in the window using the
/// font specified in `gamesettings.xml`.

void CGame::DrawGodModeText(){
  const Vector2 pos(64.0f, 30.0f); //hard-coded position
  m_pRenderer->DrawScreenText("God Mode", pos); //draw to screen
} //DrawGodModeText

/// Ask the object manager to draw the game objects. The renderer is notified of
/// the start and end of the frame so that it can let Direct3D do its
/// pipelining jiggery-pokery.

void CGame::RenderFrame(){

  m_pRenderer->BeginFrame(); //required before rendering

  /// get window size for zoom
  RECT windowRect;
  /*if (GetWindowRect(m_pRenderer->GetWindow(), &windowRect)) {
      //printf("%ld %ld %ld %ld\n", windowRect.top, windowRect.bottom, windowRect.left, windowRect.right);
      m_pRenderer->GetCamera()->SetOrthographic(
          std::abs(windowRect.right - windowRect.left),
          std::abs(windowRect.top - windowRect.bottom), 0.25f, 45.0f);  // changing values from 0.1 to 0.25f
  }                                                                     // and 100.0 to 45.0f works better after tile resizing
                                                                        // for some reason...
  */
  m_pTileManager->Draw(eSprite::GrassTile); //draw tiles
  m_pObjectManager->draw(); //draw objects
  m_pParticleEngine->Draw(); //draw particles

  //draw currency
  char text[64];
  sprintf_s(text, "Currency: %d", currency);
  m_pRenderer->DrawScreenText(text, Vector2(10.0f, 10.0f), XMVECTORF32({ 1.f, 0.843137324f, 0.f, 1.f }));
  if (m_bDrawFrameRate)DrawFrameRateText(); //draw frame rate, if required
  if (m_bGodMode)DrawGodModeText(); //draw god mode text, if required

  //draw notifications
  for (int i = 0; i < notifications.size(); i++) {
      Notification& notif = notifications[i];

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

  m_pRenderer->EndFrame(); //required after rendering
} //RenderFrame

/// Make the camera follow the player, but don't let it get too close to the
/// edge unless the world is smaller than the window, in which case we just
/// center everything.

void CGame::FollowCamera(){
  Vector3 newPos(camera.GetPos());
  m_pRenderer->SetCameraPos(newPos); //camera to player

} //FollowCamera

/// This function will be called regularly to process and render a frame
/// of animation, which involves the following. Handle keyboard input.
/// Notify the audio player at the start of each frame so that it can prevent
/// multiple copies of a sound from starting on the same frame.  
/// Move the game objects. Render a frame of animation. 

static float accumulatorOfTime = 0.0f;

void CGame::ProcessFrame(){
  KeyboardHandler(); //handle keyboard input
  ControllerHandler(); //handle controller input
  m_pAudio->BeginFrame(); //notify audio player that frame has begun
  
  

  m_pTimer->Tick([&](){ //all time-dependent function calls should go here
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

    //update notifications
    UpdateNotifications();
  });

  RenderFrame(); //render a frame of animation
  ProcessGameState(); //check for end of game
} //ProcessFrame

/// Take action appropriate to the current game state. If the game is currently
/// playing, then if the player has been killed or all turrets have been
/// killed, then enter the wait state. If the game has been in the wait
/// state for longer than 3 seconds, then restart the game.

void CGame::ProcessGameState(){
  static float t = 0; //time at start of game

  switch(m_eGameState){
    case eGameState::Playing:
      break;

    case eGameState::Waiting:
      break;
  } //switch
} //CheckForEndOfGame