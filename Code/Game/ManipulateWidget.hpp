#pragma once
#include "Engine/UI/Widget.hpp"
#include "Game/Card.hpp"
#include "Engine/UI/Button.hpp"
class Player;
class ManipulateCard : public Button
{
public:
    ManipulateCard(CardData const& data, AABB2 const& widgetArea);
private:
    CardData const& m_data;

};
enum class ManipulateEvent
{
    DismantleBreak,
    SnatchSteal
};
class ManipulateWdiget : public Widget
{
public:
    ManipulateWdiget(Renderer* render, AABB2 const& widgetArea);
    ~ManipulateWdiget();
    bool Event_ManipulateCard(EventArgs& args);
    void Update() override;
    void Render() const override;
    void AddCards(ManipulateCard* newCard);
    void DeleteCards();
    void SetConfirmButtonEvent(EventArgs & args);
    void CreateManipulateCards(std::vector<CardData> const& datas, ManipulateEvent const& event, Player* currentPlayer, int opponentPlayerIndex, EventArgs &args);
private:

    int m_currentSelectedIndex = -1;
    Player* m_currentPlayer = nullptr;
    int m_manipulatePlayerIndex = -1;
    ManipulateEvent m_currentEvent = ManipulateEvent::DismantleBreak;
    Button* m_confirmButton = nullptr;
    std::vector<ManipulateCard*> m_oppoentCards;
};