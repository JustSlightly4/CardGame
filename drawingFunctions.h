#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <functional>
#include <iomanip>
#include "functions.h"
#include "raylib.h"
#include "deckofcards.h"
#include "buttons.h"
#include "globals.h"
#ifndef DRAWINGFUNCTIONSH
#define DRAWINGFUNCTIONSH

void DrawBasicCardStats(int index, deck *Deck, Vector2 pos, float size, Data &StyleGuide);
void DrawTextS(string text, Rectangle dest, Color tint, float fontSize, Alignment orientation);
float DrawTextSWrapped(string text, Rectangle dest, Color tint, float fontSize, Alignment orientation);
void DrawButtonLine(SingleButtonGroup &buttons, Data &StyleGuide);
void DrawSettingsButtons(PlusMinusButtonGroup &buttons, Data &StyleGuide);
void DrawRules(float &scrollOffset, Data &StyleGuide);
void DrawSkills(float &scrollOffset, Data &StyleGuide);
void DrawStringVector(const vector<string> &sentences, const float &scrollOffset, Data &StyleGuide);
void DrawCardLine(deck *Deck, SingleButtonGroup &buttons, Vector2 startPos, int spacing, Data &StyleGuide);
void DrawCard(int index, deck *Deck, Vector2 pos, float size, Data &StyleGuide);
void DrawCardWithStats(int index, deck *Deck, Vector2 pos, float size, Data &StyleGuide);
void DrawCardButton(int index, deck *Deck, SingleButtonGroup &buttons, Vector2 pos, float size, Data &StyleGuide);
void DrawGame(deck *deck1, deck *deck2, GameVars &gameVars, Data &StyleGuide, Flags &flags);
void DrawButton(SingleButtonGroup &buttons, int index, Vector2 position, Data &StyleGuide);
Rectangle DrawButton(PlusMinusButtonGroup &buttons, int index, Vector2 position, Data &StyleGuide);
void DrawButtonTopLabel(SingleButtonGroup &buttons, int index, Vector2 position, Data &StyleGuide);
void DrawCardEditScreen(PlusMinusButtonGroup &buttons, CardEditVars &cardEditVars, deck *Deck, deck *dummyDeck, Data &StyleGuide);

#endif
