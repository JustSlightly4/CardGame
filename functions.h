/*
 * Eric Ryan Montgomery
 * 09/21/2025
 * For CardGameUI
 * Logic Functions not related to drawing are written here
 */
 
#ifndef FUNCTIONSH
#define FUNCTIONSH
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <functional>
#include <iomanip>
#include "raylib.h"
#include "deckofcards.h"
#include "cards.h"
#include "buttons.h"
#include "globals.h"
using namespace std;

void RegularGame(Deck &deck1, Deck &deck2, GameVars &gameVars, Flags &flags, SingleButtonGroup &buttons);
void RegularGameTurn(Deck &player, Deck &opponent, GameVars &gameVars, Flags &flags, SingleButtonGroup &buttons);
void scrollLogic(float &scrollOffset);
float GetCardSourceX(Card *card);
Rectangle GetCardSourceRec(Card *card);
Flags ResetFlags();
GameVars ResetGameVars();
CardEditVars ResetCardEditVars();
string ApplyAbility(Deck &player, Deck &opponent, int pos, int mainPos);
string NullifyOpponentsAbility(Card &player, Card &opponent);
string PhysicalAttack(Card *player, Card *opponent, Card *mainCard);
string MagicalAttack(Card *player, Card *opponent, Card *mainCard);
actions MakeAIDecisionDumb(Deck &player, Deck &opponent, GameVars &gameVars);

#endif
