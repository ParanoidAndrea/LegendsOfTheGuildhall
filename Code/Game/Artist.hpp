#pragma once
#include "Game/Player.hpp"
class April : public Player 
{
public:
    April(Game* game, int playerIndex, bool IsAI = false);
   virtual void EnterDrawPhrase() override;
};