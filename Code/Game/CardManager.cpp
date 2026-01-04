#include "Game/CardManager.hpp"
#include "Game/Gamecommon.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Core/Timer.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Game/BasicCard.hpp"
#include "Game/ScrollToolCard.hpp"

Card* CardManager::PushNewCard(Player* player, CardData const& cardData, Vec2 const& cardRelativePos, int cardIndex)
{

    AABB2 cardBound = AABB2(0.f, 0.f, CARD_SIZE_X, CARD_SIZE_Y);
    Vec2 cardPivot = Vec2(0.5f, 0.5f);
    Card* newCard = nullptr;
    switch (cardData.m_type)
    {
    case CardType::Strike:
        newCard = new Strike(player, cardData, cardBound, cardRelativePos, cardPivot, this, cardIndex);
        break;
    case CardType::Dodge:
        newCard = new Dodge(player, cardData, cardBound, cardRelativePos, cardPivot, this, cardIndex);
       break;
    case CardType::Heal:
        newCard = new Health(player, cardData, cardBound, cardRelativePos, cardPivot, this, cardIndex);
        break;
    case CardType::DismantleBreak:
        newCard = new DismantleBreak(player, cardData, cardBound, cardRelativePos, cardPivot, this, cardIndex);
        break;
    case CardType::SnatchSteal:
        newCard = new SnatchSteal(player, cardData, cardBound, cardRelativePos, cardPivot, this, cardIndex);
        break;
    case CardType::DrawTwo:
        newCard = new DrawTwo(player, cardData, cardBound, cardRelativePos, cardPivot, this, cardIndex);
        break;
    case CardType::Duel:
        newCard = new Duel(player, cardData, cardBound, cardRelativePos, cardPivot, this, cardIndex);
        break;
    case CardType::Barbarians:
        newCard = new Barbarians(player, cardData, cardBound, cardRelativePos, cardPivot, this, cardIndex);
        break;
    case CardType::HailofArrows:
        newCard = new HailOfArrows(player, cardData, cardBound, cardRelativePos, cardPivot, this, cardIndex);
        break;
    //case CardType::WardNegate:
    //    break;
    //case CardType::AcediaCapture:
    //    break;
    //case CardType::Lightning:
    //    break;
    //case CardType::Weapon:
    //    break;
    //case CardType::Armor:
    //    break;
    //case CardType::HorsePlus:
    //    break;
    //case CardType::HorseNegative:
    //    break;
    default:
        newCard = new Card(player, cardData, cardBound, cardRelativePos, cardPivot, this, cardIndex);
        break;
    }
    //Card* newCard = new Card(player, cardData, cardBound, cardRelativePos, cardPivot, this, cardIndex);

//    newCard->SetBorderColor(Rgba8(87,43,1,255), Rgba8::GREY);
    m_currentCards.push_back(newCard);
    return newCard;
}

void CardManager::Update()
{
    for (int i = 0; i < (int)m_currentCards.size(); ++i)
    {
        if (m_currentCards[i])
        m_currentCards[i]->Update();
    }
}



void CardManager::Render() const
{
    g_theRenderer->BeginRenderEvent("Card");
    for (int i = 0; i < (int)m_currentCards.size(); ++i)
    {
        if (!m_currentCards[i]->m_isSelecting && !m_currentCards[i]->m_isHovering)
        {
            m_currentCards[i]->Render();
        }
    }

    for (auto index : m_selectedIndexes)
    {
        m_currentCards[index]->Render();
    }

    for (int i = 0; i < (int)m_currentCards.size(); ++i)
    {
        if (m_currentCards[i]->m_isHovering)
        {
            m_currentCards[i]->Render();
        }
    }
    g_theRenderer->BeginRenderEvent("End");
}

void CardManager::ResetCards()
{
    for (Card* card : m_currentCards)
    {
        card->ResetLocalTransform();
    }
}

void CardManager::DisselectCurrentCards()
{
    auto it = m_selectedIndexes.begin();
    while (it != m_selectedIndexes.end())
    {
        m_currentCards[*it]->m_isSelecting = false;
        m_currentCards[*it]->ResetLocalTransform();
        ++it;
    }
    m_selectedIndexes.clear();
}

void CardManager::DiscardSelectedCard(std::string & selectCardName)
{
    auto it = m_selectedIndexes.begin();
    while (it != m_selectedIndexes.end())
    {
        selectCardName += m_currentCards[*it]->m_cardData.m_displayName;
        auto nextIt = it;
        ++nextIt;
        if (nextIt != m_selectedIndexes.end())
        {
            selectCardName += ", ";
        }
        m_currentCards[*it]->Discard();
        ++it;
    }
    //m_selectedIndexes.clear();
}

void CardManager::SelectCard(int index)
{
    auto it = std::find(m_selectedIndexes.begin(), m_selectedIndexes.end(), index);

    if (it == m_selectedIndexes.end())
    {
        m_selectedIndexes.push_back(index);
    }
    m_lastSelectIndex = index;
}

void CardManager::DeselectCard(int selectedIndex)
{
    auto it = std::find(m_selectedIndexes.begin(), m_selectedIndexes.end(), selectedIndex);

    if (it != m_selectedIndexes.end())
    {
        m_selectedIndexes.erase(it);
    }
    //_currentCards[selectedIndex]->EndSelect();
}

void CardManager::PlayCard(int index)
{
    m_currentCards[index]->Play();
}

int CardManager::GetFirstSelectIndex() const
{
    if (m_selectedIndexes.size() > 0)
    {
        return m_selectedIndexes.front();
    }
    else
    {
        return -1;
    }
}

CardType const CardManager::GetCardType(int index) const
{
    return m_currentCards[index]->m_cardData.m_type;
}

CardType const CardManager::GetSelectCardType() const
{
    if (m_selectedIndexes.size() > 0)
    {
        return m_currentCards[m_selectedIndexes.front()]->m_cardData.m_type;
    }
    else
    {
        return CardType::NUMS_COUNT;
    }
}
