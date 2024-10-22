#include "Camera.h"

Vector2 Camera::GetPos() {
	return position;
}

void Camera::SetPos(Vector2 newPos) {
	position = newPos;
}
