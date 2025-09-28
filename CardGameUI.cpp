/*
 * Eric Ryan Montgomery
 * 09/21/2025
 * Build Command: g++ -Wall -o out CardGameUI.cpp drawingFunctions.cpp functions.cpp buttons.cpp cards.pp deckofcards.cpp globals.cpp -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm
 * Web Build Command: cmd /c em++ -Wall CardGameUI.cpp drawingFunctions.cpp functions.cpp buttons.cpp cards.cpp deckofcards.cpp globals.cpp -o index.html -I include/ -L lib/ -lraylib -s USE_GLFW=3 -s FULL_ES2=1 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s ASYNCIFY=1 --preload-file textures@/textures
 * Web Execute Local Command: emrun --port 8080
 * Make Command: mingw32-make
 */
 
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <emscripten.h>
#include "functions.h"
#include "drawingFunctions.h"
#include "deckofcards.h"
#include "cards.h"
#include "buttons.h"
#include "raylib.h"
#include "globals.h"
using namespace std;

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    srand(static_cast<unsigned>(time(nullptr)));
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1600, 900, "card Game");
    Data StyleGuide;
    Flags flags;
    GameVars gameVars;
    CardEditVars cardEditVars;
    ViewCardVars viewCardVars;
    Vector2 mousePoint = {0.0f, 0.0f};
    GameScreen currentScreen = TITLE;
    GameScreen previousScreen = TITLE;
    float scrollOffset = 0;
    
    //Flag to make sure Deck is reset
	bool settingsChanged = false;
    

    SetTargetFPS(0);               // Set our game to run at unlimited frames-per-second
    //--------------------------------------------------------------------------------------

	//Texture Definitions
	shared_ptr<Texture2D> buttonTexture = make_shared<Texture2D>(LoadTexture("textures/button.png"));
	shared_ptr<Texture2D> cardTexture = make_shared<Texture2D>(LoadTexture("textures/cardTextures.png"));
	shared_ptr<Texture2D> invisTexture = make_shared<Texture2D>(LoadTexture("textures/invisTexture.png"));
	
	//Button Definitions
    SingleButtonGroup setupButtons(buttonTexture, StyleGuide);
		setupButtons.AddButton("Rules");
		setupButtons.AddButton("Info");
		setupButtons.AddButton("Settings");
		setupButtons.AddButton("Create Deck 1");
		setupButtons.AddButton("Create Deck 2");
		setupButtons.AddButton("Start");
		setupButtons.AddButton("Exit");
    SingleButtonGroup singleBackButton(buttonTexture, StyleGuide);
		singleBackButton.AddButton("Back");
    PlusMinusButtonGroup settingsButtons(buttonTexture, StyleGuide);
		settingsButtons.AddButton("Cards:");
		settingsButtons.AddButton("Deck Strength:");
		settingsButtons.AddButton("Is Player 2 AI?");
	SingleButtonGroup gameButtons(buttonTexture, StyleGuide);
		gameButtons.AddButton("Rules");
		gameButtons.AddButton("Attack");
		gameButtons.AddButton("Cast Spell");
		gameButtons.AddButton("Swap");
		gameButtons.AddButton("Charge");
		gameButtons.AddButton("Use Flask");
		gameButtons.AddButton("Main Menu");
	SingleButtonGroup deck1EditButtons(invisTexture, StyleGuide);
	SingleButtonGroup deck2EditButtons(invisTexture, StyleGuide);
		for (int i = 0; i < StyleGuide.numCards; ++i) {
			deck1EditButtons.AddButton("");
			deck2EditButtons.AddButton("");
		}
	SingleButtonGroup viewCardButtons(invisTexture, StyleGuide);
		for (int i = 0; i < 4; ++i) {
			viewCardButtons.AddButton("");
		}
	PlusMinusButtonGroup cardEditButtons(buttonTexture, StyleGuide);
		cardEditButtons.AddButton("Color: ");
		cardEditButtons.AddButton("Attribute: ");
		cardEditButtons.AddButton("Power Level: ");
		cardEditButtons.AddButton("Spell: ");
	SingleButtonGroup cardEditScreenButtons(buttonTexture, StyleGuide);
		cardEditScreenButtons.AddButton("Rules");
		cardEditScreenButtons.AddButton("Abilities");
		cardEditScreenButtons.AddButton("Wipe");
		cardEditScreenButtons.AddButton("Accept");
		cardEditScreenButtons.AddButton("Cancel");
    
    //Deck and card Definitions
    //Dummy Deck has to be defined instead of just a single Card because decks handle textures
    Deck dummyDeck(StyleGuide.numCards, cardTexture);
    Deck deck1(StyleGuide.numCards, cardTexture, false, false, StyleGuide.deckStrength);
    Deck deck2(StyleGuide.numCards, cardTexture, false, false, StyleGuide.deckStrength);
    Deck deck1Copy(0, cardTexture);
    Deck deck2Copy(0, cardTexture);
    
    // Main game loop
    bool closeWindow = false;
    while (!closeWindow)    // Detect window close button or ESC key
    {
		//UpdateStyleGuide(StyleGuide);
		mousePoint = GetMousePosition(); //Gets current mouse position
		StyleGuide.Update();
		
		//Input
		switch(currentScreen) {
			case TITLE: {
				//If enter is hit go to the setup screen
                if (IsKeyPressed(KEY_ENTER)) {
                    currentScreen = SETUP;
                    previousScreen = TITLE;
                }
                //If the mouse is hit go to the setup screen
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    currentScreen = SETUP;
                    previousScreen = TITLE;
                }
                //If the mouse is hit go to the setup screen
                if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
                    currentScreen = SETUP;
                    previousScreen = TITLE;
                }
				break;
			}
			case SETUP: {
				setupButtons.AnimationLogic(mousePoint); //Provides the animation logic for the button groups
				
				deck1EditButtons.AnimationLogic(mousePoint);
				for (int i = 0; i < deck1EditButtons.GetSize(); ++i) {
					if (deck1EditButtons[i].GetAction() == true) {
						currentScreen = EDITCARD;
						previousScreen = SETUP;
						cardEditVars.cardClickedOn = i;
						cardEditVars.playerEditing = 0;
						cardEditVars.chosenColor = deck1[i]->GetColor();
						cardEditVars.chosenAtt = deck1[i]->GetAttribute();
						cardEditVars.chosenPowerLevel = deck1[i]->GetNumber();
						cardEditVars.remainingPoints = deck1.GetRemainingPoints();
						cardEditVars.chosenSpell = deck1[i]->GetSpell();
						*dummyDeck[i] = *deck1[i];
						break;
					}
				}
				
				deck2EditButtons.AnimationLogic(mousePoint);
				for (int i = 0; i < deck2EditButtons.GetSize(); ++i) {
					if (deck2EditButtons[i].GetAction() == true) {
						currentScreen = EDITCARD;
						previousScreen = SETUP;
						cardEditVars.cardClickedOn = i;
						cardEditVars.playerEditing = 1;
						cardEditVars.chosenColor = deck2[i]->GetColor();
						cardEditVars.chosenAtt = deck2[i]->GetAttribute();
						cardEditVars.chosenPowerLevel = deck2[i]->GetNumber();
						cardEditVars.remainingPoints = deck2.GetRemainingPoints();
						cardEditVars.chosenSpell = deck2[i]->GetSpell();
						*dummyDeck[i] = *deck2[i];
						break;
					}
				}
				
                if (setupButtons[0].GetAction() == true || IsKeyPressed(KEY_R)) { //Rules Button
					currentScreen = RULES;
					previousScreen = SETUP;
				}
				if (setupButtons[1].GetAction() == true || IsKeyPressed(KEY_A)) { //Abilities Button
					currentScreen = SKILLS;
					previousScreen = SETUP;
				}
				if (setupButtons[2].GetAction() == true || IsKeyPressed(KEY_S)) { //Settings Button
					currentScreen = SETTINGS;
					previousScreen = SETUP;
				}
				if (setupButtons[3].GetAction() == true || IsKeyPressed(KEY_ONE)) { //Create Deck 1
					deck1 = Deck(StyleGuide.numCards, cardTexture, true, StyleGuide.deck1AI, StyleGuide.deckStrength);
				}
				if (setupButtons[4].GetAction() == true || IsKeyPressed(KEY_TWO)) { //Create Deck 2
					deck2 = Deck(StyleGuide.numCards, cardTexture, true, StyleGuide.deck2AI, StyleGuide.deckStrength);
				}
				if ((setupButtons[5].GetAction() == true || IsKeyPressed(KEY_ENTER))) { //Start Game
					deck1Copy = deck1;
					deck2Copy = deck2;
					deck1.ShuffleDeck();
					deck2.ShuffleDeck();
					currentScreen = GAME;
					previousScreen = SETUP;
				}
				if (setupButtons[6].GetAction() == true) { //Exit Button
					closeWindow = true;
				}
				break;
			}
			case RULES: {
				//Enables scrolling
				scrollLogic(scrollOffset, StyleGuide);
				
				//Enables the logic for the back button
				singleBackButton.AnimationLogic(mousePoint);
                
                //Goes back to previous screen
                if (singleBackButton[0].GetAction() == true || IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_B)) {
					currentScreen = previousScreen;
					previousScreen = RULES;
					scrollOffset = 0;
				}
				break;
			}
			case SKILLS: {
				//Enables scrolling
				scrollLogic(scrollOffset, StyleGuide);
				
				//Enables the logic for the back button
				singleBackButton.AnimationLogic(mousePoint);
				
				//Goes back to previous screen
                if (singleBackButton[0].GetAction() == true || IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_B)) {
					currentScreen = previousScreen;
					previousScreen = SKILLS;
					scrollOffset = 0;
				}
				break;
			}
			case SETTINGS: {
				
				//Enables the logic for the back button
				singleBackButton.AnimationLogic(mousePoint);
				settingsButtons.AnimationLogic(mousePoint);
				
				//Goes back to previous screen
                if (singleBackButton[0].GetAction() == true || IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_B)) {
					currentScreen = previousScreen;
					previousScreen = SETTINGS;
					scrollOffset = 0;
					
					//If the settings were changed reset the decks
					if (settingsChanged == true) {
						deck1 = Deck(StyleGuide.numCards, cardTexture, false, StyleGuide.deck1AI, StyleGuide.deckStrength);
						deck2 = Deck(StyleGuide.numCards, cardTexture, false, StyleGuide.deck1AI, StyleGuide.deckStrength);
						deck1EditButtons.ClearAllButtons(); //Clears and resets cardButtons
						deck2EditButtons.ClearAllButtons();
						for (int i = 0; i < StyleGuide.numCards; ++i) {
							deck1EditButtons.AddButton("");
							deck2EditButtons.AddButton("");
						}
						settingsChanged = false;
					}
					
				}
				
				//Increases or decreases the number of cards
				if (settingsButtons[0].GetAllActions() == true) {
					if (settingsButtons[0].GetAction(0) == true) {
						++StyleGuide.numCards;
						if (StyleGuide.numCards > 7) StyleGuide.numCards = 7;
					}
					if (settingsButtons[0].GetAction(1) == true) {
						--StyleGuide.numCards;
						if (StyleGuide.numCards < 3) StyleGuide.numCards = 3;
					}
					settingsChanged = true;
				}
				
				//Increases or decreases the strength of the cards
				if (settingsButtons[1].GetAllActions() == true) {
					if (settingsButtons[1].GetAction(0) == true) {
						++StyleGuide.deckStrength;
						if (StyleGuide.deckStrength > 10) StyleGuide.deckStrength = 10;
					}
					if (settingsButtons[1].GetAction(1) == true) {
						--StyleGuide.deckStrength;
						if (StyleGuide.deckStrength < 1) StyleGuide.deckStrength = 1;
					}
					settingsChanged = true;
				}
				
				//Change Deck2 to be ai or not
				if (settingsButtons[2].GetAllActions() == true) {
					if (settingsButtons[2].GetAction(0) == true) {
						StyleGuide.deck2AI = true;
						deck2.SetAI(true);
					}
					if (settingsButtons[2].GetAction(1) == true) {
						StyleGuide.deck2AI = false;
						deck2.SetAI(false);
					}
				}
				break;
			}
			case GAME: {
				//Allows animation and logic for buttons
				gameButtons.AnimationLogic(mousePoint);
				viewCardButtons.AnimationLogic(mousePoint);
				
				//Look up the game rules
				if (gameButtons[0].GetAction() == true || IsKeyPressed(KEY_R)) {
					currentScreen = RULES;
					previousScreen = GAME;
				}
				
				//View a Cards Detailed Stats
				for (int i = 0; i < 4; ++i) {
					if (viewCardButtons[i].GetAction() == true) {
						if (i == 0 || i == 3) viewCardVars.cardIndex = gameVars.round;
						else viewCardVars.cardIndex = StyleGuide.numCards - 1;
						if (i == 0 || i == 1) viewCardVars.deckNum = 1;
						else viewCardVars.deckNum = 2;
						currentScreen = VIEWCARD;
						previousScreen = GAME;
					}
				}
				
				//Turns off the swap button if last round
				if (gameVars.round >= StyleGuide.numCards - 1) {
					gameButtons[3].SetFunctionality(false);
				}
				
				//Play Game if not the end of a game
				if (gameVars.gameEnded == false) RegularGame(deck1, deck2, gameVars, flags, gameButtons, StyleGuide);
				
				//Go back to the setup
				if (gameButtons[6].GetAction() == true || IsKeyPressed(KEY_BACKSPACE)) {
					gameVars.turn = 0;
					gameVars.playerInPlay = PLAYER1;
					gameVars.currCardRole = C_MAIN;
					gameVars.round = 0;
					gameVars.dialog = "Game Start";
					flags = ResetFlags();
					gameVars = ResetGameVars();
					currentScreen = SETUP;
					previousScreen = GAME;
					gameButtons.SetFunctionality(true, 0, gameButtons.GetSize()-1);
					deck1 = deck1Copy;
					deck2 = deck2Copy;
				}
				break;
			}
			case EDITCARD: {
				//Enables the logic for the back button
				cardEditScreenButtons.AnimationLogic(mousePoint);
				cardEditButtons.AnimationLogic(mousePoint);
				
				//Button On/Off Logic
				for (int i = 0; i < cardEditButtons.GetSize(); ++i) {
					cardEditButtons[i].SetFunctionality(0, true);
					cardEditButtons[i].SetFunctionality(1, true);
				}
				
				//If no points turn off all increase buttons except for last
				if (cardEditVars.remainingPoints <= 0) { 
					for (int i = 0; i < cardEditButtons.GetSize() - 1; ++i) {
						cardEditButtons[i].SetFunctionality(0, false);
					}
				}
				
				//If max color turn off increase color button
				if (cardEditVars.chosenColor == cols[cols.size() - 1]) {
					cardEditButtons[0].SetFunctionality(0, false);
				}
				
				//If min color turn off decrease color button
				if (cardEditVars.chosenColor == cols[0]) {
					cardEditButtons[0].SetFunctionality(1, false);
				}
				
				//If max attribute turn off increase attribute button
				if (cardEditVars.chosenAtt == atts[atts.size() - 1]) {
					cardEditButtons[1].SetFunctionality(0, false);
				}
				
				//If min attribute turn off decrease attribute button
				if (cardEditVars.chosenAtt == atts[0]) {
					cardEditButtons[1].SetFunctionality(1, false);
				}
				
				//If max power level turn off increase power button
				if (cardEditVars.chosenPowerLevel >= 9) {
					cardEditButtons[2].SetFunctionality(0, false);
				}
				
				//If min power level turn off decrease power button
				if (cardEditVars.chosenPowerLevel <= 0) {
					cardEditButtons[2].SetFunctionality(1, false);
				}
				
				//If max spell turn off increase power button
				if (cardEditVars.chosenSpell == spellList[spellList.size()-1]) {
					cardEditButtons[3].SetFunctionality(0, false);
				}
				
				//If min spell turn off decrease power button
				if (cardEditVars.chosenSpell == spellList[0]) {
					cardEditButtons[3].SetFunctionality(1, false);
				}
				
				//Increase Color
				if (cardEditButtons[0].GetAction(0) == true) {
					cardEditVars.chosenColor = cols[cardEditVars.chosenColor + 1];
					--cardEditVars.remainingPoints;
					flags.cardEdited = true;
				}
				
				//Decrease Color
				if (cardEditButtons[0].GetAction(1) == true) {
					cardEditVars.chosenColor = cols[cardEditVars.chosenColor - 1];
					++cardEditVars.remainingPoints;
					flags.cardEdited = true;
				}
				
				//Increase attribute
				if (cardEditButtons[1].GetAction(0) == true) {
					cardEditVars.chosenAtt = atts[cardEditVars.chosenAtt + 1];
					--cardEditVars.remainingPoints;
					flags.cardEdited = true;
				}
				
				//Decrease attribute
				if (cardEditButtons[1].GetAction(1) == true) {
					cardEditVars.chosenAtt = atts[cardEditVars.chosenAtt - 1];
					++cardEditVars.remainingPoints;
					flags.cardEdited = true;
				}
				
				//Increase power level
				if (cardEditButtons[2].GetAction(0) == true) {
					cardEditVars.chosenPowerLevel += 1;
					--cardEditVars.remainingPoints;
					flags.cardEdited = true;
				}
				
				//Decrease power level
				if (cardEditButtons[2].GetAction(1) == true) {
					cardEditVars.chosenPowerLevel -= 1;
					++cardEditVars.remainingPoints;
					flags.cardEdited = true;
				}
				
				//Increase spell *Doesn't cause the Card to be reset
				if (cardEditButtons[3].GetAction(0) == true) {
					cardEditVars.chosenSpell = spellList[cardEditVars.chosenSpell + 1];
					dummyDeck[cardEditVars.cardClickedOn]->SetSpell(cardEditVars.chosenSpell);
				}
				
				//Decrease spell *Doesn't cause the Card to be reset
				if (cardEditButtons[3].GetAction(1) == true) {
					cardEditVars.chosenSpell = spellList[cardEditVars.chosenSpell - 1];
					dummyDeck[cardEditVars.cardClickedOn]->SetSpell(cardEditVars.chosenSpell);
				}
				
				//Edits the displayed Card not the original
				if (flags.cardEdited == true) {
					*dummyDeck[cardEditVars.cardClickedOn] = Card(cardEditVars.chosenColor, cardEditVars.chosenAtt, cardEditVars.chosenPowerLevel);
					dummyDeck[cardEditVars.cardClickedOn]->SetSpell(cardEditVars.chosenSpell);
					flags.cardEdited = false;
				}
				
				if (cardEditScreenButtons[0].GetAction() == true || IsKeyPressed(KEY_R)) { //Rules Button
					currentScreen = RULES;
					previousScreen = EDITCARD;
				}
				
				if (cardEditScreenButtons[1].GetAction() == true || IsKeyPressed(KEY_A)) { //Abilities Button
					currentScreen = SKILLS;
					previousScreen = EDITCARD;
				}
				
				if (cardEditScreenButtons[2].GetAction() == true || IsKeyPressed(KEY_W)) { //Wipe card Button
					cardEditVars.remainingPoints += (cardEditVars.chosenColor + cardEditVars.chosenAtt + cardEditVars.chosenPowerLevel);
					cardEditVars.chosenColor = cols[0];
					cardEditVars.chosenAtt = atts[0];
					cardEditVars.chosenPowerLevel = 0;
					cardEditVars.chosenSpell = FORCE;
					flags.cardEdited = true;
				}
				
				//Goes back to setup screen and saves selection
                if (cardEditScreenButtons[3].GetAction() == true || IsKeyPressed(KEY_ENTER)) {
					currentScreen = SETUP;
					previousScreen = EDITCARD;
					scrollOffset = 0;
					if (cardEditVars.playerEditing == 0) {
						*deck1[cardEditVars.cardClickedOn] = *dummyDeck[cardEditVars.cardClickedOn];
						deck1.SetRemainingPoints(cardEditVars.remainingPoints);
					}
					if (cardEditVars.playerEditing == 1) {
						*deck2[cardEditVars.cardClickedOn] = *dummyDeck[cardEditVars.cardClickedOn];
						deck2.SetRemainingPoints(cardEditVars.remainingPoints);
					}
					dummyDeck = Deck(StyleGuide.numCards, cardTexture);
					cardEditVars = ResetCardEditVars();
				}
				
				//Goes back to setup screen and cancels selection
                if (cardEditScreenButtons[4].GetAction() == true || IsKeyPressed(KEY_BACKSPACE)) {
					currentScreen = SETUP;
					previousScreen = EDITCARD;
					scrollOffset = 0;
					dummyDeck = Deck(StyleGuide.numCards, cardTexture);
					cardEditVars = ResetCardEditVars();
				}
				break;
			}
			case VIEWCARD: {
				singleBackButton.AnimationLogic(mousePoint);
				
				//Goes back to previous screen
                if (singleBackButton[0].GetAction() == true || IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_B)) {
					currentScreen = previousScreen;
					previousScreen = VIEWCARD;
				}
				break;
			}
		};
		
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawFPSOnGrid(StyleGuide);
            
            switch(currentScreen) {
				case TITLE: {
					DrawTextS("Press Enter to Start", (Rectangle){0, 0, StyleGuide.screenDimensions.x, StyleGuide.screenDimensions.y}, StyleGuide.textColor, StyleGuide.fontSize, (Alignment){CENTERX, CENTERY});
					break;
				}
				case SETUP: {
					
					//Draws both decks on the screen
					DrawTextSOnGrid("Deck 1", {3, 1}, {61, 3}, (Alignment){CENTERX, UPY}, StyleGuide); //Deck 1 Label
					DrawCardButtonRowOnGrid(deck1, deck1EditButtons, 4, {3, 3}, {61, 15}, true, StyleGuide); //Deck 1
					DrawTextSOnGrid("Deck 2", {3, 25}, {61, 27}, (Alignment){CENTERX, UPY}, StyleGuide); //Deck 2 Label
					DrawCardButtonRowOnGrid(deck2, deck2EditButtons, 4, {3, 27}, {61, 39}, true, StyleGuide); //Deck 2
					
					//Draws the setup buttons at the bottom of the screen
					DrawRectangleOnGrid({1, 54}, {63, 63}, BLACK, StyleGuide); //Rectangle behind buttons
					DrawButtonRowOnGrid(setupButtons, {2, 55}, {62, 62}, StyleGuide); //Setup buttons
					
					break;
				}
				case RULES: {
					//float DrawTextSWrapped(string text, Rectangle dest, Color tint, float fontSize, Alignment orientation, int lineThickness)
					StyleGuide.maxScroll = DrawTextSWrapped(gamerules, {0, -scrollOffset, 63 * StyleGuide.widthSegment, StyleGuide.screenDimensions.y}, StyleGuide.textColor, StyleGuide.fontSize, (Alignment){LEFTX, UPY}, 5)
						- (StyleGuide.screenDimensions.y - (11 * StyleGuide.heightSegment));
					
					//Draws the back button at the bottom of the screen
					DrawRectangleOnGrid({1, 54}, {63, 63}, BLACK, StyleGuide); //Rectangle behind button
					DrawButtonRowOnGrid(singleBackButton, {2, 55}, {62, 62}, StyleGuide); //back button
					break;
				}
				case SKILLS: {
					//float DrawTextSWrapped(string text, Rectangle dest, Color tint, float fontSize, Alignment orientation, int lineThickness)
					StyleGuide.maxScroll = DrawTextSWrapped(skills, {0, -scrollOffset, 63 * StyleGuide.widthSegment, StyleGuide.screenDimensions.y}, StyleGuide.textColor, StyleGuide.fontSize, (Alignment){LEFTX, UPY}, 5)
						- (StyleGuide.screenDimensions.y - (11 * StyleGuide.heightSegment));
					
					//Draws the back button at the bottom of the screen
					DrawRectangleOnGrid({1, 54}, {63, 63}, BLACK, StyleGuide); //Rectangle behind button
					DrawButtonRowOnGrid(singleBackButton, {2, 55}, {62, 62}, StyleGuide); //back button
					break;
				}
				case SETTINGS: {
					//Draws the settings buttons
					DrawButtonOnGrid(settingsButtons, 0, to_string(StyleGuide.numCards), {24, 9}, {40, 16}, StyleGuide);
					DrawButtonOnGrid(settingsButtons, 1, to_string(StyleGuide.deckStrength), {24, 20}, {40, 27}, StyleGuide);
					if (StyleGuide.deck2AI) DrawButtonOnGrid(settingsButtons, 2, "true", {24, 31}, {40, 38}, StyleGuide);
					else DrawButtonOnGrid(settingsButtons, 2, "false", {24, 31}, {40, 38}, StyleGuide);
					
					//Draws the back button at the bottom of the screen
					DrawRectangleOnGrid({1, 54}, {63, 63}, BLACK, StyleGuide); //Rectangle behind button
					DrawButtonRowOnGrid(singleBackButton, {2, 55}, {62, 62}, StyleGuide); //back button
					break;
				}
				case GAME: {
					DrawRectangleLinesOnGrid({7, 1}, {57, 7}, BLACK, 5, StyleGuide); //Turn Counter Box
					DrawTextSOnGrid("Round: " + to_string(gameVars.round+1), {7, 1}, {57, 4}, (Alignment){CENTERX, CENTERY}, StyleGuide, 5); //Round #
					DrawTextSOnGrid("Turn: " + to_string(gameVars.turn+1), {7, 4}, {57, 7}, (Alignment){CENTERX, CENTERY}, StyleGuide, 5); //Turn #
					DrawTextSOnGrid(gameVars.playerInPlay == 0 ? "Player 1 Turn" : "Player 2 Turn", {7, 1}, {57, 7}, (Alignment){gameVars.playerInPlay == 0 ? LEFTX : RIGHTX, CENTERY}, StyleGuide, 6);
					DrawCardButtonOnGrid(deck1, viewCardButtons, gameVars.round, 0, {7, 8}, {17, 38}, true, StyleGuide); //deck1 main Card
					DrawCardButtonOnGrid(deck2, viewCardButtons, gameVars.round, 3, {47, 8}, {57, 38}, true, StyleGuide); //deck2 main Card
					if (gameVars.round < deck1.size() - 1) { //Only draws support cards if not the last round
						DrawCardButtonOnGrid(deck1, viewCardButtons, deck1.size() - 1, 1, {18, 14}, {26, 38}, true, StyleGuide); //deck1 support Card
						DrawCardButtonOnGrid(deck2, viewCardButtons, deck1.size() - 1, 2, {38, 14}, {46, 38}, true, StyleGuide); //deck2 support Card
					}
					DrawRectangleOnGrid({1, 43.0f - ((42.0f/deck1.size()) * gameVars.player1Score)}, {6, 43}, RED, StyleGuide); //Left Score Column
					DrawRectangleOnGrid({58, 43.0f - ((42.0f/deck1.size()) * gameVars.player2Score)}, {63, 43}, BLUE, StyleGuide); //Right Score Column Outline
					DrawRectangleLinesOnGrid({1, 1}, {6, 43}, BLACK, 5, StyleGuide); //Left Score Column Outline
					DrawRectangleLinesOnGrid({58, 1}, {63, 43}, BLACK, 5, StyleGuide); //Right Score Column Outline
					DrawRectangleLinesOnGrid({1, 44}, {63, 53}, BLACK, 5, StyleGuide); //Text Box
					DrawTextSWrappedOnGrid(gameVars.dialog, {1, 44}, {63, 53}, (Alignment){CENTERX, CENTERY}, StyleGuide, 5); //Dialog in Text Box
					DrawRectangleOnGrid({1, 54}, {63, 63}, BLACK, StyleGuide); //Behind Buttons
					DrawButtonRowOnGrid(gameButtons, {2, 55}, {62, 62}, StyleGuide); //Buttons
					//Draw Star on who is playing
					if (gameVars.playerInPlay == PLAYER1) {
						if (gameVars.currCardRole == C_MAIN) DrawStarOnGrid({7, 8}, StyleGuide);
						else DrawStarOnGrid({18, 14}, StyleGuide);
					} else {
						if (gameVars.currCardRole == C_MAIN) DrawStarOnGrid({57, 8}, StyleGuide);
						else DrawStarOnGrid({46, 14}, StyleGuide);
					}
					break;
				}
				case EDITCARD: {
					
					//String for stats
					string stats = "Power: " + to_string(dummyDeck[cardEditVars.cardClickedOn]->GetPower()) +
						"\nMagical Power: " + to_string(dummyDeck[cardEditVars.cardClickedOn]->GetMagicalPower()) +
						"\nHealth: " + to_string(dummyDeck[cardEditVars.cardClickedOn]->GetHealth()) + "/" + to_string(dummyDeck[cardEditVars.cardClickedOn]->GetHealthT()) +
						"\nPhysical Resistance: " + dummyDeck[cardEditVars.cardClickedOn]->GetPhysicalResistanceStr(2) +
						"\nMagical Resistance: " + dummyDeck[cardEditVars.cardClickedOn]->GetMagicalResistanceStr(2) +
						"\nAbility: " + dummyDeck[cardEditVars.cardClickedOn]->GetAbilityStr() +
						"\nSpell: " + dummyDeck[cardEditVars.cardClickedOn]->GetSpellStr();
						
					//Draws the Card on the screen and its stats
					DrawCardOnGrid(dummyDeck, cardEditVars.cardClickedOn, {7, 8}, {17, 38}, true, StyleGuide);
					DrawTextSOnGrid(stats, {18, 8}, {25, 38}, (Alignment){LEFTX, CENTERY}, StyleGuide);
					
					//Draw how many points the Deck has on the screen
					DrawTextSOnGrid("Points Left " + to_string(cardEditVars.remainingPoints) + "/" +  to_string((cardEditVars.playerEditing == 0 ? deck1 : deck2).GetTotalPoints()), 
					{7, 1}, {57, 7}, (Alignment){CENTERX, CENTERY}, StyleGuide, 5);
					
					//Draws the settings buttons
					DrawButtonOnGrid(cardEditButtons, 0, dummyDeck[cardEditVars.cardClickedOn]->GetColorStr(), {41, 10}, {57, 17}, StyleGuide);
					DrawButtonOnGrid(cardEditButtons, 1, dummyDeck[cardEditVars.cardClickedOn]->GetAttributeStr(), {41, 19}, {57, 26}, StyleGuide);
					DrawButtonOnGrid(cardEditButtons, 2, to_string(dummyDeck[cardEditVars.cardClickedOn]->GetNumber()), {41, 28}, {57, 35}, StyleGuide);
					DrawButtonOnGrid(cardEditButtons, 3, dummyDeck[cardEditVars.cardClickedOn]->GetSpellStr(), {41, 37}, {57, 44}, StyleGuide);
					
					//Draws the back button at the bottom of the screen
					DrawRectangleOnGrid({1, 54}, {63, 63}, BLACK, StyleGuide); //Rectangle behind button
					DrawButtonRowOnGrid(cardEditScreenButtons, {2, 55}, {62, 62}, StyleGuide); //back button
					
					break;
				}
				case VIEWCARD: {
					Deck *deck;
					if (viewCardVars.deckNum == 1) deck = &deck1;
					else deck = &deck2;
					
					//String for stats
					string stats = "Power: " + to_string(deck->at(viewCardVars.cardIndex)->GetPower()) +
						"\nMagical Power: " + to_string(deck->at(viewCardVars.cardIndex)->GetMagicalPower()) +
						"\nHealth: " + to_string(deck->at(viewCardVars.cardIndex)->GetHealth()) + "/" + to_string(deck->at(cardEditVars.cardClickedOn)->GetHealthT()) +
						"\nPhysical Resistance: " + deck->at(viewCardVars.cardIndex)->GetPhysicalResistanceStr(2) +
						"\nMagical Resistance: " + deck->at(viewCardVars.cardIndex)->GetMagicalResistanceStr(2) +
						"\nAbility: " + deck->at(viewCardVars.cardIndex)->GetAbilityStr() +
						"\nSpell: " + deck->at(viewCardVars.cardIndex)->GetSpellStr();
						
					//Draws the Card on the screen and its stats
					DrawCardOnGrid(*deck, viewCardVars.cardIndex, {7, 8}, {17, 38}, true, StyleGuide);
					DrawTextSOnGrid(stats, {18, 8}, {25, 38}, (Alignment){LEFTX, CENTERY}, StyleGuide);
					
					//Draws the back button at the bottom of the screen
					DrawRectangleOnGrid({1, 54}, {63, 63}, BLACK, StyleGuide); //Rectangle behind button
					DrawButtonRowOnGrid(singleBackButton, {2, 55}, {62, 62}, StyleGuide); //back button
					break;
				}
			};

        EndDrawing();
        if (WindowShouldClose() == true) closeWindow = true;
        //----------------------------------------------------------------------------------
    }

	// De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(*buttonTexture);
    UnloadTexture(*cardTexture);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
