#pragma once
#include "Game/Gamecommon.hpp"
#include "Game/Card.hpp"
#include "Engine/Core/EventSystem.hpp"
#include <vector>
#include <deque>
class Game;
class Textbox;
class CardDeck
{
public:
    CardDeck(Game* game);
    bool Event_ShuffleDeck(EventArgs& args);
    void DrawCards(std::vector<CardData> & cards, int cardNum, std::string & drawCardNames);
    void DiscardSingleCard(CardData const& card);
    void DiscardCards(std::vector<CardData> const& cards);
    int GetCardsNumber() const;
    void DrawSpecificCard(std::vector<CardData>& cards, CardType const& specificCardType);
private:
    void InitializeDeck();
    void InitializeDeckWidget();
public:
    Textbox* m_cardDeckWidget = nullptr;
private:
    static std::vector <CardData> s_cardCollection;
    std::deque<CardData> m_drawDeck;
    std::vector<CardData> m_discardDeck;
    Game* m_game = nullptr;
};