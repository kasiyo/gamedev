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
  
  m_pTileManager = new CTileManager((size_t)m_pRenderer->GetWidth(eSprite::Tile)* 0.55f);
  m_pObjectManager = new CObjectManager; //set up the object manager 
  LoadSounds(); //load the sounds for this game

  m_pParticleEngine = new LParticleEngine2D(m_pRenderer);

  camera = PlayerCamera();

  BeginGame();
} //Initialize

/// Load the specific images needed for this game. This is where `eSprite`
/// values from `GameDefines.h` get tied to the names of sprite tags in
/// `gamesettings.xml`. Those sprite tags contain the name of the corresponding
/// image file. If the image tag or the image file are missing, then the game
/// should abort from deeper in the Engine code leaving you with an error
/// message in a dialog box.

void CGame::LoadImages(){  
  m_pRenderer->BeginResourceUpload();

  m_pRenderer->Load(eSprite::Tile,    "tile"); 
  m_pRenderer->Load(eSprite::Player,  "player");
  m_pRenderer->Load(eSprite::Bullet,  "bullet");
  m_pRenderer->Load(eSprite::Bullet2, "bullet2");
  m_pRenderer->Load(eSprite::Smoke,   "smoke");
  m_pRenderer->Load(eSprite::Spark,   "spark");
  m_pRenderer->Load(eSprite::Turret,  "turret");
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
  camera.SetPos(basePos);   // start camera at center of map

  m_pTileManager->GetObjects(turretpos, cameraPos); //get positions

  printf("camera view vector (x, y, z): %1.f %1.f, %1.f\n", m_pRenderer->GetCamera()->GetViewVector().x,
      m_pRenderer->GetCamera()->GetViewVector().y, m_pRenderer->GetCamera()->GetViewVector().z);
  
  //m_pPlayer = (CPlayer*)m_pObjectManager->create(eSprite::Player, playerpos);



  for(const Vector2& pos: turretpos)
    m_pObjectManager->create(eSprite::Turret, pos);
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

  m_pTileManager->LoadMap("Media\\Maps\\small.txt");
  //m_pTileManager->LoadMap("Media\\Maps\\basefloor.txt");
  m_pObjectManager->clear(); //clear old objects
  CreateObjects(); //create new objects (must be after map is loaded) 
  m_pAudio->stop(); //stop all  currently playing sounds
  m_pAudio->play(eSound::Start); //play start-of-game sound
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
  if (GetWindowRect(m_pRenderer->GetWindow(), &windowRect)) {
      //printf("%ld %ld %ld %ld\n", windowRect.top, windowRect.bottom, windowRect.left, windowRect.right);
      m_pRenderer->GetCamera()->SetOrthographic(
          std::abs(windowRect.right - windowRect.left),
          std::abs(windowRect.top - windowRect.bottom), 0.25f, 45.0f);  // changing values from 0.1 to 0.25f
  }                                                                     // and 100.0 to 45.0f works better after tile resizing
                                                                        // for some reason...
  m_pObjectManager->draw(); //draw objects
  m_pParticleEngine->Draw(); //draw particles
  if(m_bDrawFrameRate)DrawFrameRateText(); //draw frame rate, if required
  if(m_bGodMode)DrawGodModeText(); //draw god mode text, if required

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

void CGame::ProcessFrame(){
  KeyboardHandler(); //handle keyboard input
  ControllerHandler(); //handle controller input
  m_pAudio->BeginFrame(); //notify audio player that frame has begun
  
  m_pTimer->Tick([&](){ //all time-dependent function calls should go here
    m_pObjectManager->move(); //move all objects
    FollowCamera(); //make camera follow player
    m_pParticleEngine->step(); //advance particle animation
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
      if(/*m_pPlayer == nullptr || */ m_pObjectManager->GetNumTurrets() == 0) {
        m_eGameState = eGameState::Waiting; //now waiting
        t = m_pTimer->GetTime(); //start wait timer
      } //if
      break;

    case eGameState::Waiting:
      if(m_pTimer->GetTime() - t > 3.0f){ //3 seconds has elapsed since level end
        if(m_pObjectManager->GetNumTurrets() == 0) //player won
          m_nNextLevel = (m_nNextLevel + 1)%4; //advance next level
        BeginGame(); //restart game
      } //if
      break;
  } //switch
} //CheckForEndOfGame