#include "Game/CardDeck.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/UI/Textbox.hpp"
#include "Engine/Core/DevConsole.hpp"
#include "Game/Game.hpp"
#include <array>
std::vector<CardData> CardDeck::s_cardCollection;
CardDeck::CardDeck(Game* game)
    :m_game(game)
{
    InitializeDeck();

    std::vector <CardData> tempDeck(s_cardCollection);
    for (size_t i = 0; i < tempDeck.size(); ++i)
    {
        int swapIndex = g_rng->RollRandomIntInRange(0, (int)tempDeck.size()-1);
        std::swap(tempDeck[i], tempDeck[swapIndex]);
    }
    for (size_t i = 0; i < tempDeck.size(); ++i)
    {
        m_drawDeck.push_front(tempDeck[i]);
    }
}

struct CardDistribution 
{
    CardDistribution(int rank, std::vector<CardType> const& diamondTypes, std::vector<CardType> const&  clubTypes, std::vector<CardType> const&  heartTypes,std::vector<CardType> const&  spadeTypes)
        :m_rank(rank)
    {
        m_suits = { diamondTypes,clubTypes,heartTypes,spadeTypes };
    }
    int m_rank;
    std::array<std::vector<CardType>, 4> m_suits; // [Diamond, Club, Heart, Spade]
};

void CardDeck::InitializeDeck()
{
    s_cardCollection.clear();
    s_cardCollection.reserve(84); // Overall 84 cards
    static std::vector<CardDistribution> distribution =
    {
       CardDistribution(1,
           {CardType::Duel},
           {CardType::Duel},
           {CardType::Heal, CardType::HailofArrows},
           {CardType::Duel}
           ),
       CardDistribution(2,
           {CardType::Dodge},
           {CardType::Strike, CardType::Duel},
           {CardType::Dodge},
           {CardType::DismantleBreak}
           ),
       CardDistribution(3,
           {CardType::SnatchSteal, CardType::Dodge},
           {CardType::DismantleBreak, CardType::Strike},
           {CardType::Dodge, CardType::Heal},
           {CardType::SnatchSteal, CardType::DismantleBreak}
           ),
       CardDistribution(4,
           {CardType::SnatchSteal, CardType::Dodge},
           {CardType::DismantleBreak, CardType::Strike},
           {CardType::HailofArrows, CardType::Heal},
           {CardType::SnatchSteal, CardType::DismantleBreak}
           ),
        CardDistribution(5,
           {CardType::Strike, CardType::Dodge},
           {CardType::Strike},
           {CardType::Heal},
           {CardType::SnatchSteal}
           ),
        CardDistribution(6,
           {CardType::Strike, CardType::Dodge},
           {CardType::Strike, CardType::Duel},
           {CardType::Heal},
           {CardType::Strike, CardType::Duel}
           ),
        CardDistribution(7,
           {CardType::Strike, CardType::Dodge},
           {CardType::Strike},
           {CardType::DrawTwo, CardType::Heal},
           {CardType::Strike, CardType::Strike}
           ),
        CardDistribution(8,
           {CardType::Dodge, CardType::Dodge},
           {CardType::Strike, CardType::Strike},
           {CardType::DrawTwo, CardType::Heal},
           {CardType::Strike, CardType::Strike}
           ),
        CardDistribution(9,
           {CardType::Strike, CardType::Dodge},
           {CardType::Strike, CardType::Strike},
           {CardType::DrawTwo, CardType::Heal},
           {CardType::Strike, CardType::Strike}
           ),
        CardDistribution(10,
           {CardType::Dodge},
           {CardType::Strike, CardType::Strike},
           {CardType::Strike, CardType::Strike},
           {CardType::Strike, CardType::Strike}
           ),
        CardDistribution(11,
           {CardType::Dodge, CardType::Dodge},
           {CardType::Strike, CardType::Strike},
           {CardType::Strike, CardType::DrawTwo},
           {CardType::Barbarians}
           ),
        CardDistribution(12,
           {CardType::Heal},
           {CardType::Strike},
           {CardType::DismantleBreak},
           {CardType::SnatchSteal, CardType::DismantleBreak}
           ),
        CardDistribution(13,
           {CardType::Strike},
           {CardType::Barbarians},
           {CardType::Dodge},
           {CardType::Barbarians}
           ),
    };

    
    for (size_t i = 0; i < (int)distribution.size(); ++i)
    {
        std::vector<CardType> const& diamondCards = distribution[i].m_suits[0];
        for (auto& diamondCard : diamondCards)
        {
            CardData newCard = CardData(distribution[i].m_rank, CardSuit::Diamond);
            newCard.SetType(diamondCard);
            s_cardCollection.push_back(newCard);
        }
        std::vector<CardType> const& clubCards = distribution[i].m_suits[1];
        for (auto& clubCard : clubCards)
        {
            CardData newCard = CardData(distribution[i].m_rank, CardSuit::Club);
            newCard.SetType(clubCard);
            s_cardCollection.push_back(newCard);
        }

        std::vector<CardType> const& heartCards = distribution[i].m_suits[2];
        for (auto& heartCard : heartCards)
        {
            CardData newCard = CardData(distribution[i].m_rank, CardSuit::Heart);
            newCard.SetType(heartCard);
            s_cardCollection.push_back(newCard);
        }
        std::vector<CardType> const& spadeCards = distribution[i].m_suits[3];
        for (auto& spadeCard : spadeCards)
        {
            CardData newCard = CardData(distribution[i].m_rank, CardSuit::Spade);
            newCard.SetType(spadeCard);
            s_cardCollection.push_back(newCard);
        }
    }

    int diamondNum = 0, clubNum = 0, heartNum = 0, spadeNum = 0;
    for (int i = 0; i < (int)s_cardCollection.size(); ++i)
    {
        switch (s_cardCollection[i].m_suit)
        {
        case CardSuit::Diamond:
            diamondNum++; break;
        case CardSuit::Club:
            clubNum++; break;
        case CardSuit::Heart:
            heartNum++; break;
        case CardSuit::Spade:
            spadeNum++; break;
        }
    }

    InitializeDeckWidget();
}

void CardDeck::InitializeDeckWidget()
{
    m_cardDeckWidget = new Textbox(g_theRenderer, AABB2((1.f - 0.065f) * SCREEN_SIZE_X, SCREEN_SIZE_Y - 0.065f * SCREEN_SIZE_X, SCREEN_SIZE_X, SCREEN_SIZE_Y), Stringf("%d", m_drawDeck.size()), g_bitmapFont, Vec2(0.5f, 0.5f), 0.03f * SCREEN_SIZE_X, 0.5f);
    m_cardDeckWidget->SetTextColor(CARDDECK_TEXT);
    m_cardDeckWidget->SetBackgroundTexture(g_theRenderer->CreateOrGetTextureFromFile("Data/Images/CardDeck/CardDeck.png"));
    m_cardDeckWidget->SetIsShowingBackground(true);

}

bool CardDeck::Event_ShuffleDeck([[maybe_unused]]EventArgs& args)
{
    if (m_drawDeck.size() == 0 && m_discardDeck.size() == 0)
    {
        m_game->AddGlobalText("Error: CAN'T DRAW CARDS/SHUFFLE CARD BECAUSE THE PLAYERS HOLD ALL CARDS", Rgba8::RED);
    }
   m_discardDeck.reserve(m_discardDeck.size() + m_drawDeck.size());
   while (!m_drawDeck.empty())
   {
       m_discardDeck.push_back(m_drawDeck.front());
       m_drawDeck.pop_front();
   }
   std::vector <CardData> tempDeck(m_discardDeck);
   for (size_t i = 0; i < tempDeck.size(); ++i)
   {
       int swapIndex = g_rng->RollRandomIntInRange(0, (int)tempDeck.size()-1);
       std::swap(tempDeck[i], tempDeck[swapIndex]);
   }
   for (size_t i = 0; i < tempDeck.size(); ++i)
   {
       m_drawDeck.push_front(tempDeck[i]);
   }
   m_discardDeck.clear();
   g_theConsole->AddLine(Rgba8::WHITE,"Shuffle Deck Success");
   m_cardDeckWidget->SetTextLabel(Stringf("%d", m_drawDeck.size()));
   return true;
}

void CardDeck::DrawCards(std::vector<CardData> & cards, int cardNum, std::string & drawCardNames)
{
    if (cardNum == 0)
    {
        m_game->AddGlobalText("Error: draw card number is 0", Rgba8::RED);
        return;
    }
    
    for (int i = 0; i < cardNum -1; ++i)
    {
        if (m_drawDeck.size() == 0)
        {
            FireEvent("ShuffleDeck");
        }
        drawCardNames += m_drawDeck.front().m_displayName;
        drawCardNames += ", ";
        cards.push_back(m_drawDeck.front());
        m_drawDeck.pop_front();
    }

    drawCardNames += m_drawDeck.front().m_displayName;
    cards.push_back(m_drawDeck.front());
    m_drawDeck.pop_front();
    m_cardDeckWidget->SetTextLabel(Stringf("%d", m_drawDeck.size()));
}

void CardDeck::DiscardSingleCard(CardData const& card)
{
    m_discardDeck.push_back(card);
}

void CardDeck::DiscardCards(std::vector<CardData> const& cards)
{
    m_discardDeck.insert(m_discardDeck.end(), cards.begin(), cards.end());
}

int CardDeck::GetCardsNumber() const
{
    return (int)m_drawDeck.size();
}

void CardDeck::DrawSpecificCard(std::vector<CardData>& cards, CardType const& specificCardType)
{
    for (auto it = m_drawDeck.begin(); it != m_drawDeck.end(); ++it)
    {
        if (it->m_type == specificCardType)
        {
            cards.push_back(*it);
            m_drawDeck.erase(it);
            return;
        }
    }
    m_game->AddGlobalText("Error: draw pile doesn't have this type card", Rgba8::RED);
}
