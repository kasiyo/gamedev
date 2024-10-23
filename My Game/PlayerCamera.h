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
		
		void MoveCamera(Vector2 moveDirection, float deltaTime);
	private:
		Vector2 position;
		POINT gameCursor;
		bool gameCursorInBounds = false;
		Vector2 currentVelocity;
		Vector2 currentAcceleration;
};

