/*
 * Eric Ryan Montgomery
 * 09/21/2025
 * For CardGameUI
 * Functions for a Deck of cards without the use of raylib are written here
 */
#include "Deck.h"

//----------------------------------------------- deck Class

//Constructor
Deck::Deck(int cardLimit, shared_ptr<Texture2D> texture, bool random, int deckStrength) : cardTexture(texture) {
	this->amtCards = 0;
	this->timesUsedFlask = 0;
	this->timesSwapped = 0;
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
Deck::~Deck() {
	for (Card *c : Cards) {
		delete c;
	}
	Cards.clear();
}

// Copy Constructor
Deck::Deck(const Deck &other) : cardTexture(other.cardTexture) {
	this->cardLimit = other.cardLimit;
	this->timesUsedFlask = other.timesUsedFlask;
	this->timesSwapped = other.timesSwapped;
	this->totalPoints = other.totalPoints;
	this->remainingPoints = other.remainingPoints;
	this->amtCards = 0; //For safety reasons set amtCards equal to zero and add a Card through the AddCard function
	for (int i = 0; i < other.amtCards; ++i) {
		Card *temp = new Card(*other.Cards[i]);
		this->AddCard(temp);
	}
}

//Overload = operator so that two decks can be set equal to each other
void Deck::operator=(const Deck &rhs) {
	vector<Card::spells> appliedSpells;
	for (int i = 0; i < this->amtCards; ++i) {
		delete this->Cards[i];
	}
	this->Cards.clear();
	this->cardLimit = rhs.cardLimit;
	this->timesUsedFlask = rhs.timesUsedFlask;
	this->timesSwapped = rhs.timesSwapped;
	this->totalPoints = rhs.totalPoints;
	this->remainingPoints = rhs.remainingPoints;
	this->amtCards = 0; //For safety reasons set amtCards equal to zero and add a Card through the AddCard function
	for (int i = 0; i < rhs.amtCards; ++i) {
		Card *temp = new Card(*rhs.Cards[i]);
		this->AddCard(temp);
	}
	this->cardTexture = rhs.cardTexture;
}

//Creates a Random deck with a point limit
void Deck::CreateRandomCards() {
	
	//Colors array and initialization
	//colors colorChoice[cardLimit];
	vector<Card::colors> colorChoice(cardLimit);
	for (int i = 0; i < cardLimit; ++i) {
		colorChoice[i] = Card::C_WHITE;
	}
	
	//Attributes array and initialization
	//attributes attChoice[cardLimit];
	vector<Card::attributes> attChoice(cardLimit);
	for (int i = 0; i < cardLimit; ++i) {
		attChoice[i] = Card::C_MIMIC;
	}
	
	//Power Level array and initialization
	//int powerLevel[cardLimit]; 
	vector<int> powerLevel(cardLimit); //Power Level is worth exact amt of points. Ex: power level 5 is worth 5 points.
	for (int i = 0; i < cardLimit; ++i) {
		powerLevel[i] = 0;
	}
	
	//Chosen Spells
	//spells chosenSpells[cardLimit];
	vector<Card::spells> chosenSpells(cardLimit);
	for (int i = 0; i < cardLimit; ++i) {
		chosenSpells[i] = Card::spellList[rand() % Card::spellList.size()];
	}
	
	
	//Randomly choose an aspect of a random Card and increase it by one until
	//all points are used.
	int indexChosen;
	int aspectChosen;
	for (int i = 0; i < totalPoints; ++i) {
		indexChosen = rand() % cardLimit; //Choose a random Card in the Deck
		aspectChosen = rand() % 3; //Choice either color, attribute, or power level to upgrade
		switch(aspectChosen) {
			case(0):
				if (colorChoice[indexChosen] < Card::cols[Card::cols.size()-1]) colorChoice[indexChosen] = Card::cols[colorChoice[indexChosen] + 1];
				else --i;
				break;
			case(1):
				if (attChoice[indexChosen] < Card::atts[Card::atts.size()-1]) attChoice[indexChosen] = Card::atts[attChoice[indexChosen] + 1];
				else --i;
				break;
			case(2):
				if (powerLevel[indexChosen] < 9) powerLevel[indexChosen] += 1;
				else --i;
				break;
		};
	}
	
	//Add all the cards with there chosen aspects to the Deck
	for (int i = 0; i < cardLimit; ++i) {
		this->AddCard(colorChoice[i], attChoice[i], powerLevel[i]);
		Cards[i]->SetSpell(chosenSpells[i]);
	}
	
	remainingPoints = 0;
}

void Deck::CreateBlankCards() {
	for (int i = 0; i < cardLimit; ++i) {
		this->AddCard(Card::C_WHITE, Card::C_MIMIC, 0);
		Cards[i]->SetSpell(Card::FORCE);
	}
}

//Adds a Card to the Deck
bool Deck::AddCard(Card *newCard) {
	if (amtCards >= cardLimit) return false;
	Cards.push_back(newCard);
	this->amtCards = this->amtCards + 1;
	return true;
}

//Adds Card to the Deck
bool Deck::AddCard(Card::colors col, Card::attributes att, int num) {
	if (amtCards >= cardLimit) {
		cout << "here" << endl;
		return false;
	}
	Card *newCard = new Card(col, att, num);
	Cards.push_back(newCard);
	this->amtCards = this->amtCards + 1;
	return true;
}

//Checks if the Deck is full or not
bool Deck::Isdeck_Full() {
	if (amtCards >= cardLimit) return true;
	return false;
}

//Removes a Card from the Deck by position
bool Deck::RemoveCard(int pos) {
	if (pos < 0 || pos > amtCards-1) return false;
	delete Cards[pos];
	Cards.erase(Cards.begin() + pos);
	return true;
}

//Get a Card from the Deck
Card* Deck::at(int const index) const {
	if (index < 0 || index > amtCards-1) {
		cout << "Indexing Failure!" << endl;
		return nullptr;
	}
	return Cards[index];
}

//Overload the [] operator returns pointer
Card* Deck::operator[](int const index) const {
	if (index < 0 || index > amtCards-1) {
		cout << "Indexing Failure!" << endl;
		return nullptr;
	}
	return Cards[index];
}

//Overload << without pointer
ostream &operator<<(ostream& os, const Deck &rhs) {
	for (int i = 0; i < rhs.GetCardAmt(); ++i) {
		cout << rhs[i];
		cout << endl;
	}
	return os;
}

//Gets the card Limit in a deck
int Deck::GetCardLimit() const {
	return cardLimit;
}

//Gets the current amount of cards in a Deck
int Deck::GetCardAmt() const {
	return amtCards;
}

//Gets the current amount of cards in a Deck
int Deck::size() const {
	return amtCards;
}

//Sets flaskAmt to a user defined value
void Deck::SetFlaskAmt(int amt) {
	timesUsedFlask = amt;
}

//Sets timesSwapped to a user defined value
void Deck::SetTimesSwapped(int amt) {
	timesSwapped = amt;
}

//Return Flask Amount
int Deck::GetFlaskAmt() {
	return timesUsedFlask;
}

//Return Times Swapped
int Deck::GetTimesSwapped() const {
	return timesSwapped;
}

//Checks to see if a string is comprised of only integers
bool Deck::CheckIfNumber(string &number) {
	for (char &c : number) {
		if (isdigit(c) == false) return false;
	}
	return true;
}

std::string Deck::Swap(int i, int j) {
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
void Deck::ShuffleDeck() {
	for (int i = amtCards - 1; i > 0; --i) {
		int j = rand() % (i + 1);
		// Swap arr[i] with arr[j]
		std::swap(*Cards[i], *Cards[j]);
	}
}

//Uses a flask
string Deck::UseFlask(int &pos) {
	ostringstream oss;
	oss << Cards[pos]->FullHeal();
	++timesUsedFlask;
	return oss.str();
}

/*
//AI move calculator
actions Deck::MakeAIDecision(Deck const &opponent, int const &pos, 
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

void Deck::RestoreDeck() {
    // Use vectors to store data dynamically
    std::vector<Card::colors> cardColors(amtCards);
    std::vector<Card::attributes> cardAttributes(amtCards);
    std::vector<int> cardPowerLevels(amtCards);

    // Copy data from existing cards
    for (int i = 0; i < amtCards; ++i) {
        cardColors[i] = Cards[i]->GetColor();
        cardAttributes[i] = Cards[i]->GetAttribute();
        cardPowerLevels[i] = Cards[i]->GetNumber();
    }

    // Delete existing cards
    for (Card* c : Cards) {
        delete c;
    }
    Cards.clear();
    
    int amtCardsToRestore = amtCards;
    amtCards = 0;
	
    // Restore cards using stored attributes
    for (int i = 0; i < amtCardsToRestore; ++i) {
        if (!AddCard(cardColors[i], cardAttributes[i], cardPowerLevels[i])) {
			cout << "card Adding Failure while Restoring deck!" << endl;
		}
    }
    
    cout << "Amount Cards in deck: " << amtCards << endl;

    // Reset counters
    timesSwapped = 0;
    timesUsedFlask = 0;
}

shared_ptr<Texture2D> &Deck::GetTexture() {
	return cardTexture;
}

int Deck::GetTotalPoints() {
	return totalPoints;
}

int Deck::GetRemainingPoints() {
	return remainingPoints;
}

void Deck::SetRemainingPoints(int amt) {
	if (amt > totalPoints) amt = totalPoints;
	if (amt < 0) amt = 0;
	remainingPoints = amt;
}
