/*
 * Eric Ryan Montgomery
 * 09/21/2025
 * For CardGameUI
 * Functions for a deck of cards without the use of raylib are written here
 */
 
#ifndef DECKOFCARDSH
#define DECKOFCARDSH
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <thread>
#include <chrono>
#include <iomanip>
#include <memory>
#include <unordered_map>
#include "raylib.h"
#include "globals.h"
#include "cards.h"
using namespace std;

class deck {
	protected:
		vector<card*> Cards;
		shared_ptr<Texture2D> cardTexture;
		int cardLimit;   
		int amtCards;
		int timesUsedFlask;
		int timesSwapped;
		bool ai;
		bool CheckIfNumber(string &number);
		int totalPoints; //Amount of Points user has to spend on cards
		int remainingPoints;
		void CreateRandomCards(); //Creates random cards with points
		void CreateBlankCards(); //Creates blank cards
	public:
		deck(int amountCards, shared_ptr<Texture2D> texture, bool random = false, bool ai = false, int deckStrength = 7); //Constructor
		~deck(); //Deconstructor
		deck(const deck &other); //Copy Constructor
		void operator=(const deck &rhs);
		card* operator[](int const index) const; //Overload []
		card* at(int const index) const; //Gets the card at the index given
		friend ostream &operator<<(ostream& os, const deck &rhs); //Overload <<
		bool AddCard(card *newCard); //Adds a card to the deck
		bool AddCard(enum colors col, enum attributes att, int num); //Adds card
		bool RemoveCard(int pos); //Removes a card from the deck by position
		bool IsDeckFull(); //Checks if the deck is full or not
		int GetCardLimit() const; //Gets card limit
		int GetCardAmt() const; //Gets current amt of cards in deck
		int size() const; //Gets current amt of cards in deck
		void SetFlaskAmt(int amt); //Sets flaskAmt
		void SetTimesSwapped(int amt); //Sets timesSwapped
		int GetFlaskAmt(); //Return Flask Amount
		int GetTimesSwapped() const; //Return Times Swapped
		string Swap(int one, int two); //Swaps two cards in deck
		void ShuffleDeck(); //Fisher-Yates Algorithm to shuffle cards
		string UseFlask(int &pos); //Uses a flask
		bool IsAI(); //Check if bot or not
		void SetAI(bool b); //Set if bot or not
		void RestoreDeck(); //Restores All Card back to their original state
		shared_ptr<Texture2D> &GetTexture(); //Returns a reference to the card textures
		int GetTotalPoints(); //Returns the max amount of points this deck can have
		int GetRemainingPoints(); //Returns the remaining amount of points this deck has
		void SetRemainingPoints(int amt); //Set the points remaining in a deck to spend on cards
		
};



#endif
