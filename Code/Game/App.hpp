#pragma once
#include "Engine/Render/Renderer.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/Window/Window.hpp"
#include "Game.hpp"
//#include "Engine/Render/Renderer.hpp"
class App
{
public:
	App();
	~App();
	void Startup(std::string const& commandLine);
	void Shutdown();
	void Run();
	bool IsQuitting() const;
	static bool Quit(EventArgs& args);
	static bool LoadFromGameConfig(EventArgs& args);

	static bool PlayerReady(EventArgs& args);
	static bool SwitchPlayer(EventArgs& args);
private:

	void RegisterCommands();
	void SetScreenSize();
	void BeginFrame();
	void Update();
	void UpdateFromInputSystem();
	void Render() const;
	void EndFrame();
	

private:
	
	bool m_isAttractMode = true;
	bool m_isQuitting = false;
	bool m_isPaused = false;
	bool m_isSlowMo = false;
	bool m_isSpeedUp = false;
	Vec2 m_shipPos;
	float m_orientDegrees=30.f;

	Game* m_theGame;
};
