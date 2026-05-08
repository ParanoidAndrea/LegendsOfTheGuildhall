#include "Game/Artist.hpp"
#include "Game/Gamecommon.hpp"
#include "Game/Game.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Core/Timer.hpp"
April::April(Game* game, int playerIndex)
    :Player(game, playerIndex, "April","April.png")
{
    m_maxHealth = 3;
    m_currentHealth = 3;
    InitializeMaxHealthBarWidgets();
    InitializeCurrentHealthWidgets();
}

void April::EnterDrawPhrase()
{
    m_currentState = PlayerState::Draw;
    m_game->AddGlobalText(m_playerName + " enter Draw Phrase");
    //    EventArgs args;
    //    args.SetValue("CardNumber", 2);
    //    args.SetValue("IsLocal", 0);
    //    FireEvent("DrawCardsPlayer" + ToString(m_playerIndex),args);
    DrawCardsLocal(3);
    m_transitionTimer->Start();
    g_theEventSystem->ReplaceEventCallbackObjectMethod("TransitionEventPlayer" + ToString(m_playerIndex), reinterpret_cast<Player*>(this), &Player::Event_EndDrawPhrase);
    //std::vector 
}
