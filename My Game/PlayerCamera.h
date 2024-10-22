#pragma once
#include "Defines.h"
#include "SpriteRenderer.h"


class PlayerCamera
{
	public:
		//LBaseCamera::GetPos(); //do get and set position.
		//LBaseCamera::SetOrthographic(float width, float ht, float nearz, float farz);
		
		Vector2 GetPos();				///	get current position
		void SetPos(Vector2 newPos);	///	set new position
		
		LBaseCamera* GetCamera();
		LBaseCamera* MoveToCursor();
		HWND GetWindow();

	private:
		Vector2 position;
		POINT gameCursor;
		bool gameCursorInBounds = false;
};

