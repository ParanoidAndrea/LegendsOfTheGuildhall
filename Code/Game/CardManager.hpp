#pragma once
#include <vector>
#include <deque>
#include "Game/Card.hpp"
class Player;
class CardManager
{
public:
    CardManager() = default;
    Card* PushNewCard(Player* player, CardData const& cardData, Vec2 const& cardRelativePos, int cardIndex);
    void Update();
    void Render() const;
    void ResetCards();
    void DisselectCurrentCards();
    void DiscardSelectedCard(std::string & selectCardName);
    void SelectCard(int index);
    void DeselectCard(int selectedIndex);
    void PlayCard(int index);
    int GetFirstSelectIndex() const;
    CardType const GetCardType(int index) const;
    CardType const GetSelectCardType() const;
public:
    int m_lastSelectIndex = -1;
    Timer* m_selectTimer = nullptr;
    Timer* m_hoverTimer = nullptr;
    std::deque<int> m_selectedIndexes;
    int m_hoverIndex = -1;
    std::vector<Card*> m_currentCards;
};