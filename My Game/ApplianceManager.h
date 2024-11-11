#pragma once
#include "Common.h"
#include "BaseObject.h"
#include "GameDefines.h"

struct Appliance {
	eSprite sprite;
	char type;

	bool isOn = false;
};

class ApplianceManager: public CCommon {
	public:
		ApplianceManager();
		~ApplianceManager();

		void AddAppliance(eSprite, char);
		void TurnOnAppliance(char);
		void TurnOffAppliance(char);
		void UpdateAppliance(char, bool);
		void DrawAppliance(eSprite);
		void DrawBoundingBoxes(eSprite);
		void DeleteAppliance(char);
		Appliance* GetAppliance(char);

	private:
		Appliance** applianceMap = nullptr;
		int numAppliances;
};

