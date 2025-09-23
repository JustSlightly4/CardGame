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

/*
typedef struct DATA {
	//Constant bases
	Vector2 origin;
	Vector2 const baseScreenDimensions; //x represents width, y represents height
	float scale;
	Vector2 const baseCardTextureSize;
	Vector2 const baseButtonTextureSize;
	float const baseMargin;
	float const baseFontSize;
	Rectangle const buttonSource;
	Rectangle const baseRecDimensions;
	
	//Mutable depending on scale of bases
	Vector2 screenDimensions;
	Vector2 prevScreenDimensions;
	Rectangle recDimensions;
	Vector2 buttonTextureSize;
	Vector2 cardTextureSize;
	float fontSize;
	float margin;
	int starRadius;
	int starLineThickness;
	
	//Not depending on scale of bases
	float aspectRatio;
	float scrollSpeed;
	float lineSpacing; //Line spacing is the same as char spacing
	float sentenceSpacing; 
	float maxScroll;
	float starRotation;
	Color textColor;
	Color backgroud;
	Color starColor;
	int numCards;
	int numDecks;
	int deckStrength;
	bool deck1AI;
	bool deck2AI;
	
	DATA(float screenWidth, float screenHeight) 
	: baseScreenDimensions({(float)screenWidth, (float)screenHeight}),
	scale(((baseScreenDimensions.x/1600) + (baseScreenDimensions.y/900))/2.0), 
	baseCardTextureSize({100, 150}), baseButtonTextureSize({200, 100}),
	baseMargin(10), baseFontSize(40),
	buttonSource({0, 0, baseButtonTextureSize.x, baseButtonTextureSize.y}),
	baseRecDimensions({
			baseMargin, 
			baseScreenDimensions.y - baseButtonTextureSize.y - (baseMargin * 3), 
			baseScreenDimensions.x - (baseMargin * 2), 
			baseButtonTextureSize.y + (baseMargin * 2)
		}) {
		
		origin = {0, 0};
		
		//Mutable depending on scale of bases
		screenDimensions = baseScreenDimensions;
		prevScreenDimensions = baseScreenDimensions;
		buttonTextureSize = baseButtonTextureSize * scale;
		cardTextureSize = {100, 150};
		fontSize = baseFontSize * scale;
		margin = baseMargin * scale;
		recDimensions = {
			margin, 
			screenDimensions.y - buttonTextureSize.y - (margin * 3), 
			screenDimensions.x - (margin * 2), 
			buttonTextureSize.y + (margin * 2)
		};
		starRadius = 20;
		starLineThickness = 7;
		
		//Not depending on scale of bases
		aspectRatio = baseScreenDimensions.x / baseScreenDimensions.y;
		scrollSpeed = 50.0f;
		lineSpacing = 1.0f; //Line spacing is the same as char spacing
		sentenceSpacing = 20.0f; 
		starRotation = 0.0f;
		maxScroll = 0;
		textColor = BLACK;
		backgroud = RAYWHITE;
		starColor = GOLD;
		numCards = 5;
		numDecks = 2;
		deckStrength = 7;
		deck1AI = false;
		deck2AI = false;
	}
} Data;
*/

class Data {
public:
    // Constant bases
    Vector2 origin;
    Vector2 const baseScreenDimensions; // x = width, y = height
    Vector2 const baseCardTextureSize;
    Vector2 const baseButtonTextureSize;
    float const baseMargin;
    float const baseFontSize;
    Rectangle const buttonSource;
    Rectangle const baseRecDimensions;

    // Mutable depending on scale
    Vector2 screenDimensions;
    Vector2 prevScreenDimensions;
    Rectangle recDimensions;
    Vector2 buttonTextureSize;
    Vector2 cardTextureSize;
    float fontSize;
    float margin;
    int starRadius;
    int starLineThickness;
    float scaleX;
    float scaleY;

    // Not depending on scale
    float aspectRatio;
    float scrollSpeed;
    float lineSpacing; // Line spacing is the same as char spacing
    float sentenceSpacing;
    float maxScroll;
    float starRotation;
    Color textColor;
    Color background;
    Color starColor;
    int numCards;
    int numDecks;
    int deckStrength;
    bool deck1AI;
    bool deck2AI;
    int segments;
    float widthSegment;
    float heightSegment;

    Data() : 
        baseScreenDimensions{1600, 900}, // example values
        baseCardTextureSize{100, 150},
        baseButtonTextureSize{200, 100},
        baseMargin(10.0f),
        baseFontSize(24.0f),
        buttonSource{0, 0, 200, 100},
        baseRecDimensions{0, 0, 400, 300}
    {
        // Initialize screen dimensions
        screenDimensions = {(float)GetScreenWidth(), (float)GetScreenHeight()};
        prevScreenDimensions = screenDimensions;

        // Compute scale factors
        scaleX = screenDimensions.x / baseScreenDimensions.x;
        scaleY = screenDimensions.y / baseScreenDimensions.y;

        // Scale dependent values
        buttonTextureSize = { baseButtonTextureSize.x * scaleX, baseButtonTextureSize.y * scaleY };
        cardTextureSize   = { baseCardTextureSize.x * scaleX, baseCardTextureSize.y * scaleY };
        fontSize          = baseFontSize * scaleY;   // font usually tied to height
        margin            = baseMargin * scaleX;     // horizontal margin usually tied to width
        recDimensions = {
            margin,
            screenDimensions.y - buttonTextureSize.y - (margin * 3),
            screenDimensions.x - (margin * 2),
            buttonTextureSize.y + (margin * 2)
        };

        starRadius = 20;
        starLineThickness = 7;

        // Not depending on scale
        aspectRatio = baseScreenDimensions.x / baseScreenDimensions.y;
        scrollSpeed = 50.0f;
        lineSpacing = 1.0f;
        sentenceSpacing = 20.0f;
        starRotation = 0.0f;
        maxScroll = 0;
        textColor = BLACK;
        background = RAYWHITE;
        starColor = GOLD;
        numCards = 5;
        numDecks = 2;
        deckStrength = 7;
        deck1AI = false;
        deck2AI = false;
        segments = 64;
        widthSegment = screenDimensions.x/segments;
        heightSegment = screenDimensions.y/segments;
        Update();
    }

    void Update() {
        // Update screen dimensions
        screenDimensions.x = GetScreenWidth();
        screenDimensions.y = GetScreenHeight();

        // Star rotation animation
        starRotation += 20 * GetFrameTime();
        if (starRotation > 360.0f) starRotation = 0.0f;

        // Recompute scale if screen changed
        if (screenDimensions.x != prevScreenDimensions.x || screenDimensions.y != prevScreenDimensions.y) {
            scaleX = screenDimensions.x / baseScreenDimensions.x;
            scaleY = screenDimensions.y / baseScreenDimensions.y;

            // Update dependent dimensions
            fontSize = baseFontSize * scaleY;
            margin   = baseMargin * scaleX;

            buttonTextureSize = { baseButtonTextureSize.x * scaleX, baseButtonTextureSize.y * scaleY };
            cardTextureSize   = { baseCardTextureSize.x * scaleX, baseCardTextureSize.y * scaleY };

            recDimensions = {
                margin,
                screenDimensions.y - buttonTextureSize.y - (margin * 3),
                screenDimensions.x - (margin * 2),
                buttonTextureSize.y + (margin * 2)
            };

			widthSegment = screenDimensions.x/segments;
			heightSegment = screenDimensions.y/segments;
			
            prevScreenDimensions = screenDimensions;
        }
    }
};

enum GameScreen {
	TITLE = 0,
	SETUP = 1,
	RULES = 2,
	SKILLS = 3,
	SETTINGS = 4,
	GAME = 5,
	EDITCARD = 6,
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
	HOLY_PRESENCE = 7, //Charge up next card in Deck
	EVIL_PRESENCE = 8, //Charge down the next card in opponents Deck
	ACCURATE = 9, //Can never miss and crit hit chance is increased
	INNERGATE = 10, //Can only miss or crit hit
	NECROMANCY = 11, //Swap yourself with a dead card in the deck
	MAGIC_IMMUNITY = 12, //Take no damage against magical attacks
	DIVINEHELP = 13, //The divine strikes your foe for 10 points
	NULLIFY = 14, //Nullifies an opponents ability
	MADE_IN_HEAVEN = 15, //Resets the opponents card
	//REALITYSHIFTER = 97, //Gives you a choice of three abilities to use
	CHAOS = 98, //Chooses a random ability
	STRATEGICFIRE = 99, //Gives ability based on position in deck
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

//Flags for global purposes
typedef struct FLAGS {
	bool cardEdited = false;
	bool firstTurnFrame = true;
} Flags;

//Flags and variables for game specific purposes
typedef struct GAMEVARS {
	int who = 0; //This is the index for the card in play
	int turn = 0;
	int round = 0;
	players playerInPlay = PLAYER1; //This is the deck that is in play
	cardRoles currCardRole = C_MAIN; //This is the role that the current card is in
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

#endif
