#include "PlayerCamera.h"
#include "Renderer.h"
#include "Math.h"

Vector2 PlayerCamera::GetPos() {
	return position;
}

void PlayerCamera::SetPos(Vector2 newPos) {
	position = newPos;
}

void PlayerCamera::MoveCamera(Vector2 moveDirection, float deltaTime) {
	float smoothTime = 0.1f;
	float maxSpeed = 500.f;

	float velocityX = Math::SmoothDamp(currentVelocity.x, moveDirection.x, &currentAcceleration.x, smoothTime, maxSpeed, deltaTime);
	float velocityY = Math::SmoothDamp(currentVelocity.y, moveDirection.y, &currentAcceleration.y, smoothTime, maxSpeed, deltaTime);

	currentVelocity = Vector2(velocityX, velocityY);

	position += currentVelocity * deltaTime;	// moves camera by velocity
}

void PlayerCamera::SetCursorPos(POINT newCursorPos) {
	gameCursor = newCursorPos;
}