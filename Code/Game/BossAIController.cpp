#include "Game/BossAIController.hpp"

#include "Game/CardManager.hpp"
#include "Game/Game.hpp"
#include "Game/Player.hpp"

void BossAIController::Update(Player& player)
{
    if (!player.IsPlayerTurn())
    {
        return;
    }
    if (player.m_currentState == PlayerState::Action)
    {
        UpdateActionPhrase(player);
    }
    else if (player.m_currentState == PlayerState::Discard)
    {
        UpdateDiscardPhrase(player);
    }
}

void BossAIController::OnEnterActionPhrase(Player& player)
{
    UNUSED(player);
    m_hasActedThisActionPhrase = false;
}

void BossAIController::OnEnterDyingPhrase(Player& player, EventArgs& args)
{
    int healIndex = FindFirstCardIndex(player, CardType::Heal);
    if (healIndex >= 0)
    {
        player.m_cardManager->DisselectCurrentCards();
        player.m_cardManager->SelectCard(healIndex);
        player.m_game->AddGlobalText(player.m_playerName + " auto uses Heal");
        player.Event_DyingHeal(args);
        return;
    }

    player.m_game->AddGlobalText(player.m_playerName + " cannot heal");
    FireEvent("VictoryPlayer" + ToString(player.m_otherPlayerIndex));
}

void BossAIController::OnNeedToRespondDodge(Player& player, EventArgs& args)
{
    TryRespondWithCard(player, CardType::Dodge, args);
}

void BossAIController::OnNeedToRespondStrike(Player& player, EventArgs& args)
{
    TryRespondWithCard(player, CardType::Strike, args);
}

void BossAIController::OnNeedToRespondDuel(Player& player, EventArgs& args)
{
    TryRespondToDuel(player, args);
}

void BossAIController::UpdateActionPhrase(Player& player)
{
    if (IsWaitingForResponse(player))
    {
        return;
    }
    if (m_hasActedThisActionPhrase)
    {
        player.m_game->AddGlobalText(player.m_playerName + " auto ends Action");
        EventArgs args;
        player.Event_EndActionPhrase(args);
        return;
    }

    int cardIndex = FindFirstActionCardIndex(player);
    if (cardIndex < 0)
    {
        player.m_game->AddGlobalText(player.m_playerName + " has no auto action card");
        EventArgs args;
        player.Event_EndActionPhrase(args);
        return;
    }

    m_hasActedThisActionPhrase = true;
    player.m_cardManager->DisselectCurrentCards();
    player.m_cardManager->SelectCard(cardIndex);
    player.m_game->AddGlobalText(Stringf("%s auto plays %s", player.m_playerName.c_str(), player.m_carddataInHand[cardIndex].m_displayName.c_str()));

    EventArgs args;
    args.SetValue("CardIndex", cardIndex);
    player.Event_PlayCard(args);
}

void BossAIController::UpdateDiscardPhrase(Player& player)
{
    int discardNum = (int)player.m_carddataInHand.size() - player.m_currentHealth;
    if (discardNum <= 0)
    {
        return;
    }

    player.m_cardManager->DisselectCurrentCards();
    for (int cardIndex = 0; cardIndex < discardNum && cardIndex < (int)player.m_carddataInHand.size(); ++cardIndex)
    {
        player.m_cardManager->SelectCard(cardIndex);
    }

    player.m_game->AddGlobalText(Stringf("%s auto discards %d card(s)", player.m_playerName.c_str(), discardNum));
    EventArgs args;
    player.Event_EndDiscardPhrase(args);
}

bool BossAIController::IsWaitingForResponse(Player const& player) const
{
    ActionState const opponentActionState = player.GetAnotherPlayer()->m_actionState;
    return player.m_actionState == ActionState::PlayingStrike
        || player.m_actionState == ActionState::NeedToRespondDodge
        || player.m_actionState == ActionState::NeedToRespondStrike
        || player.m_actionState == ActionState::ManipulateCards
        || player.m_actionState == ActionState::Dying
        || opponentActionState == ActionState::NeedToRespondDodge
        || opponentActionState == ActionState::NeedToRespondStrike
        || opponentActionState == ActionState::ManipulateCards
        || opponentActionState == ActionState::Dying;
}

bool BossAIController::TryRespondWithCard(Player& player, CardType const& cardType, EventArgs& args)
{
    int cardIndex = FindFirstCardIndex(player, cardType);
    if (cardIndex >= 0)
    {
        player.m_cardManager->DisselectCurrentCards();
        player.m_cardManager->SelectCard(cardIndex);
        player.m_game->AddGlobalText(Stringf("%s auto responds with %s", player.m_playerName.c_str(), player.m_carddataInHand[cardIndex].m_displayName.c_str()));
        player.Event_RespondSuccessfully(args);
        return true;
    }

    std::string responseName = cardType == CardType::Dodge ? "Dodge" : "Strike";
    player.m_game->AddGlobalText(Stringf("%s has no %s response", player.m_playerName.c_str(), responseName.c_str()));
    player.Event_RespondTakeDamage(args);
    return false;
}

bool BossAIController::TryRespondToDuel(Player& player, EventArgs& args)
{
    int cardIndex = FindFirstCardIndex(player, CardType::Strike);
    if (cardIndex >= 0)
    {
        player.m_cardManager->DisselectCurrentCards();
        player.m_cardManager->SelectCard(cardIndex);
        args.SetValue("SelectedCardIndex", cardIndex);
        player.m_game->AddGlobalText(player.m_playerName + " auto responds to Duel with Strike");
        FireEvent("NeedToRespondDuelPlayer" + ToString(player.m_otherPlayerIndex), args);
        return true;
    }

    player.m_game->AddGlobalText(player.m_playerName + " cannot answer Duel");
    FireEvent("RespondDuelTakeDamagePlayer" + ToString(player.m_playerIndex), args);
    return false;
}

int BossAIController::FindFirstCardIndex(Player const& player, CardType const& cardType) const
{
    for (int cardIndex = 0; cardIndex < (int)player.m_carddataInHand.size(); ++cardIndex)
    {
        if (player.m_carddataInHand[cardIndex].m_type == cardType)
        {
            return cardIndex;
        }
    }
    return -1;
}

int BossAIController::FindFirstActionCardIndex(Player const& player) const
{
    CardType const priorities[] =
    {
        CardType::Strike,
        CardType::Duel,
        CardType::DrawTwo
    };

    for (CardType const& cardType : priorities)
    {
        int cardIndex = FindFirstCardIndex(player, cardType);
        if (cardIndex >= 0 && cardIndex < (int)player.m_cardManager->m_currentCards.size() && player.m_cardManager->m_currentCards[cardIndex]->IsPlayable())
        {
            return cardIndex;
        }
    }
    return -1;
}
