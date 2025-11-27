/*
 * Eric Ryan Montgomery
 * 11/25/2025
 * For CardGameUI
 * Regular Game Class
 */

 #include "RegularGame.h"
 
 //Constructor
 RegularGame::RegularGame(Deck &player1, Deck &player2, int currNumCards, int currDeckStrength, bool isDeck1AI, bool isDeck2AI) : deck1(player1), deck2(player2) {
	deck1.ShuffleDeck();
	deck2.ShuffleDeck();
	player = &deck1;
	opponent = &deck2;
	cardInPlay = 0; //This is the index for the Card in play
	turn = 0;
	round = 0;
	playerInPlay = PLAYER1; //This is the Deck that is in play
	currCardRole = C_MAIN; //This is the role that the current Card is in
	player1Score = 0;
	player2Score = 0;
	dialog = "Game Start";
	amtActions = player->at(this->round)->GetNumActions();
	gameStarted = false;
	gameEnded = false;
	numCards = currNumCards;
	deckStrength = currDeckStrength;
	deck1AI = isDeck1AI;
	deck2AI = isDeck2AI;
	firstTurnFrame = true;
	this->gameStarted = true;
 }

 //Logic for the ENTIRE GAME NOT AN INDIVIDUAL TURN
void RegularGame::PlayRegularGame(Card::actions nextDecision) {

	//Guard if game has ended
	if (gameEnded == true) return;
	
	//Check to see if a valid decision has been made
	if (nextDecision != Card::ERROR) {

		//Perform next decision through a dispatch table
		dialog = performAction[nextDecision]();
	
		//If anybody died move to next round
		if(CheckIfRoundOver()) AdvanceRound();
		
		//If used all actions move to next players turn
		if (this->amtActions <= 0) AdvanceTurn();

	}
}

int RegularGame::GetCardInPlay() {return cardInPlay;};
int RegularGame::GetTurn() {return turn;};
int RegularGame::GetRound() {return round;};
RegularGame::players RegularGame::GetPlayerInPlay() {return playerInPlay;};
RegularGame::cardRoles RegularGame::GetCurrCardRole() {return currCardRole;};
int RegularGame::GetPlayer1Score() {return player1Score;};
int RegularGame::GetPlayer2Score() {return player2Score;};
string RegularGame::GetDialog() {return dialog;};
int RegularGame::GetAmtActions() {return amtActions;};
bool RegularGame::GetGameStarted() {return gameStarted;};
bool RegularGame::GetGameEnded() {return gameEnded;};
int RegularGame::GetNumCards() {return numCards;};
int RegularGame::GetDeckStrength() {return deckStrength;};
bool RegularGame::GetDeck1AI() {return deck1AI;};
bool RegularGame::GetDeck2AI() {return deck2AI;};
bool RegularGame::GetFirstTurnFrame() {return firstTurnFrame;};
int RegularGame::GetMaxSwaps() {return maxSwaps;};
int RegularGame::GetMaxCharges() {return maxCharges;};
int RegularGame::GetMaxFlask() {return maxFlasks;};
Deck &RegularGame::GetDeck1() {return deck1;};
Deck &RegularGame::GetDeck2() {return deck2;};
Deck &RegularGame::GetPlayer() {return *player;};
Deck &RegularGame::GetOpponent() {return *opponent;};

//Applies a cards ability
string RegularGame::ApplyAbility() {
	ostringstream oss;
	if (player->at(this->cardInPlay)->GetAbilitiesActive() == false) {
		oss << player->at(this->cardInPlay)->GetName() << " could not used their ability!\n";
		return oss.str();
	}
	if (player->at(this->cardInPlay)->GetAbilityUsed() == true) {
		oss << player->at(this->cardInPlay)->GetName() << " can not use their ability again!\n";
		return oss.str();
	}
	player->at(this->cardInPlay)->SetAbilityUsed(true);
	
	//Needed Variables
	string amt = "E";
	int tempHealth;
	
	//Handle CHAOS and STRATEGICFIRE
	switch(player->at(this->cardInPlay)->GetAbility()) {
		case (Card::CHAOS):
			player->at(this->cardInPlay)->ChangeAbility(rand() % 13);
			break;
		case (Card::STRATEGICFIRE):
			if (this->round == 0 && player->GetCardLimit() > 1) { //If first and there are more cards in the Deck, use Holy or Evil Presence
				if (rand() % 2) player->at(this->cardInPlay)->ChangeAbility(Card::HOLY_PRESENCE);
				else player->at(this->cardInPlay)->ChangeAbility(Card::EVIL_PRESENCE);
			} else if (this->round == player->GetCardLimit()-1 && player->GetCardLimit() > 1) { //If last and there are more cards in the Deck, use Necromancy
				player->at(this->cardInPlay)->ChangeAbility(Card::NECROMANCY);
			} else if (opponent->at(this->round)->GetAbilityUsed() == false) { //If the opponent hasn't used their ability nullify it
				player->at(this->cardInPlay)->ChangeAbility(Card::NULLIFY);
			} else {
				player->at(this->cardInPlay)->ChangeAbility(Card::RESISTANT);
			}
			break;
		default:
			//Do Nothing
			break;
	}
	
	//All Other Cases
	switch(player->at(this->cardInPlay)->GetAbility()) {
		case (Card::MIMICRY): {
			if (player->at(this->cardInPlay)->GetColor() == opponent->at(this->round)->GetColor() || player->at(this->cardInPlay)->GetColor() == Card::C_WHITE || opponent->at(this->round)->GetColor() == Card::C_WHITE) {
				oss << player->at(this->cardInPlay)->GetName() << " used pure mimicry!\n";
				(*player->at(this->cardInPlay)) = (*opponent->at(this->round));
			} else {
				Card newCard(opponent->at(this->round)->GetColor(), opponent->at(this->round)->GetAttribute(), opponent->at(this->round)->GetNumber());
				oss << player->at(this->cardInPlay)->GetName() << " used discolored mimicry!\n";
				(*player->at(this->cardInPlay)) = newCard;
			}
			break;
		}
		case (Card::HEAVY_HANDED): {
			oss << player->at(this->cardInPlay)->GetName() << " used their ability Heavy Handed!\n";
			oss << player->at(this->cardInPlay)->AddPower(5);
			break;
		}
		case (Card::FAITHFUL): {
			oss << player->at(this->cardInPlay)->GetName() << " used their ability Faithful!\n";
			oss << player->at(this->cardInPlay)->DecUpperDice(3);
			break;
		}
		case (Card::FAST_HAND): {
			oss << player->at(this->cardInPlay)->GetName() << " used their ability Fast Hand!\n";
			oss << player->at(this->cardInPlay)->AddNumActions(1);
			break;
		}
		case (Card::HEALTHY_MIND): {
			oss << player->at(this->cardInPlay)->GetName() << " used their ability Healthy Mind!\n";
			oss << player->at(this->cardInPlay)->AddHealth(10);
			break;
		}
		case (Card::TERROR): {
			oss << player->at(this->cardInPlay)->GetName() << " used their ability Terror!\n";
			oss << player->at(this->cardInPlay)->GetName() << " terrified " << opponent->at(this->round)->GetName();
			oss << "!\n";
			oss << opponent->at(this->round)->DecPower(3);
			break;
		}
		case (Card::RESISTANT): {
			oss << player->at(this->cardInPlay)->GetName() << " used their ability Resistant!";
			oss << player->at(this->cardInPlay)->AddPhysicalResistance(0.25);
			break;
		}
		case (Card::HOLY_PRESENCE): {
			oss << player->at(this->cardInPlay)->GetName() << " used their ability Holy Presence!\n";
			if (this->round < player->GetCardLimit() - 1) {
				oss << "A friends power was increased!\n";
				player->at(this->round+1)->ChargeUp();
			}
			break;
		}
		case (Card::EVIL_PRESENCE): {
			oss << player->at(this->cardInPlay)->GetName() << " used their ability Evil Presence!\n";
			if ((this->round < opponent->GetCardLimit()-1) && (opponent->at(this->round+1)->GetCharge() == 0)) {
				oss << "An enemies power was drained!\n";
				opponent->at(this->round+1)->ChargeDown();
			}
			break;
		}
		case (Card::ACCURATE): {
			oss << player->at(this->cardInPlay)->GetName() << " used their ability Accurate!\n";
			oss << player->at(this->cardInPlay)->DecLowerDice(1);
			oss << player->at(this->cardInPlay)->DecUpperDice(3);
			break;
		}
		case (Card::INNERGATE): {
			oss << player->at(this->cardInPlay)->GetName() << " used their ability Inner Gate!\n";
			player->at(this->cardInPlay)->AddLowerDice(99);
			player->at(this->cardInPlay)->DecUpperDice(99);
			break;
		}
		case (Card::NECROMANCY): {
			oss << player->at(this->cardInPlay)->GetName() << " used their ability Necromancy!\n";
			if (this->round == 0) {
				oss << "Fallen souls strengthen " << player->at(this->cardInPlay)->GetName() << "!\n";
				player->at(this->cardInPlay)->ChargeUp();
			} else {
				//Finds the Card with the most power in the Deck before current Card
				int maxPowerIndex = 0;
				int maxPower = 0;
				for (int j = 0; j < this->round-1; ++j) {
					if (player->at(j)->GetPower() > maxPower) {
						maxPowerIndex = j;
						maxPower = player->at(j)->GetPower();
					}
				}
				(*player->at(this->cardInPlay)) = (*player->at(maxPowerIndex));
				
				player->at(this->cardInPlay)->FullHeal(); //Heals the Card back to full health
				
				//Changes ability to prevent nullify from nullifing
				//abilities that may have been used in previous rounds
				player->at(this->cardInPlay)->ChangeAbility(Card::NECROMANCY);
				player->at(this->cardInPlay)->SetAbilitiesActive(false); //Prevents repeated use of necromancy
			}
			break;
		}
		case (Card::MAGIC_IMMUNITY): {
			oss << player->at(this->cardInPlay)->GetName() << " used their ability Magic Immunity and is now entirely immune to magic!\n";
			player->at(this->cardInPlay)->AddMagicalResistance(player->at(this->cardInPlay)->GetMagicalResistance());
			break;
		}
		case (Card::DIVINEHELP): {
			oss << player->at(this->cardInPlay)->GetName() << " used their ability Divine Help!\n";
			opponent->at(this->round)->DecHealth(15);
			oss << opponent->at(this->round)->GetName() << "'s very soul was struck for 15 points!\n";
			break;
		}
		case (Card::NULLIFY): {
			oss << player->at(this->cardInPlay)->GetName() << " used their ability Nullify!\n";
			if (opponent->at(this->round)->GetAbilityUsed() == true && opponent->at(this->round)->GetAbilitiesActive() == true) {
				oss << NullifyOpponentsAbility();
				opponent->at(this->round)->SetAbilitiesActive(false);
			} else if (opponent->at(this->round)->GetAbilityUsed() == false && opponent->at(this->round)->GetAbilitiesActive() == true) {
				oss << player->at(this->cardInPlay)->GetName() << " prevented " << opponent->at(this->round)->GetName() <<
					" from ever using their ability!\n";
				opponent->at(this->round)->SetAbilitiesActive(false);
			} else {
				oss << player->at(this->cardInPlay)->GetName() << " could not nullify " << opponent->at(this->round)->GetName() << "'s ability!\n";
			}
			break;
		}
		case (Card::MADE_IN_HEAVEN): {
			tempHealth = opponent->at(this->round)->GetHealth();
			Card newCard(opponent->at(this->round)->GetColor(), opponent->at(this->round)->GetAttribute(), opponent->at(this->round)->GetNumber());
				oss << player->at(this->cardInPlay)->GetName() << " used Made In Heaven and reset their opponent!\n";
				(*opponent->at(this->round)) = newCard;
			if (opponent->at(this->round)->GetHealth() > tempHealth) {
				opponent->at(this->round)->DecHealth(opponent->at(this->round)->GetHealth() - tempHealth);
			}
			break;
		}
		default: {
			oss << player->at(this->cardInPlay)->GetName() << "'s ability is currently broken :(\n";
			break;
		}
	}
	return oss.str();
}

//Tries to undo whatever the opponents ability did
string RegularGame::NullifyOpponentsAbility() {
	ostringstream oss;
	switch(opponent->at(this->round)->GetAbility()) {
		case (Card::HEAVY_HANDED):
			oss << opponent->at(this->round)->DecPower(3);
			break;
		case (Card::FAITHFUL):
			oss << opponent->at(this->round)->AddUpperDice(3);
			break;
		case (Card::FAST_HAND):
			oss << opponent->at(this->round)->DecNumActions(1);
			break;
		case (Card::HEALTHY_MIND):
			oss << opponent->at(this->round)->DecHealth(10);
			break;
		case (Card::RESISTANT):
			oss << opponent->at(this->round)->DecPhysicalResistance(0.25);
			break;
		case (Card::ACCURATE):
			oss << opponent->at(this->round)->AddLowerDice(1);
			oss << opponent->at(this->round)->AddUpperDice(3);
			break;
		case (Card::DIVINEHELP):
			player->at(this->cardInPlay)->AddHealth(15);
			oss << "The gods have given " << player->at(this->cardInPlay)->GetName() << " their health back!\n";
			break;
		default:
			oss << player->at(this->cardInPlay)->GetName() << " could not nullify " << opponent->at(this->round)->GetName() << "'s ability!\n";
			break;
	}
	return oss.str();
}

//Physically Attack an Opponent
string RegularGame::PhysicalAttack() {
	--this->amtActions;
	ostringstream oss;
	Card::attackActions action = player->at(this->cardInPlay)->RollDiceEnum();
	switch (action) {
		case (Card::HIT):
			oss << player->at(this->cardInPlay)->GetName() << " hit " << opponent->at(this->round)->GetName()
				<< " for " << int(player->at(this->cardInPlay)->GetPower() * opponent->at(this->round)->GetPhysicalResistance()) << " points!\n";
			opponent->at(this->round)->DecHealth(player->at(cardInPlay)->GetPower() * opponent->at(this->round)->GetPhysicalResistance());
			break;
		case (Card::CRITHIT):
			oss << player->at(this->cardInPlay)->GetName() << " critically hit " << opponent->at(this->round)->GetName()
				<< " for " << int(player->at(this->cardInPlay)->GetPower() * 1.2 * opponent->at(this->round)->GetPhysicalResistance()) << " points!\n";
			opponent->at(this->round)->DecHealth(player->at(this->cardInPlay)->GetPower() * 1.2 * opponent->at(this->round)->GetPhysicalResistance());
			break;
		default:
			oss << player->at(this->cardInPlay)->GetName() << " missed!\n";
			break;
	};
	return oss.str();
}

//Magically Attack an Opponent
string RegularGame::MagicalAttack() {
	--this->amtActions;
	ostringstream oss;
	
	if (player->at(this->cardInPlay)->GetMagicalPower() <= 0) {
		oss << player->at(this->cardInPlay)->GetName() << " has no magical power!\n";
		return oss.str();
	}
	
	//Calculate base damage
	int amt = (rand() % player->at(this->cardInPlay)->GetMagicalPower()) + 1;
	
	//Apply damage and crits
	switch(player->at(this->cardInPlay)->GetSpell()) {
		case (Card::FORCE): { //Does physical damage
			//If the opponents color is the same as the Card advantage then critical hit
			if (Card::advantage.at(player->at(this->cardInPlay)->GetColor()) == opponent->at(this->round)->GetColor()) {
				amt *= 1.2;
				oss << player->at(this->cardInPlay)->GetName() << " critically cast Force on " << opponent->at(this->round)->GetName() << " for " << amt << " points!\n";
				opponent->at(this->round)->DecHealth(amt * opponent->at(this->round)->GetMagicalResistance());
					
			//Otherwise, hit on the range of [magicalPower/2 + 1, magicalPower]
			} else {
				oss << player->at(this->cardInPlay)->GetName() << " cast Force on " << opponent->at(this->round)->GetName() << " for " << int(amt * opponent->at(this->round)->GetMagicalResistance()) << " points!\n";
				opponent->at(this->round)->DecHealth(amt * opponent->at(this->round)->GetMagicalResistance());
			}
			break;
		}
		case (Card::DRAIN): { //Does power damage
			if (amt > 1) amt = amt * 0.5; //Drain Spell nerf but can't miss
			
			//If the opponents color is the same as the Card advantage then critical hit
			if (Card::advantage.at(player->at(this->cardInPlay)->GetColor()) == opponent->at(this->round)->GetColor()) {
				amt *= 1.2;
				oss << player->at(this->cardInPlay)->GetName() << " critically cast Drain on " << opponent->at(this->round)->GetName() << " for " << amt << " points!\n";
				opponent->at(this->round)->DecPower(amt * opponent->at(this->round)->GetMagicalResistance());
					
			//Otherwise, hit on the range of [magicalPower/2 + 1, magicalPower]
			} else {
				oss << player->at(this->cardInPlay)->GetName() << " cast Drain on " << opponent->at(this->round)->GetName() << " for " << int(amt * opponent->at(this->round)->GetMagicalResistance()) << " points!\n";
				opponent->at(this->round)->DecPower(amt * opponent->at(this->round)->GetMagicalResistance());
			}
			break;
		}
		case(Card::WEAKEN): {
			//If the opponents color is the same as the Card advantage then critical hit
			if (Card::advantage.at(player->at(this->cardInPlay)->GetColor()) == opponent->at(this->round)->GetColor()) {
				double amt = 0.2;
				oss << player->at(this->cardInPlay)->GetName() << " critically cast Weaken on " << opponent->at(this->round)->GetName() << "!\n";
				oss << opponent->at(this->round)->DecPhysicalResistance(amt * opponent->at(this->round)->GetMagicalResistance());
					
			//Otherwise, hit on the range of [magicalPower/2 + 1, magicalPower]
			} else {
				double amt = 0.1;
				oss << player->at(this->cardInPlay)->GetName() << " cast Weaken on " << opponent->at(this->round)->GetName() << "!\n";
				oss << opponent->at(this->round)->DecPhysicalResistance(amt * opponent->at(this->round)->GetMagicalResistance());
			}
			break;
		}
		case(Card::HEAL): {
			//If mains color is the same as the Card advantage then Greater Heal
			if (Card::advantage.at(player->at(this->cardInPlay)->GetColor()) == player->at(this->round)->GetColor()) {
				oss << player->at(this->cardInPlay)->GetName() << " cast Greater Heal for " << int (amt * 1.2) << " points!\n";
				player->at(this->round)->Heal(amt * 1.2);
					
			//Otherwise, regular heal
			} else {
				oss << player->at(this->cardInPlay)->GetName() << " cast Heal for " << amt << " points!\n";
				player->at(this->round)->Heal(amt);
			}
			break;
		}
	}
	return oss.str();
}

string RegularGame::Swap() {
	this->amtActions = 0; //Set to zero rather than minus one because this should always end turn immediately
	ostringstream oss;
	oss << player->Swap(this->round, player->GetCardAmt()-1);
	return oss.str();
}

string RegularGame::Charge() {
	--this->amtActions;
	ostringstream oss;
	oss << player->at(this->cardInPlay)->ChargeUp();
	if (player->at(this->cardInPlay)->GetCharge() == 2) {
		oss << ApplyAbility();
	}
	return oss.str();
}

string RegularGame::UseFlask() {
	ostringstream oss;
	oss << player->UseFlask(this->cardInPlay);
	return oss.str();
}

bool RegularGame::CheckIfRoundOver() {
	return (deck1[this->round]->GetHealth() <= 0 || deck2[this->round]->GetHealth() <= 0);
}

void RegularGame::AdvanceRound() {
	//Award Winning player points
	if (deck1[this->round]->GetHealth() <= 0 && deck2[this->round]->GetHealth() <= 0) {
		++this->player1Score;
		++this->player2Score;
		this->playerInPlay = PLAYER1;
		this->dialog += " Ending the round in a tie!";
	} else if (deck1[this->round]->GetHealth() <= 0) {
		++this->player2Score;
		this->playerInPlay = PLAYER1;
		this->dialog += " Winning Player 2 the round!";
	} else if (deck2[this->round]->GetHealth() <= 0) {
		++this->player1Score;
		this->playerInPlay = PLAYER2;
		this->dialog += " Winning Player 1 the round!";
	}

	//Advance to next round
	++this->round;

	//Set who is the player and opponent
	player  = &(playerInPlay == PLAYER1 ? deck1 : deck2);
	opponent = &(playerInPlay == PLAYER1 ? deck2 : deck1);
	this->currCardRole = C_MAIN; //Return to main card upon round end
	
	if (this->round >= this->numCards) { //if no next round, end game
		this->gameEnded = true;
		this->round = this->numCards - 1;
		if (this->player1Score > this->player2Score) {
			this->dialog = "Game Over. Player 1 Won!";
		} else if (this->player1Score < this->player2Score) {
			this->dialog = "Game Over. Player 2 Won!";
		} else {
			this->dialog = "Game Over. It was a tie!";
		}
		return;
	}

	//Set card in play index to the round number
	this->cardInPlay = this->round;
	
	//Reset Card variables for next round
	deck1.SetTimesSwapped(0);
	deck2.SetTimesSwapped(0);
	this->turn = 0;
	this->amtActions = player->at(this->round)->GetNumActions();
	this->firstTurnFrame = true;
}

void RegularGame::AdvanceTurn() {
	//Increase turn
	++this->turn;
	
	switch(this->currCardRole) {
		case(C_SUPPORT): { //If support Card was in play and is done
			
			//Switch players and reset to main Card
			if (this->playerInPlay == PLAYER1) this->playerInPlay = PLAYER2;
			else this->playerInPlay = PLAYER1;
			this->currCardRole = C_MAIN;

			//Swap the player and the opponent
			player  = &(playerInPlay == PLAYER1 ? deck1 : deck2);
			opponent = &(playerInPlay == PLAYER1 ? deck2 : deck1);

			//Set Number of actions and the card index in play
			this->amtActions = player->at(this->round)->GetNumActions();
			this->cardInPlay = this->round;

			break;
		}
		case(C_MAIN): { //If main Card was in play and is done
			
			//Switch to support if not the last round
			if (this->round < deck1.size()-1) {
				this->currCardRole = C_SUPPORT;
			} else { //If it is the last round, switch players instead
				if (this->playerInPlay == PLAYER1) this->playerInPlay = PLAYER2;
				else this->playerInPlay = PLAYER1;

				//Swap the player and the opponent
				player  = &(playerInPlay == PLAYER1 ? deck1 : deck2);
				opponent = &(playerInPlay == PLAYER1 ? deck2 : deck1);
			}
			
			//Set Number of actions and the card index in play
			this->amtActions = player->at(player->size()-1)->GetNumActions();
			this->cardInPlay = player->size()-1;
			break;
		}
	}
	this->firstTurnFrame = true;
}

Card::actions RegularGame::MakeAIDecisionDumb() {
	
	//Make a list of the actions that are allowed
	vector<Card::actions> allowableActions = {Card::CASTSPELL};
	
	//Go through checks to add which actions are allowed
	if (player->at(this->cardInPlay)->GetCharge() < this->maxCharges) allowableActions.push_back(Card::CHARGE);
	if (player->GetTimesSwapped() < this->maxSwaps) allowableActions.push_back(Card::SWAP);
	if (player->GetFlaskAmt() < this->maxFlasks) allowableActions.push_back(Card::FLASK);
	if (this->currCardRole == C_MAIN) allowableActions.push_back(Card::ATTACK);
	
	//Push a completely random chosen action to the queue
	player->at(this->cardInPlay)->PushAction(allowableActions[rand() % allowableActions.size()]);
	
	//Remove action from queue and return it
	return player->at(this->cardInPlay)->PopAction();
}