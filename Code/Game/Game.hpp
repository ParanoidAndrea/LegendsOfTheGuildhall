#pragma once
#include "GameCommon.hpp"
#include "Engine/Render/Camera.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Core/Rgba8.hpp"
class App;
class Clock;
class Player;
class Widget;
class CardDeck;
class Button;
class Card;
class Timer;
struct CardData;
class Game
{
public:
	Game(App* owner);
	~Game();
	App* m_owner;
	
	void Startup();
	void Update();
	void Render() const;
	bool m_IsAttract = true;
	bool m_isPaused = false;

	void SwitchPlayerInterface();
	void SwitchPlayerTurn();
	bool IsAttract() const;
	void EnterAttract();
	void ExitAttract();
	void EnterWaitingForPlayer();
	void ExitWaitingForPlayer();
	void EnterSinglePlay();
	void ExitSinglePlay();
	void EnterOnlinePlay();
	void ExitOnlinePlay();
	Player* GetAnotherPlayer() const;
	Player* GetPlayer(int playerIndex) const;
	Player* GetMainPlayer() const;
	Player* GetCurrentPlayingPlayer() const;
	void AddGlobalText(std::string const& text, Rgba8 color = Rgba8::WHITE);
	void AddTextToPlayer(std::string const& text, int playerIndex, Rgba8 color = Rgba8 ::WHITE);
	void EnterLose(int playerIndex);
    void EnterVictory(int playerIndex);

private:
	void StartupWidgets();
	void StartupWaitingForPlayerWidget();
	void StartupDebugButtons();
	void StartupPlaying();
	void StartupCardDeck();
	void StartupPlayer();
	void UpdateWaitingForPlayer();
	void UpdateAttract();
	void UpdatePlaying();
	void RenderVictory() const;
    void RenderLose() const;
	void RenderAttract() const;
	void RenderText() const;
	void DebugModeRender() const;
	void DebugModeEnable();
	void InputFunctionsInUpdates();
public:
	std::vector<Player*> m_players;
	std::vector<CardData> m_tempDiscardPie;
    Camera m_screenCamera;
	int m_mainPlayerIndex = 0;
	int m_otherPlayerIndex = 1;
	int m_currenPlayerIndex = 0;
private:
	Player* m_victoryPlayer = nullptr;
	Player* m_losePlayer = nullptr;
	Timer* m_fpsTimer = nullptr;
	std::vector<Button*> m_debugButtons;
	std::vector<Widget*> m_mainWidgets;
	//Clock* m_theGameClock = nullptr;
	GameMode m_mode = GameMode::Attract;
	bool m_Develop = false;
};
