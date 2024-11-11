#include "ApplianceManager.h"

ApplianceManager::ApplianceManager() {
	numAppliances = 0;
}

ApplianceManager::~ApplianceManager() {
	for (int i = 0; i < numAppliances; i++) {
		delete applianceMap[i];
	}
	delete[] applianceMap;
}

void ApplianceManager::AddAppliance(eSprite t, char c) {
	// Create a new appliance
}

void ApplianceManager::TurnOnAppliance(char c) {
	// Turn on an appliance
}

void ApplianceManager::TurnOffAppliance(char c) {
	// Turn off an appliance
}

void ApplianceManager::UpdateAppliance(char c, bool b) {
	// Update an appliance
}

void ApplianceManager::DrawAppliance(eSprite t) {
	// Draw an appliance
}

void ApplianceManager::DrawBoundingBoxes(eSprite t) {
	// Draw bounding boxes
}

void ApplianceManager::DeleteAppliance(char c) {
	// Delete an appliance
}

Appliance* ApplianceManager::GetAppliance(char c) {
	// Get an appliance
	return nullptr;
}