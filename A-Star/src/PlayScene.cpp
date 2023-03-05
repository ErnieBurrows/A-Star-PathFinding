#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "InputType.h"
// #include <iostream>

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::Start();
}

PlayScene::~PlayScene() = default;

void PlayScene::Draw()
{
	m_map.Render();

	Cell targetCell = m_map.GridPosition(m_pShip->GetTransform()->position);

	if (m_renderPath) for (const Cell& cell : m_path) m_map.RenderTile(cell, glm::vec3(1.0f, 0.0f, 1.0f));

	// m_map.RenderTile(targetCell, MUD);
	
	DrawDisplayList();

	SDL_SetRenderDrawColor(Renderer::Instance().GetRenderer(), 255, 255, 255, 255);
}

void PlayScene::Update()
{
	glm::vec2 adj = glm::vec2((Config::SCREEN_WIDTH / GRID_SIZE) * 0.5, (Config::SCREEN_HEIGHT / GRID_SIZE) * 0.5);

	glm::vec2 currentTarget = m_pShip->GetTargetPosition();

	if (m_isActive && !m_path.empty())
	{
		if (!m_isMoving)
		{
			m_pShip->GetTransform()->position = m_map.PixelPosition(m_map.start) + adj;
			m_isMoving = true;
		}
		if (m_it < 0)
		{
			currentTarget = m_map.PixelPosition(m_map.start) + adj;
			m_it++;
		}
		if (Util::Distance(m_pShip->GetTransform()->position, m_pShip->GetTargetPosition()) < 40)
		{
			if (m_it < m_path.size() - 1) m_it++;
			currentTarget = m_map.PixelPosition(m_path.at(m_it)) + adj;
		}
		if (Util::Distance(m_pShip->GetTransform()->position, m_map.PixelPosition(m_map.end)) < 40)
		{
			m_pShip->GetTransform()->position = m_map.PixelPosition(m_map.start) + adj;
			m_isActive = false;
			m_isMoving = false;
			m_it = -1;
			SoundManager::Instance().PlaySound("yay");
		}
	}
	else
	{
		m_pShip->GetTransform()->position = m_map.PixelPosition(m_map.start) + adj;
		m_isActive = false;
		m_isMoving = false;
		m_it = -1;
	}

	m_pShip->SetEnabled(m_isActive);

	m_pShip->SetTargetPosition(currentTarget);

	UpdateDisplayList();
}

void PlayScene::Clean()
{
	RemoveAllChildren();
}


void PlayScene::HandleEvents()
{
	EventManager::Instance().Update();

	// GetPlayerInput();

	GetKeyboardInput();
}

// void PlayScene::GetPlayerInput()
// {
// 	switch (m_pCurrentInputType)
// 	{
// 	case static_cast<int>(InputType::GAME_CONTROLLER):
// 	{
// 		// handle player movement with GameController
// 		if (SDL_NumJoysticks() > 0)
// 		{
// 			if (EventManager::Instance().GetGameController(0) != nullptr)
// 			{
// 				constexpr auto dead_zone = 10000;
// 				if (EventManager::Instance().GetGameController(0)->STICK_LEFT_HORIZONTAL > dead_zone)
// 				{
// 					m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_RIGHT);
// 					m_playerFacingRight = true;
// 				}
// 				else if (EventManager::Instance().GetGameController(0)->STICK_LEFT_HORIZONTAL < -dead_zone)
// 				{
// 					m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_LEFT);
// 					m_playerFacingRight = false;
// 				}
// 				else
// 				{
// 					if (m_playerFacingRight)
// 					{
// 						m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_RIGHT);
// 					}
// 					else
// 					{
// 						m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_LEFT);
// 					}
// 				}
// 			}
// 		}
// 	}
// 	break;
// 	case static_cast<int>(InputType::KEYBOARD_MOUSE):
// 	{
// 		// handle player movement with mouse and keyboard
// 		if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_A))
// 		{
// 			m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_LEFT);
// 			m_playerFacingRight = false;
// 		}
// 		else if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_D))
// 		{
// 			m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_RIGHT);
// 			m_playerFacingRight = true;
// 		}
// 		else
// 		{
// 			if (m_playerFacingRight)
// 			{
// 				m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_RIGHT);
// 			}
// 			else
// 			{
// 				m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_LEFT);
// 			}
// 		}
// 	}
// 	break;
// 	case static_cast<int>(InputType::ALL):
// 	{
// 		if (SDL_NumJoysticks() > 0)
// 		{
// 			if (EventManager::Instance().GetGameController(0) != nullptr)
// 			{
// 				constexpr auto dead_zone = 10000;
// 				if (EventManager::Instance().GetGameController(0)->STICK_LEFT_HORIZONTAL > dead_zone
// 					|| EventManager::Instance().IsKeyDown(SDL_SCANCODE_D))
// 				{
// 					m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_RIGHT);
// 					m_playerFacingRight = true;
// 				}
// 				else if (EventManager::Instance().GetGameController(0)->STICK_LEFT_HORIZONTAL < -dead_zone
// 					|| EventManager::Instance().IsKeyDown(SDL_SCANCODE_A))
// 				{
// 					m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_LEFT);
// 					m_playerFacingRight = false;
// 				}
// 				else
// 				{
// 					if (m_playerFacingRight)
// 					{
// 						m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_RIGHT);
// 					}
// 					else
// 					{
// 						m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_LEFT);
// 					}
// 				}
// 			}
// 		}
// 		else if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_A))
// 		{
// 			m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_LEFT);
// 			m_playerFacingRight = false;
// 		}
// 		else if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_D))
// 		{
// 			m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_RIGHT);
// 			m_playerFacingRight = true;
// 		}
// 		else
// 		{
// 			if (m_playerFacingRight)
// 			{
// 				m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_RIGHT);
// 			}
// 			else
// 			{
// 				m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_LEFT);
// 			}
// 		}
// 	}
// 	break;
// 	}
// }

void PlayScene::GetKeyboardInput()
{
	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_ESCAPE))
	{
		Game::Instance().Quit();
	}

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_1))
	{
		Game::Instance().ChangeSceneState(SceneState::START);
	}

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_2))
	{
		Game::Instance().ChangeSceneState(SceneState::END);
	}
	
	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_F))
	{
		m_path = m_map.FindPath();
	}
	
	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_M))
	{
		m_isActive = true;
	}

	if (EventManager::Instance().KeyPressed(SDL_SCANCODE_GRAVE))
	{
		if (!m_inDebug) m_inDebug = true;
		else m_inDebug = false;
	}

	if (EventManager::Instance().MousePressed(1) && m_inDebug)
	{
		SDL_GetMouseState(&mouseX, &mouseY);
		m_mousePosition = { mouseX, mouseY };

		if (!(m_map.GridPosition(m_mousePosition) == m_map.end))
		{
			m_map.start = m_map.GridPosition(m_mousePosition);
			m_path.clear();
		}
	}

	if (EventManager::Instance().MousePressed(3) && m_inDebug)
	{
		SDL_GetMouseState(&mouseX, &mouseY);
		m_mousePosition = { mouseX, mouseY };

		if (!(m_map.GridPosition(m_mousePosition) == m_map.start))
		{
			m_map.end = m_map.GridPosition(m_mousePosition);
			m_path.clear();
		}

		m_map.UpdateScores();
	}
}

void PlayScene::Start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	// Set Input Type
	m_pCurrentInputType = static_cast<int>(InputType::KEYBOARD_MOUSE);

	m_map.Init(this);
	// m_path = m_map.FindPath();
	
	glm::vec2 adj = glm::vec2((Config::SCREEN_WIDTH / GRID_SIZE) * 0.5, (Config::SCREEN_HEIGHT / GRID_SIZE) * 0.5);


	// Ship Sprite
	m_pShip = new Ship();
	m_pShip->GetTransform()->position = m_map.PixelPosition(m_map.start) + adj;
	AddChild(m_pShip);
	m_pShip->SetTargetPosition(m_map.PixelPosition(m_map.start) + adj);

	SoundManager::Instance().Load("../Assets/audio/BlindingLights.mp3", "bgMusic", SoundType::SOUND_MUSIC);
	SoundManager::Instance().Load("../Assets/audio/yay.ogg", "yay", SoundType::SOUND_SFX);
	SoundManager::Instance().SetMusicVolume(20);
	SoundManager::Instance().PlayMusic("bgMusic", -1, 0);

	/* Instructions Label */
	m_pInstructionsLabel = new Label("Press [F] to find Shortest Path, [M] to move ship.", "Consolas");
	m_pInstructionsLabel->GetTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 500.0f);

	AddChild(m_pInstructionsLabel);

	/* DO NOT REMOVE */
	ImGuiWindowFrame::Instance().SetGuiFunction([this] { GUI_Function(); });
}

void PlayScene::GUI_Function() 
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Your Window Title Goes Here", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	bool euclidean = m_map.GetDistanceType() == EUCLIDEAN ? true : false;
	if (ImGui::Checkbox("Is Euclidean", &euclidean))
	{
		m_map.SetDistanceType(euclidean ? EUCLIDEAN : MANHATTAN);
		m_path.clear();
		m_map.UpdateScores();
	}

	// Cell start = m_map.start;
	// if (ImGui::SliderInt2("Start", (int*)&start, 1, GRID_SIZE - 2))
	// {
	// 	m_map.start = start;
	// 	m_path.clear();
	// 	// m_map.UpdateScores();
	// }

	//  Cell end = m_map.end;
	//  if (ImGui::SliderInt2("End", (int*)&end, 1, GRID_SIZE - 2))
	//  {
	//  	m_map.end = end;
	//  	m_path.clear();
	//  	m_map.UpdateScores();
	//  }

	bool colour = m_map.GetColoured();
	if (ImGui::Checkbox("Colour", &colour))
	{
		m_map.SetColoured(colour);
	}

	bool cost = m_map.GetCostVis();
	if (ImGui::Checkbox("Cost", &cost))
	{
		m_map.SetCostVis(cost);
	}

	bool path = m_renderPath;
	if (ImGui::Checkbox("Show Path", &path))
	{
		m_renderPath = path;
	}
	
	ImGui::End();
}
