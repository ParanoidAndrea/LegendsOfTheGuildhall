#include "Game/Player.hpp"
#include "Game/CardDeck.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Game/Game.hpp"
#include "Engine/UI/Textbox.hpp"
#include "Engine/UI/ScrollBox.hpp"
#include "Engine/Core/NetSystem.hpp"
#include "Engine/UI/Button.hpp"
#include "Engine/Core/Timer.hpp"
#include "Game/ManipulateWidget.hpp"
#include "Game/PlayerController.hpp"
#include <algorithm>
Player::Player(Game* game, int playerIndex, std::string const& playerName, std::string const& playerIconPath) :m_game(game), m_playerIndex(playerIndex), m_playerIconPath(playerIconPath), m_playerName(playerName)
{
    m_camera = Camera(Vec2(), Vec2(SCREEN_SIZE_X, SCREEN_SIZE_Y));
    m_cardManager = new CardManager();
    m_buttonWidget = new Widget(g_theRenderer, SCREEN_AREA);
    m_buttonWidget->SetIsShowingBackground(false);
    m_manipulateWidget = new ManipulateWdiget(g_theRenderer, SCREEN_AREA);
    m_manipulateWidget->m_renderColor = Rgba8(0, 0, 0, 125);
    m_transitionTimer = new Timer(0.75f, g_theGameClock);
    InitializeCardsInHandWidget();
    InitializeWidgets();
    InitializeCenteralInfoWidget();
    InitializeGameInfoScrollBox();
    PlayerInitialization();
    CreateNormalButton();
    SubscribeEvents();
}

Player::~Player()
{
    g_theEventSystem->UnsubscribeAllEventCallbackObjectMethods(this);
    delete m_controller;
    m_controller = nullptr;
}

void Player::SetController(PlayerController* controller)
{
    delete m_controller;
    m_controller = controller;
}


void Player::SubscribeEvents()
{
    g_theEventSystem->SubscribeEventCallbackObjectMethod("SelectCardPlayer"              + ToString(m_playerIndex), this, &Player::Event_SelectCard);
    g_theEventSystem->SubscribeEventCallbackObjectMethod("DiscardSelectedCardsPlayer"    + ToString(m_playerIndex), this, &Player::Event_DiscardSelectCards);
    g_theEventSystem->SubscribeEventCallbackObjectMethod("StealCardPlayer"               + ToString(m_playerIndex), this, &Player::Event_StealCard);
    g_theEventSystem->SubscribeEventCallbackObjectMethod("DiscardCardPlayer"             + ToString(m_playerIndex), this, &Player::Event_DiscardCard);
    g_theEventSystem->SubscribeEventCallbackObjectMethod("DrawCardsPlayer"               + ToString(m_playerIndex), this, &Player::Event_DrawCards);
    g_theEventSystem->SubscribeEventCallbackObjectMethod("HealPlayer"                    + ToString(m_playerIndex), this, &Player::Event_Heal);
    g_theEventSystem->SubscribeEventCallbackObjectMethod("TakeDamagePlayer"              + ToString(m_playerIndex), this, &Player::Event_TakeDamage);

    g_theEventSystem->SubscribeEventCallbackObjectMethod("StartTurnPlayer"               + ToString(m_playerIndex), this, &Player::Event_StartPlayerTurn);
    g_theEventSystem->SubscribeEventCallbackObjectMethod("PlayCardPlayer"                + ToString(m_playerIndex), this, &Player::Event_PlayCard);
    g_theEventSystem->SubscribeEventCallbackObjectMethod("NeedToRespondDodgePlayer"      + ToString(m_playerIndex), this, &Player::Event_NeedToRespondDodge);
    g_theEventSystem->SubscribeEventCallbackObjectMethod("NeedToRespondStrikePlayer"     + ToString(m_playerIndex), this, &Player::Event_NeedToRespondStrike);
    g_theEventSystem->SubscribeEventCallbackObjectMethod("NeedToRespondDuelPlayer"       + ToString(m_playerIndex), this, &Player::Event_NeedToRespondDuel);
    g_theEventSystem->SubscribeEventCallbackObjectMethod("InitializeDuelPlayer"          + ToString(m_playerIndex), this, &Player::Event_InitialzieDuel);
    g_theEventSystem->SubscribeEventCallbackObjectMethod("RespondSuccessfullyPlayer"     + ToString(m_playerIndex), this, &Player::Event_RespondSuccessfully);

    g_theEventSystem->SubscribeEventCallbackObjectMethod("RespondTakeDamagePlayer"       + ToString(m_playerIndex), this, &Player::Event_RespondTakeDamage);
    g_theEventSystem->SubscribeEventCallbackObjectMethod("RespondDuelTakeDamagePlayer"   + ToString(m_playerIndex), this, &Player::Event_RespondDuelTakeDamage);
    g_theEventSystem->SubscribeEventCallbackObjectMethod("ManipulateOpponentCardPlayer"  + ToString(m_playerIndex), this, &Player::Event_ManipulateOpponentCard);
    g_theEventSystem->SubscribeEventCallbackObjectMethod("ReactToBeManipulatedCard"      + ToString(m_playerIndex), this, &Player::Event_ReactToBeManipulatedCard);

    g_theEventSystem->SubscribeEventCallbackObjectMethod("EndInitialPhrasePlayer"        + ToString(m_playerIndex), this, &Player::Event_EndInitialPhrase);
    g_theEventSystem->SubscribeEventCallbackObjectMethod("EndJudgementPhrasePlayer"      + ToString(m_playerIndex), this, &Player::Event_EndJudgementPhrase);
    g_theEventSystem->SubscribeEventCallbackObjectMethod("EndActionPhrasePlayer"         + ToString(m_playerIndex), this, &Player::Event_EndActionPhrase);
    g_theEventSystem->SubscribeEventCallbackObjectMethod("EndDiscardPhrasePlayer"        + ToString(m_playerIndex), this, &Player::Event_EndDiscardPhrase);
    g_theEventSystem->SubscribeEventCallbackObjectMethod("EndEndPhrasePlayer"            + ToString(m_playerIndex), this, &Player::Event_EndEndPhrase);
    g_theEventSystem->SubscribeEventCallbackObjectMethod("DebugDrawCardPlayer"           + ToString(m_playerIndex), this, &Player::Debug_DrawSpecificCard);

    g_theEventSystem->SubscribeEventCallbackObjectMethod("EnterDyingPhrasePlayer"        + ToString(m_playerIndex), this, &Player::Event_EnterDyingPhrase);
    g_theEventSystem->SubscribeEventCallbackObjectMethod("DyingHealPlayer"               + ToString(m_playerIndex), this, &Player::Event_DyingHeal);
    g_theEventSystem->SubscribeEventCallbackObjectMethod("LosePlayer"                    + ToString(m_playerIndex), this, &Player::Event_Lose);
    g_theEventSystem->SubscribeEventCallbackObjectMethod("VictoryPlayer"                 + ToString(m_playerIndex), this, &Player::Event_Victory);
}

void Player::InitializeWidgets()
{

    InitializePlayerWidget();
    InitializeMaxHealthBarWidgets();
    InitializeCurrentHealthWidgets();
    AABB2 numBox = IsMainPlayer() ? AABB2((EQUIPMENT_PADDING_X + CARD_PADDING_X - 0.06f) * SCREEN_SIZE_X, CARD_PADDING_Y * SCREEN_SIZE_Y - 0.05f * SCREEN_SIZE_X, (EQUIPMENT_PADDING_X + CARD_PADDING_X - 0.01f) * SCREEN_SIZE_X, CARD_PADDING_Y * SCREEN_SIZE_Y) :
        AABB2((0.5f - 0.5f * PLAYER_PADDING_X - 0.06f) * SCREEN_SIZE_X, (1.f - CARD_PADDING_Y) * SCREEN_SIZE_Y, (0.5f - 0.5f * PLAYER_PADDING_X - 0.01f) * SCREEN_SIZE_X, (1.f - CARD_PADDING_Y) * SCREEN_SIZE_Y + 0.05f * SCREEN_SIZE_X);
    m_cardsInHandNumsText = new Textbox(g_theRenderer, numBox, Stringf("%d", m_carddataInHand.size()), g_bitmapFont, Vec2(0.5f, 0.5f), 0.2f * CARD_PADDING_Y * SCREEN_SIZE_Y, GLOBAL_FONT_ASPECT, CARDDECK_TEXT, true);
    //m_cardsInHandNumsText->AddChild(new Widget(g_theRenderer, Vec2(1.f, 1.f), Vec2(0.5f, 0.5f), Vec2(0.5f, 0.5f), "Data/Images/CardDeck/CardsInHand.png"));
    m_cardsInHandNumsText->SetBackgroundTexture(g_theRenderer->CreateOrGetTextureFromFile("Data/Images/CardDeck/CardsInHand.png"));
    m_cardsInHandNumsText->SetIsShowingBackground(true);
}

void Player::PlayerInitialization()
{

    m_currentState = PlayerState::Initial;
    //m_cardsInHandNumsText->SetBorderColor(Rgba8::GREY, Rgba8::WHITE);
}
void Player::InitializeCardsInHandWidget()
{
    delete m_cardsInHandWidget;
    m_cardsInHandWidget = new Widget(g_theRenderer, AABB2(EQUIPMENT_PADDING_X* SCREEN_SIZE_X, 0.f, (EQUIPMENT_PADDING_X + CARD_PADDING_X) * SCREEN_SIZE_X, CARD_PADDING_Y * SCREEN_SIZE_Y));
    //m_cardsInHandWidget->SetBorderColor(Rgba8::BLACK, Rgba8::GREY);
    m_cardsInHandWidget->SetIsShowingBackground(false);

}

void Player::InitializePlayerWidget()
{
    delete m_playerWidget;
    AABB2 box = IsMainPlayer() ? AABB2((EQUIPMENT_PADDING_X + CARD_PADDING_X) * SCREEN_SIZE_X, 0.f, (EQUIPMENT_PADDING_X + CARD_PADDING_X + PLAYER_PADDING_X) * SCREEN_SIZE_X, CARD_PADDING_Y * SCREEN_SIZE_Y) : AABB2((0.5f - 0.5f * PLAYER_PADDING_X) * SCREEN_SIZE_X, (1.f - CARD_PADDING_Y) * SCREEN_SIZE_Y, (0.5f + 0.5f * PLAYER_PADDING_X) * SCREEN_SIZE_X, SCREEN_SIZE_Y);
    m_playerWidget = new Widget(g_theRenderer, box, "Data/Images/Player/PlayerBackground2.png");
    Textbox* nameString = new Textbox(g_theRenderer, Vec2(0.5f, 0.1f), Vec2(0.1f, 1.f), Vec2(0.f, 1.f), m_playerName, g_bitmapFont, Vec2(0.f, 0.5f), 0.1f * CARD_PADDING_Y * SCREEN_SIZE_Y, GLOBAL_FONT_ASPECT, Rgba8::BLACK, false);
    m_playerWidget->AddChild(new Widget(g_theRenderer, box, "Data/Images/Player/" + m_playerIconPath));
    m_playerWidget->SetBorderColor(Rgba8(87,43,1,255), Rgba8::GREY);
    m_playerWidget->AddChild(nameString);
}

void Player::InitializeCenteralInfoWidget()
{
    m_centralInfoText = new Textbox(g_theRenderer, AABB2(0.f, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.025f), SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.075f)), "", g_bitmapFont, Vec2(0.5f, 0.5f), SCREEN_SIZE_Y * 0.045f, GLOBAL_FONT_ASPECT, Rgba8::WHITE);

    //m_centralInfoText ->SetIsShowingBackground(true);
}

void Player::CreateNormalButton()
{
    m_buttonWidget->DeleteChildren();
    EventArgs args;
    //args.SetValue("PlayerIndex", "0");
    args.SetValue("CardNumber", 4);
//     Button* discardButton = new Button(AABB2((0.5f - 0.15f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.1f), (0.5f - 0.05f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.15f)), args, "Discard", "DiscardSelectedCardsPlayer"+ToString(m_playerIndex), "RobotoMonoSemiBold128", g_theRenderer);
// 
//     Button* drawButton = new Button(AABB2((0.5f + 0.05f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.1f), (0.5f + 0.15f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.15f)), args, "Draw 4", "DrawCardsPlayer"+ToString(m_playerIndex), "RobotoMonoSemiBold128", g_theRenderer);
//     //drawButton->SetValue("CardNumber", 4);
//     Button* healButton = new Button(AABB2((0.5f + 0.05f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.2f), (0.5f + 0.15f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.25f)), args, "Heal", "HealPlayer"+ToString(m_playerIndex), "RobotoMonoSemiBold128", g_theRenderer);
// 
//     Button* damageButton = new Button(AABB2((0.5f - 0.15f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.2f), (0.5f - 0.05f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.25f)), args, "Take Damage", "TakeDamagePlayer"+ToString(m_playerIndex), "RobotoMonoSemiBold128", g_theRenderer);
// 
//     m_buttonWidget->m_children.push_back(discardButton);
//     m_buttonWidget->m_children.push_back(drawButton);
//     m_buttonWidget->m_children.push_back(healButton);
//     m_buttonWidget->m_children.push_back(damageButton);
    if (m_isDebug)
    {
        m_buttonWidget->m_children.push_back(new Button(AABB2((0.5f + 0.2f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.15f), (0.5f + 0.3f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.2f)), args, "Switch Player", "SwitchPlayer", "RobotoMonoSemiBold128", g_theRenderer, g_theAudio, g_buttonTexture,BUTTON_TEXT_NORMAL, BUTTON_TEXT_HOVER));
    }
    for (Widget* widget : m_buttonWidget->m_children)
    {
        Button* button = dynamic_cast<Button*>(widget);
        if (button)
        {
            //button->SetTextNormalColorAndHoverColor(BUTTON_TEXT_NORMAL, BUTTON_TEXT_HOVER);
            button->DisableTextShadow();
            button->SetTextAspect(GLOBAL_FONT_ASPECT);
        }
    }
}

void Player::CreateActionPhraseButtons()
{
    m_buttonWidget->DeleteChildren();
    EventArgs args;
    Button* playButton = new Button(AABB2((0.5f - 0.15f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.1f), (0.5f - 0.05f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.15f)), args, "Play", "PlayCardPlayer" + ToString(m_playerIndex), "RobotoMonoSemiBold128", g_theRenderer, g_theAudio,g_buttonTexture,BUTTON_TEXT_NORMAL, BUTTON_TEXT_HOVER);

    Button* endButton = new Button(AABB2((0.5f + 0.05f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.1f), (0.5f + 0.15f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.15f)), args, "End", "EndActionPhrasePlayer" + ToString(m_playerIndex), "RobotoMonoSemiBold128", g_theRenderer, g_theAudio,g_buttonTexture,BUTTON_TEXT_NORMAL, BUTTON_TEXT_HOVER);
    m_buttonWidget->m_children.push_back(playButton);
    m_buttonWidget->m_children.push_back(endButton);
    if (m_isDebug)
    {
       m_buttonWidget->m_children.push_back(new Button(AABB2((0.5f + 0.2f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.15f), (0.5f + 0.3f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.2f)), args, "Switch Player", "SwitchPlayer", "RobotoMonoSemiBold128", g_theRenderer, g_theAudio,g_buttonTexture,BUTTON_TEXT_NORMAL, BUTTON_TEXT_HOVER));
    }
    for (Widget* widget : m_buttonWidget->m_children)
    {
        Button* button = dynamic_cast<Button*>(widget);
        if (button)
        {
            //button->SetTextNormalColorAndHoverColor(BUTTON_TEXT_NORMAL, BUTTON_TEXT_HOVER);
            button->DisableTextShadow();
            button->SetTextAspect(GLOBAL_FONT_ASPECT);
        }
    }
}

void Player::CreateDiscardPhraseButtons()
{
    m_buttonWidget->DeleteChildren();
    EventArgs args;

    Button* discardButton = new Button(AABB2((0.5f - 0.05f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.1f), (0.5f + 0.05f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.15f)), args, "Discard", "EndDiscardPhrasePlayer" + ToString(m_playerIndex), "RobotoMonoSemiBold128", g_theRenderer, g_theAudio,g_buttonTexture, BUTTON_TEXT_NORMAL, BUTTON_TEXT_HOVER);
    m_buttonWidget->m_children.push_back(discardButton);
    if (m_isDebug)
    {
        m_buttonWidget->m_children.push_back(new Button(AABB2((0.5f + 0.2f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.15f), (0.5f + 0.3f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.2f)), args, "Switch Player", "SwitchPlayer", "RobotoMonoSemiBold128", g_theRenderer, g_theAudio,g_buttonTexture, BUTTON_TEXT_NORMAL, BUTTON_TEXT_HOVER));
    }
    for (Widget* widget : m_buttonWidget->m_children)
    {
        Button* button = dynamic_cast<Button*>(widget);
        if (button)
        {
            //button->SetTextNormalColorAndHoverColor(BUTTON_TEXT_NORMAL, BUTTON_TEXT_HOVER);
            button->DisableTextShadow();
            button->SetTextAspect(GLOBAL_FONT_ASPECT);
        }
    }
}

void Player::CreateDyingPhraseButtons(EventArgs& args)
{
    std::string healEventName = "DyingHealPlayer"+ToString(m_playerIndex);
    std::string loseEventName = "LosePlayer"+ToString(m_playerIndex);
    m_buttonWidget->DeleteChildren();
    if (m_isDebug)
    {
        m_buttonWidget->m_children.push_back(new Button(AABB2((0.5f + 0.2f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.15f), (0.5f + 0.3f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.2f)), args, "Switch Player", "SwitchPlayer", "RobotoMonoSemiBold128", g_theRenderer, g_theAudio, g_buttonTexture, BUTTON_TEXT_NORMAL, BUTTON_TEXT_HOVER));
    }
    Button* healButton = new Button(AABB2((0.5f - 0.15f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.1f), (0.5f - 0.05f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.15f)), args, "Heal", healEventName, "RobotoMonoSemiBold128", g_theRenderer, g_theAudio, g_buttonTexture, BUTTON_TEXT_NORMAL, BUTTON_TEXT_HOVER);

    Button* loseButton = new Button(AABB2((0.5f + 0.05f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.1f), (0.5f + 0.15f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.15f)), args, "You DIE", loseEventName, "RobotoMonoSemiBold128", g_theRenderer, g_theAudio, g_buttonTexture, BUTTON_TEXT_NORMAL, BUTTON_TEXT_HOVER);

    m_buttonWidget->m_children.push_back(healButton);
    m_buttonWidget->m_children.push_back(loseButton);
    for (Widget* widget : m_buttonWidget->m_children)
    {
        Button* button = dynamic_cast<Button*>(widget);
        if (button)
        {
            //button->SetTextNormalColorAndHoverColor(BUTTON_TEXT_NORMAL, BUTTON_TEXT_HOVER);
            button->DisableTextShadow();
            button->SetTextAspect(GLOBAL_FONT_ASPECT);
        }
    }
}

void Player::CreatePlayAndTakeDamageButtons(std::string const& confirmEventName, std::string const& cancelEventName, EventArgs const& args)
{
    m_buttonWidget->DeleteChildren();
    if (m_isDebug)
    {
        m_buttonWidget->m_children.push_back(new Button(AABB2((0.5f + 0.2f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.15f), (0.5f + 0.3f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.2f)), args, "Switch Player", "SwitchPlayer", "RobotoMonoSemiBold128", g_theRenderer, g_theAudio, g_buttonTexture,BUTTON_TEXT_NORMAL, BUTTON_TEXT_HOVER));
    }
    Button* confirmButton = new Button(AABB2((0.5f - 0.15f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.1f), (0.5f - 0.05f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.15f)), args, "Play", confirmEventName, "RobotoMonoSemiBold128", g_theRenderer, g_theAudio, g_buttonTexture,BUTTON_TEXT_NORMAL, BUTTON_TEXT_HOVER);

    Button* cancelButton = new Button(AABB2((0.5f + 0.05f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.1f), (0.5f + 0.15f) * SCREEN_SIZE_X, SCREEN_SIZE_Y * (CARD_PADDING_Y + 0.15f)), args, "Take Damage", cancelEventName, "RobotoMonoSemiBold128", g_theRenderer, g_theAudio, g_buttonTexture,BUTTON_TEXT_NORMAL, BUTTON_TEXT_HOVER);

    m_buttonWidget->m_children.push_back(confirmButton);
    m_buttonWidget->m_children.push_back(cancelButton);
    for (Widget* widget : m_buttonWidget->m_children)
    {
        Button* button = dynamic_cast<Button*>(widget);
        if (button)
        {
            //button->SetTextNormalColorAndHoverColor(BUTTON_TEXT_NORMAL, BUTTON_TEXT_HOVER);
            button->DisableTextShadow();
            button->SetTextAspect(GLOBAL_FONT_ASPECT);
        }
    }
}


void Player::SetCentralInfoText(std::string const& text)
{
    m_centralInfoText->SetTextLabel(text);
}

void Player::InitializeMaxHealthBarWidgets()
{
    m_maxHealthBars.clear();
    for (int i = 0; i < m_maxHealth; ++i)
    {
        AABB2 box = IsMainPlayer() ? AABB2((1.f - HEALTH_PADDING_X) * SCREEN_SIZE_X, i * HEALTH_ICON_PADDING_Y * SCREEN_SIZE_Y, SCREEN_SIZE_X, (i + 1) * HEALTH_ICON_PADDING_Y * SCREEN_SIZE_Y) : AABB2((0.5f + 0.5f * PLAYER_PADDING_X) * SCREEN_SIZE_X, (1.f - (i+1) * HEALTH_ICON_PADDING_Y) * SCREEN_SIZE_Y, (0.5f + 0.5f * PLAYER_PADDING_X + HEALTH_PADDING_X) * SCREEN_SIZE_X, (1.f - i * HEALTH_ICON_PADDING_Y) * SCREEN_SIZE_Y);
        Widget* healthBar = new Widget(g_theRenderer, box, "Data/Images/HealthBar/Empty_New.png");
        m_maxHealthBars.push_back(healthBar);
    }
}

void Player::InitializeCurrentHealthWidgets()
{
    m_currentHealthBars.clear();
    for (int i = 0; i < m_currentHealth; ++i)
    {
         AABB2 box = IsMainPlayer() ? AABB2((1.f - HEALTH_PADDING_X) * SCREEN_SIZE_X, i * HEALTH_ICON_PADDING_Y * SCREEN_SIZE_Y, SCREEN_SIZE_X, (i + 1) * HEALTH_ICON_PADDING_Y * SCREEN_SIZE_Y) 
             : AABB2((0.5f + 0.5f * PLAYER_PADDING_X) * SCREEN_SIZE_X, (1.f - (i+1) * HEALTH_ICON_PADDING_Y) * SCREEN_SIZE_Y, (0.5f + 0.5f * PLAYER_PADDING_X + HEALTH_PADDING_X) * SCREEN_SIZE_X, (1.f - i * HEALTH_ICON_PADDING_Y) * SCREEN_SIZE_Y);
        Widget* healthBar = new Widget(g_theRenderer, box, "Data/Images/HealthBar/Heart_New.png");
        m_currentHealthBars.push_back(healthBar);
    }
}

bool Player::Event_Victory([[maybe_unused]]EventArgs& args)
{
    m_game->EnterVictory(m_playerIndex);
    return true;
}

bool Player::Event_Lose([[maybe_unused]]EventArgs& args)
{
    m_game->EnterLose(m_playerIndex);
    if (g_netState == NetState::REMOTE)
    {
        g_netSystem->Send("VictoryPlayer" + ToString(m_otherPlayerIndex));
    }
    return true;
}

void Player::InitializeGameInfoScrollBox()
{
    m_gameInfoBox = new ScrollBox(g_theRenderer, g_bitmapFont, AABB2(0.f, SCREEN_SIZE_Y * 0.5f, SCREEN_SIZE_X * 0.25f, SCREEN_SIZE_Y), 25.f,GLOBAL_FONT_ASPECT);
}

bool Player::IsMainPlayer() const
{
    return m_playerIndex == m_game->m_mainPlayerIndex;
}

bool Player::IsPlayerTurn() const
{
    return m_playerIndex == m_game->m_currenPlayerIndex;
}

bool Player::IsDiscardPhrase() const
{
    return m_currentState == PlayerState::Discard;
}

void Player::SwitchMainPlayer()
{
    InitializePlayerWidget();
    InitializeMaxHealthBarWidgets();
    InitializeCurrentHealthWidgets();
}

Player* Player::GetAnotherPlayer() const
{
    return m_playerIndex == 0 ? m_game->m_players[1] : m_game->m_players[0];
}

int Player::GetAnotherPlayerIndex() const
{
    return m_playerIndex == 0 ? 1 : 0;
}

void Player::ChangeCardsInHand()
{
    size_t cardsNum = m_carddataInHand.size();
    //m_cardsInHandWidget->ClearChildren();
    for (auto card : m_cardManager->m_currentCards)
    {
        delete card;
        card = nullptr;
    }
    m_cardManager->m_currentCards.clear();
    float cardXPadding = cardsNum * CARD_SIZE_X < 1.f ? 1.f / cardsNum * CARD_SIZE_X * 1.25f : 1.f / (cardsNum + 1);
    for (int i = 0; i < (int)m_carddataInHand.size(); ++i)
    {
        //m_cardManager->m_currentCards[i]->SetBorderColor(Rgba8::BLACK, Rgba8::GREY);
        m_cardsInHandWidget->AddChild(m_cardManager->PushNewCard(this, m_carddataInHand[i], Vec2(cardXPadding * i, 0.5f), i));
        //m_cardManager->m_currentCards[i]->SetWidgetAreaFromParent(m_cardsInHandWidget, false);
        m_cardManager->m_currentCards[i]->m_originalCenter = m_cardManager->m_currentCards[i]->m_widgetArea.GetCenter();
    }

    m_cardsInHandNumsText->SetTextLabel(Stringf("%d", m_carddataInHand.size()));
}

void Player::AddGameInfoText(std::string const& text, Rgba8 color)
{
    m_gameInfoBox->AddText(text, color);
}



void Player::Update(float deltaSeconds)
{
    UNUSED(deltaSeconds);
    if (IsMainPlayer())
    {
        UpdateActive();
    }

    if (IsMainPlayer() && m_actionState!= ActionState::ManipulateCards)
    {
        m_buttonWidget->Update();
    }
    if (!m_transitionTimer->IsStopped() && m_transitionTimer->HasPeriodElapsed())
    {
        m_transitionTimer->Stop();
        FireEvent("TransitionEventPlayer"+ToString(m_playerIndex));
    }
    if (m_controller)
    {
        m_controller->Update(*this);
    }
}

void Player::UpdateActive()
{
    if (m_actionState == ActionState::ManipulateCards)
    {
        m_manipulateWidget->Update();
    }
    else
    {
        m_cardManager->Update(); //m_cardsInHandWidget->Update();
        m_gameInfoBox->Update();
    }
}

bool Player::Event_StartPlayerTurn([[maybe_unused]]EventArgs& args)
{
    
    m_game->m_currenPlayerIndex = m_playerIndex;
    if (m_currentState == PlayerState::Initial)
    {
        return true;
    }
    EnterInitialPhrase();
    if (g_netState == NetState::REMOTE && IsMainPlayer())
    {
        g_netSystem->Send("StartTurnPlayer" + ToString(m_playerIndex));
    }
    return true;
}

void Player::StartPlayerTurnLocal()
{
    m_game->m_currenPlayerIndex = m_playerIndex;
    EnterInitialPhrase();
}

void Player::EnterInitialPhrase()
{


    m_currentState = PlayerState::Initial;
    m_game->AddGlobalText("Start "+m_playerName + "'s turn");
    m_transitionTimer->Start();
    if (m_transitionTimer->m_startTime == 0.f)
    {
        m_transitionTimer->m_startTime = 0.01f;
    }
    g_theEventSystem->ReplaceEventCallbackObjectMethod("TransitionEventPlayer"+ToString(m_playerIndex), this, &Player::Event_EndInitialPhrase);
}

bool Player::Event_EndInitialPhrase([[maybe_unused]]EventArgs& args)
{
    if (m_currentState == PlayerState::Judgement)
    {
        return true;
    }
    EnterJudgementPhrase();
    if (g_netState == NetState::REMOTE && IsMainPlayer())
    {
        g_netSystem->Send("EndInitialPhrasePlayer" + ToString(m_playerIndex));
    }
    return true;
}

void Player::EnterJudgementPhrase()
{
    m_currentState = PlayerState::Judgement;
    m_game->AddGlobalText(m_playerName + " enter Judgement Phrase");
    if (m_judgementArea.empty())
    {
        m_transitionTimer->Start();
        g_theEventSystem->ReplaceEventCallbackObjectMethod("TransitionEventPlayer" + ToString(m_playerIndex), this, &Player::Event_EndJudgementPhrase);
    }

}

bool Player::Event_EndJudgementPhrase([[maybe_unused]]EventArgs& args)
{
    if (m_currentState == PlayerState::Draw)
    {
        return true;
    }
    EnterDrawPhrase();
    if (g_netState == NetState::REMOTE && IsMainPlayer())
    {
        g_netSystem->Send("EndJudgementPhrasePlayer" + ToString(m_playerIndex));
    }
    return true;
}

void Player::EnterDrawPhrase()
{
    m_currentState = PlayerState::Draw;
    m_game->AddGlobalText(m_playerName + " enter Draw Phrase");
//    EventArgs args;
//    args.SetValue("CardNumber", 2);
//    args.SetValue("IsLocal", 0);
//    FireEvent("DrawCardsPlayer" + ToString(m_playerIndex),args);
    DrawCardsLocal(2);
    m_transitionTimer->Start();
    g_theEventSystem->ReplaceEventCallbackObjectMethod("TransitionEventPlayer" + ToString(m_playerIndex), this, &Player::Event_EndDrawPhrase);
    //std::vector 
}

bool Player::Event_EndDrawPhrase([[maybe_unused]]EventArgs& args)
{
    if (m_currentState == PlayerState::Action)
    {
        return true;
    }
    EnterActionPhrase();
    if (g_netState == NetState::REMOTE && IsMainPlayer())
    {
        g_netSystem->Send("EndDrawPhrasePlayer" + ToString(m_playerIndex));
    }
    return true;
}

void Player::EnterActionPhrase()
{
    CreateActionPhraseButtons();
    m_currentState = PlayerState::Action;
    
    m_game->AddGlobalText(m_playerName + " start Action");
    m_actionState = ActionState::HaveNotPlayedStrike;
    if (m_controller)
    {
        m_controller->OnEnterActionPhrase(*this);
    }

    //m_currentActionState
    //m_transitionTimer->Start();
    //g_theEventSystem->ReplaceEventCallbackObjectMethod("TransitionEventPlayer" + ToString(m_playerIndex), this, &Player::Event_EndActionPhrase);
}

bool Player::Event_EndActionPhrase([[maybe_unused]]EventArgs& args)
{
    if (m_currentState == PlayerState::Discard)
    {
        return true;
    }
    EnterDiscardPhrase();
    if (g_netState == NetState::REMOTE && IsMainPlayer())
    {
        g_netSystem->Send("EndActionPhrasePlayer" + ToString(m_playerIndex));
    }
    return true;
}

void Player::EnterDiscardPhrase()
{
    m_currentState = PlayerState::Discard;
    m_cardManager->DisselectCurrentCards();
    m_discardNum = (int)m_carddataInHand.size() - m_currentHealth;

    if (m_discardNum <= 0)
    {
        //m_currentState = PlayerState::End;
        CreateNormalButton();
        m_transitionTimer->Start();
        g_theEventSystem->ReplaceEventCallbackObjectMethod("TransitionEventPlayer" + ToString(m_playerIndex), this, &Player::Event_EndDiscardPhrase);
    }
    else
    {
        SetCentralInfoText("You Need to Discard " + ToString(m_discardNum )+ " Cards");
        CreateDiscardPhraseButtons();
    }
}

bool Player::Event_EndDiscardPhrase([[maybe_unused]]EventArgs& args)
{
    if (m_currentState == PlayerState::End)
    {
        return true;
    }
    m_discardNum = (int)m_carddataInHand.size() - m_currentHealth;
    if (m_discardNum <= 0)
    {
        EnterEndPhrase();
    }
    else if (m_cardManager->m_selectedIndexes.size() < m_discardNum)
    {
        Event_DiscardSelectCards(args);
        EnterDiscardPhrase();

    }
    else
    {
        Event_DiscardSelectCards(args);
        CreateNormalButton();
        EnterEndPhrase();

    }
    if (g_netState == NetState::REMOTE && IsMainPlayer())
    {
        g_netSystem->Send("EndDiscardPhrasePlayer" + ToString(m_playerIndex), args);
    }
    return true;
}

void Player::EnterEndPhrase()
{
   
    m_currentState = PlayerState::End;
    m_actionState = ActionState::Normal;
    SetCentralInfoText("");
    m_game->AddGlobalText(m_playerName + " end their turn");

    m_transitionTimer->Start();
    g_theEventSystem->ReplaceEventCallbackObjectMethod("TransitionEventPlayer" + ToString(m_playerIndex), this, &Player::Event_EndEndPhrase);
}



bool Player::Event_EnterDyingPhrase(EventArgs& args)
{
    args.SetValue("OpponentActionState", (int)GetAnotherPlayer()->m_actionState);
    args.SetValue("ActionState", (int)m_actionState);
    m_actionState = ActionState::Dying;
    CreateDyingPhraseButtons(args);
    SetCentralInfoText("You are dying... You need to heal, or DIE");
    GetAnotherPlayer()->m_actionState = ActionState::Normal;
    GetAnotherPlayer()->SetCentralInfoText(Stringf("%s are dying...",m_playerName.c_str()));
    GetAnotherPlayer()->m_buttonWidget->m_isVisible = false;
    if (g_netState == NetState::REMOTE && IsMainPlayer())
    {
        g_netSystem->Send("EnterDyingPhrasePlayer" + ToString(m_playerIndex), args);
    }
    if (m_controller)
    {
        m_controller->OnEnterDyingPhrase(*this, args);
    }
    return true;
}

bool Player::Event_EndEndPhrase([[maybe_unused]]EventArgs& args)
{
    if (m_currentState != PlayerState::End)
    {
        return true;
    }
    m_transitionTimer->Stop();
    m_game->SwitchPlayerTurn();
    if (g_netState == NetState::REMOTE && IsMainPlayer())
    {
        g_netSystem->Send("EndEndPhrasePlayer" + ToString(m_playerIndex), args);
    }
    return true;
}

bool Player::IsSelectingCard() const
{
    return !m_cardManager->m_selectedIndexes.empty();
}

std::vector<CardData> const Player::GetCardsInHand() const
{
    return m_carddataInHand;
}

void Player::TakeDamage()
{
    m_currentHealth = GetClamped(--m_currentHealth, 0, m_maxHealth);
    InitializeCurrentHealthWidgets();
    m_game->AddGlobalText(Stringf("%s take 1 damage", m_playerName.c_str()));
}

bool Player::ReactToDuel(int cardIndex)
{
    if (cardIndex != -1)
    {

        return true;
    }
    if (IsSelectingCard())
    {
        if (m_cardManager->GetSelectCardType() == CardType::Strike)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

}

void Player::AddCard(CardData const& data)
{
    m_carddataInHand.push_back(data);
    ChangeCardsInHand();
}

//void Player::DiscardCard(int cardIndex)
//{
//    m_carddataInHand.erase(m_carddataInHand.begin() + cardIndex);
//
//}

bool Player::Event_DrawCards(EventArgs& args)
{
    int cardsNum = args.GetValue("CardNumber", 0);
    std::string drawCardNames;
    g_cardDeck->DrawCards(m_carddataInHand, cardsNum, drawCardNames);
    ChangeCardsInHand();

    AddGameInfoText(Stringf("%s draw %d cards: %s", m_playerName.c_str(), cardsNum, drawCardNames.c_str()));
    m_game->AddTextToPlayer(Stringf("%s draw %d cards", m_playerName.c_str(), cardsNum), m_playerIndex == 0 ? 1 : 0);

    if (args.GetValue("IsLocal", true) && g_netState == NetState::REMOTE && IsMainPlayer())
    {
        g_netSystem->Send("DrawCardsPlayer"+ToString(m_playerIndex), args);
    }
    return true;
}

bool Player::Event_StealCard(EventArgs& args)
{
    int cardIndex = args.GetValue("CardIndex", -1);
    if (cardIndex < 0)
    {
        m_game->AddGlobalText("ERROR the CardIndex of Discarded Card", Rgba8::RED);
        return true;
    }
    GetAnotherPlayer()->AddCard(m_carddataInHand[cardIndex]);
    g_cardDeck->DiscardSingleCard(m_carddataInHand[cardIndex]);
    std::string cardName = m_carddataInHand[cardIndex].m_displayName;
    m_carddataInHand.erase(m_carddataInHand.begin() + cardIndex);
    m_cardManager->m_selectedIndexes.clear();
    ChangeCardsInHand();
    m_game->AddGlobalText(Stringf("%s steal card: %s", GetAnotherPlayer()->m_playerName.c_str(), cardName.c_str()));
    if (g_netState == NetState::REMOTE && IsMainPlayer() && args.GetValue("HasSentMessage", false))
    {
        g_netSystem->Send("StealCardPlayer" + ToString(m_playerIndex), args);
    }
    return true;
}

void Player::DrawCardsLocal(int cardNumber)
{
    std::string drawCardNames;
    g_cardDeck->DrawCards(m_carddataInHand, cardNumber, drawCardNames);
    ChangeCardsInHand();

    AddGameInfoText(Stringf("%s draw %d cards: %s", m_playerName.c_str(), cardNumber, drawCardNames.c_str()));
    m_game->AddTextToPlayer(Stringf("%s draw %d cards", m_playerName.c_str(), cardNumber), m_playerIndex == 0 ? 1 : 0);
}

bool Player::Event_DiscardCard(EventArgs& args)
{
    int cardIndex = args.GetValue("CardIndex", -1);
    if (cardIndex < 0)
    {
        m_game->AddGlobalText("ERROR the CardIndex of Discarded Card", Rgba8::RED);
        return true;
    }
    g_cardDeck->DiscardSingleCard(m_carddataInHand[cardIndex]);
    std::string cardName = m_carddataInHand[cardIndex].m_displayName;
    m_carddataInHand.erase(m_carddataInHand.begin() + cardIndex);
    m_cardManager->m_selectedIndexes.clear();
    ChangeCardsInHand();
    m_game->AddGlobalText(Stringf("%s discard card: %s", m_playerName.c_str(), cardName.c_str()));
    if (g_netState == NetState::REMOTE && IsMainPlayer() && args.GetValue("HasSentMessage", false))
    {
        g_netSystem->Send("DiscardCardPlayer" + ToString(m_playerIndex), args);
    }
    return true;
}

bool Player::Event_DiscardSelectCards([[maybe_unused]]EventArgs& args)
{
    std::string selectedCardNames;

    m_cardManager->DiscardSelectedCard(selectedCardNames);
    for (int index: m_cardManager->m_selectedIndexes)
    {
        g_cardDeck->DiscardSingleCard(m_carddataInHand[index]);
    }

    std::deque<int> indices = m_cardManager->m_selectedIndexes;
    if (m_cardManager->m_selectedIndexes.empty())
    {
        return true;
    }
    std::sort(indices.begin(), indices.end(), std::greater<int>());  // Sort in descending order

    for (int index : indices) 
    {
        m_carddataInHand.erase(m_carddataInHand.begin() + index);
    }
    m_cardManager->m_selectedIndexes.clear();
    ChangeCardsInHand();
    m_game->AddGlobalText(Stringf("%s discard selected cards: %s", m_playerName.c_str(), selectedCardNames.c_str()));

    if (g_netState == NetState::REMOTE && IsMainPlayer())
    {
        g_netSystem->Send("DiscardSelectedCardsPlayer"+ToString(m_playerIndex));
    }
    return true;

}

bool Player::Event_DyingHeal(EventArgs& args)
{
    int cardIndex = args.GetValue("CardIndex", -1);
    if (cardIndex < 0)
    {
        if (!IsSelectingCard())
        {
            return true;
        }
        cardIndex = m_cardManager->GetFirstSelectIndex();
        g_cardDeck->DiscardSingleCard(m_carddataInHand[cardIndex]);
        std::string cardName = m_carddataInHand[cardIndex].m_displayName;
        m_carddataInHand.erase(m_carddataInHand.begin() + cardIndex);
        m_cardManager->m_selectedIndexes.clear();
    }
    SetCentralInfoText("");
    GetAnotherPlayer()->SetCentralInfoText("");

    ++m_currentHealth;
    InitializeCurrentHealthWidgets();
    m_game->AddGlobalText(Stringf("%s gain 1 health", m_playerName.c_str()));
    m_actionState = (ActionState)args.GetValue("ActionState", 0);
    GetAnotherPlayer()->m_buttonWidget->m_isVisible = true;
    if (m_currentState == PlayerState::Action)
    {
       CreateActionPhraseButtons();
    }
    else if (m_currentState == PlayerState::Judgement)
    {
        CreateNormalButton();
    }
    else 
    {
        CreateNormalButton();
    }
    GetAnotherPlayer()->m_actionState = (ActionState)args.GetValue("OpponentActionState", 0);
    
    if (g_netState == NetState::REMOTE && IsMainPlayer())
    {
        args.SetValue("CardIndex", cardIndex);
        g_netSystem->Send("HealPlayer" + ToString(m_playerIndex),args);
    }
    return true;
}

bool Player::Event_Heal([[maybe_unused]]EventArgs& args)
{
    m_currentHealth = GetClamped(++m_currentHealth, 0, m_maxHealth);
    InitializeCurrentHealthWidgets();
    m_game->AddGlobalText(Stringf("%s gain 1 health", m_playerName.c_str()));

    if (g_netState == NetState::REMOTE && IsMainPlayer())
    {
        g_netSystem->Send("HealPlayer" + ToString(m_playerIndex));
    }
    return true;
}

bool Player::Event_TakeDamage([[maybe_unused]]EventArgs& args)
{
    TakeDamage();
    if (m_currentHealth < 1)
    {
        FireEvent("EnterDyingPhrasePlayer" + ToString(m_playerIndex));
    }
    if (g_netState == NetState::REMOTE && IsMainPlayer())
    {
        g_netSystem->Send("TakeDamagePlayer"+ToString(m_playerIndex));
    }
    return true;
}

bool Player::Event_ManipulateOpponentCard([[maybe_unused]]EventArgs& args)
{
    args.SetValue("PreviousActionState", (int)m_actionState);
    m_actionState = ActionState::ManipulateCards;
    m_manipulateWidget->CreateManipulateCards(GetAnotherPlayer()->m_carddataInHand, (ManipulateEvent)args.GetValue("ManipulateEvent", 0), this, GetAnotherPlayerIndex(), args);
    if (g_netState == NetState::REMOTE && IsMainPlayer())
    {
        g_netSystem->Send("ManipulateOpponentCardPlayer" + ToString(m_playerIndex));
    }
    return true;
}

bool Player::Event_ReactToBeManipulatedCard([[maybe_unused]]EventArgs& args)
{
    if (g_netState == NetState::REMOTE && IsMainPlayer())
    {
        g_netSystem->Send("ReactToBeManipulatedCardPlayer" + ToString(m_playerIndex));
    }
    return true;
}

bool Player::Event_NeedToRespondDodge( EventArgs& args)
{
    m_actionState = ActionState::NeedToRespondDodge;
    args.SetValue("RespondCardType", (int)CardType::Dodge);
    args.SetValue("SucessGlobalText", m_playerName + " dodges successfully.");
    SetCentralInfoText("You need to play a Dodge, or take damage");
    CreatePlayAndTakeDamageButtons("RespondSuccessfullyPlayer" + ToString(m_playerIndex), "RespondTakeDamagePlayer" + ToString(m_playerIndex), args);
    if (g_netState == NetState::REMOTE && IsMainPlayer())
    {
        g_netSystem->Send("NeedToRespondDodgePlayer" + ToString(m_playerIndex), args);
    }
    if (m_controller)
    {
        m_controller->OnNeedToRespondDodge(*this, args);
    }
    return true;
}


bool Player::Event_NeedToRespondStrike(EventArgs& args)
{
    m_actionState = ActionState::NeedToRespondStrike;
    args.SetValue("RespondCardType", (int)CardType::Strike);

    SetCentralInfoText("You need to play a Strike, or take damage");
    CreatePlayAndTakeDamageButtons("RespondSuccessfullyPlayer" + ToString(m_playerIndex), "RespondTakeDamagePlayer" + ToString(m_playerIndex), args);
    if (g_netState == NetState::REMOTE && IsMainPlayer())
    {
        g_netSystem->Send("NeedToRespondStrikePlayer" + ToString(m_playerIndex), args);
    }
    if (m_controller)
    {
        m_controller->OnNeedToRespondStrike(*this, args);
    }
    return true;
}

bool Player::Event_InitialzieDuel(EventArgs& args)
{
    m_actionState = ActionState::NeedToRespondStrike;
    //    args.SetValue("RespondCardType", (int)CardType::Strike);

    SetCentralInfoText("You need to play a Strike to respond Duel, or take damage");
    CreatePlayAndTakeDamageButtons("NeedToRespondDuelPlayer" + ToString(m_otherPlayerIndex), "RespondDuelTakeDamagePlayer" + ToString(m_playerIndex), args);
    if (g_netState == NetState::REMOTE && IsMainPlayer())
    {
        g_netSystem->Send("InitialzieDuelPlayer" + ToString(m_playerIndex), args);
    }
    if (m_controller)
    {
        m_controller->OnNeedToRespondDuel(*this, args);
    }
    return true;
}

bool Player::Event_NeedToRespondDuel(EventArgs& args)
{
   
    if (!GetAnotherPlayer()->ReactToDuel(args.GetValue("SelectedCardIndex",-1)))
    {
        return true;
    }
    int cardIndex = args.GetValue("SelectedCardIndex",-1);
    if (cardIndex < 0)
    {
        cardIndex = GetAnotherPlayer()->m_cardManager->GetFirstSelectIndex();
    }

    g_cardDeck->DiscardSingleCard(GetAnotherPlayer()->m_carddataInHand[cardIndex]);
    std::string cardName = GetAnotherPlayer()->m_carddataInHand[cardIndex].m_displayName;
    GetAnotherPlayer()->m_carddataInHand.erase(GetAnotherPlayer()->m_carddataInHand.begin() + cardIndex);
    GetAnotherPlayer()->m_cardManager->m_selectedIndexes.clear();

    m_game->AddGlobalText(Stringf("%s react to Duel", GetAnotherPlayer()->m_playerName.c_str()));
    GetAnotherPlayer()->ChangeCardsInHand();
    GetAnotherPlayer()->SetCentralInfoText("Waiting for response...");
    GetAnotherPlayer()-> CreateNormalButton();
    GetAnotherPlayer()-> m_actionState = ActionState::Normal;

    args.SetValue("SelectedCardIndex", -1);
     m_actionState = ActionState::NeedToRespondStrike;
//    args.SetValue("RespondCardType", (int)CardType::Strike);

    SetCentralInfoText("You need to play a Strike to respond Duel, or take damage");
    CreatePlayAndTakeDamageButtons("NeedToRespondDuelPlayer" + ToString(m_otherPlayerIndex), "RespondDuelTakeDamagePlayer" + ToString(m_playerIndex), args);

    if (g_netState == NetState::REMOTE && !IsMainPlayer())
    {
        args.SetValue("SelectedCardIndex", cardIndex);
        g_netSystem->Send("NeedToRespondDuelPlayer" + ToString(m_playerIndex), args);
    }
    if (m_controller)
    {
        m_controller->OnNeedToRespondDuel(*this, args);
    }
    return true;
}

bool Player::Event_RespondSuccessfully(EventArgs& args)
{
    int cardIndex = m_cardManager->GetFirstSelectIndex();
    if (cardIndex < 0)
    {
        return true;
    }
    if (m_cardManager->GetCardType(cardIndex) ==static_cast<CardType>( args.GetValue("RespondCardType", (int)CardType::NUMS_COUNT)))
    {
        m_game->AddGlobalText( args.GetValue("SucessGlobalText", Stringf("%s Respond Successfully",m_playerName.c_str())));
        g_cardDeck->DiscardSingleCard(m_carddataInHand[cardIndex]);
        m_carddataInHand.erase(m_carddataInHand.begin() + cardIndex);
        m_cardManager->m_selectedIndexes.clear();
        ChangeCardsInHand();
        m_actionState = static_cast<ActionState>(args.GetValue("TargetNextActionState",  (int)m_actionState));
        GetAnotherPlayer()->m_actionState =  static_cast<ActionState>(args.GetValue("InitiatorNextActionState", 0));
        ResetNormalPhrase();
        GetAnotherPlayer()->ResetActionPhrase();
    }
    if (g_netState == NetState::REMOTE && IsMainPlayer())
    {
        g_netSystem->Send("RespondSuccessfullyPlayer" + ToString(m_playerIndex), args);
    }
    return true;
}

bool Player::Event_RespondTakeDamage(EventArgs& args)
{
    TakeDamage();
    m_actionState = static_cast<ActionState>(args.GetValue("TargetNextActionState", (int)m_actionState));
    GetAnotherPlayer()->m_actionState = static_cast<ActionState>(args.GetValue("InitiatorNextActionState", (int)GetAnotherPlayer()->m_actionState));
    ResetNormalPhrase();
    GetAnotherPlayer()->ResetActionPhrase();
    m_cardManager->ResetCards();
    if (m_currentHealth < 1)
    {
        FireEvent("EnterDyingPhrasePlayer" + ToString(m_playerIndex));
    }
    if (g_netState == NetState::REMOTE && IsMainPlayer())
    {
        g_netSystem->Send("RespondTakeDamagePlayer" + ToString(m_playerIndex), args);
    }
    return true;
}



bool Player::Event_RespondDuelTakeDamage(EventArgs& args)
{
    TakeDamage();
    Player* initiator = m_game->GetPlayer(args.GetValue("PlayerIndex", 0));
    initiator->GetAnotherPlayer()->m_actionState = static_cast<ActionState>(args.GetValue("TargetNextActionState", (int)initiator->GetAnotherPlayer()->m_actionState));
    initiator->m_actionState = static_cast<ActionState>(args.GetValue("InitiatorNextActionState", (int)initiator->m_actionState));
    initiator->GetAnotherPlayer()->ResetNormalPhrase();
    initiator->ResetActionPhrase();
    initiator->m_cardManager->ResetCards();
    if (m_currentHealth < 1)
    {
        FireEvent("EnterDyingPhrasePlayer" + ToString(m_playerIndex));
    }
    if (g_netState == NetState::REMOTE && IsMainPlayer())
    {
        g_netSystem->Send("RespondDuelTakeDamagePlayer" + ToString(m_playerIndex), args);
    }
    return true;
}

bool Player::Debug_DrawSpecificCard(EventArgs& args)
{
    HashedCaseInsensitiveString cardName = args.GetValue("CardType", "NONE");
    CardType cardType = CardType::NUMS_COUNT;
    if (cardName == HashedCaseInsensitiveString("Strike"))                  cardType = CardType::Strike;
    else if (cardName == HashedCaseInsensitiveString("Dodge"))              cardType = CardType::Dodge;
    else if (cardName == HashedCaseInsensitiveString("Heal"))               cardType = CardType::Heal;
    else if (cardName == HashedCaseInsensitiveString("DismantleBreak"))     cardType = CardType::DismantleBreak;
    else if (cardName == HashedCaseInsensitiveString("SnatchSteal"))        cardType = CardType::SnatchSteal;
    else if (cardName == HashedCaseInsensitiveString("DrawTwo"))            cardType = CardType::DrawTwo;
    else if (cardName == HashedCaseInsensitiveString("Duel"))               cardType = CardType::Duel;
    else if (cardName == HashedCaseInsensitiveString("Barbarians"))         cardType = CardType::Barbarians;
    else if (cardName == HashedCaseInsensitiveString("HailofArrows"))       cardType = CardType::HailofArrows;
    else if (cardName == HashedCaseInsensitiveString("WardNegate"))         cardType = CardType::WardNegate;
    else if (cardName == HashedCaseInsensitiveString("AcediaCapture"))      cardType = CardType::AcediaCapture;
    else if (cardName == HashedCaseInsensitiveString("Lightning"))          cardType = CardType::Lightning;
    else if (cardName == HashedCaseInsensitiveString("Weapon"))             cardType = CardType::Weapon;
    else if (cardName == HashedCaseInsensitiveString("Armor"))              cardType = CardType::Armor;
    else if (cardName == HashedCaseInsensitiveString("HorsePlus"))          cardType = CardType::HorsePlus;
    else if (cardName == HashedCaseInsensitiveString("HorseNegative"))      cardType = CardType::HorseNegative;
    if (cardType == CardType::NUMS_COUNT)
    {
        return true;
    }
    g_cardDeck->DrawSpecificCard(m_carddataInHand, cardType);
    ChangeCardsInHand();

    if (g_netState == NetState::REMOTE && IsMainPlayer())
    {
        g_netSystem->Send("DebugDrawCardPlayer" + ToString(m_playerIndex), args);
    }

    return true;

}

void Player::ResetNormalPhrase()
{
    SetCentralInfoText("");
    CreateNormalButton();
}

void Player::ResetActionPhrase()
{
    SetCentralInfoText("");
    CreateActionPhraseButtons();
}

void Player::ExitStrike()
{
    SetCentralInfoText("");
    m_actionState = ActionState::HavePlayedStrike;
    CreateActionPhraseButtons();
}

void Player::ExitDodge()
{
    SetCentralInfoText("");
    CreateNormalButton();
    m_actionState = ActionState::Normal;
}

bool Player::Event_SelectCard(EventArgs& args)
{
    int cardIndex = args.GetValue("CardIndex", 0);
    m_cardManager->SelectCard(cardIndex);
    return true;
}

bool Player::Event_PlayCard(EventArgs& args)
{
    int cardIndex = -1;
    if (!m_cardManager->m_selectedIndexes.empty())
    {
        cardIndex = m_cardManager->m_selectedIndexes[0];
        args.SetValue("CardIndex", cardIndex);
    }
    else
    {
        cardIndex = args.GetValue("CardIndex", -1);
    }
    if (cardIndex < 0 || cardIndex >= (int)m_carddataInHand.size())
    {
        m_game->AddGlobalText("ERROR the CardIndex of Played Card", Rgba8::RED);
        return true;
    }
    m_cardManager->PlayCard(cardIndex);
    g_cardDeck->DiscardSingleCard(m_carddataInHand[cardIndex]);
    m_carddataInHand.erase(m_carddataInHand.begin() + cardIndex);
    m_cardManager->m_selectedIndexes.clear();
    ChangeCardsInHand();
    if (g_netState == NetState::REMOTE && IsMainPlayer())
    {
        g_netSystem->Send("PlayCardPlayer" + ToString(m_playerIndex), args);
    }
    return true;
}



void Player::Render() const
{
    g_theRenderer->BeginCamera(m_camera);
    if (IsMainPlayer())
    {
        RenderActive();
    }
    else
    {
        RenderInactive();
    }
    g_theRenderer->EndCamera(m_camera);


}

void Player::RenderActive() const
{
    m_cardManager->Render();
    m_playerWidget->Render();
    for (int i = 0; i < m_maxHealthBars.size(); ++i)
    {
        m_maxHealthBars[i]->Render();
    }
    for (int i = 0; i < m_currentHealthBars.size(); ++i)
    {
        m_currentHealthBars[i]->Render();
    }

    //m_cardsInHandWidget->Render();

    m_cardsInHandNumsText->Render();
    m_gameInfoBox->Render();
    m_buttonWidget->Render();
    m_centralInfoText->Render();


}



void Player::RenderInactive() const
{
    m_playerWidget->Render();
    for (int i = 0; i < m_maxHealthBars.size(); ++i)
    {
        m_maxHealthBars[i]->Render();
    }
    for (int i = 0; i < m_currentHealthBars.size(); ++i)
    {
        m_currentHealthBars[i]->Render();
    }
    m_cardsInHandNumsText->Render();
}

