#pragma once
#include "Defines.h"
#include "SpriteRenderer.h"
#include "Math.h"

class PlayerCamera
{
	public:
		//	TODO: set orthographic maybe.
		
		Vector2 GetPos();				///	get current position
		void SetPos(Vector2 newPos);	///	set new position
		Vector2 GetBasePos();
		void SetBasePos(Vector2 newBasePos);

		void MoveCamera(Vector2 moveDirection, float deltaTime);
		
		void SetCursorPos(POINT newCursorPos);
	private:
		Vector2 position;
		Vector2 basePos;
		POINT gameCursor;
		bool gameCursorInBounds = false;
		Vector2 currentVelocity;
		Vector2 currentAcceleration;
};

