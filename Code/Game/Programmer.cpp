#include "Game/Programmer.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Game/Game.hpp"
#include "Game/Gamecommon.hpp"
Yenna::Yenna(Game* game, int playerIndex, bool IsAI /*= false*/)
    :Player(game, playerIndex,"Yenna", "Yenna_Logo.png", IsAI)
{

}

Son::Son(Game* game, int playerIndex, bool IsAI /*= false*/)
:Player(game, playerIndex,"Son", "Son.png", IsAI)
{

}

void Son::TakeDamage()
{
    m_currentHealth = GetClamped(--m_currentHealth, 0, m_maxHealth);
    InitializeCurrentHealthWidgets();
    m_game->AddGlobalText(Stringf("%s take 1 damage and Draw 1 card", m_playerName.c_str()));
    DrawCardsLocal(1);
}

DavidF::DavidF(Game* game, int playerIndex, bool IsAI /*= false*/)
:Player(game, playerIndex,"David", "DavidF.png", IsAI)
{
    m_isPlayingStrikeInfinite = true;
}
