/*
 * Eric Ryan Montgomery
 * 09/21/2025
 * For CardGameUI
 * Functions for a deck of cards without the use of raylib are written here
 */
#include "deckofcards.h"

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
	//colors colorChoice[cardLimit];
	vector<colors> colorChoice(cardLimit);
	for (int i = 0; i < cardLimit; ++i) {
		colorChoice[i] = C_WHITE;
	}
	
	//Attributes array and initialization
	//attributes attChoice[cardLimit];
	vector<attributes> attChoice(cardLimit);
	for (int i = 0; i < cardLimit; ++i) {
		attChoice[i] = C_MIMIC;
	}
	
	//Power Level array and initialization
	//int powerLevel[cardLimit]; 
	vector<int> powerLevel(cardLimit); //Power Level is worth exact amt of points. Ex: power level 5 is worth 5 points.
	for (int i = 0; i < cardLimit; ++i) {
		powerLevel[i] = 0;
	}
	
	//Chosen Spells
	//spells chosenSpells[cardLimit];
	vector<spells> chosenSpells(cardLimit);
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
