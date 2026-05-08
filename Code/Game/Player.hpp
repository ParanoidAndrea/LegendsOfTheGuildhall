#pragma once
#include "Game/Gamecommon.hpp"
#include "Engine/Render/Camera.hpp"
#include <vector>
#include "Game/Card.hpp"
#include "Game/CardManager.hpp"
#include "Engine/Core/EventSystem.hpp"
class Game;
class Textbox;
class ScrollBox;
class ManipulateWdiget;
class PlayerController;

enum class PlayerState
{
    Initial,
    Judgement,
    Draw,
    Action,
    Discard,
    End,
};
enum class ActionState
{
    Normal,
    HaveNotPlayedStrike,
    PlayingStrike,
    HavePlayedStrike,
    NeedToRespondDodge,
    NeedToRespondStrike,
    ManipulateCards,
    Dying

};
class Player
{
public:
    Player(Game* game, int playerIndex, std::string const& playerName, std::string const& playerIconPath);
    ~Player();
    void InitializeWidgets();
    void ChangeCardsInHand();
    void AddGameInfoText(std::string const& text, Rgba8 color = Rgba8 ::WHITE);
    void Update(float deltaSeconds);
    void UpdateActive();
    void Render() const;
    void RenderActive() const;
    void RenderInactive() const;

    void CreateNormalButton();
    void CreatePlayAndTakeDamageButtons(std::string const& confirmEventName, std::string const& cancelEventName, EventArgs const& args);
    void CreateActionPhraseButtons();
    void CreateDiscardPhraseButtons();
    void CreateDyingPhraseButtons(EventArgs& args);
    void SetCentralInfoText(std::string const& text);
    virtual void PlayerInitialization();
    void InitializeMaxHealthBarWidgets();
    void InitializeCurrentHealthWidgets();

    bool Event_Victory(EventArgs& args);
    bool Event_Lose(EventArgs& args);

   void EnterInitialPhrase();
   void EnterJudgementPhrase();
   virtual void EnterDrawPhrase();
   void EnterActionPhrase();
   void EnterDiscardPhrase();
   void EnterEndPhrase();
   bool Event_EnterDyingPhrase(EventArgs& args);
   void StartPlayerTurnLocal();
   bool Event_StartPlayerTurn(EventArgs& args);  
   bool Event_EndInitialPhrase(EventArgs& args);
   bool Event_EndJudgementPhrase(EventArgs& args);
   bool Event_EndDrawPhrase(EventArgs& args);
   bool Event_EndActionPhrase(EventArgs& args);
   bool Event_EndDiscardPhrase(EventArgs& args);
   bool Event_EndEndPhrase(EventArgs& args);

    bool IsSelectingCard() const;
    bool IsMainPlayer() const;
    bool IsPlayerTurn() const;
    bool IsDiscardPhrase() const;
    void SwitchMainPlayer();
    Player* GetAnotherPlayer() const;
    int GetAnotherPlayerIndex() const;
    std::vector<CardData> const GetCardsInHand() const;

    virtual void TakeDamage();
    bool ReactToDuel(int cardIndex);
    void AddCard(CardData const& data);
    bool Event_SelectCard(EventArgs& args);
    bool Event_PlayCard(EventArgs& args);
    void DrawCardsLocal(int cardNumber);
    bool Event_DrawCards(EventArgs& args);
    bool Event_StealCard(EventArgs& args);
    bool Event_DiscardCard(EventArgs& args);
    bool Event_DiscardSelectCards(EventArgs& args);
    bool Event_DyingHeal(EventArgs& args);
    bool Event_Heal(EventArgs& args);
    bool Event_TakeDamage(EventArgs& args);
    bool Event_ManipulateOpponentCard(EventArgs& args);
    virtual bool Event_ReactToBeManipulatedCard(EventArgs& args);
    bool Event_NeedToRespondDodge(EventArgs& args);
    bool Event_NeedToRespondStrike(EventArgs& args);
    bool Event_InitialzieDuel(EventArgs& args);
    bool Event_NeedToRespondDuel(EventArgs& args);
    bool Event_RespondSuccessfully(EventArgs& args);
    bool Event_RespondTakeDamage(EventArgs& args);
    bool Event_RespondDuelTakeDamage(EventArgs& args);
    bool Debug_DrawSpecificCard(EventArgs& args);

    void SetController(PlayerController* controller);
    void ResetNormalPhrase();
    void ResetActionPhrase();
    void ExitStrike();
    void ExitDodge();
private:
    void SubscribeEvents();
    void InitializeCardsInHandWidget();
    void InitializePlayerWidget();
    void InitializeCenteralInfoWidget();

    void InitializeGameInfoScrollBox();

public:
    bool m_isPlayingStrikeInfinite = false;
    Timer* m_transitionTimer = nullptr;
    PlayerState m_currentState = PlayerState::End;
    ActionState m_actionState  = ActionState::Normal;
    Camera m_camera;
    int m_maxHealth = 4;
    int m_currentHealth = 4;
    int m_playerIndex = 0;
    int m_otherPlayerIndex = 1;
    bool m_isDebug = false;
    PlayerController* m_controller = nullptr;
    Widget* m_cardsInHandWidget = nullptr;
    Textbox* m_centralInfoText = nullptr;
    Widget* m_playerWidget = nullptr;
    Widget* m_enemyWidget = nullptr;
    Widget* m_buttonWidget = nullptr;
    ManipulateWdiget* m_manipulateWidget = nullptr;
    std::vector<Widget*> m_currentHealthBars;
    std::vector<Widget*> m_maxHealthBars;

    Textbox* m_cardsInHandNumsText = nullptr;
    ScrollBox* m_gameInfoBox = nullptr;

    int m_discardNum = 0;
    CardManager* m_cardManager = nullptr;
    Game* m_game = nullptr;
    std::string m_playerIconPath = "";
    std::string m_playerName = "";


    std::vector<CardData*> m_judgementArea;
    std::vector<CardData> m_carddataInHand;
};

