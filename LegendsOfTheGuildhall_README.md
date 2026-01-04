# Legends of the Guildhall

A 2D turn-based card game inspired by "Legends of the Three Kingdoms," developed as part of DFS2 coursework using a custom C++ game engine with networking capabilities.

## 🎮 Game Overview

**Legends of the Guildhall** is a strategic turn-based card game where players choose unique characters and engage in tactical combat using various card types. Each player has distinct abilities and health values, creating asymmetric gameplay that requires different strategies.

### Core Gameplay Features

- **Character Selection**: Players choose from unique characters with different abilities and maximum health
- **Turn-Based Combat**: Six-phase turn structure with strategic decision points
- **Card-Based Actions**: Multiple card types including Basic cards, Scroll Tool cards, and Equipment cards
- **Health Management**: Players must manage health and can die if health drops below 0
- **Networking Support**: Both local and online multiplayer gameplay

## 🎯 Game Rules

### Victory Conditions
- **Win**: All opponents are eliminated
- **Lose**: Your health drops to 0 and you cannot play a Heal card

### Turn Structure
Each player's turn consists of six phases:
1. **Initial Phase**: Setup and preparation
2. **Judgment Phase**: Resolve any judgment effects
3. **Drawing Phase**: Draw cards from the deck
4. **Action Phase**: Play cards and take actions
5. **Discard Phase**: Discard excess cards
6. **End Phase**: Cleanup and turn transition

### Card Types

#### Basic Cards
- **Strike**: Attack another player
- **Dodge**: Defend against Strike attacks
- **Heal**: Restore health points

#### Scroll Tool Cards
- **Dismantle Break**: Destroy opponent equipment
- **Snatch Steal**: Steal cards from opponents
- **Draw Two**: Draw additional cards
- **Duel**: Force opponent into Strike/Dodge combat
- **Barbarians**: Area-of-effect attack
- **Hail of Arrows**: Multi-target damage

#### Equipment Cards *(Planned)*
- **Weapon**: Increase attack range/damage
- **Armor**: Provide defensive bonuses
- **Horse +1/-1**: Modify distance calculations

## 🏗️ Technical Implementation

### Architecture Overview

The game is built using a custom C++ engine with the following key systems:

#### Core Systems
- **Custom Game Engine**: Built from scratch using DirectX 11
- **Event System**: State-based event handling for game actions
- **Networking**: Client-server architecture supporting online play
- **UI System**: Card-based interface with hover/select animations
- **Audio Integration**: FMOD audio system support

#### Key Components

##### Card System
```cpp
class Card : public Widget
{
    // Polymorphic card behavior with different subclasses
    virtual void Play() = 0;
    virtual bool IsPlayable() const = 0;
    // Animation and interaction handling
};
```

##### Player System
```cpp
class Player
{
    // State management for turn phases
    PlayerState m_currentState;
    ActionState m_actionState;
    // Character-specific abilities through inheritance
    virtual void EnterDrawPhrase();
};
```

##### Networking Integration
- **Event-based messaging**: Game state synchronized through events
- **Client-server model**: Authoritative server with client prediction
- **State serialization**: Game state transmitted via NameProperties

### Technical Achievements

1. **Card Deck Behavior**
   - Static card collection initialization
   - Shuffle mechanics with discard pile recycling
   - Dynamic card drawing and management

2. **Card-based UI System**
   - Widget-based card representation
   - Smooth hover and selection animations
   - Manager pattern for card organization

3. **Polymorphic Design**
   - Card inheritance hierarchy for different behaviors
   - Player subclasses for character-specific abilities
   - Virtual function overrides for customization

4. **Enhanced Event System**
   - State enum storage in NameProperties
   - Simplified event registration
   - Network-compatible event parsing

5. **State-based Gameplay**
   - Turn phase management
   - Action state tracking
   - Transition validation

6. **Networking Implementation**
   - Message-based communication
   - State synchronization
   - Multiplayer game flow management

## 🎨 Visual Features

- **Custom Card Artwork**: Unique designs for each card type
- **Character Portraits**: Individual character representations
- **UI Animations**: Smooth card hover, selection, and transition effects
- **Health Visualization**: Dynamic health bar display
- **Background Themes**: Immersive game board and attract screen

## 🚀 Getting Started

### Prerequisites
- Windows 10/11
- DirectX 11 compatible graphics card
- Visual Studio 2019 or later

### Building the Project
1. Clone the repository
2. Open `LegendsOfTheGuildhall.sln` in Visual Studio
3. Build the LegendsOfTheGuildhall project
4. Run the executable from the `Run/` directory

### Game Controls
- **Left Mouse**: Select/play cards
- **Tilde (~)**: Open developer console
- **UI Buttons**: Navigate game phases and actions

## 📊 Development Insights

### Challenges Overcome
- **Networking Complexity**: Adapted single-player logic for multiplayer architecture
- **State Management**: Implemented robust turn-based state transitions
- **Event System**: Developed network-compatible event handling
- **UI Responsiveness**: Created smooth card interaction animations

### Key Learnings
- **Networking Mindset**: Different approach required compared to single-player development
- **State-based Design**: Critical for turn-based game architecture
- **Event-driven Programming**: Essential for networked gameplay
- **Abstract Class Utilization**: Powerful for polymorphic game objects

### Development Statistics
- **Development Time**: Approximately 50% spent on networking debugging
- **Architecture**: Custom C++ engine with DirectX 11 rendering
- **Code Organization**: Modular design with clear separation of concerns

## 🔮 Future Enhancements

### Planned Features
1. **Equipment System**: Implement weapon, armor, and horse cards
2. **Judgment Mechanism**: Add judgment phase card effects
3. **Character Expansion**: Create cards representing Guildhall community members
4. **Enhanced UI**: Additional animations and visual feedback
5. **AI Opponents**: Implement computer-controlled players

### Technical Improvements
- **Performance Optimization**: Reduce networking overhead
- **Error Handling**: Improved network error recovery
- **Debug Tools**: Enhanced debugging capabilities
- **Memory Management**: Optimize card and widget allocation

## 👥 Credits

**Developer**: Yenna Zhou  
**Course**: DFS2 (Digital Game Development)  
**Institution**: SMU Guildhall  
**Engine**: Custom C++ DirectX 11 Engine  
**Audio**: FMOD Integration  
**Networking**: Custom TCP/UDP Implementation  

## 📄 License

This project was developed as part of academic coursework at SMU Guildhall. All rights reserved.

---

*"A strategic card game that combines classic gameplay mechanics with modern networking technology, showcasing advanced C++ programming and game architecture skills."*
