#pragma once

#include "Game/Gamecommon.hpp"
#include "Engine/Core/EventSystem.hpp"

class Player;

class PlayerController
{
public:
    virtual ~PlayerController() = default;

    virtual void Update(Player& player) { UNUSED(player); }
    virtual void OnEnterActionPhrase(Player& player) { UNUSED(player); }
    virtual void OnEnterDyingPhrase(Player& player, EventArgs& args) { UNUSED(player); UNUSED(args); }
    virtual void OnNeedToRespondDodge(Player& player, EventArgs& args) { UNUSED(player); UNUSED(args); }
    virtual void OnNeedToRespondStrike(Player& player, EventArgs& args) { UNUSED(player); UNUSED(args); }
    virtual void OnNeedToRespondDuel(Player& player, EventArgs& args) { UNUSED(player); UNUSED(args); }
};
