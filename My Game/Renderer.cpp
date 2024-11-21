/// \file Renderer.cpp
/// \brief Code for the renderer class CRenderer.

#include "Renderer.h"
#include "ComponentIncludes.h"
#include "Abort.h"

CRenderer::CRenderer():
  LSpriteRenderer(eSpriteMode::Batched2D){
} //constructor

/// Load the specific images needed for this game.
/// This is where eSpriteType values from GameDefines.h get
/// tied to the names of sprite tags in gamesettings.xml. Those
/// sprite tags contain the name of the corresponding image file.
/// If the image tag or the image file are missing, then
/// the game should abort from deeper in the Engine code,
/// leaving you with a dialog box that tells you what
/// went wrong.

void CRenderer::LoadImages(){  
  BeginResourceUpload();

  Load(eSprite::Background, "background");  


  EndResourceUpload();
} //LoadImages

HWND CRenderer::GetWindow() {
	return m_Hwnd;
}

LBaseCamera* CRenderer::GetCamera() {
	return this->m_pCamera;
}

int CRenderer::GetWindowWidth() {
	return m_nWinWidth;
}

int CRenderer::GetWindowHeight() {
	return m_nWinHeight;
}

float CRenderer::GetWindowScaleFromBase() {
	return (float)m_nWinWidth / 1024.0f;
}

void LoadScreenFont(char* c) {
	/// --- TODO: Add code to load a font for the load screen. --- ///
	LoadScreenFont(c);
	//return nullptr;
}