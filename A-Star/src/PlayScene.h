#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "ship.h"
// #include "Plane.h"
// #include "Player.h"
#include "Button.h"
#include "Label.h"
#include "TileMap.h"
#include <glm/gtc/type_ptr.hpp>
class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene() override;

	// Scene LifeCycle Functions
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;
	virtual void HandleEvents() override;
	virtual void Start() override;
private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;
	glm::vec2 m_mousePosition;
	int mouseX, mouseY;
	glm::vec2 m_target;
	TileMap m_map;
	Path m_path;
	int m_it = -1;
	bool m_isActive = false;
	bool m_isMoving = false;
	bool m_inDebug = false;
	bool m_renderPath = false;

	// Plane* m_pPlaneSprite{};
	// Player* m_pPlayer{};
	// bool m_playerFacingRight{};
	Ship* m_pShip{};

	// UI Items
	// Button* m_pBackButton{};
	// Button* m_pNextButton{};
	Label* m_pInstructionsLabel{};

	// Input Control
	int m_pCurrentInputType{};
	// void GetPlayerInput();
	void GetKeyboardInput();


};

#endif /* defined (__PLAY_SCENE__) */