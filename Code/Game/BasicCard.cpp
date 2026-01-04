#include "Game/BasicCard.hpp"
#include "Game/Player.hpp"
#include "Game/Game.hpp"
Strike::Strike(Player* owner, CardData const& cardData, AABB2 const& widgetArea, Vec2 const& widgetRelativePos, Vec2 const& widgetPivot, CardManager* cardManager, size_t cardIndex)
    :Card(owner, cardData, widgetArea, widgetRelativePos, widgetPivot, cardManager, cardIndex, false)
{
    m_backgroundTexture = g_theRenderer->CreateOrGetTextureFromFile("Data/Images/Card/Strike.png");
}

void Strike::Play()
{
    m_owner->m_game->AddGlobalText(m_owner->m_playerName + " Strikes " + m_owner->GetAnotherPlayer()->m_playerName);
    m_owner->CreateNormalButton();
    m_owner->SetCentralInfoText("Waiting for response...");
    m_owner->m_actionState = ActionState::PlayingStrike;
    EventArgs args;
    args.SetValue("TargetNextActionState", (int)ActionState::Normal);
    args.SetValue("InitiatorNextActionState", (int)(m_owner->m_isPlayingStrikeInfinite ? ActionState::HaveNotPlayedStrike : ActionState::HavePlayedStrike));
    FireEvent("NeedToRespondDodgePlayer" + ToString(GetOpponentPlayerIndex()), args);
}

bool Strike::IsPlayable() const
{
    return IsOwnerTheActionState(ActionState::HaveNotPlayedStrike) || m_owner->IsDiscardPhrase() || IsOwnerTheActionState(ActionState::NeedToRespondStrike);
}

Dodge::Dodge(Player* owner, CardData const& cardData, AABB2 const& widgetArea, Vec2 const& widgetRelativePos, Vec2 const& widgetPivot, CardManager* cardManager, size_t cardIndex)
    :Card(owner, cardData, widgetArea, widgetRelativePos, widgetPivot, cardManager, cardIndex, false)
{
    m_backgroundTexture = g_theRenderer->CreateOrGetTextureFromFile("Data/Images/Card/Dodge.png");
}

void Dodge::Play()
{

}

bool Dodge::IsPlayable() const
{
    return IsOwnerTheActionState(ActionState::NeedToRespondDodge) || m_owner->IsDiscardPhrase();
}

Health::Health(Player* owner, CardData const& cardData, AABB2 const& widgetArea, Vec2 const& widgetRelativePos, Vec2 const& widgetPivot, CardManager* cardManager, size_t cardIndex)
     :Card(owner, cardData, widgetArea, widgetRelativePos, widgetPivot, cardManager, cardIndex, false)
{
    m_backgroundTexture = g_theRenderer->CreateOrGetTextureFromFile("Data/Images/Card/Heal.png");
}

void Health::Play()
{
    m_owner->m_game->AddGlobalText(m_owner->m_playerName + " Heal");
    FireEvent("HealPlayer" + ToString(m_owner->m_playerIndex));
}

bool Health::IsPlayable() const
{
    return (IsOwnerThePlayerState(PlayerState::Action) && m_owner->m_currentHealth < m_owner->m_maxHealth) || m_owner->IsDiscardPhrase() || IsOwnerTheActionState(ActionState::Dying);
}
