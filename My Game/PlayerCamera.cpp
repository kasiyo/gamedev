#include "PlayerCamera.h"
#include "Renderer.h"

Vector2 PlayerCamera::GetPos() {
	return position;
}

void PlayerCamera::SetPos(Vector2 newPos) {
	position = newPos;
}

/*LBaseCamera* PlayerCamera::GetCamera() {
	return CRenderer::GetCamera();
}*/

/*LBaseCamera* PlayerCamera::MoveToCursor() {

}*/