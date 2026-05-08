#pragma once
#include "Game/Player.hpp"
class Yenna : public Player
{
public:
    Yenna(Game* game, int playerIndex);
};

class DavidF : public Player
{
public:
    DavidF(Game* game, int playerIndex);
};

class Son : public Player
{
public:
    Son(Game* game, int playerIndex);
    virtual void TakeDamage() override;
};