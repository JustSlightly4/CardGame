/*
 * Eric Ryan Montgomery
 * 09/21/2025
 * For CardGameUI
 * Functions for a single Card without the use of raylib are written here
 */
 
#ifndef CARDSH
#define CARDSH
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
using namespace std;
	
class Card {
	public:
		//Public Enums
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

		enum spells {
			FORCE,
			DRAIN,
			WEAKEN,
			HEAL,
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

		//Public Vectors
		//List of colors and attributes
		static const vector<Card::colors> cols;
		static const vector<Card::attributes> atts;
		static const vector<Card::spells> spellList;
		static const vector<Card::actions> actionsList;
		static const std::unordered_map<Card::colors, Card::colors> advantage;

		//Public Functions:
		Card (enum colors color, enum attributes attribute, int number); //Constructor
		void operator=(const Card &rhs);//Overload =
		string GetName() const; //Return a cards name
		int GetNumber() const; //Return a cards number
		int GetPower() const; //Return a cards physical power
		int GetMagicalPower() const; //Return a cards magical power
		int GetHealth() const; //Return a cards health
		int GetHealthT() const; //Return a cards total health
		int GetCharge() const; //Return a cards charge
		int GetNumActions() const; //Return amount of actions
		double GetPhysicalResistance() const; //Return a cards physicalResistance
		string GetPhysicalResistanceStr(int precision) const;
		double GetMagicalResistance() const; //Return a cards magical resistance
		string GetMagicalResistanceStr(int precision) const;
		int GetID() const; //Return a cards ID
		enum colors GetColor() const; //Return a cards color as enum
		string GetColorStr() const; //Return a cards color as a str
		Color GetColorRaylib() const; //Returns a cards color as Raylibs definition
		enum attributes GetAttribute() const; //Return a cards attribute as enum
		string GetAttributeStr() const; //Return a cards attribute as a string
		enum abilities GetAbility() const; //Return a cards ability as enum
		string GetAbilityStr() const; //Return a cards ability as a string
		enum attackActions RollDiceEnum(); //Rolls the dice for a Card return enum
		int RollDiceInt(int limit); //Rolls the dice for a Card return int
		ostringstream Print(); //Print card Details
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
		string ChargeUp(); //Charges up Card
		void ChargeDown(); //Decreases a cards charge
		string AddPower(int amount); //Increases the cards power
		string AddMagicalPower(int amount); //Increase the cards power
		string AddHealth(int amount); //Increases the cards health
		void SetHealth(int amount); //Sets the cards healths
		string Heal(int amount); //Heals the cards by an amount
		string FullHeal(); //Fully heals the Card
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
		
		//--------------------------------------------------------------------------

	protected:
		//User Defined Variables:
		colors color; //Color of the Card
		attributes attribute; //Attribute of the Card
		int number; //Power level of the Card
		
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
		int charge; //Number of times a Card has charged up
		int numActions; //Number of actions a Card can take in one turn
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

};


#endif
