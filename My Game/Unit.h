#pragma once
#include "Object.h"
#include "GameDefines.h"

class Unit: public CObject {
	private:
		int m_iHealth = 0; ///< Health.
		int m_iDamage = 0; ///< Damage.
		int m_iRange = 0; ///< Range.
		int m_iSpeed = 0; ///< Speed.
		int m_iCost = 0; ///< Cost.
	public:
		Unit(const Vector2& p);
		~Unit();

		void Draw(eSprite);

		void SetHealth(int health);
		void SetDamage(int damage);
		void SetRange(int range);
		void SetSpeed(int speed);
		void SetCost(int cost);
		void TakeDamage(int damage);
		void DealDamage(Unit* unit);


};

