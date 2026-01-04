#include "App.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/Clock.hpp"
#include "Engine/Core/DevConsole.hpp"
#include "Engine/Core/NetSystem.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Game/Player.hpp"
#include "Engine/UI/UISystem.hpp"
// Created and owned by the App
Renderer* g_theRenderer = nullptr;
InputSystem* g_theInput = nullptr;
Window* g_theWindow = nullptr;
RandomNumberGenerator* g_rng = nullptr;
DevConsole* g_theConsole = nullptr;
XboxController g_controller;
Texture* g_testTexture = nullptr;
GameState g_gameState;
NetState g_netState = NetState::NONE;
NetSystem* g_netSystem = nullptr;
BitmapFont* g_bitmapFont = nullptr;
AudioSystem* g_theAudio = nullptr;

float SCREEN_SIZE_X = 0.f;
float SCREEN_SIZE_Y = 0.f;
float SCREEN_CENTER_X = 0.f;
float SCREEN_CENTER_Y = 0.f;
float EQUIPMENT_PADDING_X = 0.f;
float CARD_PADDING_Y = 0.f;
float CARD_PADDING_X = 0.f;
float PLAYER_PADDING_X = 0.f;
float HEALTH_PADDING_X = 0.f;
float HEALTH_ICON_PADDING_Y = 0.f;
float CARD_SIZE_Y = 0.f;
float CARD_SIZE_X = 0.f;
float INHAND_CARD_MAX_Y = 0.f;
AABB2 SCREEN_AREA;
AABB2 CARD_SIZE;
App::App()
{
	m_theGame = new Game(this);
}
App::~App()
{
}
//-----------------------------------------------------------------------------------------------
void App::Startup(std::string const& commandLine)
{

    UISystemConfig uiSystemConfig;
    g_uiSystem = new UISystem(uiSystemConfig);
    g_uiSystem->Startup();

    g_rng = new RandomNumberGenerator();
    EventArgs args;
    LoadFromGameConfig(args);
    NetSystemConfig netSystemConfig;
    netSystemConfig.m_hostAddressString = g_gameConfigBlackboard.GetValue("netHostAddress", "");
    netSystemConfig.m_sendBufferSize = g_gameConfigBlackboard.GetValue("netSendBufferSize", netSystemConfig.m_sendBufferSize);
    netSystemConfig.m_recvBufferSize = g_gameConfigBlackboard.GetValue("netRecvBufferSize", netSystemConfig.m_recvBufferSize);

    InputConfig inputConfig;
    g_theInput = new InputSystem(inputConfig);

    EventSystemConfig eventSystemConfig;
    g_theEventSystem = new EventSystem(eventSystemConfig);

    DevConsoleConfig devConsoleConfig;

    //AudioConfig audioConfig;
    //g_theAudio = new AudioSystem(audioConfig);
    //g_theAudio->Startup();

    g_theConsole = new DevConsole(devConsoleConfig);

    g_theEventSystem->Startup();
    g_theConsole->Startup();

    RegisterCommands();

    g_theConsole->ExcuteCommand(commandLine);
    if (g_gameConfigBlackboard.GetValue("netMode", "NONE") == "Client" || g_gameConfigBlackboard.GetValue("netMode", "NONE") == "Server")
    {
        g_netState = NetState::LOCAL;
    }

    g_theConsole->AddLine(DevConsole::INFO_MINOR, "Type help for the lists of the commands");
    g_theConsole->AddLine(DevConsole::INFO_MAJOR, "LeftMouse - Select");

    WindowConfig windowConfig;
    windowConfig.m_inputSystem  = g_theInput;
    windowConfig.m_windowTitle  = g_gameConfigBlackboard.GetValue("windowTitle", windowConfig.m_windowTitle);
    windowConfig.m_aspectRatio  = g_gameConfigBlackboard.GetValue("windowAspect", windowConfig.m_aspectRatio);
    windowConfig.m_screenSize   = g_gameConfigBlackboard.GetValue("windowSize", windowConfig.m_screenSize);
    windowConfig.m_screenPos    = g_gameConfigBlackboard.GetValue("windowPosition", windowConfig.m_screenPos);
    windowConfig.m_isFullscreen = g_gameConfigBlackboard.GetValue("windowFullscreen", windowConfig.m_isFullscreen);
    g_theWindow = new Window(windowConfig);
    g_theWindow->StartUp();
    SetScreenSize();

    RenderConfig renderConfig;
    renderConfig.m_window = g_theWindow;
    g_theRenderer = new Renderer(renderConfig);
   
    g_theInput->Startup();
    g_theRenderer->Startup();
    devConsoleConfig.m_defalutRenderer = g_theRenderer;
    g_theConsole->SetConfig(devConsoleConfig);
//     DebugRenderConfig debugRenderConfig;
//     debugRenderConfig.m_renderer = g_theRenderer;
//     DebugRenderSystemStartup(debugRenderConfig);

    netSystemConfig.m_modeString = "NONE";
    g_netSystem = new NetSystem(netSystemConfig);

    g_bitmapFont = g_theRenderer->CreateOrGetBitmapFont("Data/Fonts/RobotoMonoSemiBold128");
    m_theGame = new Game(this);
    m_theGame->Startup();

    devConsoleConfig.m_camera = &m_theGame->m_screenCamera;
    devConsoleConfig.m_defalutRenderer = g_theRenderer;
    g_theConsole->SetConfig(devConsoleConfig);




}


//-----------------------------------------------------------------------------------------------
void App::Shutdown()
{
	delete m_theGame;
	m_theGame = nullptr;
	g_theEventSystem->Shutdown();
	g_theInput->Shutdown();
	g_theWindow->ShutDown();
    //g_theAudio->Shutdown();
    g_theConsole->Shutdown();
	g_theRenderer->Shutdown();
    g_netSystem->Shutdown();
    g_uiSystem->ShutDown();
	delete g_theEventSystem;
	g_theEventSystem = nullptr;
	delete g_theInput;
	g_theInput = nullptr;
	delete g_theWindow;
	g_theWindow = nullptr;
	delete g_theRenderer;
	g_theRenderer = nullptr;
	delete g_theAudio;
	g_theAudio = nullptr;
	delete g_theConsole;
	g_theConsole = nullptr;
}


bool App::Quit(EventArgs& args)
{
	UNUSED(args);
	g_theApp->m_isQuitting = true; // sets m_isQuitting=true
	return true;
}

bool App::LoadFromGameConfig(EventArgs& args)
{
    std::string gameConfigFilePath = args.GetValue("file", "");
    XmlDocument document;
    XmlResult result;
    if (gameConfigFilePath == "")
    {
        result = document.LoadFile("Data/GameConfig.xml");
    }
    else
    {
        result = document.LoadFile(gameConfigFilePath.c_str());
    }

    if (result != tinyxml2::XML_SUCCESS)
    {
        ERROR_RECOVERABLE(Stringf("Invaild GameConfig Filename: %s", gameConfigFilePath.c_str()));
        return true;
    }
    XmlElement* rootElement = document.RootElement();
    g_gameConfigBlackboard.PopulateFromXmlElementAttributes(*rootElement);
    //g_theConsole->ExecuteXmlCommandScriptNode(*rootElement);
    return true;
}


bool App::PlayerReady(EventArgs& args)
{
    UNUSED(args);
    // 	if (g_netState == NetState::REMOTE)
    // 	{
    // 		FireEvent("StartGame");
    // 	}
    g_theApp->m_theGame->ExitWaitingForPlayer();
    return true;
}

void App::RegisterCommands()
{
    g_theEventSystem->SubscribeEventCallbackFunction("Quit",           App::Quit);
    g_theEventSystem->SubscribeEventCallbackFunction("LoadGameConfig", App::LoadFromGameConfig);
    g_theEventSystem->SubscribeEventCallbackFunction("PlayerReady",    App::PlayerReady);
    g_theEventSystem->SubscribeEventCallbackFunction("SwitchPlayer",   App::SwitchPlayer);
}

void App::SetScreenSize()
{
    SCREEN_SIZE_X = g_theWindow->GetScreenSize().x;
    SCREEN_SIZE_Y = g_theWindow->GetScreenSize().y;
    SCREEN_CENTER_X = SCREEN_SIZE_X / 2.f;
    SCREEN_CENTER_Y = SCREEN_SIZE_Y / 2.f;
    SCREEN_AREA = AABB2(0.f, 0.f, SCREEN_SIZE_X, SCREEN_SIZE_Y);
    CARD_PADDING_Y = 1.f / 3.f;

    EQUIPMENT_PADDING_X = 0.15f;
    
    PLAYER_PADDING_X = 0.15f;

    HEALTH_ICON_PADDING_Y = CARD_PADDING_Y/3.5f;
    HEALTH_PADDING_X = HEALTH_ICON_PADDING_Y*0.5f ;
    CARD_PADDING_X = 1.f-EQUIPMENT_PADDING_X -PLAYER_PADDING_X - HEALTH_PADDING_X;
    CARD_SIZE_X = 0.1f * CARD_PADDING_X * SCREEN_SIZE_X;
    CARD_SIZE_Y = 1.5f * CARD_SIZE_X;
    CARD_SIZE = AABB2(0.f, 0.f, CARD_SIZE_X, CARD_SIZE_Y);
    INHAND_CARD_MAX_Y = 0.15f * CARD_SIZE_Y;
}

void App::BeginFrame()
{   
	g_theEventSystem->BeginFrame();
	g_theInput->BeginFrame();
	g_theWindow->BeginFrame();
	g_theRenderer->BeginFrame();
	//g_theAudio->BeginFrame();
	g_controller = g_theInput->GetController(0);
	g_theConsole->BeginFrame();
    g_netSystem->BeginFrame();
    g_uiSystem->BeginFrame();
}


void App::Update()
{
	UpdateFromInputSystem();
	m_theGame->Update();
	
}




void App::UpdateFromInputSystem()
{
    if (g_theInput->WasKeyJustPressed(KEYCODE_TILDE))
    {
		g_theConsole->ToggleOpen();
	}

}


void App::Render() const
{
	if (m_theGame)
	{
		m_theGame->Render();
	}
	g_theRenderer->BeginCamera(*g_theConsole->GetCamera());
	g_theConsole->Render();
	g_theRenderer->EndCamera(*g_theConsole->GetCamera());

	

}

void App::EndFrame()
{
    g_netSystem->EndFrame();
	g_theEventSystem->EndFrame();
	g_theInput->EndFrame();
	g_theWindow->EndFrame();
	g_theRenderer->EndFrame();
	//g_theAudio->EndFrame();
	g_theConsole->EndFrame();
    g_uiSystem->EndFrame();
}


void App::Run()
{
	Clock::TickSystemClock();
	BeginFrame();	
	Update();
	Render();		
	EndFrame();		
}


bool App::IsQuitting() const
{
	return m_isQuitting;
}


bool App::SwitchPlayer(EventArgs& args)
{
    UNUSED(args);
    g_theApp->m_theGame->SwitchPlayerInterface();
    return true;
}



