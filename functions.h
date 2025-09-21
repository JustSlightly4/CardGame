/*
 * Eric Ryan Montgomery
 * 03/17/2025
 * For CardGameUI
 * Function/Classes that are not part of the cards/deck are written here
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
#include "buttons.h"
#include "globals.h"
using namespace std;

void RegularGame(deck &deck1, deck &deck2, GameVars &gameVars, Flags &flags, SingleButtonGroup &buttons, Data &StyleGuide);
void RegularGameTurn(deck &player, deck &opponent, GameVars &gameVars, Flags &flags, SingleButtonGroup &buttons, Data &StyleGuide);
void scrollLogic(float &scrollOffset, Data &StyleGuide);
void UpdateStyleGuide(Data &StyleGuide);
float GetCardSourceX(card *Card, Data &StyleGuide);
Flags ResetFlags();
GameVars ResetGameVars();
CardEditVars ResetCardEditVars();
string ApplyAbility(deck &player, deck &opponent, int pos, int mainPos);
string NullifyOpponentsAbility(card &player, card &opponent);
string PhysicalAttack(card *player, card *opponent, card *mainCard);
string MagicalAttack(card *player, card *opponent, card *mainCard);
actions MakeAIDecisionDumb(deck &player, deck &opponent, GameVars &gameVars);

#endif
