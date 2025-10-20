/*
 * Eric Ryan Montgomery
 * 09/21/2025
 * For CardGameUI
 * Global Variables/Structs/Class are written here
 */
 
#ifndef GLOBALSH
#define GLOBALSH
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "raylib.h"
using namespace std;

Vector2 operator+(const Vector2& lhs, const Vector2& rhs);
Rectangle operator*(const Rectangle& lhs, const int& rhs);
Rectangle operator*(const Rectangle& lhs, const float& rhs);
Vector2 operator*(const Vector2& lhs, const int& rhs);
Vector2 operator*(const Vector2& lhs, const float& rhs);
bool operator==(const Vector2& lhs, const Vector2& rhs);
bool operator!=(const Vector2& lhs, const Vector2& rhs);

typedef struct NAMEDFONT {
	string name;
	Font font;
	int fontSize;
} NamedFont;

class StyleGuide {
public:
    // Constant bases
    static constexpr Vector2 origin = {0, 0};
    static constexpr Rectangle cardSource = {0, 0, 100, 150};
    static constexpr Rectangle buttonSource = {0, 0, 200, 100};
    static constexpr int segments = 64;
    static constexpr Color textColor = BLACK;
    static constexpr Color background = RAYWHITE;
    static constexpr Color starColor = GOLD;
    static constexpr int starRadius = 20;
    static constexpr int starLineThickness = 7;
    static constexpr float scrollSpeed = 50.0f;
    
    // Drawing Constants
    static constexpr Vector2 REC_START = {1, 53};
    static constexpr Vector2 REC_END = {63, 63};
    static constexpr Color REC_COLOR = BLACK;
    static constexpr Vector2 REC_BTN_START1 = {2, 54};
    static constexpr Vector2 REC_BTN_END1 = {62, 58};
    static constexpr Vector2 REC_BTN_START2 = {2, 58};
    static constexpr Vector2 REC_BTN_END2 = {62, 62};
	
	//Mutable
    Vector2 screenDimensions;
    float maxScroll;
    float starRotation;
    int numCards;
    int deckStrength;
    bool deck1AI;
    bool deck2AI;
    float widthSegment;
    float heightSegment;
    std::vector<NamedFont>::iterator currentFont;

    StyleGuide();
    void Update();
};

enum GameScreen {
	TITLE = 0,
	SETUP = 1,
	RULES = 2,
	SKILLS = 3,
	SETTINGS = 4,
	GAME = 5,
	EDITCARD = 6,
	VIEWCARD = 7,
};

enum AlignmentX {
	CENTERX = 0,
	RIGHTX = 1,
	LEFTX = 2,
};

enum AlignmentY {
	CENTERY = 0,
	DOWNY = 1,
	UPY = 2,
};

typedef struct ALIGNMENT {
	AlignmentX x;
	AlignmentY y;
	bool reduceTextSize = true;
} Alignment;

enum colors {
	C_WHITE = 0,
	C_GREEN = 1,
	C_BLUE = 2,
	C_YELLOW = 3,
	C_RED = 4,
	C_ORANGE = 5,
};

enum attributes {
	C_MIMIC = 0,
	C_DEX = 1,
	C_INT = 2,
	C_STR = 3,
	C_FTH = 4,
	C_ARC = 5,
	C_DUMMY = 6,
};

enum abilities {
	MIMICRY = 0, //Copies Opponent *Two Versions
	HEAVY_HANDED = 1, //Adds 3 power
	FAITHFUL = 2, //Increases Crit Hit chance by max
	FAST_HAND = 3, //Adds one action
	HEALTHY_MIND = 4, //Adds 10 health
	TERROR = 5, //Decreases opponents power by 3
	RESISTANT = 6, //Take half damage against physical attacks
	HOLY_PRESENCE = 7, //Charge up next Card in deck
	EVIL_PRESENCE = 8, //Charge down the next Card in opponents deck
	ACCURATE = 9, //Can never miss and crit hit chance is increased
	INNERGATE = 10, //Can only miss or crit hit
	NECROMANCY = 11, //Swap yourself with a dead Card in the Deck
	MAGIC_IMMUNITY = 12, //Take no damage against magical attacks
	DIVINEHELP = 13, //The divine strikes your foe for 10 points
	NULLIFY = 14, //Nullifies an opponents ability
	MADE_IN_HEAVEN = 15, //Resets the opponents Card
	//REALITYSHIFTER = 97, //Gives you a choice of three abilities to use
	CHAOS = 98, //Chooses a random ability
	STRATEGICFIRE = 99, //Gives ability based on position in Deck
};

enum spells {
	FORCE,
	DRAIN,
	WEAKEN,
	HEAL,
};

enum actions {
	ATTACK = 0,
	SWAP = 1,
	CHARGE = 2,
	FLASK = 3,
	CASTSPELL = 4,
	ERROR = 99,
};

enum attackActions {
	MISS = 0,
	HIT = 1,
	CRITHIT = 2,
};

enum players {
	PLAYER1,
	PLAYER2,
};

enum cardRoles {
	C_MAIN,
	C_SUPPORT,
};

typedef struct CARDEDITVARS {
	int cardClickedOn = -1;
    int playerEditing = -1;
    int remainingPoints = -1;
    colors chosenColor = C_WHITE;
    attributes chosenAtt = C_MIMIC;
    int chosenPowerLevel = 0;
    spells chosenSpell = FORCE;
} CardEditVars;

typedef struct VIEWCARDVARS {
	int cardIndex;
	int deckNum;
} ViewCardVars;

//Flags for global purposes
typedef struct FLAGS {
	bool cardEdited = false;
	bool firstTurnFrame = true;
} Flags;

//Flags and variables for game specific purposes
typedef struct GAMEVARS {
	int who = 0; //This is the index for the Card in play
	int turn = 0;
	int round = 0;
	players playerInPlay = PLAYER1; //This is the Deck that is in play
	cardRoles currCardRole = C_MAIN; //This is the role that the current Card is in
	int player1Score = 0;
	int player2Score = 0;
	string dialog = "";
	int amtActions = 0;
	bool gameStarted = false;
	bool gameEnded = false;
	/*
	 * static constexpr makes these varable const and compile time known
	 * It also prevents errors when doing something like this:
	 * GameVars gameVars1 = gameVars2;
	 * because these variable can't be changed
	 */
	static constexpr int maxSwaps = 2;
	static constexpr int maxCharges = 2;
	static constexpr int maxFlasks = 2;
} GameVars;


extern std::vector<colors> cols;
extern std::vector<attributes> atts;
extern std::vector<spells> spellList;
extern std::vector<actions> actionsList;
extern std::unordered_map<colors, colors> advantage;
extern StyleGuide styleGuide;
extern vector<NamedFont> fonts;
extern string gamerules;
extern string skills;

#endif
