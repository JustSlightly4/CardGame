/*
 * Eric Ryan Montgomery
 * 03/17/2025
 * For CardGameUI
 * Functions for deck/cards without the use of raylib are written here
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
using namespace std;
	
class card {
	protected:
	//User Defined Variables:
	colors color; //Color of the card
	attributes attribute; //Attribute of the card
	int number; //Power level of the card
	
	//Calculated Variables:
	int power; //Power depends on color, attribute, and number
	int magicalPower; //Power depends on color, attribute, and number 
	int health; // health depends on color, attribute, and number
	int totalHealth; // health depends on color, attribute, and number
	string name; //name depends on color and attribute
	abilities ability; //Ability depends on color and attribute
	int ID; //Used to identify cards
	
	//Modifier Variables 
	int const powerMod = 1; //Used to scale a cards power
	int const healthMod = 1; //Used to scale a cards health
	
	//Other Variables
	int charge; //Number of times a card has charged up
	int numActions; //Number of actions a card can take in one turn
	double physicalResistance; //Physical Resistance to incoming damage
	double magicalResistance; //Magical Resistance to incoming damage
	int upperDice; //Highest Dice Roll for positive effects
	int lowerDice; //Lowest Dice Roll for negative effects
	bool abilityUsed; //If ability has been used or not
	bool abilitiesActive; //If abilities are working or not
	spells spell;
	
	//AI Queue
	queue<actions> actionQueue;
	
	//Private Functions:
	void CalcPhysicalPower(); //Physical Power Calculator
	void CalcMagicalPower(); //Magical Power Calculator
	void CalcHealth(); //Health Calculator
	void CalcName(); //Name Calculator
	void CalcAbility(); //Ability Calculator
	void CalcID(); //ID Calculator
	
	//-------------------------------------------------------
	
	public:
	//Public Functions:
	card (enum colors color, enum attributes attribute, int number); //Constructor
	void operator=(const card &rhs);//Overload =
	string GetName() const; //Return a cards name
	int GetNumber() const; //Return a cards number
	int GetPower() const; //Return a cards physical power
	int GetMagicalPower() const; //Return a cards magical power
	int GetHealth() const; //Return a cards health
	int GetHealthT() const; //Return a cards total health
	int GetCharge() const; //Return a cards charge
	int GetNumActions() const; //Return amount of actions
	double GetPhysicalResistance() const; //Return a cards physicalResistance
	double GetMagicalResistance() const; //Return a cards magical resistance
	int GetID() const; //Return a cards ID
	enum colors GetColor() const; //Return a cards color as enum
	string GetColorStr() const; //Return a cards color as a str
	Color GetColorRaylib() const; //Returns a cards color as Raylibs definition
	enum attributes GetAttribute() const; //Return a cards attribute as enum
	string GetAttributeStr() const; //Return a cards attribute as a string
	enum abilities GetAbility() const; //Return a cards ability as enum
	string GetAbilityStr() const; //Return a cards ability as a string
	enum attackActions RollDiceEnum(); //Rolls the dice for a card return enum
	int RollDiceInt(int limit); //Rolls the dice for a card return int
	ostringstream Print(); //Print Card Details
	bool ChangeAbility(int ability); //Changes a cards ability
	void ChangeAbility(abilities ab); //Changes a cards ability
	bool GetAbilityUsed(); //Returns if the cards ability is used or not
	void SetAbilityUsed(bool b); //Sets if a cards ability is used or not
	bool GetAbilitiesActive(); //Returns if the cards ability is active or not
	void SetAbilitiesActive(bool b); //Sets if a cards ability is active or not
	spells GetSpell(); //Gets a cards spell
	string GetSpellStr(); //Gets a cards spell as a string
	void SetSpell(spells s); //Sets a cards spell
	
	//Public Debuf Functions
	string ChargeUp(); //Charges up card
	void ChargeDown(); //Decreases a cards charge
	string AddPower(int amount); //Increases the cards power
	string AddMagicalPower(int amount); //Increase the cards power
	string AddHealth(int amount); //Increases the cards health
	void SetHealth(int amount); //Sets the cards healths
	string Heal(int amount); //Heals the cards by an amount
	string FullHeal(); //Fully heals the card
	string DecPower(int amount); //Decreases a cards power
	string DecBothHealth(int amount); //Dec's health and totalHealth
	string DecTotalHealth(int amount); //Dec's totalHeath
	string DecHealth(int amount); //Dec's health
	string AddPhysicalResistance(double amount); //Decreases physicalResistance
	string DecPhysicalResistance(double amount); //Increases physicalResistance
	string AddMagicalResistance(double amount); //Decreases magicalResistance
	string DecMagicalResistance(double amount); //Increases magicalResistance
	string AddUpperDice(int amount); //Increases Upper Dice
	string DecUpperDice(int amount); //Decreases Upper Dice
	string AddLowerDice(int amount); //Increases Lower Dice
	string DecLowerDice(int amount); //Decreases Lower Dice
	string AddNumActions(int amount); //Increases numActions
	string DecNumActions(int amount); //Decreases numActions
	void Kill(); //Sets a cards health to zero
	
	//Action Queue Functions
	void PushAction(actions a); //Push new action into the AI's action queue
	actions PopAction(); //Remove the next action from the AI's action queue
	actions GetNextAction(); //Gets the next action in the action queue
	bool IsActionQueueEmpty(); //Check if cards action queue is empty
	void ClearActionQueue(); //Completely removes all elements from action queue
	size_t GetActionQueueSize(); //Returns the action queues current size
	
};

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
