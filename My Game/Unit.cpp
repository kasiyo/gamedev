#include "Unit.h"

//Unit::Unit(eSprite t, const Vector2& p) : LBaseObject(t, p) {};

void Unit::draw() {
	m_pRenderer->Draw(&desc); // Draw the unit.
};

void Unit::move() {
	if (!is_stationary) {
		lerpInfo.currDuration += m_pTimer->GetFrameTime();
		float percentComplete = (std::min)(lerpInfo.currDuration / lerpInfo.maxDuration, 1.0f);

		if (lerpInfo.currDuration >= lerpInfo.maxDuration) {
			lerpInfo.currDuration = 0.0f;
			desc.m_vPos = lerpInfo.target;
			desc.m_vPos.x += this->tile->x_offset;	// for offset
			desc.m_vPos.y += (this->tile->y_offset);	// for offset
			is_stationary = true;
			return;
		}	// if lerpInfo.currDuration >= lerpInfo.maxDuration
		else {
			desc.m_vPos.x = Math::lerp(desc.m_vPos.x, lerpInfo.target.x, percentComplete);
			desc.m_vPos.y = Math::lerp(desc.m_vPos.y, lerpInfo.target.y + (this->tile->y_offset + 10.0f), percentComplete);
		}	// else lerpInfo.currDuration < lerpInfo.maxDuration
	}	// if !is_stationary
	else { return; }
};

/// --- Move the unit to the tile. --- ///
void Unit::moveTo(Tile* t, const WPARAM k) {
	printf("running moveTo function\n");

	float percentComplete = (std::min)(lerpInfo.currDuration / lerpInfo.maxDuration, 1.0f);
	Vector2 endPos = t->pos;

	lerpInfo.target = endPos;

	// update sprite frame according to movement direction
	switch (k) {
		case VK_LEFT: {
			desc.m_nCurrentFrame = 3;
			break;
		}
		case VK_RIGHT: {
			desc.m_nCurrentFrame = 1;
			break;
		}
		case VK_UP: {
			desc.m_nCurrentFrame = 2;
			break;
		}
		case VK_DOWN: {
			desc.m_nCurrentFrame = 0;
			break;
		}
	}
};