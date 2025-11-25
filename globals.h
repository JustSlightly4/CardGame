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
#include "raylib.h"
#include "cards.h"
#include "deckofcards.h"

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
    Card::colors chosenColor = Card::C_WHITE;
    Card::attributes chosenAtt = Card::C_MIMIC;
    int chosenPowerLevel = 0;
    Card::spells chosenSpell = Card::FORCE;
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
class GameVars {
	public:
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

	int numCards = 5;
    int deckStrength = 7;
    bool deck1AI = false;
    bool deck2AI = false;
	/*
	 * static constexpr makes these varable const and compile time known
	 * It also prevents errors when doing something like this:
	 * GameVars gameVars1 = gameVars2;
	 * because these variable can't be changed
	 */
	static constexpr int maxSwaps = 2;
	static constexpr int maxCharges = 2;
	static constexpr int maxFlasks = 2;

	void Reset() {
		who = 0; //This is the index for the Card in play
		turn = 0;
		round = 0;
		playerInPlay = PLAYER1; //This is the Deck that is in play
		currCardRole = C_MAIN; //This is the role that the current Card is in
		player1Score = 0;
		player2Score = 0;
		dialog = "";
		amtActions = 0;
		gameStarted = false;
		gameEnded = false;
	}
};

extern std::unordered_map<Card::colors, Card::colors> advantage;
extern vector<NamedFont> fonts;
extern string gamerules;
extern string skills;

#endif
