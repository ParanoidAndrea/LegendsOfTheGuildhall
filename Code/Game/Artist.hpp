#pragma once
#include "Game/Player.hpp"
class April : public Player 
{
public:
    April(Game* game, int playerIndex);
   virtual void EnterDrawPhrase() override;
};