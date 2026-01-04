#pragma once
#include "Game/Card.hpp"
class Barbarians : public Card
{
public:
    Barbarians(Player* owner, CardData const& cardData, AABB2 const& widgetArea, Vec2 const& widgetRelativePos, Vec2 const& widgetPivot, CardManager* cardManager, size_t cardIndex);
    virtual void Play() override;
    virtual bool IsPlayable() const override;
};

class HailOfArrows : public Card
{
public:
    HailOfArrows(Player* owner, CardData const& cardData, AABB2 const& widgetArea, Vec2 const& widgetRelativePos, Vec2 const& widgetPivot, CardManager* cardManager, size_t cardIndex);
    virtual void Play() override;
    virtual bool IsPlayable() const override;
};

class DismantleBreak : public Card
{
public:
    DismantleBreak(Player* owner, CardData const& cardData, AABB2 const& widgetArea, Vec2 const& widgetRelativePos, Vec2 const& widgetPivot, CardManager* cardManager, size_t cardIndex);
    virtual void Play() override;
    virtual bool IsPlayable() const override;
};

class SnatchSteal : public Card
{
public:
    SnatchSteal(Player* owner, CardData const& cardData, AABB2 const& widgetArea, Vec2 const& widgetRelativePos, Vec2 const& widgetPivot, CardManager* cardManager, size_t cardIndex);
    virtual void Play() override;
    virtual bool IsPlayable() const override;
};

class DrawTwo : public Card
{
public:
    DrawTwo(Player* owner, CardData const& cardData, AABB2 const& widgetArea, Vec2 const& widgetRelativePos, Vec2 const& widgetPivot, CardManager* cardManager, size_t cardIndex);
    virtual void Play() override;
    virtual bool IsPlayable() const override;
};

class Duel : public Card
{
public:
    Duel(Player* owner, CardData const& cardData, AABB2 const& widgetArea, Vec2 const& widgetRelativePos, Vec2 const& widgetPivot, CardManager* cardManager, size_t cardIndex);
    virtual void Play() override;
    virtual bool IsPlayable() const override;
};