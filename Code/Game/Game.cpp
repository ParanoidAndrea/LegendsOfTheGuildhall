#include "Game.hpp"
#include "Game/App.hpp"
#include "Engine/Core/SimpleTriangleFont.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/Clock.hpp"
#include "Engine/Core/DevConsole.hpp"
#include "Game/Player.hpp"
#include "Engine/UI/Widget.hpp"
#include "Engine/UI/Button.hpp"
#include "Game/CardDeck.hpp"
#include "Engine/Render/BitmapFont.hpp"
#include "Game/Artist.hpp"
#include "Game/BossAIController.hpp"
#include "Game/Programmer.hpp"
#include "Engine/UI/Textbox.hpp"
#include "Engine/Core/NetSystem.hpp"
#include "Engine/Core/Timer.hpp"
#include "Game/ManipulateWidget.hpp"
CardDeck* g_cardDeck = nullptr;
Clock* g_theGameClock = nullptr;
Texture* g_buttonTexture = nullptr;
Texture* g_victoryTexture = nullptr;
Texture* g_loseTexture = nullptr;
Game::Game(App* owner)
	:m_owner(owner)
{
	
	m_screenCamera = Camera(Vec2(),Vec2(SCREEN_SIZE_X,SCREEN_SIZE_Y));
}

Game::~Game()
{
	delete g_theGameClock;
	g_theGameClock = nullptr;
	for (Player* player : m_players)
	{
		delete player;
		player = nullptr;
	}
	g_theEventSystem->UnsubscribeEventCallbackObjectMethod("ShuffleDeck", g_cardDeck, &CardDeck::Event_ShuffleDeck);
}

void Game::Startup()
{
	g_victoryTexture = g_theRenderer->CreateOrGetTextureFromFile("Data/Images/Background/Victory.png");
	g_loseTexture = g_theRenderer->CreateOrGetTextureFromFile("Data/Images/Background/Lose.png");
	g_buttonTexture = g_theRenderer->CreateOrGetTextureFromFile("Data/Images/Button/Button1.png");
	g_theGameClock = new Clock(Clock::GetSystemClock());
	m_fpsTimer = new Timer(0.5f);
	StartupWidgets();

	if (g_netState == NetState::NONE)
	{
		EnterSinglePlay();
	}
	else
	{
		EnterWaitingForPlayer();
	}
	//EnterAttract();
}



void Game::StartupWidgets()
{
	m_mainWidgets.resize((size_t)GameMode::NUMS);
	m_mainWidgets[(size_t)GameMode::Attract] = new Widget(g_theRenderer, SCREEN_AREA, "Data/Images/Background/AttractScreen.png", true);
	m_mainWidgets[(size_t)GameMode::WaitingForPlayer] = new Widget(g_theRenderer, SCREEN_AREA, "Data/Images/Background/AttractScreen.png", true);
	m_mainWidgets[(size_t)GameMode::Playing] = new Widget(g_theRenderer, SCREEN_AREA, "Data/Images/Background/CardBoard.png", true);

	StartupWaitingForPlayerWidget();
	//StartupDebugButtons();
}

void Game::StartupWaitingForPlayerWidget()
{
	Textbox* text = new Textbox(g_theRenderer, Vec2(0.5f, 0.5f), Vec2(0.5f, 0.5f), Vec2(0.5f, 0.5f), "Waiting For Player...", g_bitmapFont, Vec2(0.5f, 0.5f), SCREEN_SIZE_Y * 0.25f, GLOBAL_FONT_ASPECT, Rgba8::WHITE, false);
	m_mainWidgets[(size_t)GameMode::WaitingForPlayer]->AddChild(text);
}

void Game::StartupDebugButtons()
{
//     EventArgs args;
// 	//args.SetValue("PlayerIndex", "0");
//     args.SetValue("CardNumber", 4);
//     Button* discardButton = new Button(AABB2((0.5f - 0.15f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.1f), (0.5f - 0.05f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.15f)), args, "Discard", "MainPlayerDiscardSelectedCards", "RobotoMonoSemiBold128", g_theRenderer);
// 
//     Button* drawButton = new Button(AABB2((0.5f + 0.05f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.1f), (0.5f + 0.15f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.15f)), args, "Draw 4", "MainPlayerDrawCards", "RobotoMonoSemiBold128", g_theRenderer);
// 	//drawButton->SetValue("CardNumber", 4);
// 	Button* healButton = new Button(AABB2((0.5f + 0.05f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.2f), (0.5f + 0.15f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.25f)), args, "Heal", "MainPlayerHeal", "RobotoMonoSemiBold128", g_theRenderer);
// 
// 	Button* damageButton = new Button(AABB2((0.5f - 0.15f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.2f), (0.5f - 0.05f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.25f)), args, "Take Damage", "MainPlayerTakeDamage", "RobotoMonoSemiBold128", g_theRenderer);
// 
// 	Button* switchButton = new Button(AABB2((0.5f + 0.2f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y  + 0.15f), (0.5f + 0.3f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.2f)), args, "Switch Player", "SwitchPlayer", "RobotoMonoSemiBold128", g_theRenderer);
//     m_mainWidgets[(size_t)GameMode::Playing]->m_children.push_back(discardButton);
//     m_mainWidgets[(size_t)GameMode::Playing]->m_children.push_back(drawButton);
//     m_mainWidgets[(size_t)GameMode::Playing]->m_children.push_back(healButton);
//     m_mainWidgets[(size_t)GameMode::Playing]->m_children.push_back(damageButton);
// 	m_mainWidgets[(size_t)GameMode::Playing]->m_children.push_back(switchButton);
// 	for (Widget* widget : m_mainWidgets[(size_t)GameMode::Playing]->m_children)
// 	{
// 		Button* button = dynamic_cast<Button*>(widget);
// 		if (button)
// 		{
//             button->SetTextNormalColorAndHoverColor(BUTTON_TEXT_NORMAL, BUTTON_TEXT_HOVER);
//             button->DisableTextShadow();
//             button->SetTextAspect(GLOBAL_FONT_ASPECT);
// 		}
// 	}
}



void Game::StartupPlaying()
{
    StartupCardDeck();
    StartupPlayer();
}

void Game::StartupCardDeck()
{
	g_cardDeck = new CardDeck(this);
	g_theEventSystem->SubscribeEventCallbackObjectMethod("ShuffleDeck", g_cardDeck, &CardDeck::Event_ShuffleDeck);
}

void Game::StartupPlayer()
{
	if (g_netSystem->IsClient())
	{

	}
	Player* player1 = new DavidF(this, 0);
	Player* player2 = new Son(this, 1);

	m_players.push_back(player1);
	m_players.push_back(player2);
	player1->m_otherPlayerIndex = 1;
	player2->m_otherPlayerIndex = 0;
	//m_mainWidgets[(size_t)GameMode::Playing]->AddChildren(playingCards);
    //EventArgs args;
    //args.SetValue("PlayerIndex", Stringf("%d", 0));
    //args.SetValue("CardsNumber", "4");
    //FireEvent("DrawCards", args);
    //args.SetValue("PlayerIndex", Stringf("%d", 1));
    //FireEvent("DrawCards", args);
    //player1->DrawCards(4);
    //player2->DrawCards(4);
	//FireEvent("startturnplayer0");
}

void Game::Update()
{

	if (m_mode == GameMode::Playing)
	{
		for (int i = 0; i < (int)m_players.size(); ++i)
		{
			m_players[i]->Update(g_theGameClock->GetDeltaSeconds());
		}
		m_mainWidgets[(int)GameMode::Playing]->Update();
		InputFunctionsInUpdates();
	}
	else if(m_mode == GameMode::Attract)
	{
		UpdateAttract();
	}
	else if (m_mode == GameMode::WaitingForPlayer)
	{
		UpdateWaitingForPlayer();
	}
	else if (m_mode == GameMode::LOSE || m_mode == GameMode::VICTORY)
	{
		if (g_theInput->WasKeyJustPressed(KEYCODE_ESC))
		{
			FireEvent("Quit");
		}
	}
    if (m_fpsTimer->HasPeriodElapsed() || m_fpsTimer->IsStopped())
    {
        m_fpsTimer->Start();
    }
}

void Game::UpdateWaitingForPlayer()
{
    //if (g_theInput->WasKeyJustPressed(KEYCODE_ESC))
    //{
    //    FireEvent("EnterAttract");
    //    g_theInput->ResetInputState(KEYCODE_ESC);
    //}

    if (g_netSystem->IsConnected())
    {
        FireEvent("PlayerReady");
    }
}




void Game::UpdateAttract()
{
	if (g_theInput->WasKeyJustPressed(KEYCODE_ESC))
	{
		FireEvent("Quit");

		return;
	}
    if (g_theInput->WasKeyJustPressed('N') || g_theInput->WasKeyJustPressed(' '))
    {
        m_mode = GameMode::Playing;
        return;
    }
}

void Game::UpdatePlaying()
{
	for (size_t i = 0; i < m_players.size(); ++i)
	{
		m_players[i]->Update(g_theGameClock->GetDeltaSeconds());
	}
}

void Game::RenderVictory() const
{
	std::vector<Vertex_PCU> verts;
	g_theRenderer->SetBlendMode(BlendMode::ALPHA);
	g_theRenderer->SetRasterizerMode(RasterizerMode::SOLID_CULL_NONE);

    g_theRenderer->BindTexture(g_victoryTexture);
    g_theRenderer->BindShader(nullptr);
    g_theRenderer->SetModelConstants();
	AddVertsForAABB2D(verts, SCREEN_AREA, Rgba8::WHITE);
	g_theRenderer->DrawVertexArray((int)verts.size(), verts.data());
	m_victoryPlayer->m_playerWidget->Render();

}

void Game::RenderLose() const
{
    std::vector<Vertex_PCU> verts;
    g_theRenderer->SetBlendMode(BlendMode::ALPHA);
    g_theRenderer->SetRasterizerMode(RasterizerMode::SOLID_CULL_NONE);
    g_theRenderer->BindTexture(g_loseTexture);
    g_theRenderer->BindShader(nullptr);
    g_theRenderer->SetModelConstants();
	AddVertsForAABB2D(verts, SCREEN_AREA, Rgba8::WHITE);
    g_theRenderer->DrawVertexArray((int)verts.size(), verts.data());
	m_losePlayer->m_playerWidget->Render();
}

void Game::InputFunctionsInUpdates()
{

	if (g_theInput->WasKeyJustPressed(0x70))
	{
		DebugModeEnable();
	}

	if (g_theInput->WasKeyJustPressed(KEYCODE_ESC))
	{
		ExitSinglePlay();
	}

// 	if (g_theInput->WasKeyJustPressed('P'))
// 	{
// 		g_theGameClock->TogglePause();
// 	}
// 
// 	if (g_theInput->IsKeyDown('T'))
// 	{
// 		g_theGameClock->SetTimeScale(0.1f);
// 	}
// 	if (g_theInput->WasKeyJustPressed('O'))
// 	{
// 		g_theGameClock->StepSingleFrame();
// 	}

	if (g_theInput->WasKeyJustPressed('S'))
	{
		FireEvent("ShuffleDeck");
	}
}



void Game::Render() const
{
    Rgba8 clearColor(0, 0, 0, 255);
    g_theRenderer->ClearScreen(clearColor);
    g_theRenderer->BeginCamera(m_screenCamera);
    //RenderPlayModeUI();	
    if (m_mainWidgets[(size_t)m_mode])
    {
        m_mainWidgets[(size_t)m_mode]->Render();
    }
    g_theRenderer->EndCamera(m_screenCamera);
    if (m_mode == GameMode::Attract)
    {
        g_theRenderer->BeginCamera(m_screenCamera);
        RenderAttract();
        g_theRenderer->EndCamera(m_screenCamera);
    }
    else if(m_mode == GameMode::Playing)
    {

        for (size_t i = 0; i < (int)m_players.size(); ++i)
        {
			if (m_players[i])
			{
				m_players[i]->Render();
			}
        }
        if (GetMainPlayer()->m_actionState == ActionState::ManipulateCards)
        {
            GetMainPlayer()->m_manipulateWidget->Render();
        }
		g_cardDeck->m_cardDeckWidget->Render();
    }
    else if (m_mode == GameMode::VICTORY)
    {
        g_theRenderer->BeginCamera(m_screenCamera);
        RenderVictory();
        g_theRenderer->EndCamera(m_screenCamera);
    }
    else if(m_mode == GameMode::LOSE)
    {
        g_theRenderer->BeginCamera(m_screenCamera);
        RenderLose();
        g_theRenderer->EndCamera(m_screenCamera);
    }

	g_theRenderer->BeginCamera(m_screenCamera);
	RenderText();
	g_theRenderer->EndCamera(m_screenCamera);
}

void Game::RenderAttract() const
{   
	g_theRenderer->BindTexture(nullptr);
	g_theRenderer->BindShader(nullptr);
	g_theRenderer->SetModelConstants();
}





void Game::RenderText() const
{
	//std::string gameDeckInfo = Stringf("Remain Cards in Deck: %d", g_cardDeck->GetCardsNumber());
	//std::vector<Vertex_PCU> verts;
	//g_bitmapFont->AddVertsForTextBox2D(verts, AABB2(0.f, 0.f, SCREEN_SIZE_X, SCREEN_SIZE_Y), 20.f, gameDeckInfo, Rgba8::WHITE, 0.5f, Vec2(1.f, 1.f));
	//g_theRenderer->BindTexture(&g_bitmapFont->GetTexture());
	//g_theRenderer->SetBlendMode(BlendMode::ALPHA);
	//g_theRenderer->SetModelConstants();
	//g_theRenderer->DrawVertexArray((int)verts.size(), verts.data());

	static std::vector<Vertex_PCU> verts;
	if (m_fpsTimer->GetElapsedFraction() <= 0.f && 1.f / Clock::GetSystemClock().GetDeltaSeconds() <= 20000.f)
	{
		verts.clear();
		g_bitmapFont->AddVertsForTextBox2D(verts, AABB2(0.f, 0.f, SCREEN_SIZE_X, SCREEN_SIZE_Y), 20.f, Stringf("FPS: %4.0f", 1.f / Clock::GetSystemClock().GetDeltaSeconds()), Rgba8::WHITE, 0.5f, Vec2(1.f, 1.f));
	}
    g_theRenderer->BindTexture(&g_bitmapFont->GetTexture());
    g_theRenderer->SetBlendMode(BlendMode::ALPHA);
    g_theRenderer->SetModelConstants();
    g_theRenderer->DrawVertexArray((int)verts.size(), verts.data());
}

void Game::DebugModeRender() const
{
	
}



void Game::DebugModeEnable()
{
	m_Develop = !m_Develop; 
}


void Game::SwitchPlayerInterface()
{
	m_mainPlayerIndex = m_mainPlayerIndex == 0 ? 1 : 0;
	m_otherPlayerIndex = m_mainPlayerIndex == 0 ? 1 : 0;
	m_players[0]->InitializeWidgets();
	m_players[1]->InitializeWidgets();
	//m_players[m_mainPlayerIndex]->ChangeCardsInHand();

}

void Game::SwitchPlayerTurn()
{
	m_currenPlayerIndex = m_currenPlayerIndex == 0 ? 1 : 0;
	FireEvent("StartTurnPlayer" + ToString(m_currenPlayerIndex));
}

bool Game::IsAttract() const
{
	return m_mode == GameMode::Attract;
}

void Game::EnterAttract()
{
	m_mode = GameMode::Attract;
}

void Game::ExitAttract()
{
	EnterSinglePlay();
}

void Game::EnterWaitingForPlayer()
{
	m_mode = GameMode::WaitingForPlayer;
    if (g_netState == NetState::LOCAL)
    {
        NetSystemConfig netSystemConfig = g_netSystem->GetNetSystemConfig();
        netSystemConfig.m_modeString = g_gameConfigBlackboard.GetValue("netMode", "NONE");
        g_netSystem = new NetSystem(netSystemConfig);
        g_netSystem->Startup();

        g_netState = NetState::REMOTE;
    }

}

void Game::ExitWaitingForPlayer()
{
    if (g_netSystem->IsClient())
    {
        m_mainPlayerIndex = 1;
		m_otherPlayerIndex = 0;
    }
	else 
	{
        m_mainPlayerIndex = 0;
        m_otherPlayerIndex = 1;
	}
	EnterOnlinePlay();
}

void Game::EnterSinglePlay()
{
	m_mode = GameMode::Playing;
	StartupPlaying();
	m_players[0]->m_currentState = PlayerState::End;
		m_players[1]->m_currentState = PlayerState::End;
	g_theConsole->ExecuteXmlCommandScriptFile("Data/CommandScript.xml");
}

void Game::ExitSinglePlay()
{
	EnterAttract();
}

void Game::EnterOnlinePlay()
{
	StartupPlaying();
	EventArgs args;
	args.SetValue("cardNumber", 4);
	args.SetValue("IsLocal", false);
    FireEvent("DrawCardsPlayer0", args);
	args.SetValue("cardNumber", 6);
    FireEvent("DrawCardsPlayer1", args);
    m_players[0]->m_currentState = PlayerState::End;
    m_players[1]->m_currentState = PlayerState::End;
	FireEvent("StartTurnPlayer0", args);
	
	m_mode = GameMode::Playing;

}

void Game::ExitOnlinePlay()
{
	EnterAttract();
}

Player* Game::GetAnotherPlayer() const
{
	return m_players[m_otherPlayerIndex];
}

Player* Game::GetPlayer(int playerIndex) const
{
    for (size_t i = 0; i < m_players.size(); ++i)
    {
        if (i == playerIndex)
        {
            return m_players[i];
        }
    }
	return nullptr;
}

Player* Game::GetMainPlayer() const
{
	return m_players[m_mainPlayerIndex];
}

Player* Game::GetCurrentPlayingPlayer() const
{
	return m_players[m_currenPlayerIndex];
}

void Game::AddGlobalText(std::string const& text, Rgba8 color)
{
	m_players[0]->AddGameInfoText(text, color);
	m_players[1]->AddGameInfoText(text, color);
}

void Game::AddTextToPlayer(std::string const& text, int playerIndex, Rgba8 color)
{
	m_players[playerIndex]->AddGameInfoText(text, color);
}

void Game::EnterLose(int playerIndex)
{
	m_mode = GameMode::LOSE;
	m_losePlayer = m_players[playerIndex];
	AABB2 box((0.5f - 0.05f) * SCREEN_SIZE_X, (0.5f - 0.1f) * SCREEN_SIZE_Y, (0.5f + 0.05f) * SCREEN_SIZE_X, (0.5f + 0.1f) * SCREEN_SIZE_Y);
    m_losePlayer->m_playerWidget = new Widget(g_theRenderer, box, "Data/Images/Player/PlayerBackground2.png");
    m_losePlayer->m_playerWidget->AddChild(new Widget(g_theRenderer, box, "Data/Images/Player/" + m_losePlayer->m_playerIconPath));
    m_losePlayer->m_playerWidget->SetBorderColor(Rgba8(87, 43, 1, 255), Rgba8::GREY);
}

void Game::EnterVictory(int playerIndex)
{
    m_mode = GameMode::VICTORY;
    m_victoryPlayer = m_players[playerIndex];
    AABB2 box((0.5f - 0.05f) * SCREEN_SIZE_X, (0.5f - 0.1f) * SCREEN_SIZE_Y, (0.5f + 0.05f) * SCREEN_SIZE_X, (0.5f + 0.1f) * SCREEN_SIZE_Y);
    m_victoryPlayer->m_playerWidget = new Widget(g_theRenderer, box, "Data/Images/Player/PlayerBackground2.png");
    m_victoryPlayer->m_playerWidget->AddChild(new Widget(g_theRenderer, box, "Data/Images/Player/" + m_victoryPlayer->m_playerIconPath));
    m_victoryPlayer->m_playerWidget->SetBorderColor(Rgba8(87, 43, 1, 255), Rgba8::GREY);
}
