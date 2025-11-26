/*
 * Eric Ryan Montgomery
 * 11/25/2025
 * For CardGameUI
 * State Classes/Variables are written here
 */
 
#ifndef STATEVARIABLESH
#define STATEVARIABLESH
#include <iostream>
#include <string>
#include <vector>
#include "raylib.h"
#include "Card.h"
#include "Deck.h"

using namespace std;

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

//These variables will be used to setup the intial game state
class PreGameVars {
	public:
		int numCards = 5;
		int deckStrength = 7;
		bool deck1AI = false;
		bool deck2AI = false;
};

//These variables are used when editing a card
class CardEditVars {
	public:
		int cardClickedOn = -1;
		int playerEditing = -1;
		int remainingPoints = -1;
		Card::colors chosenColor = Card::C_WHITE;
		Card::attributes chosenAtt = Card::C_MIMIC;
		int chosenPowerLevel = 0;
		Card::spells chosenSpell = Card::FORCE;
		bool cardEdited = false;

		void Set(Deck &deck, int index, int deckEditing) {
			this->cardClickedOn = index;
			this->playerEditing = deckEditing;
			this->chosenColor = deck[index]->GetColor();
			this->chosenAtt = deck[index]->GetAttribute();
			this->chosenPowerLevel = deck[index]->GetNumber();
			this->remainingPoints = deck.GetRemainingPoints();
			this->chosenSpell = deck[index]->GetSpell();
			this->cardEdited = true;
		}

		void Reset() {
			this->cardClickedOn = -1;
			this->playerEditing = -1;
			this->remainingPoints = -1;
			this->chosenColor = Card::C_WHITE;
			this->chosenAtt = Card::C_MIMIC;
			this->chosenPowerLevel = 0;
			this->chosenSpell = Card::FORCE;
			this->cardEdited = false;
		}
};

//These variables are used when viewing a card
class ViewCardVars {
	public:
		int cardIndex;
		int deckNum;

		void Set(int cardIndex, int deckNum) {
			this->cardIndex = cardIndex;
			this->deckNum = deckNum;
		}
};

extern vector<NamedFont> fonts;

#endif
