#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Math/AABB2.hpp"
#define UNUSED(x) (void)(x)

enum class NetState
{
    NONE,
    LOCAL,
    REMOTE,
};

enum class GameMode
{
	Attract,
    WaitingForPlayer,
	Playing,
    VICTORY,
    LOSE,
    NUMS
};

enum class GameState
{

};

class App;
class Renderer;
class InputSystem;
class Window;
class AudioSystem;
class RandomNumberGenerator;
class XboxController;
class NetSystem;
class BitmapFont;
class Texture;
class CardDeck;
class Clock;

constexpr float MAX_SCREEN_SHAKE = 10.f;
constexpr float SCREEN_SHAKE_REDUCTION_PER_SEC = 10.f;
constexpr int ENTITIES_HEALTH = 3;
constexpr float SELECT_CARD_SCALE = 1.15f;
constexpr float HOVER_CARD_SCALE = 1.05f;
constexpr float GLOBAL_FONT_ASPECT = 0.5f;
extern float SCREEN_SIZE_X;
extern float SCREEN_SIZE_Y;
extern float EQUIPMENT_PADDING_X;
extern float CARD_PADDING_Y;
extern float CARD_PADDING_X;
extern float PLAYER_PADDING_X;
extern float HEALTH_PADDING_X;
extern float HEALTH_ICON_PADDING_Y;
extern float CARD_SIZE_X;
extern float CARD_SIZE_Y;
extern AABB2 CARD_SIZE;
extern float OPPONENT_PLAYER_PADDING_X;
extern float OPPONENT_PLAYER_PADDING_Y;

// constexpr float WORLD_SIZE_X = 200.f;
// constexpr float WORLD_SIZE_Y = 100.f;
// constexpr float WORLD_CENTER_X  = WORLD_SIZE_X / 2.f;
// constexpr float WORLD_CENTER_Y  = WORLD_SIZE_Y / 2.f;
extern float SCREEN_CENTER_X;
extern float SCREEN_CENTER_Y;
extern float INHAND_CARD_MAX_Y;

extern NetState g_netState;
extern AABB2 SCREEN_AREA;
static Rgba8 BUTTON_TEXT_NORMAL = Rgba8(192, 115, 31, 255);
static Rgba8 BUTTON_TEXT_HOVER  = Rgba8(192, 115, 31, 125);
static Rgba8 CARDDECK_TEXT = Rgba8(248, 244, 227, 255);
static Rgba8 MEDIUM_DARKGREY = Rgba8(100, 100, 100, 255);
static Rgba8 DEEP_GREEN = Rgba8(92, 159, 63, 255);
static Rgba8 YELLOW = Rgba8(255, 255, 6, 255);
static Rgba8 BLACK_GREY = Rgba8(102, 153, 204, 255);
static Rgba8 WHITE = Rgba8(255, 255, 255, 255);
static Rgba8 BLACK = Rgba8(0, 0, 0, 255);
static Rgba8 DARK_RED = Rgba8(158, 50, 47, 255);
extern App* g_theApp;
extern Texture* g_buttonTexture;
extern RandomNumberGenerator* g_rng;
extern InputSystem* g_theInput;
extern AudioSystem* g_theAudio;
extern Renderer* g_theRenderer;
extern Texture* g_testTexture;
extern NetSystem* g_netSystem;
extern Window* g_theWindow;
extern BitmapFont* g_bitmapFont;
extern CardDeck* g_cardDeck;
extern Clock* g_theGameClock;
void DebugDrawRing(Vec2 const& center, float radius, float thickness, Rgba8 const& color);
void DebugDrawGlow(Vec2 const& center, float radius, Rgba8 const& centerColor, Rgba8 const& edgeColor);
void DebugDrawLine(Vec2 const& startPos, Vec2 const& endPos, float thickness, Rgba8 const& color);
void DebugDrawGlow(Vec2 const& center, float radius, Rgba8 const& centerColor, Rgba8 const& edgeColor);
void WarpVertexes(int numVerts, Vertex_PCU* verts, Vec2 const& warpCenter, float warpAmount);
