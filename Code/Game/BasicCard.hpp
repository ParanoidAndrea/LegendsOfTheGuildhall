#pragma once
#include "Game/Card.hpp"
class Strike : public Card 
{
public:
    Strike(Player* owner, CardData const& cardData, AABB2 const& widgetArea, Vec2 const& widgetRelativePos, Vec2 const& widgetPivot, CardManager* cardManager, size_t cardIndex);
    virtual void Play() override;
    virtual bool IsPlayable() const override;
};


class Dodge : public Card
{
public:
    Dodge(Player* owner, CardData const& cardData, AABB2 const& widgetArea, Vec2 const& widgetRelativePos, Vec2 const& widgetPivot, CardManager* cardManager, size_t cardIndex);
    virtual void Play() override;
    virtual bool IsPlayable() const override;
};

class Health : public Card
{
public:
    Health(Player* owner, CardData const& cardData, AABB2 const& widgetArea, Vec2 const& widgetRelativePos, Vec2 const& widgetPivot, CardManager* cardManager, size_t cardIndex);
    virtual void Play() override;
    virtual bool IsPlayable() const override;
};