/*
 * Eric Ryan Montgomery
 * 03/17/2025
 * For CardGameUI
 * Functions for deck/cards without the use of raylib are written here
 */
#include "deckofcards.h"

//----------------------------------------------- Card Class
//Constructor
card::card(enum colors color, enum attributes attribute, int number) {
	this->color = color;
	this->attribute = attribute;
	this->number = number;
	this->charge = 0;
	this->numActions = 1;
	this->physicalResistance = 1;
	this->magicalResistance = 1;
	this->upperDice = 20;
	this->lowerDice = 1;
	this->abilityUsed = false;
	this->abilitiesActive = true;
	this->spell = FORCE;
	CalcID();
	CalcPhysicalPower();
	CalcMagicalPower();
	CalcHealth();
	CalcName();
	CalcAbility();
}

//Overload =
void card::operator=(const card &rhs) {
	this->ID = rhs.ID;
	this->color = rhs.color;
	this->attribute = rhs.attribute;
	this->number = rhs.number;
	this->power = rhs.power;
	this->magicalPower = rhs.magicalPower;
	this->health = rhs.health;
	this->totalHealth = rhs.totalHealth;
	this->name = rhs.name;
	this->ability = rhs.ability;
	this->charge = rhs.charge;
	this->numActions = rhs.numActions;
	this->physicalResistance = rhs.physicalResistance;
	this->magicalResistance = rhs.magicalResistance;
	this->upperDice = rhs.upperDice;
	this->lowerDice = rhs.lowerDice;
	this->abilityUsed = rhs.abilityUsed;
	this->abilitiesActive = rhs.abilitiesActive;
	this->spell = rhs.spell;
}

//Overload << with pointer
ostream &operator<<(ostream &os, const card *rhs) {
	os << rhs->GetName() << " (P: " << rhs->GetPower() << 
		", H: " << rhs->GetHealth() << ")";
	return os;
}

//Overload << without pointer
ostream &operator<<(ostream &os, const card &rhs) {
	os << rhs.GetName() << " (P: " << rhs.GetPower() << 
		", H: " << rhs.GetHealth() << ")";
	return os;
}

//Power Calculator
void card::CalcPhysicalPower() {
	power = 0;
	
	//Colors
	switch (color) {
		case (C_WHITE):
			power += 1;
			break;
		case (C_GREEN):
			power += 2;
			break;
		case (C_BLUE):
			power += 3;
			break;
		case (C_YELLOW):
			power += 4;
			break;
		case (C_RED):
			power += 5;
			break;
		case (C_ORANGE):
			power += 6;
			break;
		default:
			power += 1;
			break;
	}
	
	//Attributes
	switch (attribute) {
		case (C_MIMIC):
			power += 1;
			break;
		case (C_DEX):
			power += 2;
			break;
		case (C_INT):
			power += 3;
			break;
		case (C_STR):
			power += 4;
			break;
		case (C_FTH):
			power += 5;
			break;
		case (C_ARC):
			power += 6;
			break;
		default:
			power += 1;
			break;
	}
	
	power += (number/2);
	
	if (power <= 0) power = 1;
	power *= powerMod;
}

void card::CalcMagicalPower() {
	magicalPower = number * 2;
}

//Health Calculator
void card::CalcHealth() {
	health = 0;
	
	//Colors
	switch (color) {
		case (C_WHITE):
			health += 1;
			break;
		case (C_GREEN):
			health += 2;
			break;
		case (C_BLUE):
			health += 3;
			break;
		case (C_YELLOW):
			health += 4;
			break;
		case (C_RED):
			health += 5;
			break;
		case (C_ORANGE):
			health += 6;
			break;
		default:
			health += 1;
			break;
	}
	
	//Attributes
	switch (attribute) {
		case (C_MIMIC):
			health += 1;
			break;
		case (C_DEX):
			health += 2;
			break;
		case (C_INT):
			health += 3;
			break;
		case (C_STR):
			health += 4;
			break;
		case (C_FTH):
			health += 5;
			break;
		case (C_ARC):
			health += 6;
			break;
		default:
			health += 1;
			break;
	}
	
	health *= 5;
	health += number;
	
	if (health <= 0) health = 1;
	health *= healthMod;
	totalHealth = health;
}

//Name Calculator
void card::CalcName() {
	name = "Fatal Error";
	
	switch(ID) {
		//White
		case (101): //Mimic
			name = "Plain Mimic";
			break;
		case (102): //Str
			name = "Weighty Card";
			break;
		case (103): //FTH
			name = "Holy Card";
			break;
		case (104): //DEX
			name = "Speedy Card";
			break;
		case (105): //INT
			name = "Brillant Card";
			break;
		case (106): //ARC
			name = "Arcane Sigil";
			break;
		
		//Green
		case (201):
			name = "Green Mimic";
			break;
		case (202):
			name = "Undead Rage";
			break;
		case (203):
			name = "Divine Corpse";
			break;
		case (204):
			name = "The Necromancer";
			break;
		case (205):
			name = "Reawakened Soul";
			break;
		case (206):
			name = "Earth Sigil";
			break;
		
		//Blue 
		case (301):
			name = "Blue Mimic";
			break;
		case (302):
			name = "Sea Hammer";
			break;
		case (303):
			name = "Aqua Light";
			break;
		case (304):
			name = "Dancing Katana";
			break;
		case (305):
			name = "Diluted Knowlege";
			break;
		case (306):
			name = "Ocean Sigil";
			break;
		
		//Yellow
		case (401): //Mimic
			name = "Yellow Mimic";
			break;
		case (402): //STR
			name = "Pure Thunder";
			break;
		case (403): //FTH
			name = "Holy Light";
			break;
		case (404): //DEX
			name = "Lightning Hand";
			break;
		case (405): //INT
			name = "Photon";
			break;
		case (406):
			name = "Luminous Sigil";
			break;
		
		//Red
		case (501):
			name = "Red Mimic";
			break;
		case (502):
			name = "Ignited Rage";
			break;
		case (503):
			name = "Burning Faith";
			break;
		case (504):
			name = "Red Stance";
			break;
		case (505):
			name = "Ablazed Mind";
			break;
		case (506):
			name = "Bloody Sigil";
			break;
			
		//Orange
		case (601): //Mimic
			name = "Orange Mimic";
			break;
		case (602): //STR
			name = "Radiant Sentinel";
			ability = TERROR;
			break;
		case (603): //FTH
			name = "Solar Crusader";
			break;
		case (604): //DEX
			name = "Cinderblade";
			break;
		case (605): //INT
			name = "Scholar of the Sun";
			break;
		case (606):
			name = "Heavenly Sigil";
			break;
			
		//If no other
		default:
			name = "Blank Card";
			break;
	}
}

//Ability Calculator
void card::CalcAbility() {
	
	switch(ID) {
		//White
		case (101): //Mimic
			ability = MIMICRY;
			break;
		case (102): //Str
			ability = HEAVY_HANDED;
			break;
		case (103): //FTH
			ability = FAITHFUL;
			break;
		case (104): //DEX
			ability = FAST_HAND;
			break;
		case (105): //INT
			ability = HEALTHY_MIND;
			break;
		
		//Green
		case (201):
			ability = MIMICRY;
			break;
		case (202):
			ability = TERROR;
			break;
		case (203):
			ability = EVIL_PRESENCE;
			break;
		case (204):
			ability = NECROMANCY;
			break;
		case (205):
			ability = NULLIFY;
			break;
		
		//Blue 
		case (301):
			ability = MIMICRY;
			break;
		case (302):
			ability = ACCURATE;
			break;
		case (303):
			ability = HOLY_PRESENCE;
			break;
		case (304):
			ability = INNERGATE;
			break;
		case (305):
			ability = NULLIFY;
			break;
		
		//Yellow
		case (401): //Mimic
			ability = MIMICRY;
			break;
		case (402): //STR
			ability = HEAVY_HANDED;
			break;
		case (403): //FTH
			ability = HOLY_PRESENCE;
			break;
		case (404): //DEX
			ability = FAST_HAND;
			break;
		case (405): //INT
			ability = CHAOS;
			break;
		
		//Red
		case (501):
			ability = MIMICRY;
			break;
		case (502):
			ability = RESISTANT;
			break;
		case (503):
			ability = FAITHFUL;
			break;
		case (504):
			ability = MADE_IN_HEAVEN;
			break;
		case (505):
			ability = MAGIC_IMMUNITY;
			break;
		
		//Orange
		case (601): //Mimic
			ability = MIMICRY;
			break;
		case (602): //STR
			ability = TERROR;
			break;
		case (603): //FTH
			ability = MAGIC_IMMUNITY;
			break;
		case (604): //DEX
			ability = ACCURATE;
			break;
		case (605): //INT
			ability = HEAVY_HANDED;
			break;
		default:
			ability = MIMICRY;
			break;
	}
}

//ID Calculator
void card::CalcID() {
	ID = 0;
	
	//Colors
	switch (color) {
		case (C_WHITE):
			ID += 100;
			break;
		case (C_GREEN):
			ID += 200;
			break;
		case (C_BLUE):
			ID += 300;
			break;
		case (C_YELLOW):
			ID += 400;
			break;
		case (C_RED):
			ID += 500;
			break;
		case (C_ORANGE):
			ID += 600;
			break;
		default:
			ID += 0;
			break;
	}
	
	//Attributes
	switch (attribute) {
		case (C_MIMIC):
			ID += 1;
			break;
		case (C_STR):
			ID += 2;
			break;
		case (C_FTH):
			ID += 3;
			break;
		case (C_DEX):
			ID += 4;
			break;
		case (C_INT):
			ID += 5;
			break;
		case (C_ARC):
			ID += 6;
			break;
		default:
			ID += 0;
			break;
	}
}

//Changes a cards ability
bool card::ChangeAbility(int ability) {
	vector<abilities> list {
		MIMICRY,
		HEAVY_HANDED,
		FAITHFUL,
		FAST_HAND,
		HEALTHY_MIND,
		TERROR,
		RESISTANT,
		HOLY_PRESENCE,
		EVIL_PRESENCE,
		ACCURATE,
		INNERGATE,
		NECROMANCY,
		DIVINEHELP,
	};
	if (ability < 0 || ability > int(list.size())-1) return false;
	this->ability = list.at(ability);
	return true;
}

void card::ChangeAbility(abilities ab) {
	ability = ab;
}

//Return a cards ability as a string
string card::GetAbilityStr() const {
	switch (ability) {
		case (MIMICRY):
			return "Mimicry";
		case (HEAVY_HANDED):
			return "Heavy Handed";
		case (FAITHFUL):
			return "Faithful";
		case (FAST_HAND):
			return "Fast Hand";
		case (HEALTHY_MIND):
			return "Healthy Mind";
		case (TERROR):
			return "Terror";
		case (RESISTANT):
			return "Resistant";
		case (HOLY_PRESENCE):
			return "Holy Presence";
		case (EVIL_PRESENCE):
			return "Evil Presence";
		case (ACCURATE):
			return "Accurate";
		case (INNERGATE):
			return "Inner Gate";
		case (NECROMANCY):
			return "Necromancy";
		case (MAGIC_IMMUNITY):
			return "Magic Immunity";
		case (DIVINEHELP):
			return "Divine Help";
		case (NULLIFY):
			return "Nullify";
		case (MADE_IN_HEAVEN):
			return "Made In Heaven";
		case (CHAOS):
			return "Chaos";
		case (STRATEGICFIRE):
			return "Strategic Fire";
		default:
			return "Error";
	};
}

//Return a cards name
string card::GetName() const {
	return name;
}

//Return a cards number
int card::GetNumber() const {
	return number;
}

//Return a cards power
int card::GetPower() const {
	return power;
}

//Return a cards magical power
int card::GetMagicalPower() const {
	return magicalPower;
}

//Return a cards health
int card::GetHealth() const {
	return health;
}

//Return a cards total health
int card::GetHealthT() const {
	return totalHealth;
}

//Return a cards charge
int card::GetCharge() const {
	return charge;
}

//Return amount of actions
int card::GetNumActions() const {
	return numActions;
}

//Return a cards physicalResistance
double card::GetPhysicalResistance() const {
	return physicalResistance;
}

//Return a cards magical resistance
double card::GetMagicalResistance() const {
	return magicalResistance;
}

//Return a cards ID
int card::GetID() const {
	return ID;
}

//Return a cards color as enum
enum colors card::GetColor() const {
	return color;
}

//Return a cards color as a string
string card::GetColorStr() const {
	switch (color) {
		case (C_WHITE):
			return "White";
		case (C_GREEN):
			return "Green";
		case (C_BLUE):
			return "Blue";
		case (C_YELLOW):
			return "Yellow";
		case (C_RED):
			return "Red";
		case (C_ORANGE):
			return "Orange";
		default:
			return "Error";
	};
}

//Return a cards color as a Raylib color
Color card::GetColorRaylib() const {
	switch (color) {
		case (C_WHITE):
			return WHITE;
		case (C_GREEN):
			return GREEN;
		case (C_BLUE):
			return BLUE;
		case (C_YELLOW):
			return YELLOW;
		case (C_RED):
			return RED;
		case (C_ORANGE):
			return ORANGE;
		default:
			return WHITE;
	};
}

//Return a cards attribute as enum
enum attributes card::GetAttribute() const {
	return attribute;
}

//Return a cards attrubute as a string
string card::GetAttributeStr() const {
	switch (attribute) {
		case (C_MIMIC):
			return "Mimic";
		case (C_STR):
			return "Strength";
		case (C_FTH):
			return "Faith";
		case (C_DEX):
			return "Dexterity";
		case (C_INT):
			return "Intellect";
		case (C_ARC):
			return "Arcane";
		default:
			return "Error";
	};
}

//Return a cards ability as enum
enum abilities card::GetAbility() const {
	return ability;
}

//Charges up card
string card::ChargeUp() {
	ostringstream oss;
	if (charge < 0) {
		charge = 0;
		name.erase(name.begin() + name.size()-1);
		oss << name << " regained their strength!";
	} else {
		++charge;
		this->health += 10;
		this->totalHealth += 10;
		this->power += 3;
		if (magicalPower > 0) this->magicalPower += 3;
		oss << name << " charged up!";
		this->name += '+';
	}
	return oss.str();
}

//Decreases a cards charge only if it is zero
void card::ChargeDown() {
	switch(charge) {
		case(0):
			name += '-';
			charge = -1;
			break;
		default:
			break;
	}
}

//Increases the cards power
string card::AddPower(int amount) {
	ostringstream oss;
	this->power += amount;
	oss << name << "'s power was increased by "
		<< amount << " points!";
	return oss.str();
}

//Increases the cards magical power
string card::AddMagicalPower(int amount) {
	ostringstream oss;
	this->magicalPower += amount;
	oss << name << "'s magical power was increased by "
		<< amount << " points!";
	return oss.str();
}

//Increases the cards health
string card::AddHealth(int amount) {
	ostringstream oss;
	this->health += amount;
	this->totalHealth += amount;
	oss << name << "'s health was increased by "
		<< amount << " points!";
	return oss.str();
}

//Heals the cards by an amount
string card::Heal(int amount) {
	ostringstream oss;
	this->health += amount;
	if (health > totalHealth) {
		health = totalHealth;
		oss << name << " was fully healed!";
	} else {
		oss << name << "'s health was increased by "
			<< amount << " points!";
	}
	return oss.str();
}

//Fully heals the card
string card::FullHeal() {
	ostringstream oss;
	health = totalHealth;
	oss << name << " was fully healed!";
	return oss.str();
}

//Decreases a cards power
string card::DecPower(int amount) {
	ostringstream oss;
	power -= amount;
	if (power < 1) {
		oss << name << " has lost the will to live!";
		health = 0;
	} else {
		oss << name << "'s power was decreased by "
			<< amount << " points!";
	}
	return oss.str();
}

//Decreases a cards health and total health
string card::DecBothHealth(int amount) {
	ostringstream oss;
	health -= amount;
	totalHealth -= amount;
	if (health < 1 || totalHealth < 1) {
		oss << name << " has perished!";
	} else {
		oss << name << "'s health was decreased by "
			<< amount << " points!";
	}
	return oss.str();
}

//Decreases a cards total health
string card::DecTotalHealth(int amount) {
	ostringstream oss;
	totalHealth -= amount;
	if (health > totalHealth) health = totalHealth;
	if (totalHealth < 1) {
		oss << name << " has perished!";
	} else {
		oss << name << "'s total health was decreased by "
			<< amount << " points!";
	}
	return oss.str();
}

//Decreases a cards health
string card::DecHealth(int amount) {
	ostringstream oss;
	health -= amount;
	if (health < 1) {
		oss << name << " has perished!";
	} else {
		oss << name << "'s health was decreased by "
			<< amount << " points!";
	}
	return oss.str();
}

//Increases a cards physicalResistance
string card::AddPhysicalResistance(double amount) {
	ostringstream oss;
	physicalResistance -= amount;
	if (physicalResistance < 0.1) {
		physicalResistance = physicalResistance - 0.1;
		if (physicalResistance < 0) physicalResistance *= -1;
		amount = amount - physicalResistance;
		physicalResistance = 0.1;
	}
	oss << name << "'s physical resistance increased by "
		<< (amount * 100) << " percent!";
	return oss.str();
}

//Decreases a cards physicalResistance
string card::DecPhysicalResistance(double amount) {
	ostringstream oss;
	physicalResistance += amount;
	if (physicalResistance > 2) {
		physicalResistance = physicalResistance - 2;
		if (physicalResistance < 0) physicalResistance *= -1;
		amount = amount - physicalResistance;
		physicalResistance = 2;
	}
	oss << name << "'s physical resistance decreased by "
		<< (amount * 100) << " percent!";
	return oss.str();
}

//Increases a cards physicalResistance
string card::AddMagicalResistance(double amount) {
	ostringstream oss;
	magicalResistance -= amount;
	if (magicalResistance < 0.1) {
		magicalResistance = magicalResistance - 0.1;
		if (magicalResistance < 0) magicalResistance *= -1;
		amount = amount - magicalResistance;
		magicalResistance = 0.1;
	}
	oss << name << "'s magical resistance increased by "
		<< (amount * 100) << " percent!";
	return oss.str();
}

//Decreases a cards physicalResistance
string card::DecMagicalResistance(double amount) {
	ostringstream oss;
	magicalResistance += amount;
	if (magicalResistance > 2) {
		magicalResistance = magicalResistance - 2;
		if (magicalResistance < 0) magicalResistance *= -1;
		amount = amount - magicalResistance;
		magicalResistance = 2;
	}
	oss << name << "'s magical resistance decreased by "
		<< (amount * 100) << " percent!";
	return oss.str();
}

//Increases Upper Dice thus decreasing # of crit hits
string card::AddUpperDice(int amount) {
	ostringstream oss;
	upperDice += amount;
	if (upperDice > 21) {
		upperDice = 21;
		oss << name << "'s critical chance can not go "
			<< "any lower!";
	} else {
		oss << name << "'s critical chance decreased!";
	}
	return oss.str();
}

//Decreases Upper Dice thus increasing # of crit hits
string card::DecUpperDice(int amount) {
	ostringstream oss;
	upperDice -= amount;
	if (upperDice < 10) {
		upperDice = 10;
		oss << name << "'s critical chance can not go "
			<< "any higher!";
	} else {
		oss << name << "'s critical chance increased!";
	}
	return oss.str();
}

//Increases Lower Dice thus increasing # of missed hits
string card::AddLowerDice(int amount) {
	ostringstream oss;
	lowerDice += amount;
	if (lowerDice > 10) {
		lowerDice = 10;
		oss << name << "'s miss chance can not go "
			<< "any higher!";
	} else {
		oss << name << "'s miss chance increased!";
	}
	return oss.str();
}

//Decreases Lower Dice thus decreasing # of missed hits
string card::DecLowerDice(int amount) {
	ostringstream oss;
	lowerDice -= amount;
	if (lowerDice < 0) {
		lowerDice = 0;
		oss << name << "'s miss chance can not go "
			<< "any lower!";
	} else {
		oss << name << "'s miss chance decreased!";
	}
	return oss.str();
}

//Increases a cards number of actions
string card::AddNumActions(int amount) {
	numActions += amount;
	ostringstream oss;
	oss << name << " gained " << amount
		<< " action(s)!";
	return oss.str();
}

//Decreases a cards number of actions
string card::DecNumActions(int amount) {
	ostringstream oss;
	numActions -= amount;
	
	//If amount > this->numActions find the number
	//that this function will actually decrease numActions
	//by.
	if (numActions < 1) {
		numActions = numActions - 1;
		if (numActions < 0) numActions *= -1;
		amount = amount - numActions;
		numActions = 1;
	}
	oss << name << " lost " << amount
		<< " action(s)!";
	return oss.str();
}

//Rolls a Dice and Returns a HIT, MISS, OR CRITHIT
enum attackActions card::RollDiceEnum() {
	int roll = (rand() % 20) + 1;
	if (roll <= lowerDice) return MISS;
	if (roll >= upperDice) return CRITHIT;
	return HIT;
}

//Rolls a Dice and returns an integer between 1 and 20
int card::RollDiceInt(int limit) {
	return (rand() % limit) + 1;
}

//Kills Card
void card::Kill() {
	health = 0;
}

//Returns if the cards ability is used or not
bool card::GetAbilityUsed() {
	return abilityUsed;
}

//Sets if a cards ability is used or not
void card::SetAbilityUsed(bool b) {
	abilityUsed = b;
}

//Returns if the cards ability is active or not
bool card::GetAbilitiesActive() {
	return abilitiesActive;
}

//Sets if a cards ability is active or not
void card::SetAbilitiesActive(bool b) {
	abilitiesActive = b;
}

//Gets a cards spell
spells card::GetSpell() {
	return spell;
}

//Gets a cards spell as a string
string card::GetSpellStr() {
	switch(spell) {
		case (FORCE):
			return "Force";
		case (DRAIN):
			return "Drain";
		case (WEAKEN):
			return "Weaken";
		case (HEAL):
			return "Heal";
		default:
			return "Error";
	}
}

//Sets a cards spell
void card::SetSpell(spells s) {
	spell = s;
}

//Push new action into the AI's action queue
void card::PushAction(actions a) {
	this->actionQueue.push(a);
}

//Remove the next action from the AI's action queue
actions card::PopAction() {
	if (!actionQueue.empty()) {
		actions temp = actionQueue.front();
		actionQueue.pop();
		return temp;
	}
	return ERROR;
}

//Gets the next action in the action queue
actions card::GetNextAction() {
	if (!actionQueue.empty()) {
		return actionQueue.front();
	}
	return ERROR;
}

//Check if cards action queue is empty
bool card::IsActionQueueEmpty() {
	return actionQueue.empty();
}

//Completely removes all elements from action queue
void card::ClearActionQueue() {
	while(!actionQueue.empty()) {
		actionQueue.pop();
	}
}

//Returns the action queues current size
size_t card::GetActionQueueSize() {
	return actionQueue.size();
}

//----------------------------------------------- Deck Class
//Constructor
deck::deck(int cardLimit, shared_ptr<Texture2D> texture, bool random, bool ai, int deckStrength) : cardTexture(texture) {
	this->amtCards = 0;
	this->timesUsedFlask = 0;
	this->timesSwapped = 0;
	this->ai = ai;
	this->cardLimit = cardLimit;
	this->totalPoints = deckStrength * cardLimit;
	this->remainingPoints = totalPoints;
	if (random == true) {
		CreateRandomCards();
	} else {
		CreateBlankCards();
	}
}

//Deconstructor
deck::~deck() {
	for (card *c : Cards) {
		delete c;
	}
	Cards.clear();
}

// Copy Constructor
deck::deck(const deck &other) : cardTexture(other.cardTexture) {
	this->ai = other.ai;
	this->cardLimit = other.cardLimit;
	this->timesUsedFlask = other.timesUsedFlask;
	this->timesSwapped = other.timesSwapped;
	this->totalPoints = other.totalPoints;
	this->remainingPoints = other.remainingPoints;
	this->amtCards = 0; //For safety reasons set amtCards equal to zero and add a card through the AddCard function
	for (int i = 0; i < amtCards; ++i) {
		card *temp = new card(*other.Cards[i]);
		this->AddCard(temp);
	}
}

//Overload = operator so that two decks can be set equal to each other
void deck::operator=(const deck &rhs) {
	vector<spells> appliedSpells;
	for (int i = 0; i < this->amtCards; ++i) {
		delete this->Cards[i];
	}
	this->Cards.clear();
	this->cardLimit = rhs.cardLimit;
	this->timesUsedFlask = rhs.timesUsedFlask;
	this->timesSwapped = rhs.timesSwapped;
	this->ai = rhs.ai;
	this->totalPoints = rhs.totalPoints;
	this->remainingPoints = rhs.remainingPoints;
	this->amtCards = 0; //For safety reasons set amtCards equal to zero and add a card through the AddCard function
	for (int i = 0; i < rhs.amtCards; ++i) {
		card *temp = new card(*rhs.Cards[i]);
		this->AddCard(temp);
	}
	this->cardTexture = rhs.cardTexture;
}

//Creates a Random Deck with a point limit
void deck::CreateRandomCards() {
	
	//Colors array and initialization
	colors colorChoice[cardLimit];
	for (int i = 0; i < cardLimit; ++i) {
		colorChoice[i] = C_WHITE;
	}
	
	//Attributes array and initialization
	attributes attChoice[cardLimit];
	for (int i = 0; i < cardLimit; ++i) {
		attChoice[i] = C_MIMIC;
	}
	
	//Power Level array and initialization
	int powerLevel[cardLimit]; //Power Level is worth exact amt of points. Ex: power level 5 is worth 5 points.
	for (int i = 0; i < cardLimit; ++i) {
		powerLevel[i] = 0;
	}
	
	//Chosen Spells
	spells chosenSpells[cardLimit];
	for (int i = 0; i < cardLimit; ++i) {
		chosenSpells[i] = spellList[rand() % spellList.size()];
	}
	
	
	//Randomly choose an aspect of a random card and increase it by one until
	//all points are used.
	int indexChosen;
	int aspectChosen;
	for (int i = 0; i < totalPoints; ++i) {
		indexChosen = rand() % cardLimit; //Choose a random card in the deck
		aspectChosen = rand() % 3; //Choice either color, attribute, or power level to upgrade
		switch(aspectChosen) {
			case(0):
				if (colorChoice[indexChosen] < cols[cols.size()-1]) colorChoice[indexChosen] = cols[colorChoice[indexChosen] + 1];
				else --i;
				break;
			case(1):
				if (attChoice[indexChosen] < atts[atts.size()-1]) attChoice[indexChosen] = atts[attChoice[indexChosen] + 1];
				else --i;
				break;
			case(2):
				if (powerLevel[indexChosen] < 9) powerLevel[indexChosen] += 1;
				else --i;
				break;
		};
	}
	
	//Add all the cards with there chosen aspects to the deck
	for (int i = 0; i < cardLimit; ++i) {
		this->AddCard(colorChoice[i], attChoice[i], powerLevel[i]);
		Cards[i]->SetSpell(chosenSpells[i]);
	}
	
	remainingPoints = 0;
}

void deck::CreateBlankCards() {
	for (int i = 0; i < cardLimit; ++i) {
		this->AddCard(C_WHITE, C_MIMIC, 0);
		Cards[i]->SetSpell(FORCE);
	}
}

//Adds a card to the deck
bool deck::AddCard(card *newCard) {
	if (amtCards >= cardLimit) return false;
	Cards.push_back(newCard);
	this->amtCards = this->amtCards + 1;
	return true;
}

//Adds card to the deck
bool deck::AddCard(enum colors col, enum attributes att, int num) {
	if (amtCards >= cardLimit) {
		cout << "here" << endl;
		return false;
	}
	card *newCard = new card(col, att, num);
	Cards.push_back(newCard);
	this->amtCards = this->amtCards + 1;
	return true;
}

//Checks if the deck is full or not
bool deck::IsDeckFull() {
	if (amtCards >= cardLimit) return true;
	return false;
}

//Removes a card from the deck by position
bool deck::RemoveCard(int pos) {
	if (pos < 0 || pos > amtCards-1) return false;
	delete Cards[pos];
	Cards.erase(Cards.begin() + pos);
	return true;
}

//Get a card from the deck
card* deck::at(int const index) const {
	if (index > amtCards-1) {
		cout << "Indexing Failure!" << endl;
		return nullptr;
	}
	return Cards[index];
}

//Overload the [] operator returns pointer
card* deck::operator[](int const index) const {
	if (index > amtCards-1) {
		cout << "Indexing Failure!" << endl;
		return nullptr;
	}
	return Cards[index];
}

//Overload << without pointer
ostream &operator<<(ostream& os, const deck &rhs) {
	for (int i = 0; i < rhs.GetCardAmt(); ++i) {
		cout << rhs[i];
		cout << endl;
	}
	return os;
}

//Gets the Card Limit in a Deck
int deck::GetCardLimit() const {
	return cardLimit;
}

//Gets the current amount of cards in a deck
int deck::GetCardAmt() const {
	return amtCards;
}

//Gets the current amount of cards in a deck
int deck::size() const {
	return amtCards;
}

//Sets flaskAmt to a user defined value
void deck::SetFlaskAmt(int amt) {
	timesUsedFlask = amt;
}

//Sets timesSwapped to a user defined value
void deck::SetTimesSwapped(int amt) {
	timesSwapped = amt;
}

//Return Flask Amount
int deck::GetFlaskAmt() {
	return timesUsedFlask;
}

//Return Times Swapped
int deck::GetTimesSwapped() const {
	return timesSwapped;
}

//Checks to see if a string is comprised of only integers
bool deck::CheckIfNumber(string &number) {
	for (char &c : number) {
		if (isdigit(c) == false) return false;
	}
	return true;
}

std::string deck::Swap(int i, int j) {
    std::ostringstream oss;
    
    if (i >= amtCards || j >= amtCards || i == j) 
        return oss.str(); // Return empty string if indices are invalid or identical

    // Swap cards using a temporary pointer instead of dynamic allocation
    std::swap(Cards[i], Cards[j]);

    ++timesSwapped;

    // Construct a meaningful message
    oss << Cards[j]->GetName() << " was swapped for " << Cards[i]->GetName() << "!";
    return oss.str();
}

//Fisher-Yates Algorithm to shuffle cards
void deck::ShuffleDeck() {
	for (int i = amtCards - 1; i > 0; --i) {
		int j = rand() % (i + 1);
		// Swap arr[i] with arr[j]
		std::swap(*Cards[i], *Cards[j]);
	}
}

//Uses a flask
string deck::UseFlask(int &pos) {
	ostringstream oss;
	oss << Cards[pos]->FullHeal();
	++timesUsedFlask;
	return oss.str();
}

/*
//AI move calculator
actions deck::MakeAIDecision(deck const &opponent, int const &pos, 
int const &turn) {
	actions nextChoice = ERROR;
	
	//Remove decisions if they have been made worthless
	//Remove Charges if max charge
	if (!AIQueue.empty() && AIQueue.front() == CHARGE && 
		Cards[pos]->GetCharge() >= 2) {
		while(AIQueue.front() == CHARGE) {
			AIQueue.pop();
		}
	}
	
	//Remove Swaps if out of swaps
	if (!AIQueue.empty() && AIQueue.front() == SWAP &&
		(timesSwapped >= 2 || pos >= cardLimit-1)) {
		while(AIQueue.front() == SWAP) {
			AIQueue.pop();
		}
	}
	
	//Remove uses of flask if out of flask
	if (!AIQueue.empty() && AIQueue.front() == FLASK &&
		timesUsedFlask >= 2) {
		while(AIQueue.front() == FLASK) {
			AIQueue.pop();
		}
	}
	
	//AI to continue making previously made choices
	if (!AIQueue.empty()) {
		nextChoice = AIQueue.front();
		AIQueue.pop();
		return nextChoice;
	}
	
	int const num = (rand() % 20) + 1;
	
	//Instant win choices
	//95% to attack if power is greater than opponents health
	if (Cards[pos]->GetPower() >= opponent[pos]->GetHealth() && num > 1) {
		AIQueue.push(ATTACK);
		
	//Preserve Self Choices
	//If below 30% health 50% chance to use flask if availiable
	} else if (Cards[pos]->GetHealth()/Cards[pos]->GetHealthT() <= 0.30 && 
		timesUsedFlask < 2 && num > 10) {
			AIQueue.push(FLASK);
	
	//Concrete Counters
	//Counter Fast Hands ability with NULLIFY OR MADE IN HEAVEN
	} else if (opponent[pos]->GetAbility() == FAST_HAND && 
		(Cards[pos]->GetAbility() == NULLIFY || Cards[pos]->GetAbility() == MADE_IN_HEAVEN) && 
		Cards[pos]->GetCharge() < 2) {
		for (int i = Cards[pos]->GetCharge(); i < 2; ++i) {
			AIQueue.push(CHARGE);
		}
		
	//Other Strategies
	//If below 30% health 25% chance to use swap if availiable
	} else if (Cards[pos]->GetHealth()/Cards[pos]->GetHealthT() <= 0.30 && 
		timesSwapped < 2 && pos < cardLimit-1 && num > 15) {
			AIQueue.push(SWAP);
			cout << "Chose first swap" << endl;
		
	//If Turn 1 or 2 10% chance to use a charge/charge/attack sequence
	} else if ((turn == 1 || turn == 2) && num > 18) {
			AIQueue.push(CHARGE);
			AIQueue.push(CHARGE);
			AIQueue.push(ATTACK);
	
	//If nothing else was chosen make a somewhat random choice
	} else {
		if (num > 15 && timesUsedFlask < 2) {
			AIQueue.push(FLASK);
		} else if (num > 13 && timesSwapped < 2 && pos < cardLimit-1) {
			AIQueue.push(SWAP);
			cout << "Chose last swap" << endl;
		} else if (num > 10 && Cards[pos]->GetCharge() < 2) {
			AIQueue.push(CHARGE);
		} else if (num > 5 && Cards[pos]->GetMagicalPower() > 0) {
			AIQueue.push(CASTSPELL);
		} else {
			AIQueue.push(ATTACK);
		}
	}
	
	//Return final choice
	nextChoice = AIQueue.front();
	AIQueue.pop();
	return nextChoice;
}
*/

//Check if bot or not
bool deck::IsAI() {
	return ai;
}

//Set if bot or not
void deck::SetAI(bool b) {
	ai = b;
}

void deck::RestoreDeck() {
    // Use vectors to store data dynamically
    std::vector<colors> cardColors(amtCards);
    std::vector<attributes> cardAttributes(amtCards);
    std::vector<int> cardPowerLevels(amtCards);

    // Copy data from existing cards
    for (int i = 0; i < amtCards; ++i) {
        cardColors[i] = Cards[i]->GetColor();
        cardAttributes[i] = Cards[i]->GetAttribute();
        cardPowerLevels[i] = Cards[i]->GetNumber();
    }

    // Delete existing cards
    for (card* c : Cards) {
        delete c;
    }
    Cards.clear();
    
    int amtCardsToRestore = amtCards;
    amtCards = 0;
	
    // Restore cards using stored attributes
    for (int i = 0; i < amtCardsToRestore; ++i) {
        if (!AddCard(cardColors[i], cardAttributes[i], cardPowerLevels[i])) {
			cout << "Card Adding Failure while Restoring Deck!" << endl;
		}
    }
    
    cout << "Amount Cards in Deck: " << amtCards << endl;

    // Reset counters
    timesSwapped = 0;
    timesUsedFlask = 0;
}

shared_ptr<Texture2D> &deck::GetTexture() {
	return cardTexture;
}

int deck::GetTotalPoints() {
	return totalPoints;
}

int deck::GetRemainingPoints() {
	return remainingPoints;
}

void deck::SetRemainingPoints(int amt) {
	if (amt > totalPoints) amt = totalPoints;
	if (amt < 0) amt = 0;
	remainingPoints = amt;
}
