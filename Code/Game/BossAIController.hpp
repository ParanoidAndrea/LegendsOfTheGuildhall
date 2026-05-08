#pragma once

#include "Game/PlayerController.hpp"
#include "Game/Card.hpp"

class BossAIController : public PlayerController
{
public:
    virtual void Update(Player& player) override;
    virtual void OnEnterActionPhrase(Player& player) override;
    virtual void OnEnterDyingPhrase(Player& player, EventArgs& args) override;
    virtual void OnNeedToRespondDodge(Player& player, EventArgs& args) override;
    virtual void OnNeedToRespondStrike(Player& player, EventArgs& args) override;
    virtual void OnNeedToRespondDuel(Player& player, EventArgs& args) override;

private:
    void UpdateActionPhrase(Player& player);
    void UpdateDiscardPhrase(Player& player);
    bool IsWaitingForResponse(Player const& player) const;
    bool TryRespondWithCard(Player& player, CardType const& cardType, EventArgs& args);
    bool TryRespondToDuel(Player& player, EventArgs& args);
    int FindFirstCardIndex(Player const& player, CardType const& cardType) const;
    int FindFirstActionCardIndex(Player const& player) const;

private:
    bool m_hasActedThisActionPhrase = false;
};
