/*
 * Eric Ryan Montgomery
 * 09/21/2025
 * Build Command: g++ -Wall -o out CardGameUI.cpp drawingFunctions.cpp functions.cpp buttons.cpp cards.pp deckofcards.cpp globals.cpp -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm
 * Web Build Command: cmd /c em++ -Wall CardGameUI.cpp drawingFunctions.cpp functions.cpp buttons.cpp cards.cpp deckofcards.cpp globals.cpp -o index.html -I include/ -L lib/ -lraylib -s USE_GLFW=3 -s FULL_ES2=1 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s ASYNCIFY=1 --preload-file textures@/textures
 * Web Execute Local Command: 
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

void PrintVirtualFS_EMS() {
    EM_ASM({
        console.log("=== Virtual FS Contents ===");

        function printDir(path) {
            try {
                var entries = FS.readdir(path);
                for (var i = 0; i < entries.length; i++) {
                    if (entries[i] === "." || entries[i] === "..") continue;
                    var fullPath = path + "/" + entries[i];
                    console.log(fullPath);

                    if (FS.isDir(FS.lookupPath(fullPath).node.mode)) {
                        printDir(fullPath);
                    }
                }
            } catch (e) {
                console.error("Error reading directory:", e);
            }
        }

        printDir("/");
    });
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1600, 900, "Card Game");
    Data StyleGuide;
    Flags flags;
    GameVars gameVars;
    CardEditVars cardEditVars;
    Vector2 mousePoint = {0.0f, 0.0f};
    GameScreen currentScreen = TITLE;
    GameScreen previousScreen = TITLE;
    float scrollOffset = 0;
    
    //Flag to make sure deck is reset
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
    
    //Deck and Card Definitions
    //Dummy deck has to be defined instead of just a single card because decks handle textures
    deck *dummyDeck = new deck(StyleGuide.numCards, cardTexture);
    deck *deck1 = new deck(StyleGuide.numCards, cardTexture, false, false, StyleGuide.deckStrength);
    deck *deck2 = new deck(StyleGuide.numCards, cardTexture, false, false, StyleGuide.deckStrength);
    deck *deck1Copy = new deck(0, cardTexture);
    deck *deck2Copy = new deck(0, cardTexture);
    
    // Main game loop
    bool closeWindow = false;
    while (!closeWindow)    // Detect window close button or ESC key
    {
		//UpdateStyleGuide(StyleGuide);
		mousePoint = GetMousePosition(); //Gets current mouse position
		StyleGuide.Update();
		
		//Input
		switch(currentScreen) {
			case TITLE:
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
			case SETUP:
				setupButtons.AnimationLogic(mousePoint); //Provides the animation logic for the button groups
				
				deck1EditButtons.AnimationLogic(mousePoint);
				for (int i = 0; i < deck1EditButtons.GetSize(); ++i) {
					if (deck1EditButtons[i].GetAction() == true) {
						currentScreen = EDITCARD;
						previousScreen = SETUP;
						cardEditVars.cardClickedOn = i;
						cardEditVars.playerEditing = 0;
						cardEditVars.chosenColor = deck1->at(i)->GetColor();
						cardEditVars.chosenAtt = deck1->at(i)->GetAttribute();
						cardEditVars.chosenPowerLevel = deck1->at(i)->GetNumber();
						cardEditVars.remainingPoints = deck1->GetRemainingPoints();
						cardEditVars.chosenSpell = deck1->at(i)->GetSpell();
						*dummyDeck->at(i) = *deck1->at(i);
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
						cardEditVars.chosenColor = deck2->at(i)->GetColor();
						cardEditVars.chosenAtt = deck2->at(i)->GetAttribute();
						cardEditVars.chosenPowerLevel = deck2->at(i)->GetNumber();
						cardEditVars.remainingPoints = deck2->GetRemainingPoints();
						cardEditVars.chosenSpell = deck2->at(i)->GetSpell();
						*dummyDeck->at(i) = *deck2->at(i);
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
					delete deck1;
					deck1 = new deck(StyleGuide.numCards, cardTexture, true, StyleGuide.deck1AI, StyleGuide.deckStrength);
				}
				if (setupButtons[4].GetAction() == true || IsKeyPressed(KEY_TWO)) { //Create Deck 2
					delete deck2;
					deck2 = new deck(StyleGuide.numCards, cardTexture, true, StyleGuide.deck2AI, StyleGuide.deckStrength);
				}
				if ((setupButtons[5].GetAction() == true || IsKeyPressed(KEY_ENTER))) { //Start Game
					*deck1Copy = *deck1;
					*deck2Copy = *deck2;
					deck1->ShuffleDeck();
					deck2->ShuffleDeck();
					currentScreen = GAME;
					previousScreen = SETUP;
				}
				if (setupButtons[6].GetAction() == true) { //Exit Button
					closeWindow = true;
				}
				break;
			case RULES:
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
			case SKILLS:
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
			case SETTINGS:
				
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
						delete deck1; //Reset Deck1
						delete deck2; //Reset Deck2
						deck1 = new deck(StyleGuide.numCards, cardTexture, false, StyleGuide.deck1AI, StyleGuide.deckStrength);
						deck2 = new deck(StyleGuide.numCards, cardTexture, false, StyleGuide.deck1AI, StyleGuide.deckStrength);
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
						deck2->SetAI(true);
					}
					if (settingsButtons[2].GetAction(1) == true) {
						StyleGuide.deck2AI = false;
						deck2->SetAI(false);
					}
				}
				break;
			case GAME:
				//Allows animation and logic but game buttons
				gameButtons.AnimationLogic(mousePoint);
				
				//Look up the game rules
				if (gameButtons[0].GetAction() == true || IsKeyPressed(KEY_R)) {
					currentScreen = RULES;
					previousScreen = GAME;
				}
				
				//Turns off the swap button if last round
				if (gameVars.round >= StyleGuide.numCards - 1) {
					gameButtons[3].SetFunctionality(false);
				}
				
				//Play Game if not the end of a game
				if (gameVars.gameEnded == false) RegularGame(*deck1, *deck2, gameVars, flags, gameButtons, StyleGuide);
				
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
					*deck1 = *deck1Copy;
					*deck2 = *deck2Copy;
				}
				break;
			case EDITCARD:
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
				
				//Increase spell *Doesn't cause the card to be reset
				if (cardEditButtons[3].GetAction(0) == true) {
					cardEditVars.chosenSpell = spellList[cardEditVars.chosenSpell + 1];
					dummyDeck->at(cardEditVars.cardClickedOn)->SetSpell(cardEditVars.chosenSpell);
				}
				
				//Decrease spell *Doesn't cause the card to be reset
				if (cardEditButtons[3].GetAction(1) == true) {
					cardEditVars.chosenSpell = spellList[cardEditVars.chosenSpell - 1];
					dummyDeck->at(cardEditVars.cardClickedOn)->SetSpell(cardEditVars.chosenSpell);
				}
				
				//Edits the displayed card not the original
				if (flags.cardEdited == true) {
					*dummyDeck->at(cardEditVars.cardClickedOn) = card(cardEditVars.chosenColor, cardEditVars.chosenAtt, cardEditVars.chosenPowerLevel);
					dummyDeck->at(cardEditVars.cardClickedOn)->SetSpell(cardEditVars.chosenSpell);
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
				
				if (cardEditScreenButtons[2].GetAction() == true || IsKeyPressed(KEY_W)) { //Wipe Card Button
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
						*deck1->at(cardEditVars.cardClickedOn) = *dummyDeck->at(cardEditVars.cardClickedOn);
						deck1->SetRemainingPoints(cardEditVars.remainingPoints);
					}
					if (cardEditVars.playerEditing == 1) {
						*deck2->at(cardEditVars.cardClickedOn) = *dummyDeck->at(cardEditVars.cardClickedOn);
						deck2->SetRemainingPoints(cardEditVars.remainingPoints);
					}
					delete dummyDeck; //Reset dummyDeck
					dummyDeck = new deck(StyleGuide.numCards, cardTexture);
					cardEditVars = ResetCardEditVars();
				}
				
				//Goes back to setup screen and cancels selection
                if (cardEditScreenButtons[4].GetAction() == true || IsKeyPressed(KEY_BACKSPACE)) {
					currentScreen = SETUP;
					previousScreen = EDITCARD;
					scrollOffset = 0;
					delete dummyDeck; //Reset dummyDeck
					dummyDeck = new deck(StyleGuide.numCards, cardTexture);
					cardEditVars = ResetCardEditVars();
				}
				break;
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
					DrawCardButtonRowOnGrid(*deck1, deck1EditButtons, 4, {3, 1}, {61, 13}, true, StyleGuide);
					DrawCardButtonRowOnGrid(*deck2, deck2EditButtons, 4, {3, 23}, {61, 35}, true, StyleGuide);
					
					//Draws the setup buttons at the bottom of the screen
					DrawRectangleOnGrid({1, 54}, {63, 63}, BLACK, StyleGuide);
					DrawButtonRowOnGrid(setupButtons, {2, 55}, {62, 62}, StyleGuide);
					
					break;
				}
				case RULES: {
					DrawRules(scrollOffset, StyleGuide);
					DrawButtonLine(singleBackButton, StyleGuide);
					break;
				}
				case SKILLS: {
					DrawSkills(scrollOffset, StyleGuide);
					DrawButtonLine(singleBackButton, StyleGuide);
					break;
				}
				case SETTINGS: {
					DrawSettingsButtons(settingsButtons, StyleGuide);
					DrawButtonLine(singleBackButton, StyleGuide);
					break;
				}
				case GAME: {
					DrawRectangleLinesOnGrid({7, 1}, {57, 7}, BLACK, 5, StyleGuide); //Turn Counter Box
					DrawTextSOnGrid("Round: " + to_string(gameVars.round+1), {7, 1}, {57, 4}, (Alignment){CENTERX, CENTERY}, StyleGuide, 5); //Round #
					DrawTextSOnGrid("Turn: " + to_string(gameVars.turn+1), {7, 4}, {57, 7}, (Alignment){CENTERX, CENTERY}, StyleGuide, 5); //Turn #
					if (gameVars.playerInPlay == 0) DrawTextSOnGrid("Player 1 Turn", {7, 1}, {57, 7}, (Alignment){LEFTX, CENTERY}, StyleGuide, 6); //Shows if it's P1's turn
					if (gameVars.playerInPlay == 1) DrawTextSOnGrid("Player 2 Turn", {7, 1}, {57, 7}, (Alignment){RIGHTX, CENTERY}, StyleGuide, 6); //Shows if it's P2's turn
					DrawCardOnGrid(*deck1, gameVars.round, {7, 8}, {17, 38}, true, StyleGuide); //deck1 main card
					DrawCardOnGrid(*deck2, gameVars.round, {47, 8}, {57, 38}, true, StyleGuide); //deck2 main card
					if (gameVars.round < deck1->size() - 1) { //Only draws support cards if not the last round
						DrawCardOnGrid(*deck1, deck1->size() - 1, {18, 14}, {26, 38}, true, StyleGuide); //deck1 support card
						DrawCardOnGrid(*deck2, deck1->size() - 1, {38, 14}, {46, 38}, true, StyleGuide); //deck2 support card
					}
					DrawRectangleOnGrid({1, 43.0f - ((42.0f/deck1->size()) * gameVars.player1Score)}, {6, 43}, RED, StyleGuide); //Left Score Column
					DrawRectangleOnGrid({58, 43.0f - ((42.0f/deck1->size()) * gameVars.player2Score)}, {63, 43}, BLUE, StyleGuide); //Right Score Column Outline
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
				case(EDITCARD): {
					if (cardEditVars.playerEditing == 0) DrawCardEditScreen(cardEditButtons, cardEditVars, deck1, dummyDeck, StyleGuide);
					if (cardEditVars.playerEditing == 1) DrawCardEditScreen(cardEditButtons, cardEditVars, deck2, dummyDeck, StyleGuide);
					DrawButtonLine(cardEditScreenButtons, StyleGuide);
					break;
				}
			};

        EndDrawing();
        if (WindowShouldClose() == true) closeWindow = true;
        //----------------------------------------------------------------------------------
    }

	// De-Initialization
    //--------------------------------------------------------------------------------------
    delete dummyDeck;
    delete deck1;
    delete deck2;
    UnloadTexture(*buttonTexture);
    UnloadTexture(*cardTexture);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
