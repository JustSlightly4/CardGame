/*
 * Eric Ryan Montgomery
 * 09/21/2025
 * For CardGameUI
 * Functions for a Deck of cards without the use of raylib are written here
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
#include "cards.h"
using namespace std;

class Deck {
	public:
		//Public Functions
		Deck(int amountCards, shared_ptr<Texture2D> texture, bool random = false, bool ai = false, int deckStrength = 7); //Constructor
		~Deck(); //Deconstructor
		Deck(const Deck &other); //Copy Constructor
		void operator=(const Deck &rhs);
		Card* operator[](int const index) const; //Overload []
		Card* at(int const index) const; //Gets the Card at the index given
		friend ostream &operator<<(ostream& os, const Deck &rhs); //Overload <<
		bool AddCard(Card *newCard); //Adds a Card to the Deck
		bool AddCard(enum Card::colors col, enum Card::attributes att, int num); //Adds Card
		bool RemoveCard(int pos); //Removes a Card from the Deck by position
		bool Isdeck_Full(); //Checks if the Deck is full or not
		int GetCardLimit() const; //Gets Card limit
		int GetCardAmt() const; //Gets current amt of cards in Deck
		int size() const; //Gets current amt of cards in Deck
		void SetFlaskAmt(int amt); //Sets flaskAmt
		void SetTimesSwapped(int amt); //Sets timesSwapped
		int GetFlaskAmt(); //Return Flask Amount
		int GetTimesSwapped() const; //Return Times Swapped
		string Swap(int one, int two); //Swaps two cards in Deck
		void ShuffleDeck(); //Fisher-Yates Algorithm to shuffle cards
		string UseFlask(int &pos); //Uses a flask
		bool IsAI(); //Check if bot or not
		void SetAI(bool b); //Set if bot or not
		void RestoreDeck(); //Restores All card back to their original state
		shared_ptr<Texture2D> &GetTexture(); //Returns a reference to the Card textures
		int GetTotalPoints(); //Returns the max amount of points this Deck can have
		int GetRemainingPoints(); //Returns the remaining amount of points this Deck has
		void SetRemainingPoints(int amt); //Set the points remaining in a Deck to spend on cards

	protected:
		//Private Functions
		vector<Card*> Cards;
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
		
};



#endif
