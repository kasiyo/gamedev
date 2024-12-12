/// \file Renderer.cpp
/// \brief Code for the renderer class CRenderer.

#include "Renderer.h"
#include "ComponentIncludes.h"
#include "Abort.h"

CRenderer::CRenderer() :
	LSpriteRenderer(eSpriteMode::Batched2D) {} //constructor

/// Load the specific images needed for this game.
/// This is where eSpriteType values from GameDefines.h get
/// tied to the names of sprite tags in gamesettings.xml. Those
/// sprite tags contain the name of the corresponding image file.
/// If the image tag or the image file are missing, then
/// the game should abort from deeper in the Engine code,
/// leaving you with a dialog box that tells you what
/// went wrong.

void CRenderer::LoadImages() {
	BeginResourceUpload();

	//Load(eSprite::Background, "background");
	Load(eSprite::Black, "black");
	Load(eSprite::Orange, "orange");

	EndResourceUpload();
} //LoadImages

HWND CRenderer::GetWindow() {
	return m_Hwnd;
}

LBaseCamera *CRenderer::GetCamera() {
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

/// Calculate isometric projection.
void CRenderer::CalculateIso(int i, int j, float scale, float descSize, Vector2 &v) {
	const float diagonalShift = 0.5f;
	const float isoXScale = 1.5f;
	const float isoYScale = 0.75f;
	const float offset = 0.25f;

	float isoX = ((j - i) * diagonalShift) * (descSize * isoXScale) - (descSize * offset);
	float isoY = ((j + i) * diagonalShift) * (descSize * isoYScale) + (descSize * offset);

	v.x = isoX * scale;
	v.y = (m_nWinHeight / 2) + (-isoY * scale);
}

/// Draw game over.
void CRenderer::DrawGameOver() {
	XMVECTORF32 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	XMVECTORF32 black = { 0.0f, 0.0f, 0.0f, 1.0f };
	XMVECTORF32 white = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vector2 bottomLeft(25.0f, 700.0f);

	Vector2 camPos(this->m_pCamera->GetPos().x, this->m_pCamera->GetPos().y);
	Draw(eSprite::Black, Vector2(this->m_pCamera->GetPos().x, this->m_pCamera->GetPos().y));

	const wchar_t *text = L"Game Over";
	if (m_pFont == nullptr) {
		return;
	}

	DrawCenteredText("loser.", white);
	DrawScreenText("press enter to replay", bottomLeft, white);
	//DrawScreenText("Game Over", 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
}

/// Draw win.
void CRenderer::DrawWin() {
	XMVECTORF32 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	XMVECTORF32 black = { 0.0f, 0.0f, 0.0f, 1.0f };
	XMVECTORF32 white = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vector2 bottomLeft(25.0f, 700.0f);

	Vector2 camPos(this->m_pCamera->GetPos().x, this->m_pCamera->GetPos().y);
	Draw(eSprite::Orange, Vector2(this->m_pCamera->GetPos().x, this->m_pCamera->GetPos().y));

	const wchar_t *text = L"Game Over";
	if (m_pFont == nullptr) {
		return;
	}

	DrawCenteredText("u win.", black);
	DrawScreenText("press enter to replay", bottomLeft, white);
	//DrawScreenText("Game Over", 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
}

void CRenderer::LoadTextSprites() {
	BeginResourceUpload();
	//Load(eSprite::Text, "text");

	EndResourceUpload();
}

/*void LoadScreenFont(const char* c) {
	/// --- TODO: Add code to load a font for the load screen. --- ///
	LoadScreenFont(c);
	//return nullptr;
}*/

void CRenderer::DrawSpriteOrder() {
	Tile *t = nullptr;

}