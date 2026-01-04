#include "Game/ScrollToolCard.hpp"
#include "Game/Player.hpp"
#include "Game/Game.hpp"
#include "Game/ManipulateWidget.hpp"
Barbarians::Barbarians(Player* owner, CardData const& cardData, AABB2 const& widgetArea, Vec2 const& widgetRelativePos, Vec2 const& widgetPivot, CardManager* cardManager, size_t cardIndex)
    :Card(owner, cardData, widgetArea, widgetRelativePos, widgetPivot, cardManager, cardIndex, false)
{
    m_backgroundTexture = g_theRenderer->CreateOrGetTextureFromFile("Data/Images/Card/Barbarians.png");
}

void Barbarians::Play()
{
    m_owner->m_game->AddGlobalText(m_owner->m_playerName + " play Barbarian.");
    m_owner->CreateNormalButton();
    m_owner->SetCentralInfoText("Waiting for response...");

    EventArgs args;
    args.SetValue("TargetNextActionState", (int)GetOpponentPlayer()->m_actionState);
    args.SetValue("InitiatorNextActionState", (int)m_owner->m_actionState);
//    args.SetValue("SucessGlobalText", GetOpponentPlayer()->m_playerName + " react the Barbarian.");

    m_owner->m_actionState = ActionState::Normal;
    FireEvent("NeedToRespondStrikePlayer" + ToString(GetOpponentPlayerIndex()),args);
}

bool Barbarians::IsPlayable() const
{
    return IsScrollToolCardPlayable();
}

HailOfArrows::HailOfArrows(Player* owner, CardData const& cardData, AABB2 const& widgetArea, Vec2 const& widgetRelativePos, Vec2 const& widgetPivot, CardManager* cardManager, size_t cardIndex)
    :Card(owner, cardData, widgetArea, widgetRelativePos, widgetPivot, cardManager, cardIndex, false)
{
    m_backgroundTexture = g_theRenderer->CreateOrGetTextureFromFile("Data/Images/Card/HailOfArrows.png");
}

void HailOfArrows::Play()
{
    m_owner->m_game->AddGlobalText(m_owner->m_playerName + " play Hail Of Arrows.");
    m_owner->CreateNormalButton();
    m_owner->SetCentralInfoText("Waiting for response...");
    EventArgs args;
    args.SetValue("TargetNextActionState", (int)GetOpponentPlayer()->m_actionState);
    args.SetValue("InitiatorNextActionState", (int)m_owner->m_actionState);

    m_owner->m_actionState = ActionState::Normal;
    FireEvent("NeedToRespondDodgePlayer" + ToString(GetOpponentPlayerIndex()), args);
}

bool HailOfArrows::IsPlayable() const
{
    return IsScrollToolCardPlayable();
}

DismantleBreak::DismantleBreak(Player* owner, CardData const& cardData, AABB2 const& widgetArea, Vec2 const& widgetRelativePos, Vec2 const& widgetPivot, CardManager* cardManager, size_t cardIndex)
    :Card(owner, cardData, widgetArea, widgetRelativePos, widgetPivot, cardManager, cardIndex, false)
{
    m_backgroundTexture = g_theRenderer->CreateOrGetTextureFromFile("Data/Images/Card/Dismantle.png");
}

void DismantleBreak::Play()
{
    EventArgs args;
    args.SetValue("ManipulateEvent", (int)ManipulateEvent::DismantleBreak);
    FireEvent("ManipulateOpponentCardPlayer" + ToString(m_owner->m_playerIndex),args);
}

bool DismantleBreak::IsPlayable() const
{
    return IsScrollToolCardPlayable() && !GetOpponentPlayer()->m_carddataInHand.empty();
}


SnatchSteal::SnatchSteal(Player* owner, CardData const& cardData, AABB2 const& widgetArea, Vec2 const& widgetRelativePos, Vec2 const& widgetPivot, CardManager* cardManager, size_t cardIndex)
    :Card(owner, cardData, widgetArea, widgetRelativePos, widgetPivot, cardManager, cardIndex, false)
{
    m_backgroundTexture = g_theRenderer->CreateOrGetTextureFromFile("Data/Images/Card/SnatchSteal.png");
}

void SnatchSteal::Play()
{
    EventArgs args;
    args.SetValue("ManipulateEvent", (int)ManipulateEvent::SnatchSteal);
    FireEvent("ManipulateOpponentCardPlayer" + ToString(m_owner->m_playerIndex),args);
}

bool SnatchSteal::IsPlayable() const
{
    return IsScrollToolCardPlayable() && !GetOpponentPlayer()->m_carddataInHand.empty();
}

DrawTwo::DrawTwo(Player* owner, CardData const& cardData, AABB2 const& widgetArea, Vec2 const& widgetRelativePos, Vec2 const& widgetPivot, CardManager* cardManager, size_t cardIndex)
    :Card(owner, cardData, widgetArea, widgetRelativePos, widgetPivot, cardManager, cardIndex, false)
{
    m_backgroundTexture = g_theRenderer->CreateOrGetTextureFromFile("Data/Images/Card/DrawTwo.png");
}

void DrawTwo::Play()
{
    EventArgs args;
    args.SetValue("CardNumber", 2);
    FireEvent("DrawCardsPlayer" + ToString(m_owner->m_playerIndex), args);
}

bool DrawTwo::IsPlayable() const
{
    return IsScrollToolCardPlayable();
}

Duel::Duel(Player* owner, CardData const& cardData, AABB2 const& widgetArea, Vec2 const& widgetRelativePos, Vec2 const& widgetPivot, CardManager* cardManager, size_t cardIndex)
    :Card(owner, cardData, widgetArea, widgetRelativePos, widgetPivot, cardManager, cardIndex, false)
{
    m_backgroundTexture = g_theRenderer->CreateOrGetTextureFromFile("Data/Images/Card/Duel.png");
}

void Duel::Play()
{
    EventArgs args;
    args.SetValue("PlayerIndex", m_owner->m_playerIndex);
    args.SetValue("TargetNextActionState", (int)GetOpponentPlayer()->m_actionState);
    args.SetValue("InitiatorNextActionState", (int)m_owner->m_actionState);
    FireEvent("InitializeDuelPlayer" + ToString(GetOpponentPlayerIndex()),args);
    m_owner->SetCentralInfoText("Waiting for response...");
    m_owner->CreateNormalButton();
    m_owner->m_actionState = ActionState::Normal;
}

bool Duel::IsPlayable() const
{
    return IsScrollToolCardPlayable();
}
