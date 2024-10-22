#pragma once
#include "Defines.h"

class Camera
{
	public:
		//LBaseCamera::GetPos(); //do get and set position.
		//LBaseCamera::SetOrthographic(float width, float ht, float nearz, float farz);
		
		//get current position
		Vector2 GetPos();
		//set new position
		void SetPos(Vector2 newPos);
		
	private:
		Vector2 position;
};

