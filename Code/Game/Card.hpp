#pragma once
#include <string>
#include "Engine/Math/Mat44.hpp"
#include "Engine/UI/Widget.hpp"
class Player;
class Timer;
class CardManager;
enum class PlayerState; 
enum class ActionState;
enum class CardSuit
{
    Diamond,
    Club   ,
    Heart  ,
    Spade  ,
};

enum class CardType
{
    Strike,
    Dodge,
    Heal,
    DismantleBreak,
    SnatchSteal,
    DrawTwo,
    Duel,
    Barbarians,
    HailofArrows,
    WardNegate,
    AcediaCapture,
    Lightning,
    Weapon,
    Armor,
    HorsePlus,
    HorseNegative,
    NUMS_COUNT
};

enum class CardMainType
{
    Basic,
    ScrollTool,
    Equipment,
    UNKNOWN
};

enum class CardStatus
{
    InHand,
    Played,
    Discarded,
    Others
};

struct CardData 
{
public:
    CardData() = default;
    CardData(int rank, CardSuit const& suit);
    void SetType(CardType const& type);
    void SetMainType(CardType const& type);
    void SetDisplayName(CardType const& type);
    void SetDescritpion(CardType const& type);
public:
    CardSuit m_suit = CardSuit::Diamond;
    int m_rank = 0;
    std::string m_displayName = "UNKNOWN";
    std::string m_description = "UNKNOWN";
    CardType m_type = CardType::NUMS_COUNT;
    CardMainType m_mainType = CardMainType::UNKNOWN;
};
class Card : public Widget
{
public:
    Card(Player* owner, CardData const& cardData, AABB2 const& widgetArea, Vec2 const& widgetRelativePos, Vec2 const& widgetPivot, CardManager* cardManager, size_t cardIndex, bool isCreatingNamebox = true);
    virtual ~Card();
    void Update();
    void Discard();
    void ResetLocalTransform();
    void SetNewTransform(Mat44 const& transform);
    virtual void Play() {}
    virtual bool IsSelectable() const;
    virtual bool IsPlayable() const;
    bool IsScrollToolCardPlayable() const;
    Player* GetOpponentPlayer() const;
    int  GetMainPlayerIndex() const;
    int  GetOpponentPlayerIndex() const;
    void EndSelect();
    bool IsOwnerThePlayerState(PlayerState const& playerState) const;
    bool IsOwnerTheActionState(ActionState const& actionState) const;
private:
    void ChangePlayableColor();
    bool InCardArea(Vec2 const& mousePos) const;
    void NormalUpdateCardStatus();
    void DiscardPhraseUpdateCardStatus();
    bool StartHover();
    void EndHover();
    bool StartSelect();

    void OnDrag();
    void OnSelect(Timer* timer);
    void OnHover(Timer* timer);
    void OnClick();
    void OnDiscard(Timer* timer);
public:
    Vec2 m_originalCenter;
    Vec2 m_localPosition;
    EulerAngles m_orientation;
    CardStatus m_status = CardStatus::InHand;
    CardData const& m_cardData;
    bool m_isDiscarding = false;
    bool m_isHovering = false;
    bool m_isSelecting = false;
    Timer* m_transitionTimer = nullptr;
    Player* m_owner = nullptr;
    size_t m_cardIndex = 0;
    CardManager* m_cardManager = nullptr;
};