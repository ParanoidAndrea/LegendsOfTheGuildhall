#pragma once
#include "Game/Player.hpp"
class Yenna : public Player
{
public:
    Yenna(Game* game, int playerIndex, bool IsAI = false);
};

class DavidF : public Player
{
public:
    DavidF(Game* game, int playerIndex, bool IsAI = false);
};

class Son : public Player
{
public:
    Son(Game* game, int playerIndex, bool IsAI = false);
    virtual void TakeDamage() override;
};