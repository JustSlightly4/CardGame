/*
 * Eric Ryan Montgomery
 * 11/25/2025
 * For CardGameUI
 * Regular Game Class
 */

 #include "RegularGame.h"

//Resets the game
void RegularGame::Reset() {
	who = 0; //This is the index for the Card in play
	turn = 0;
	round = 0;
	playerInPlay = PLAYER1; //This is the Deck that is in play
	currCardRole = C_MAIN; //This is the role that the current Card is in
	player1Score = 0;
	player2Score = 0;
	dialog = "Game Start";
	amtActions = 0;
	gameStarted = false;
	gameEnded = false;
}

 //Logic for the ENTIRE GAME NOT AN INDIVIDUAL TURN
void RegularGame::PlayRegularGame(Deck &deck1, Deck &deck2, SingleButtonGroup &buttons) {
	
	//Correctly Initialize Starting Variables
	if (this->gameStarted == false) {
		this->gameStarted = true;
		this->round = 0;
		this->turn = 0;
		this->playerInPlay = PLAYER1;
		this->dialog = "Game Start";
		this->amtActions = deck1[this->round]->GetNumActions();
	}
	
	//Take Player Turn
	if (this->playerInPlay == PLAYER1) {
		PlayRegularGameTurn(deck1, deck2, buttons);
	} else if (this->playerInPlay == PLAYER2) {
		PlayRegularGameTurn(deck2, deck1, buttons);
	}
	
	//If anybody died move to next round
	if ((deck1[this->round]->GetHealth() <= 0 || deck2[this->round]->GetHealth() <= 0)) {
		
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
		this->currCardRole = C_MAIN;
		
		++this->round;
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
			buttons.SetFunctionality(false, 0, buttons.GetSize()-2);
			return;
		}
		
		//Reset Card variables for next round
		deck1.SetTimesSwapped(0);
		deck2.SetTimesSwapped(0);
		this->turn = 0;
		if (this->playerInPlay == 0) this->amtActions = deck1[this->round]->GetNumActions();
		else this->amtActions = deck2[this->round]->GetNumActions();
		this->firstTurnFrame = true;
	}
	
	//If used all actions move to next players turn
	if (this->amtActions <= 0) {
		
		//Increase turn
		++this->turn;
		
		switch(this->currCardRole) {
			case(C_SUPPORT): { //If support Card was in play and is done
				
				//Switch players and reset to main Card
				if (this->playerInPlay == PLAYER1) this->playerInPlay = PLAYER2;
				else this->playerInPlay = PLAYER1;
				this->currCardRole = C_MAIN;
				
				//Give the correct number of actions to whichever Card is playing
				if (this->playerInPlay == PLAYER1 ) this->amtActions = deck1[this->round]->GetNumActions();
				else this->amtActions = deck2[this->round]->GetNumActions();
				break;
			}
			case(C_MAIN): { //If main Card was in play and is done
				
				//Switch to support if not the last round
				if (this->round < deck1.size()-1) {
					this->currCardRole = C_SUPPORT;
				} else { //If it is the last round, switch players instead
					if (this->playerInPlay == PLAYER1) this->playerInPlay = PLAYER2;
					else this->playerInPlay = PLAYER1;
				}
				
				//Give the correct number of actions to whichever Card is playing
				if (this->playerInPlay == PLAYER1 ) this->amtActions = deck1[deck1.size()-1]->GetNumActions();
				else this->amtActions = deck2[deck2.size()-1]->GetNumActions();
				break;
			}
		}
		this->firstTurnFrame = true;
	}
}

//LOGIC FOR AN INDIVIDUAL TURN IN A GAME
void RegularGame::PlayRegularGameTurn(Deck &player, Deck &opponent, SingleButtonGroup &buttons) {
	
	//Decide if main or support Card is playing
	if (this->currCardRole == C_MAIN) this->who = this->round;
	else this->who = player.size()-1;
	
	//Set AIDecision to error to denote it hasnt make a decision yet
	//and don't make a decision on the first frame.
	//Once on the second frame, then wait a few seconds and make a decision
	Card::actions AIDecision = Card::ERROR;
	if (player.IsAI() == true && this->firstTurnFrame == false) {
		WaitTime(1.5);
		AIDecision = MakeAIDecisionDumb(player, opponent);
	}
	
	//Enable all buttons
	for (int i = 0; i < buttons.GetSize(); ++i) {
		buttons[i].SetFunctionality(true);
	}
	
	//Disable swap button if already used twice
	if (player.GetTimesSwapped() >= this->maxSwaps) {
		buttons[3].SetFunctionality(false);
	}
	
	//Disable charge button if already used twice
	if (player[this->who]->GetCharge() >= this->maxCharges) {
		buttons[4].SetFunctionality(false);
	}
	
	//Disable flask button if already used twice
	if (player.GetFlaskAmt() >= this->maxFlasks) {
		buttons[5].SetFunctionality(false);
	}
	
	//Disable attack button if support Card
	if (this->currCardRole == C_SUPPORT) {
		buttons[1].SetFunctionality(false);
	}
	
	//If Ai disable all buttons and wait on next frame
	if (player.IsAI() == true) {
		for (int i = 0; i < buttons.GetSize() - 1; ++i) {
			buttons[i].SetFunctionality(false);
		}
		//if (flags.firstTurnFrame == false) AIDecision = player.MakeAIDecision(opponent, this->round, this->turn);
	}
	
	//Physical Attack
	if (((buttons[1].GetAction() == true || IsKeyPressed(KEY_A)) && buttons[1].GetFunctionality() == true)
	|| AIDecision == Card::ATTACK) {
		this->dialog = PhysicalAttack(player[this->who], opponent[this->round], player[this->round]);
		--this->amtActions;
	}
	
	//Magical Attack
	if (((buttons[2].GetAction() == true || IsKeyPressed(KEY_P)) && buttons[2].GetFunctionality() == true)
	|| AIDecision == Card::CASTSPELL) {
		this->dialog = MagicalAttack(player[this->who], opponent[this->round], player[this->round]);
		--this->amtActions;
	}
	
	//Swap
	if (((buttons[3].GetAction() == true || IsKeyPressed(KEY_S)) && buttons[3].GetFunctionality() == true)
	|| AIDecision == Card::SWAP) {
		this->dialog = player.Swap(this->round, player.GetCardAmt()-1);
		this->amtActions = 0; //Set to zero rather than minus one because this should always end turn immediately
	}
	
	//Charge
	if (((buttons[4].GetAction() == true || IsKeyPressed(KEY_C)) && buttons[4].GetFunctionality() == true)
	|| AIDecision == Card::CHARGE) {
		this->dialog = player[this->who]->ChargeUp();
		--this->amtActions;
		if (player[this->who]->GetCharge() == 2) {
			this->dialog = ApplyAbility(player, opponent, this->who, this->round);
		}
	}
	
	//Use Flask *does not use players turn
	if ((buttons[5].GetAction() == true || IsKeyPressed(KEY_F)
	|| AIDecision == Card::FLASK) 
	&& buttons[5].GetFunctionality() == true) {
		this->dialog = player.UseFlask(this->who);
	}
	
	if (player.IsAI() == true && this->firstTurnFrame == true) {
		this->firstTurnFrame = false;
	}
}

//Applies a cards ability
string RegularGame::ApplyAbility(Deck &player, Deck &opponent, int pos, int mainPos) {
	ostringstream oss;
	if (player[pos]->GetAbilitiesActive() == false) {
		oss << player[pos]->GetName() << " could not used their ability!" << endl;
		return oss.str();
	}
	if (player[pos]->GetAbilityUsed() == true) {
		oss << player[pos]->GetName() << " can not use their ability again!" << endl;
		return oss.str();
	}
	player[pos]->SetAbilityUsed(true);
	
	//Needed Variables
	string amt = "E";
	int tempHealth;
	
	//Handle CHAOS and STRATEGICFIRE
	switch(player[pos]->GetAbility()) {
		case (Card::CHAOS):
			player[pos]->ChangeAbility(rand() % 13);
			break;
		case (Card::STRATEGICFIRE):
			if (mainPos == 0 && player.GetCardLimit() > 1) { //If first and there are more cards in the Deck, use Holy or Evil Presence
				if (rand() % 2) player[pos]->ChangeAbility(Card::HOLY_PRESENCE);
				else player[pos]->ChangeAbility(Card::EVIL_PRESENCE);
			} else if (mainPos == player.GetCardLimit()-1 && player.GetCardLimit() > 1) { //If last and there are more cards in the Deck, use Necromancy
				player[pos]->ChangeAbility(Card::NECROMANCY);
			} else if (opponent[mainPos]->GetAbilityUsed() == false) { //If the opponent hasn't used their ability nullify it
				player[pos]->ChangeAbility(Card::NULLIFY);
			} else {
				player[pos]->ChangeAbility(Card::RESISTANT);
			}
			break;
		default:
			//Do Nothing
			break;
	}
	
	//All Other Cases
	switch(player[pos]->GetAbility()) {
		case (Card::MIMICRY): {
			if (player[pos]->GetColor() == opponent[mainPos]->GetColor() || player[pos]->GetColor() == Card::C_WHITE || opponent[mainPos]->GetColor() == Card::C_WHITE) {
				oss << player[pos]->GetName() << " used pure mimicry!";
				(*player[pos]) = (*opponent[mainPos]);
			} else {
				Card newCard(opponent[mainPos]->GetColor(), opponent[mainPos]->GetAttribute(), opponent[mainPos]->GetNumber());
				oss << player[pos]->GetName() << " used discolored mimicry!";
				(*player[pos]) = newCard;
			}
			break;
		}
		case (Card::HEAVY_HANDED): {
			oss << player[pos]->GetName() << " used their ability Heavy Handed!" << endl;
			oss << player[pos]->AddPower(5);
			break;
		}
		case (Card::FAITHFUL): {
			oss << player[pos]->GetName() << " used their ability Faithful!" << endl;
			oss << player[pos]->DecUpperDice(3);
			break;
		}
		case (Card::FAST_HAND): {
			oss << player[pos]->GetName() << " used their ability Fast Hand!" << endl;
			oss << player[pos]->AddNumActions(1);
			break;
		}
		case (Card::HEALTHY_MIND): {
			oss << player[pos]->GetName() << " used their ability Healthy Mind!" << endl;
			oss << player[pos]->AddHealth(10);
			break;
		}
		case (Card::TERROR): {
			oss << player[pos]->GetName() << " used their ability Terror!" << endl;
			oss << player[pos]->GetName() << " terrified " << opponent[mainPos]->GetName();
			oss << "!" << endl;
			oss << opponent[mainPos]->DecPower(3);
			break;
		}
		case (Card::RESISTANT): {
			oss << player[pos]->GetName() << " used their ability Resistant!" << endl;
			oss << player[pos]->AddPhysicalResistance(0.25);
			break;
		}
		case (Card::HOLY_PRESENCE): {
			oss << player[pos]->GetName() << " used their ability Holy Presence!" << endl;
			if (mainPos < player.GetCardLimit() - 1) {
				oss << endl;
				oss << "A friends power was increased!";
				player[mainPos+1]->ChargeUp();
			}
			break;
		}
		case (Card::EVIL_PRESENCE): {
			oss << player[pos]->GetName() << " used their ability Evil Presence!" << endl;
			if ((mainPos < opponent.GetCardLimit()-1) && (opponent[mainPos+1]->GetCharge() == 0)) {
				oss << endl;
				oss << "An enemies power was drained!";
				opponent[mainPos+1]->ChargeDown();
			}
			break;
		}
		case (Card::ACCURATE): {
			oss << player[pos]->GetName() << " used their ability Accurate!" << endl;
			oss << player[pos]->DecLowerDice(1) << endl;
			oss << player[pos]->DecUpperDice(3);
			break;
		}
		case (Card::INNERGATE): {
			oss << player[pos]->GetName() << " used their ability Inner Gate!" << endl;
			player[pos]->AddLowerDice(99);
			player[pos]->DecUpperDice(99);
			break;
		}
		case (Card::NECROMANCY): {
			oss << player[pos]->GetName() << " used their ability Necromancy!" << endl;
			if (mainPos == 0) {
				oss << "Fallen souls strengthen " << player[pos]->GetName() << endl;
				player[pos]->ChargeUp();
			} else {
				//Finds the Card with the most power in the Deck before current Card
				int maxPowerIndex = 0;
				int maxPower = 0;
				for (int j = 0; j < mainPos-1; ++j) {
					if (player[j]->GetPower() > maxPower) {
						maxPowerIndex = j;
						maxPower = player[j]->GetPower();
					}
				}
				(*player[pos]) = (*player[maxPowerIndex]);
				
				player[pos]->FullHeal(); //Heals the Card back to full health
				
				//Changes ability to prevent nullify from nullifing
				//abilities that may have been used in previous rounds
				player[pos]->ChangeAbility(Card::NECROMANCY);
				player[pos]->SetAbilitiesActive(false); //Prevents repeated use of necromancy
			}
			break;
		}
		case (Card::MAGIC_IMMUNITY): {
			oss << player[pos]->GetName() << " used their ability Magic Immunity and is now entirely immune to magic!" << endl;
			player[pos]->AddMagicalResistance(player[pos]->GetMagicalResistance());
			break;
		}
		case (Card::DIVINEHELP): {
			oss << player[pos]->GetName() << " used their ability Divine Help!" << endl;
			opponent[mainPos]->DecHealth(15);
			oss << opponent[mainPos]->GetName() << "'s very soul was struck for 15 points!";
			break;
		}
		case (Card::NULLIFY): {
			oss << player[pos]->GetName() << " used their ability Nullify!" << endl;
			if (opponent[mainPos]->GetAbilityUsed() == true && opponent[mainPos]->GetAbilitiesActive() == true) {
				oss << NullifyOpponentsAbility(*player[pos], *opponent[mainPos]);
				opponent[mainPos]->SetAbilitiesActive(false);
			} else if (opponent[mainPos]->GetAbilityUsed() == false && opponent[mainPos]->GetAbilitiesActive() == true) {
				oss << player[pos]->GetName() << " prevented " << opponent[mainPos]->GetName() <<
					" from ever using their ability!";
				opponent[mainPos]->SetAbilitiesActive(false);
			} else {
				oss << player[pos]->GetName() << " could not nullify " << opponent[mainPos]->GetName() << "'s ability2!" << endl;
			}
			break;
		}
		case (Card::MADE_IN_HEAVEN): {
			tempHealth = opponent[mainPos]->GetHealth();
			Card newCard(opponent[mainPos]->GetColor(), opponent[mainPos]->GetAttribute(), opponent[mainPos]->GetNumber());
				oss << player[pos]->GetName() << " used Made In Heaven and reset their opponent!";
				(*opponent[mainPos]) = newCard;
			if (opponent[mainPos]->GetHealth() > tempHealth) {
				opponent[mainPos]->DecHealth(opponent[mainPos]->GetHealth() - tempHealth);
			}
			break;
		}
		default: {
			oss << player[pos]->GetName() << "'s ability is currently broken :(" << endl;
			break;
		}
	}
	return oss.str();
}

//Tries to undo whatever the opponents ability did
string RegularGame::NullifyOpponentsAbility(Card &player, Card &opponent) {
	ostringstream oss;
	switch(opponent.GetAbility()) {
		case (Card::HEAVY_HANDED):
			oss << opponent.DecPower(3);
			break;
		case (Card::FAITHFUL):
			oss << opponent.AddUpperDice(3);
			break;
		case (Card::FAST_HAND):
			oss << opponent.DecNumActions(1);
			break;
		case (Card::HEALTHY_MIND):
			oss << opponent.DecHealth(10);
			break;
		case (Card::RESISTANT):
			oss << opponent.DecPhysicalResistance(0.25);
			break;
		case (Card::ACCURATE):
			oss << opponent.AddLowerDice(1) << endl;
			oss << opponent.AddUpperDice(3);
			break;
		case (Card::DIVINEHELP):
			player.AddHealth(15);
			oss << "The gods have given " << player.GetName() << " their health back!" << endl;
			break;
		default:
			oss << player.GetName() << " could not nullify " << opponent.GetName() << "'s ability!" << endl;
			break;
	}
	return oss.str();
}

//Physically Attack an Opponent
string RegularGame::PhysicalAttack(Card *player, Card *opponent, Card *mainCard) {
	ostringstream oss;
	Card::attackActions action = player->RollDiceEnum();
	switch (action) {
		case (Card::HIT):
			oss << player->GetName() << " hit " << opponent->GetName()
				<< " for " << int(player->GetPower() * opponent->GetPhysicalResistance()) << " points!";
			opponent->DecHealth(player->GetPower() * opponent->GetPhysicalResistance());
			break;
		case (Card::CRITHIT):
			oss << player->GetName() << " critically hit " << opponent->GetName()
				<< " for " << int(player->GetPower() * 1.2 * opponent->GetPhysicalResistance()) << " points!";
			opponent->DecHealth(player->GetPower() * 1.2 * opponent->GetPhysicalResistance());
			break;
		default:
			oss << player->GetName() << " missed!";
			break;
	};
	return oss.str();
}

//Magically Attack an Opponent
string RegularGame::MagicalAttack(Card *player, Card *opponent, Card *mainCard) {
	ostringstream oss;
	
	if (player->GetMagicalPower() <= 0) {
		oss << player->GetName() << " has no magical power!" << endl;
		return oss.str();
	}
	
	//Calculate base damage
	int amt = (rand() % player->GetMagicalPower()) + 1;
	
	//Apply damage and crits
	switch(player->GetSpell()) {
		case (Card::FORCE): { //Does physical damage
			//If the opponents color is the same as the Card advantage then critical hit
			if (Card::advantage.at(player->GetColor()) == opponent->GetColor()) {
				amt *= 1.2;
				oss << player->GetName() << " critically cast Force on " << opponent->GetName() << " for " << amt << " points!";
				opponent->DecHealth(amt * opponent->GetMagicalResistance());
					
			//Otherwise, hit on the range of [magicalPower/2 + 1, magicalPower]
			} else {
				oss << player->GetName() << " cast Force on " << opponent->GetName() << " for " << int(amt * opponent->GetMagicalResistance()) << " points!";
				opponent->DecHealth(amt * opponent->GetMagicalResistance());
			}
			break;
		}
		case (Card::DRAIN): { //Does power damage
			if (amt > 1) amt = amt * 0.5; //Drain Spell nerf but can't miss
			
			//If the opponents color is the same as the Card advantage then critical hit
			if (Card::advantage.at(player->GetColor()) == opponent->GetColor()) {
				amt *= 1.2;
				oss << player->GetName() << " critically cast Drain on " << opponent->GetName() << " for " << amt << " points!";
				opponent->DecPower(amt * opponent->GetMagicalResistance());
					
			//Otherwise, hit on the range of [magicalPower/2 + 1, magicalPower]
			} else {
				oss << player->GetName() << " cast Drain on " << opponent->GetName() << " for " << int(amt * opponent->GetMagicalResistance()) << " points!";
				opponent->DecPower(amt * opponent->GetMagicalResistance());
			}
			break;
		}
		case(Card::WEAKEN): {
			//If the opponents color is the same as the Card advantage then critical hit
			if (Card::advantage.at(player->GetColor()) == opponent->GetColor()) {
				double amt = 0.2;
				oss << player->GetName() << " critically cast Weaken on " << opponent->GetName() << "!" << endl;
				oss << opponent->DecPhysicalResistance(amt * opponent->GetMagicalResistance());
					
			//Otherwise, hit on the range of [magicalPower/2 + 1, magicalPower]
			} else {
				double amt = 0.1;
				oss << player->GetName() << " cast Weaken on " << opponent->GetName() << "!" << endl;
				oss << opponent->DecPhysicalResistance(amt * opponent->GetMagicalResistance());
			}
			break;
		}
		case(Card::HEAL): {
			//If mains color is the same as the Card advantage then Greater Heal
			if (Card::advantage.at(player->GetColor()) == mainCard->GetColor()) {
				oss << player->GetName() << " cast Greater Heal for " << int (amt * 1.2) << " points!";
				mainCard->Heal(amt * 1.2);
					
			//Otherwise, regular heal
			} else {
				oss << player->GetName() << " cast Heal for " << amt << " points!";
				mainCard->Heal(amt);
			}
			break;
		}
	}
	return oss.str();
}

Card::actions RegularGame::MakeAIDecisionDumb(Deck &player, Deck &opponent) {
	
	//Make a list of the actions that are allowed
	vector<Card::actions> allowableActions = {Card::CASTSPELL};
	
	//Go through checks to add which actions are allowed
	if (player[this->who]->GetCharge() < this->maxCharges) allowableActions.push_back(Card::CHARGE);
	if (player.GetTimesSwapped() < this->maxSwaps) allowableActions.push_back(Card::SWAP);
	if (player.GetFlaskAmt() < this->maxFlasks) allowableActions.push_back(Card::FLASK);
	if (this->currCardRole == C_MAIN) allowableActions.push_back(Card::ATTACK);
	
	//Push a completely random chosen action to the queue
	player[this->who]->PushAction(allowableActions[rand() % allowableActions.size()]);
	
	//Remove action from queue and return it
	return player[this->who]->PopAction();
}