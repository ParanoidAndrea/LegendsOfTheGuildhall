#include "Game/Card.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Game/Gamecommon.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/UI/Textbox.hpp"
#include "Engine/Math/Easing.hpp"
#include "Engine/Core/Timer.hpp"
#include "Engine/Core/Clock.hpp"
#include "Game/CardManager.hpp"
#include "Game/Player.hpp"
#include "Engine/Core/NetSystem.hpp"
#include "Game/Game.hpp"
CardData::CardData(int rank, CardSuit const& suit)
    :m_rank(rank), m_suit(suit)
{

}

void CardData::SetType(CardType const& type)
{
    m_type = type;
    SetMainType(type);
    SetDisplayName(type);
    SetDescritpion(type);
}

void CardData::SetMainType(CardType const& type)
{
    switch (type)
    {
    case CardType::Strike:
        m_mainType = CardMainType::Basic;
        break;
    case CardType::Dodge:
        m_mainType = CardMainType::Basic;
        break;
    case CardType::Heal:
        m_mainType = CardMainType::Basic;
        break;
    case CardType::DismantleBreak:
        m_mainType = CardMainType::ScrollTool;
        break;
    case CardType::SnatchSteal:
        m_mainType = CardMainType::ScrollTool;
        break;
    case CardType::DrawTwo:
        m_mainType = CardMainType::ScrollTool;
        break;
    case CardType::Duel:
        m_mainType = CardMainType::ScrollTool;
        break;
    case CardType::Barbarians:
        m_mainType = CardMainType::ScrollTool;
        break;
    case CardType::HailofArrows:
        m_mainType = CardMainType::ScrollTool;
        break;
    case CardType::WardNegate:
        m_mainType = CardMainType::ScrollTool;
        break;
    case CardType::AcediaCapture:
        m_mainType = CardMainType::ScrollTool;
        break;
    case CardType::Lightning:
        m_mainType = CardMainType::ScrollTool;
        break;
    case CardType::Weapon:
        m_mainType = CardMainType::Equipment;
        break;
    case CardType::Armor:
        m_mainType = CardMainType::Equipment;
        break;
    case CardType::HorsePlus:
        m_mainType = CardMainType::Equipment;
        break;
    case CardType::HorseNegative:
        m_mainType = CardMainType::Equipment;
        break;
    case CardType::NUMS_COUNT:
        m_mainType = CardMainType::UNKNOWN;
        break;
    }
}

void CardData::SetDisplayName(CardType const& type)
{
    switch (type)
    {
    case CardType::Strike:
        m_displayName = "Strike";
        break;
    case CardType::Dodge:
        m_displayName = "Dodge";
        break;
    case CardType::Heal:
        m_displayName = "Heal";
        break;
    case CardType::DismantleBreak:
        m_displayName = "Dismantle Break";
        break;
    case CardType::SnatchSteal:
        m_displayName = "Snatch Steal";
        break;
    case CardType::DrawTwo:
        m_displayName = "Draw Two";
        break;
    case CardType::Duel:
        m_displayName = "Duel";
        break;
    case CardType::Barbarians:
        m_displayName = "Barbarians";
        break;
    case CardType::HailofArrows:
        m_displayName = "Hail of Arrows";
        break;
    case CardType::WardNegate:
        m_displayName = "Ward Negate";
        break;
    case CardType::AcediaCapture:
        m_displayName = "Acadia Capture";
        break;
    case CardType::Lightning:
        m_displayName = "Lightning";
        break;
    case CardType::Weapon:
        m_displayName = "Unnamed Weapon";
        break;
    case CardType::Armor:
        m_displayName = "Unnamed Armor";
        break;
    case CardType::HorsePlus:
        m_displayName = "Horse +1";
        break;
    case CardType::HorseNegative:
        m_displayName = "Horse -1";
        break;
    case CardType::NUMS_COUNT:
        m_displayName = "UNKNOWN";
        break;
    }
}

void CardData::SetDescritpion(CardType const& type)
{
    switch (type)
    {
    case CardType::Strike:
        m_description = "Strike";
        break;
    case CardType::Dodge:
        m_description = "Dodge";
        break;
    case CardType::Heal:
        m_description = "Heal";
        break;
    case CardType::DismantleBreak:
        m_description = "Dismantle Break";
        break;
    case CardType::SnatchSteal:
        m_description = "Snatch Steal";
        break;
    case CardType::DrawTwo:
        m_description = "Draw Two";
        break;
    case CardType::Duel:
        m_description = "Duel";
        break;
    case CardType::Barbarians:
        m_description = "Barbarians";
        break;
    case CardType::HailofArrows:
        m_description = "Hail of Arrows";
        break;
    case CardType::WardNegate:
        m_description = "Ward Negate";
        break;
    case CardType::AcediaCapture:
        m_description = "Acadia Capture";
        break;
    case CardType::Lightning:
        m_description = "Lightning";
        break;
    case CardType::Weapon:
        m_description = "+1";
        break;
    case CardType::Armor:
        m_description = "Unnamed Armor";
        break;
    case CardType::HorsePlus:
        m_description = "+1";
        break;
    case CardType::HorseNegative:
        m_description = "-1";
        break;
    case CardType::NUMS_COUNT:
        m_description = "UNKNOWN";
        break;
    }
}


Card::Card(Player* owner, CardData const& cardData, AABB2 const& widgetArea, Vec2 const& widgetRelativePos, Vec2 const& widgetPivot, CardManager* cardManager, size_t cardIndex, bool isCreatingNamebox) :Widget(g_theRenderer,widgetArea,widgetRelativePos,widgetPivot)
	,m_cardData(cardData)
	,m_owner(owner)
    ,m_cardManager(cardManager)
    ,m_cardIndex(cardIndex)
{
    std::string suitPath = "";
    switch (cardData.m_suit)
    {
    case CardSuit::Diamond:
        suitPath ="Data/Images/Card/Diamond.png";
        break;
    case CardSuit::Club:
        suitPath ="Data/Images/Card/Club.png";
        break;
    case CardSuit::Heart:
        suitPath ="Data/Images/Card/Heart.png";
        break;
    case CardSuit::Spade:
        suitPath ="Data/Images/Card/Spade.png";
        break;
    }
    Widget* suitWidget  = new Widget(g_theRenderer, AABB2(0.f, 0.f, 0.1f * CARD_SIZE_Y, 0.1f * CARD_SIZE_Y),Vec2(0.f,1.f),Vec2(0.f,1.f),suitPath);
    Textbox* rankBox = new Textbox(g_theRenderer, Vec2(1.f, 1.f), Vec2(0.15f, 1.f), Vec2(0.f, 1.f), Stringf("%d", cardData.m_rank), g_bitmapFont, Vec2(0.f, 1.f), 0.1f * CARD_SIZE_Y, GLOBAL_FONT_ASPECT, Rgba8::BLACK, false);
    AddChild(suitWidget);AddChild(rankBox);
    if (isCreatingNamebox)
    {
        Textbox* nameBox = new Textbox(g_theRenderer, Vec2(1.f, 0.25f), Vec2(0.5f, 0.75f), Vec2(0.5f, 0.5f), cardData.m_displayName, g_bitmapFont, Vec2(0.5f, 0.5f), 0.25f * CARD_SIZE_Y, GLOBAL_FONT_ASPECT, Rgba8::BLACK, false);

        AddChild(nameBox);
    }
    ChangePlayableColor();

}

Card::~Card()
{
    delete m_transitionTimer;
    m_transitionTimer = nullptr;
    m_isHovering = false;
    m_isSelecting= false;
}

void Card::ChangePlayableColor()
{
    if (!IsPlayable())
    {
        m_borderColor = Rgba8::BLACK;
        m_renderColor = Rgba8::GREY;
    }
    else
    {
        m_borderColor = Rgba8::BLACK;
        m_renderColor = Rgba8::WHITE;
    }
}

bool Card::InCardArea(Vec2 const& mousePos) const
{
	return IsPointInsideAABB2D(mousePos, m_widgetArea);
}

void Card::Update()
{
    if (m_owner->IsMainPlayer())
    {
        ChangePlayableColor();
        
        if (m_owner->IsDiscardPhrase())
        {
            DiscardPhraseUpdateCardStatus();
        }
        else
        {
            if (!IsPlayable())
            {
                return;
            }
            NormalUpdateCardStatus();
        }

        if (m_isHovering)
        {
            OnHover(m_transitionTimer);
        }
        if (m_isSelecting)
        {
            OnSelect(m_transitionTimer);
        }
        
    }
}

void Card::NormalUpdateCardStatus()
{

    //Hover
    if (m_cardManager->m_hoverIndex == -1 && InCardArea(g_theRenderer->GetRenderConfig().m_window->GetMouseScreenPos()))
    {
        StartHover();
    }
    else if (!InCardArea(g_theRenderer->GetRenderConfig().m_window->GetMouseScreenPos()))
    {
        if (m_isHovering)
        {
            EndHover();
        }
    }

    //Select
    if (g_theInput->WasKeyJustPressed(KEYCODE_LEFT_MOUSE))
    {
        if (InCardArea(g_theRenderer->GetRenderConfig().m_window->GetMouseScreenPos()))
        {
            if (!m_isSelecting)
            {
                m_cardManager->DisselectCurrentCards();
                StartSelect();
            }
            else
            {
                EndSelect();
            }
            
        }
        //else if (m_owner->IsDiscardPhrase())
        //{
        //    if (m_isSelecting)
        //    {
        //        EndSelect();
        //    }
        //}
    }
}

void Card::DiscardPhraseUpdateCardStatus()
{
    if (m_cardManager->m_hoverIndex == -1 && InCardArea(g_theRenderer->GetRenderConfig().m_window->GetMouseScreenPos()))
    {
        StartHover();
    }
    else if (!InCardArea(g_theRenderer->GetRenderConfig().m_window->GetMouseScreenPos()))
    {
        if (m_isHovering)
        {
            EndHover();
        }
    }

    if (g_theInput->WasKeyJustPressed(KEYCODE_LEFT_MOUSE))
    {
        
        if (InCardArea(g_theRenderer->GetRenderConfig().m_window->GetMouseScreenPos()))
        {
            if (!m_isSelecting)
            {
                if (m_owner->m_discardNum == m_cardManager->m_selectedIndexes.size())
                {
                    m_cardManager->m_currentCards[m_cardManager->m_selectedIndexes.front()]->EndSelect();
                    //m_cardManager->DeselectCard(m_cardManager->m_currentCards.);
                }
                StartSelect();
            }
            else
            {
                EndSelect();
            }
        }
        //else if (m_owner->IsDiscardPhrase())
        //{
        //    if (m_isSelecting)
        //    {
        //        EndSelect();
        //    }
        //}
    }
}

void Card::OnClick()
{
}

bool Card::StartHover()
{
    //if (m_cardManager->m_isHovering)
    //{
    //    return false;
    //}
    if (!m_isSelecting && !m_isHovering)
    {
        m_transitionTimer = new Timer(0.1f);
        m_transitionTimer->Start();
        m_isHovering = true;
        m_cardManager->m_hoverIndex = (int)m_cardIndex;
        return true;
    }
    else
    {
        return false;
    }
}

void Card::EndHover()
{
    if (!m_isSelecting)
    {
        m_isHovering = false;
        m_cardManager->m_hoverIndex = -1;
        m_transitionTimer->Stop();
        ResetLocalTransform();
    }
}

void Card::OnSelect(Timer* selectTimer)
{
    if (m_status == CardStatus::InHand)
    {
        if (m_localPosition.y >= INHAND_CARD_MAX_Y || selectTimer->HasPeriodElapsed())
        {

            m_localPosition.y = INHAND_CARD_MAX_Y;
            m_widgetArea.SetDimensions(Vec2(SELECT_CARD_SCALE * CARD_SIZE_X, SELECT_CARD_SCALE * CARD_SIZE_Y));
            //_widgetArea.SetCenter(m_originalCenter + Vec2(0.f, INHAND_CARD_MAX_Y));
        }
        else if (!selectTimer->HasPeriodElapsed())
        {
            
            m_localPosition.y = Interpolate(0.f,INHAND_CARD_MAX_Y,SmoothStep3(selectTimer->GetElapsedFraction())); 
            float rangeFrac = RangeMapClamped(SmoothStep3(selectTimer->GetElapsedFraction()), 0.f, 1.f, HOVER_CARD_SCALE, SELECT_CARD_SCALE);
            m_widgetArea.SetDimensions(Vec2(rangeFrac * CARD_SIZE_X, rangeFrac * CARD_SIZE_Y));
            //m_widgetArea.InterpolateFromCenter(m_originalCenter + Vec2(0.f, INHAND_CARD_MAX_Y), SmoothStep3(selectTimer->GetElapsedFraction()));
        }
       // m_
        //Mat44 transform = Mat44::CreateTranslation2D(m_widgetArea.GetCenter()-m_originalCenter);
        //PrintLineToDebug(Stringf("%f", selectTimer->GetElapsedFraction()));
        Mat44 transform = Mat44::CreateTranslation2D(m_localPosition);
        //m_widgetArea.Translate()
        SetNewTransform(transform);
        
    }
}

bool Card::StartSelect()
{
    m_transitionTimer = new Timer(0.25f);
    m_transitionTimer->Start();
    m_isSelecting = true;

    m_cardManager->SelectCard((int)m_cardIndex);
    if (g_netState == NetState::REMOTE)
    {
        g_netSystem->Send("SelectCardPlayer" + ToString(m_owner->m_playerIndex) + " CardIndex=" + ToString((int)m_cardIndex));
    }
    m_isHovering = false;
    m_cardManager->m_hoverIndex = -1;
    return true;

}
void Card::EndSelect()
{
    m_isSelecting = false;
    m_cardManager->DeselectCard((int)m_cardIndex);
    ResetLocalTransform();
}

bool Card::IsOwnerThePlayerState(PlayerState const& playerState) const
{
    return m_owner->m_currentState == playerState;
}

bool Card::IsOwnerTheActionState(ActionState const& actionState) const
{
    return m_owner->m_actionState == actionState;
}

void Card::OnDrag()
{

}

void Card::OnHover(Timer* timer)
{
    if (timer->HasPeriodElapsed())
    {
        m_widgetArea.SetDimensions(Vec2(HOVER_CARD_SCALE * CARD_SIZE_X, HOVER_CARD_SCALE * CARD_SIZE_Y));
    }
    else
    {
        float rangeFrac = RangeMapClamped(SmoothStep3(timer->GetElapsedFraction()), 0.f, 1.f, 1.f, HOVER_CARD_SCALE);
        m_widgetArea.SetDimensions(Vec2(rangeFrac * CARD_SIZE_X, rangeFrac * CARD_SIZE_Y));
    }
}

void Card::Discard()
{
}

void Card::OnDiscard(Timer* timer)
{
    UNUSED(timer);
}



void Card::ResetLocalTransform()
{
    m_localPosition = Vec2();
    //m_widgetArea.SetCenter(m_originalCenter);
    m_widgetArea.SetDimensions(Vec2(CARD_SIZE_X, CARD_SIZE_Y));
    SetNewTransform(m_orientation.GetAsMatrix_IFwd_JLeft_KUp());
}

void Card::SetNewTransform(Mat44 const& transform)
{
    m_transformMatrix = transform;
    for (auto child : m_children)
    {
        child->m_transformMatrix = transform;
    }
}

bool Card::IsSelectable() const
{
   return m_owner->IsDiscardPhrase() || IsPlayable();
}

bool Card::IsPlayable() const
{
    return m_owner->IsDiscardPhrase();
}

bool Card::IsScrollToolCardPlayable() const
{
   return (IsOwnerTheActionState(ActionState::HaveNotPlayedStrike) || IsOwnerTheActionState(ActionState::HavePlayedStrike)) && IsOwnerThePlayerState(PlayerState::Action) || IsOwnerThePlayerState(PlayerState::Discard);
}

Player* Card::GetOpponentPlayer() const
{
    return m_owner->m_game->GetAnotherPlayer();
}

int Card::GetMainPlayerIndex() const
{
    return m_owner->m_playerIndex;
}

int Card::GetOpponentPlayerIndex() const
{
    return m_owner->GetAnotherPlayer()->m_playerIndex;
}

