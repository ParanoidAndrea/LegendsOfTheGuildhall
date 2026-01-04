#include "ManipulateWidget.hpp"
#include "Game/Card.hpp"
#include "Game/Gamecommon.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Game/Player.hpp"
#include "Engine/Core/NetSystem.hpp"
ManipulateCard::ManipulateCard(CardData const& data, AABB2 const& widgetArea) :Button(widgetArea, g_theRenderer, "Data/Images/Card/CardBack.png",Rgba8::WHITE, "", "RobotoMonoSemiBold128")
    ,m_data(data)
{
//     EventArgs args;
//     args.SetValue("CardIndex", originalCardIndex);
//     SetArgs(args);
    m_isHiddenBorderOnHover = false;
}
ManipulateWdiget::ManipulateWdiget(Renderer* render, AABB2 const& widgetArea)
    :Widget(render, widgetArea)
{
    g_theEventSystem->SubscribeEventCallbackObjectMethod("ManipulateCard", this, &ManipulateWdiget::Event_ManipulateCard);
}

ManipulateWdiget::~ManipulateWdiget()
{
    g_theEventSystem->UnsubscribeAllEventCallbackObjectMethods(this);
}

bool ManipulateWdiget::Event_ManipulateCard(EventArgs& args)
{
    if (m_currentSelectedIndex == -1 || m_currentPlayer == nullptr || m_currentPlayer->m_actionState != ActionState::ManipulateCards || m_currentPlayer->m_currentState != PlayerState::Action  )
    {
        return false;
    }
    args.SetValue("CardIndex", m_currentSelectedIndex);
    
    switch (m_currentEvent)
    {
    case ManipulateEvent::DismantleBreak:
        if (g_netState == NetState::REMOTE && m_currentPlayer->IsMainPlayer())
        {
            g_netSystem->Send("DiscardCardPlayer" + ToString(m_manipulatePlayerIndex), args);
        }
        args.SetValue("HasSentMessage", true);
        FireEvent("DiscardCardPlayer" + ToString(m_manipulatePlayerIndex), args);
        
        break;
    case ManipulateEvent::SnatchSteal:
        if (g_netState == NetState::REMOTE && m_currentPlayer->IsMainPlayer())
        {
            g_netSystem->Send("StealCardPlayer" + ToString(m_manipulatePlayerIndex), args);
        }
        args.SetValue("HasSentMessage", true);
        FireEvent("StealCardPlayer" + ToString(m_manipulatePlayerIndex), args);
        break;
    }
    m_currentPlayer->m_actionState = (ActionState)args.GetValue("PreviousActionState", (int)m_currentPlayer->m_actionState);
    m_currentSelectedIndex = -1;
    m_currentPlayer = nullptr;
    
    return true;
}

void ManipulateWdiget::Update()
{
    Vec2 mousePos = g_theWindow->GetMouseScreenPos();
    for (int i = 0; i < (int)m_oppoentCards.size(); ++i)
    {
        if (IsPointInsideAABB2D(mousePos, m_oppoentCards[i]->m_widgetArea) && g_theInput->WasKeyJustPressed(KEYCODE_LEFT_MOUSE))
        {
            m_currentSelectedIndex = i;
        }
        m_oppoentCards[i]->Update();

       

        if (i == m_currentSelectedIndex)
        {
            m_oppoentCards[i]->SetBorderColor(Rgba8(255, 0, 255, 255), Rgba8(255, 0, 255, 255));
        }
        else
        {
            m_oppoentCards[i]->m_isShowingBorder = false;
        }
    }
    

    
    m_confirmButton->Update();
}

void ManipulateWdiget::Render() const
{
    RenderBackground();
    for (size_t i = 0; i < m_oppoentCards.size(); ++i)
    {
        m_oppoentCards[i]->Render();
    }

    m_confirmButton->Render();
}

void ManipulateWdiget::AddCards(ManipulateCard* newCard)
{
    m_oppoentCards.push_back(newCard);
}

void ManipulateWdiget::DeleteCards()
{
    std::vector<ManipulateCard*> cardsToDelete;
    cardsToDelete.reserve(m_oppoentCards.size());

    for (ManipulateCard* card : m_oppoentCards)
    {
        if (card && card != reinterpret_cast<Widget*>(0xFFFFFFFFFFFFFFFF))
        {
            cardsToDelete.push_back(card);
        }
    }

    m_oppoentCards.clear();

    for (ManipulateCard* card : cardsToDelete)
    {
        g_uiSystem->DeleteWidget(card);
    }
}

void ManipulateWdiget::SetConfirmButtonEvent(EventArgs & args)
{
    delete m_confirmButton;
    m_confirmButton = new Button(AABB2((0.5f - 0.05f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (0.1f), (0.5f + 0.05f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (0.1f + 0.05f)), args, "Confirm", "ManipulateCard", "RobotoMonoSemiBold128", g_theRenderer, g_theAudio, g_buttonTexture, BUTTON_TEXT_NORMAL, BUTTON_TEXT_HOVER);
    m_confirmButton->DisableTextShadow();
    m_confirmButton->SetTextAspect(GLOBAL_FONT_ASPECT);
}

void ManipulateWdiget::CreateManipulateCards(std::vector<CardData> const& datas, ManipulateEvent const& event, Player* currentPlayer, int opponentPlayerIndex, EventArgs &args)
{
    DeleteCards();
    SetConfirmButtonEvent(args);
    m_currentSelectedIndex = -1;
    m_currentEvent = event;
    m_currentPlayer = currentPlayer;
    m_manipulatePlayerIndex = opponentPlayerIndex;

    AABB2 cardWidgetArea = AABB2(0.1f * SCREEN_SIZE_X, 0.1f * SCREEN_SIZE_Y, 0.9f * SCREEN_SIZE_X, 0.9f * SCREEN_SIZE_Y);
    int cardsNum = (int)datas.size();
    //m_cardsInHandWidget->ClearChildren();

    // Calculate cards per row (handle odd numbers properly)
    int cardsFirstRow = (cardsNum + 1) / 2;
    int cardsSecondRow = cardsNum - cardsFirstRow;

    // Calculate total width needed for each row with proper spacing
    float spacing = CARD_SIZE_X * 0.2f; // 20% of card width as spacing
    float firstRowWidth = (cardsFirstRow * CARD_SIZE_X) + ((cardsFirstRow - 1) * spacing);
    float secondRowWidth = (cardsSecondRow * CARD_SIZE_X) + ((cardsSecondRow - 1) * spacing);

    // Calculate available width
    float availableWidth = cardWidgetArea.m_maxs.x - cardWidgetArea.m_mins.x;

    // Calculate starting position for the first card in each row
    float firstRowStartX = cardWidgetArea.m_mins.x + (availableWidth - firstRowWidth) / 2;
    float secondRowStartX = cardWidgetArea.m_mins.x + (availableWidth - secondRowWidth) / 2;

    // First row
    for (int i = 0; i < cardsFirstRow; ++i)
    {
        AABB2 cardArea = CARD_SIZE;
        Vec2 cardCenter = Vec2(firstRowStartX + (CARD_SIZE_X / 2) + (CARD_SIZE_X + spacing) * i, 0.7f * SCREEN_SIZE_Y);
        cardArea.SetCenter(cardCenter);
        AddCards(new ManipulateCard(datas[i], cardArea));
    }
    
    // Second row
    for (int i = 0; i < cardsSecondRow; ++i)
    {
        AABB2 cardArea = CARD_SIZE;
        Vec2 cardCenter = Vec2(secondRowStartX + (CARD_SIZE_X / 2) + (CARD_SIZE_X + spacing) * i, 0.3f * SCREEN_SIZE_Y);
        cardArea.SetCenter(cardCenter);
        AddCards(new ManipulateCard(datas[cardsFirstRow + i], cardArea));
    }

}


