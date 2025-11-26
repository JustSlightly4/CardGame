/*
 * Eric Ryan Montgomery
 * 09/21/2025
 * Build Command: g++ -Wall -o out CardGame.cpp UIDrawer.cpp RegularGame.cpp Buttons.cpp Card.cpp Deck.cpp StateVariables.cpp Globals.cpp -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm
 * Web Build Command: cmd /c em++ -Wall CardGame.cpp UIDrawer.cpp RegularGame.cpp Buttons.cpp Card.cpp Deck.cpp StateVariables.cpp Globals.cpp -o index.html -I include/ -L lib/ -lraylib -s USE_GLFW=3 -s FULL_ES2=1 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s ASYNCIFY=1 -s ASYNCIFY_STACK_SIZE=1048576 --preload-file textures@/textures --preload-file fonts@/fonts
 * Web Execute Local Command: emrun --port 8080 index.html
 * Make Commands:
 * 		1. make clean
 * 		2. make
 * 		3. make run
 */
 
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <emscripten.h>
#include "RegularGame.h"
#include "UIDrawer.h"
#include "Deck.h"
#include "Card.h"
#include "Buttons.h"
#include "Globals.h"
#include "raylib.h"
#include "StateVariables.h"
using namespace std;

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    srand(static_cast<unsigned>(time(nullptr)));
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1600, 900, "card Game");
    unique_ptr<RegularGame> regularGame;
	PreGameVars preGameVars;
    CardEditVars cardEditVars;
    ViewCardVars viewCardVars;
    Vector2 mousePoint = {0.0f, 0.0f};
    GameScreen currentScreen = TITLE;
    GameScreen previousScreen = TITLE;
	UIDrawer drawer;
    
    //Flag to make sure Deck is reset
	bool settingsChanged = false;

    SetTargetFPS(0);               // Set our game to run at unlimited frames-per-second
    //--------------------------------------------------------------------------------------

	//Texture Definitions
	shared_ptr<Texture2D> buttonTexture = make_shared<Texture2D>(LoadTexture("textures/button.png"));
	shared_ptr<Texture2D> cardTexture = make_shared<Texture2D>(LoadTexture("textures/cardTextures.png"));
	shared_ptr<Texture2D> invisTexture = make_shared<Texture2D>(LoadTexture("textures/invisTexture.png"));
	
	//Font Definitions
	fonts.push_back({"Montserrat", LoadFont("fonts/MontserratExtrabold.ttf"), 24});
	fonts.push_back({"Super Shiny", LoadFont("fonts/SuperShiny.ttf"), 20});
	fonts.push_back({"Akieir", LoadFont("fonts/Akieir.ttf"), 26});
	drawer.currentFont = fonts.begin();
	
	
	//Button Definitions
    SingleButtonGroup setupButtons1(buttonTexture);
		setupButtons1.AddButton("Rules");
		setupButtons1.AddButton("Info");
		setupButtons1.AddButton("Settings");
		setupButtons1.AddButton("Create Deck 1");
		setupButtons1.AddButton("Create Deck 2");
		setupButtons1.AddButton("Start");
		setupButtons1.AddButton("Exit");
	SingleButtonGroup setupButtons2(buttonTexture);
		for (int i = 0; i < setupButtons1.GetSize(); ++i) {
			setupButtons2.AddButton("");
		}
    SingleButtonGroup singleBackButton(buttonTexture);
		singleBackButton.AddButton("Back");
    PlusMinusButtonGroup settingsButtons(buttonTexture);
		settingsButtons.AddButton("Number of Cards", to_string(preGameVars.numCards));
		settingsButtons.AddButton("Deck Strength", to_string(preGameVars.deckStrength));
		settingsButtons.AddButton("Is Player 2 AI?", "false");
		settingsButtons.AddButton("Font", "Montserrat");
	SingleButtonGroup gameButtons(buttonTexture);
		gameButtons.AddButton("Rules");
		gameButtons.AddButton("Attack");
		gameButtons.AddButton("Cast Spell");
		gameButtons.AddButton("Swap");
		gameButtons.AddButton("Charge");
		gameButtons.AddButton("Use Flask");
		gameButtons.AddButton("Main Menu");
	SingleButtonGroup deck1EditButtons(invisTexture);
	SingleButtonGroup deck2EditButtons(invisTexture);
		for (int i = 0; i < preGameVars.numCards; ++i) {
			deck1EditButtons.AddButton("");
			deck2EditButtons.AddButton("");
		}
	SingleButtonGroup viewCardButtons(invisTexture);
		for (int i = 0; i < 4; ++i) {
			viewCardButtons.AddButton("");
		}
	PlusMinusButtonGroup cardEditButtons(buttonTexture);
		cardEditButtons.AddButton("Color", "");
		cardEditButtons.AddButton("Attribute", "");
		cardEditButtons.AddButton("Power Level", "");
		cardEditButtons.AddButton("Spell", "");
	SingleButtonGroup cardEditScreenButtons(buttonTexture);
		cardEditScreenButtons.AddButton("Rules");
		cardEditScreenButtons.AddButton("Abilities");
		cardEditScreenButtons.AddButton("Wipe");
		cardEditScreenButtons.AddButton("Accept");
		cardEditScreenButtons.AddButton("Cancel");
    
    //Deck and card Definitions
    //Dummy Deck has to be defined instead of just a single Card because decks handle textures
    Deck dummyDeck(preGameVars.numCards, cardTexture);
    Deck deck1(preGameVars.numCards, cardTexture, false, preGameVars.deckStrength);
    Deck deck2(preGameVars.numCards, cardTexture, false, preGameVars.deckStrength);
    
    // Main game loop
    bool closeWindow = false;
    while (!closeWindow)    // Detect window close button or ESC key
    {
		//UpdateStyleGuide(StyleGuide);
		mousePoint = GetMousePosition(); //Gets current mouse position
		drawer.Update();
		
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
				setupButtons1.AnimationLogic(mousePoint); //Provides the animation logic for the button groups
				setupButtons2.AnimationLogic(mousePoint);
				
				deck1EditButtons.AnimationLogic(mousePoint);
				for (int i = 0; i < deck1EditButtons.GetSize(); ++i) {
					if (deck1EditButtons[i].GetAction() == true) {
						currentScreen = EDITCARD;
						previousScreen = SETUP;
						cardEditVars.Set(deck1, i, 0);
						*dummyDeck[i] = *deck1[i];
						break;
					}
				}
				
				deck2EditButtons.AnimationLogic(mousePoint);
				for (int i = 0; i < deck2EditButtons.GetSize(); ++i) {
					if (deck2EditButtons[i].GetAction() == true) {
						currentScreen = EDITCARD;
						previousScreen = SETUP;
						cardEditVars.Set(deck2, i, 1);
						*dummyDeck[i] = *deck2[i];
						break;
					}
				}
				
                if (setupButtons1["Rules"].GetAction() == true || IsKeyPressed(KEY_R)) { //Rules Button
					currentScreen = RULES;
					previousScreen = SETUP;
				}
				if (setupButtons1["Info"].GetAction() == true || IsKeyPressed(KEY_A)) { //Abilities Button
					currentScreen = SKILLS;
					previousScreen = SETUP;
				}
				if (setupButtons1["Settings"].GetAction() == true || IsKeyPressed(KEY_S)) { //Settings Button
					currentScreen = SETTINGS;
					previousScreen = SETUP;
				}
				if (setupButtons1["Create Deck 1"].GetAction() == true || IsKeyPressed(KEY_ONE)) { //Create Deck 1
					deck1 = Deck(preGameVars.numCards, cardTexture, true, preGameVars.deckStrength);
				}
				if (setupButtons1["Create Deck 2"].GetAction() == true || IsKeyPressed(KEY_TWO)) { //Create Deck 2
					deck2 = Deck(preGameVars.numCards, cardTexture, true, preGameVars.deckStrength);
				}
				if ((setupButtons1["Start"].GetAction() == true || IsKeyPressed(KEY_ENTER))) { //Start Game
					regularGame = make_unique<RegularGame>(deck1, deck2, preGameVars.numCards, preGameVars.deckStrength, preGameVars.deck1AI, preGameVars.deck2AI);
					currentScreen = GAME;
					previousScreen = SETUP;
				}
				if (setupButtons1["Exit"].GetAction() == true) { //Exit Button
					closeWindow = true;
				}
				break;
			}
			case RULES: {
				
				//Enables the logic for the back button
				singleBackButton.AnimationLogic(mousePoint);
                
                //Goes back to previous screen
                if (singleBackButton[0].GetAction() == true || IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_B)) {
					currentScreen = previousScreen;
					previousScreen = RULES;
					drawer.scrollOffset = 0;
				}
				break;
			}
			case SKILLS: {

				//Enables the logic for the back button
				singleBackButton.AnimationLogic(mousePoint);
				
				//Goes back to previous screen
                if (singleBackButton[0].GetAction() == true || IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_B)) {
					currentScreen = previousScreen;
					previousScreen = SKILLS;
					drawer.scrollOffset = 0;
				}
				break;
			}
			case SETTINGS: {
				
				//Enables the logic for the back button
				singleBackButton.AnimationLogic(mousePoint);
				settingsButtons.AnimationLogic(mousePoint);
				
				//Reset Button On/Off Logic
				for (int i = 0; i < cardEditButtons.GetSize(); ++i) {
					settingsButtons[i].SetFunctionality(0, true);
					settingsButtons[i].SetFunctionality(1, true);
				}
				
				//Goes back to previous screen
                if (singleBackButton[0].GetAction() == true || IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_B)) {
					currentScreen = previousScreen;
					previousScreen = SETTINGS;
					drawer.scrollOffset = 0;
					
					//If the settings were changed reset the decks
					if (settingsChanged == true) {
						deck1 = Deck(preGameVars.numCards, cardTexture, false, preGameVars.deckStrength);
						deck2 = Deck(preGameVars.numCards, cardTexture, false, preGameVars.deckStrength);
						deck1EditButtons.ClearAllButtons(); //Clears and resets cardButtons
						deck2EditButtons.ClearAllButtons();
						for (int i = 0; i < preGameVars.numCards; ++i) {
							deck1EditButtons.AddButton("");
							deck2EditButtons.AddButton("");
						}
						settingsChanged = false;
					}
					
				}
				
				//Increases or decreases the number of cards
				if (settingsButtons["Number of Cards"].GetAction(0) == true) {
					++preGameVars.numCards;
					if (preGameVars.numCards > 7) preGameVars.numCards = 7;
					settingsButtons["Number of Cards"].SetLabel(to_string(preGameVars.numCards));
					settingsChanged = true;
				}
				
				if (settingsButtons["Number of Cards"].GetAction(1) == true) {
					--preGameVars.numCards;
					if (preGameVars.numCards < 3) preGameVars.numCards = 3;
					settingsButtons["Number of Cards"].SetLabel(to_string(preGameVars.numCards));
					settingsChanged = true;
				}
				
				//Increases or decreases the strength of the cards
				if (settingsButtons["Deck Strength"].GetAction(0) == true) {
					++preGameVars.deckStrength;
					if (preGameVars.deckStrength > 10) preGameVars.deckStrength = 10;
					settingsButtons["Deck Strength"].SetLabel(to_string(preGameVars.deckStrength));
					settingsChanged = true;
				}
				
				if (settingsButtons["Deck Strength"].GetAction(1) == true) {
					--preGameVars.deckStrength;
					if (preGameVars.deckStrength < 1) preGameVars.deckStrength = 1;
					settingsButtons["Deck Strength"].SetLabel(to_string(preGameVars.deckStrength));
					settingsChanged = true;
				}
				
				//Change Deck2 to be ai or not
				if (settingsButtons["Is Player 2 AI?"].GetAction(0) == true) {
					preGameVars.deck2AI = true;
					settingsButtons["Is Player 2 AI?"].SetLabel("true");
				}
				
				if (settingsButtons["Is Player 2 AI?"].GetAction(1) == true) {
					preGameVars.deck2AI = false;
					settingsButtons["Is Player 2 AI?"].SetLabel("false");
				}
				
				//Changes the font ++
				if (settingsButtons["Font"].GetAction(0) == true) {
					++drawer.currentFont;
					if (drawer.currentFont == fonts.end()) drawer.currentFont = fonts.begin(); // wrap around
					settingsButtons["Font"].SetLabel(drawer.currentFont->name);
				}
				
				//Changes the font --
				if (settingsButtons["Font"].GetAction(1) == true) {
					if (drawer.currentFont == fonts.begin()) drawer.currentFont = fonts.end();
					--drawer.currentFont;
					settingsButtons["Font"].SetLabel(drawer.currentFont->name);
				}
				
				break;
			}
			case GAME: {
				//Allows animation and logic for buttons
				gameButtons.AnimationLogic(mousePoint);
				viewCardButtons.AnimationLogic(mousePoint);

				Card::actions decision = Card::ERROR;
				
				//View a Cards Detailed Stats
				for (int i = 0; i < 4; ++i) {
					if (viewCardButtons[i].GetAction() == true) {
						if (i == 0) viewCardVars.Set(regularGame->GetRound(), 0); //Deck1 Main Card
						if (i == 1) viewCardVars.Set(regularGame->GetNumCards() - 1, 0); //Deck1 Support Card
						if (i == 2) viewCardVars.Set(regularGame->GetNumCards() - 1, 1); //Deck2 Main Card
						if (i == 3) viewCardVars.Set(regularGame->GetRound(), 1); //Deck2 Support Card
						currentScreen = VIEWCARD;
						previousScreen = GAME;
					}
				}

				//Enable all buttons
				gameButtons.SetFunctionality(true, 0, gameButtons.GetSize()-1); //----------------------------------------------
				
				//Disable swap button if already used twice
				if (regularGame->GetPlayer().GetTimesSwapped() >= regularGame->GetMaxSwaps()) {
					gameButtons["Swap"].SetFunctionality(false);
				}

				//Turns off the swap button if last round
				if (regularGame->GetRound() >= regularGame->GetNumCards() - 1) {
					gameButtons["Swap"].SetFunctionality(false);
				}
				
				//Disable charge button if already used twice
				if (regularGame->GetPlayer().at(regularGame->GetCardInPlay())->GetCharge() >= regularGame->GetMaxCharges()) {
					gameButtons["Charge"].SetFunctionality(false);
				}
				
				//Disable flask button if already used twice
				if (regularGame->GetPlayer().GetFlaskAmt() >= regularGame->GetMaxFlask()) {
					gameButtons["Use Flask"].SetFunctionality(false);
				}
				
				//Disable attack button if support Card
				if (regularGame->GetCurrCardRole() == RegularGame::C_SUPPORT) {
					gameButtons["Attack"].SetFunctionality(false);
				}

				//Disable all buttons if game is done
				if (regularGame->GetGameEnded()) gameButtons.SetFunctionality(false, 0, gameButtons.GetSize()-2);

				//Look up the game rules
				if (gameButtons["Rules"].GetAction() == true || IsKeyPressed(KEY_R)) {
					currentScreen = RULES;
					previousScreen = GAME;
				}

				//Go back to the setup
				if (gameButtons["Main Menu"].GetAction() == true || IsKeyPressed(KEY_BACKSPACE)) {
					regularGame.reset();
					currentScreen = SETUP;
					previousScreen = GAME;
					gameButtons.SetFunctionality(true, 0, gameButtons.GetSize()-1);
				}
				
				//Physical Attack
				if (((gameButtons["Attack"].GetAction() == true || IsKeyPressed(KEY_A)) && gameButtons["Attack"].GetFunctionality() == true)) decision = Card::ATTACK;
				
				//Magical Attack
				if (((gameButtons["Cast Spell"].GetAction() == true || IsKeyPressed(KEY_P)) && gameButtons["Cast Spell"].GetFunctionality() == true)) decision = Card::CASTSPELL;
				
				//Swap
				if (((gameButtons["Swap"].GetAction() == true || IsKeyPressed(KEY_S)) && gameButtons["Swap"].GetFunctionality() == true)) decision = Card::SWAP;
				
				//Charge
				if (((gameButtons["Charge"].GetAction() == true || IsKeyPressed(KEY_C)) && gameButtons["Charge"].GetFunctionality() == true)) decision = Card::CHARGE;
				
				//Flask *does not use players turn
				if ((gameButtons["Use Flask"].GetAction() == true || IsKeyPressed(KEY_F)) && gameButtons["Use Flask"].GetFunctionality() == true) decision = Card::FLASK;
				
				//Play Game if not the end of a game
				regularGame->PlayRegularGame(decision);
				
				break;
			}
			case EDITCARD: {
				//Enables the logic for the back button
				cardEditScreenButtons.AnimationLogic(mousePoint);
				cardEditButtons.AnimationLogic(mousePoint);
				
				//Reset Button On/Off Logic
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
				if (cardEditVars.chosenColor == Card::cols[Card::cols.size() - 1]) {
					cardEditButtons["Color"].SetFunctionality(0, false);
				}
				
				//If min color turn off decrease color button
				if (cardEditVars.chosenColor == Card::cols[0]) {
					cardEditButtons["Color"].SetFunctionality(1, false);
				}
				
				//If max attribute turn off increase attribute button
				if (cardEditVars.chosenAtt == Card::atts[Card::atts.size() - 1]) {
					cardEditButtons["Attribute"].SetFunctionality(0, false);
				}
				
				//If min attribute turn off decrease attribute button
				if (cardEditVars.chosenAtt == Card::atts[0]) {
					cardEditButtons["Attribute"].SetFunctionality(1, false);
				}
				
				//If max power level turn off increase power button
				if (cardEditVars.chosenPowerLevel >= 9) {
					cardEditButtons["Power Level"].SetFunctionality(0, false);
				}
				
				//If min power level turn off decrease power button
				if (cardEditVars.chosenPowerLevel <= 0) {
					cardEditButtons["Power Level"].SetFunctionality(1, false);
				}
				
				//If max spell turn off increase power button
				if (cardEditVars.chosenSpell == Card::spellList[Card::spellList.size()-1]) {
					cardEditButtons["Spell"].SetFunctionality(0, false);
				}
				
				//If min spell turn off decrease power button
				if (cardEditVars.chosenSpell == Card::spellList[0]) {
					cardEditButtons["Spell"].SetFunctionality(1, false);
				}
				
				//Increase Color
				if (cardEditButtons["Color"].GetAction(0) == true) {
					cardEditVars.chosenColor = Card::cols[cardEditVars.chosenColor + 1];
					--cardEditVars.remainingPoints;
				}
				
				//Decrease Color
				if (cardEditButtons["Color"].GetAction(1) == true) {
					cardEditVars.chosenColor = Card::cols[cardEditVars.chosenColor - 1];
					++cardEditVars.remainingPoints;
				}
				
				//Increase attribute
				if (cardEditButtons["Attribute"].GetAction(0) == true) {
					cardEditVars.chosenAtt = Card::atts[cardEditVars.chosenAtt + 1];
					--cardEditVars.remainingPoints;
				}
				
				//Decrease attribute
				if (cardEditButtons["Attribute"].GetAction(1) == true) {
					cardEditVars.chosenAtt = Card::atts[cardEditVars.chosenAtt - 1];
					++cardEditVars.remainingPoints;
				}
				
				//Increase power level
				if (cardEditButtons["Power Level"].GetAction(0) == true) {
					cardEditVars.chosenPowerLevel += 1;
					--cardEditVars.remainingPoints;
				}
				
				//Decrease power level
				if (cardEditButtons["Power Level"].GetAction(1) == true) {
					cardEditVars.chosenPowerLevel -= 1;
					++cardEditVars.remainingPoints;
				}
				
				//Increase spell *Doesn't cause the Card to be reset
				if (cardEditButtons["Spell"].GetAction(0) == true) {
					cardEditVars.chosenSpell = Card::spellList[cardEditVars.chosenSpell + 1];
					dummyDeck[cardEditVars.cardClickedOn]->SetSpell(cardEditVars.chosenSpell);
				}
				
				//Decrease spell *Doesn't cause the Card to be reset
				if (cardEditButtons["Spell"].GetAction(1) == true) {
					cardEditVars.chosenSpell = Card::spellList[cardEditVars.chosenSpell - 1];
					dummyDeck[cardEditVars.cardClickedOn]->SetSpell(cardEditVars.chosenSpell);
				}
				
				//Edits the displayed Card not the original
				if (cardEditVars.cardEdited == true) {
					*dummyDeck[cardEditVars.cardClickedOn] = Card(cardEditVars.chosenColor, cardEditVars.chosenAtt, cardEditVars.chosenPowerLevel);
					dummyDeck[cardEditVars.cardClickedOn]->SetSpell(cardEditVars.chosenSpell);
				}
				
				if (cardEditScreenButtons["Rules"].GetAction() == true || IsKeyPressed(KEY_R)) { //Rules Button
					currentScreen = RULES;
					previousScreen = EDITCARD;
				}
				
				if (cardEditScreenButtons["Abilities"].GetAction() == true || IsKeyPressed(KEY_A)) { //Abilities Button
					currentScreen = SKILLS;
					previousScreen = EDITCARD;
				}
				
				if (cardEditScreenButtons["Wipe"].GetAction() == true || IsKeyPressed(KEY_W)) { //Wipe card Button
					cardEditVars.remainingPoints += (cardEditVars.chosenColor + cardEditVars.chosenAtt + cardEditVars.chosenPowerLevel);
					cardEditVars.chosenColor = Card::cols[0];
					cardEditVars.chosenAtt = Card::atts[0];
					cardEditVars.chosenPowerLevel = 0;
					cardEditVars.chosenSpell = Card::FORCE;
				}
				
				//Goes back to setup screen and saves selection
                if (cardEditScreenButtons["Accept"].GetAction() == true || IsKeyPressed(KEY_ENTER)) {
					currentScreen = SETUP;
					previousScreen = EDITCARD;
					drawer.scrollOffset = 0;
					if (cardEditVars.playerEditing == 0) {
						*deck1[cardEditVars.cardClickedOn] = *dummyDeck[cardEditVars.cardClickedOn];
						deck1.SetRemainingPoints(cardEditVars.remainingPoints);
					}
					if (cardEditVars.playerEditing == 1) {
						*deck2[cardEditVars.cardClickedOn] = *dummyDeck[cardEditVars.cardClickedOn];
						deck2.SetRemainingPoints(cardEditVars.remainingPoints);
					}
					dummyDeck = Deck(preGameVars.numCards, cardTexture);
					cardEditVars.Reset();
				}
				
				//Goes back to setup screen and cancels selection
                if (cardEditScreenButtons["Cancel"].GetAction() == true || IsKeyPressed(KEY_BACKSPACE)) {
					currentScreen = SETUP;
					previousScreen = EDITCARD;
					drawer.scrollOffset = 0;
					dummyDeck = Deck(preGameVars.numCards, cardTexture);
					cardEditVars.Reset();
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
            drawer.DrawFPSOnGrid();
            
            switch(currentScreen) {
				case TITLE: {
					drawer.DrawTextS("Press Enter to Start", (Rectangle){0, 0, drawer.screenDimensions.x, drawer.screenDimensions.y}, 
					drawer.textColor, drawer.currentFont->fontSize, {UIDrawer::CENTERX, UIDrawer::CENTERY});
					break;
				}
				case SETUP: {
					
					//Draws both decks on the screen
					drawer.DrawTextSOnGrid("Deck 1", {3, 1}, {61, 3}, {UIDrawer::CENTERX, UIDrawer::UPY}); //Deck 1 Label
					drawer.DrawCardButtonRowOnGrid(deck1, deck1EditButtons, 4, {3, 3}, {61, 15}, true); //Deck 1
					drawer.DrawTextSOnGrid("Deck 2", {3, 25}, {61, 27}, {UIDrawer::CENTERX, UIDrawer::UPY}); //Deck 2 Label
					drawer.DrawCardButtonRowOnGrid(deck2, deck2EditButtons, 4, {3, 27}, {61, 39}, true); //Deck 2
					
					//Draws the setup buttons at the bottom of the screen
					drawer.DrawRectangleOnGrid(drawer.REC_START, drawer.REC_END, drawer.REC_COLOR); //Rectangle behind buttons
					//DrawButtonRowOnGrid(setupButtons, {2, 55}, {62, 62}); //Setup buttons
					drawer.DrawButtonRowOnGrid(setupButtons1, drawer.REC_BTN_START1, drawer.REC_BTN_END1);
					drawer.DrawButtonRowOnGrid(setupButtons2, drawer.REC_BTN_START2, drawer.REC_BTN_END2);
					
					break;
				}
				case RULES: {
					//float DrawTextSWrapped(string text, Rectangle dest, Color tint, float fontSize, Alignment orientation, int lineThickness)
					drawer.maxScroll = drawer.DrawTextSWrapped(gamerules, {0, -drawer.scrollOffset, 63 * drawer.widthSegment, drawer.screenDimensions.y}, drawer.textColor, drawer.currentFont->fontSize, {UIDrawer::LEFTX, UIDrawer::UPY}, 5)
						- (drawer.screenDimensions.y - (11 * drawer.heightSegment));
					
					//Draws the back button at the bottom of the screen
					drawer.DrawRectangleOnGrid(drawer.REC_START, drawer.REC_END, drawer.REC_COLOR); //Rectangle behind buttons
					drawer.DrawButtonRowOnGrid(singleBackButton, drawer.REC_BTN_START1, drawer.REC_BTN_END2); //back button
					break;
				}
				case SKILLS: {
					//float DrawTextSWrapped(string text, Rectangle dest, Color tint, float fontSize, Alignment orientation, int lineThickness)
					drawer.maxScroll = drawer.DrawTextSWrapped(skills, {0, -drawer.scrollOffset, 63 * drawer.widthSegment, drawer.screenDimensions.y}, drawer.textColor, drawer.currentFont->fontSize, {UIDrawer::LEFTX, UIDrawer::UPY}, 5)
						- (drawer.screenDimensions.y - (11 * drawer.heightSegment));
					
					//Draws the back button at the bottom of the screen
					drawer.DrawRectangleOnGrid(drawer.REC_START, drawer.REC_END, drawer.REC_COLOR); //Rectangle behind buttons
					drawer.DrawButtonRowOnGrid(singleBackButton, drawer.REC_BTN_START1, drawer.REC_BTN_END2); //back button
					break;
				}
				case SETTINGS: {
					//Draws the settings buttons
					drawer.DrawButtonColumnOnGrid(settingsButtons, {24, 9}, {40, 49});
					
					//Draws the back button at the bottom of the screen
					drawer.DrawRectangleOnGrid(drawer.REC_START, drawer.REC_END, drawer.REC_COLOR); //Rectangle behind buttons
					drawer.DrawButtonRowOnGrid(singleBackButton, drawer.REC_BTN_START1, drawer.REC_BTN_END2); //back button
					break;
				}
				case GAME: {
					drawer.DrawRectangleLinesOnGrid({7, 1}, {57, 7}, BLACK, 5); //Turn Counter Box
					drawer.DrawTextSOnGrid("Round: " + to_string(regularGame->GetRound()+1), {7, 1}, {57, 4}, {UIDrawer::CENTERX, UIDrawer::CENTERY}, 5); //Round #
					drawer.DrawTextSOnGrid("Turn: " + to_string(regularGame->GetTurn()+1), {7, 4}, {57, 7}, {UIDrawer::CENTERX, UIDrawer::CENTERY}, 5); //Turn #
					drawer.DrawTextSOnGrid(regularGame->GetPlayerInPlay() == RegularGame::PLAYER1 ? "Player 1 Turn" : "Player 2 Turn", {7, 1}, {57, 7}, {regularGame->GetPlayerInPlay() == RegularGame::PLAYER1 ? UIDrawer::LEFTX : UIDrawer::RIGHTX, UIDrawer::CENTERY}, 6);
					drawer.DrawCardButtonOnGrid(regularGame->GetDeck1(), viewCardButtons, regularGame->GetRound(), 0, {7, 8}, {17, 38}, true); //deck1 main Card
					drawer.DrawCardButtonOnGrid(regularGame->GetDeck2(), viewCardButtons, regularGame->GetRound(), 3, {47, 8}, {57, 38}, true); //deck2 main Card
					if (regularGame->GetRound() < deck1.size() - 1) { //Only draws support cards if not the last round
						drawer.DrawCardButtonOnGrid(regularGame->GetDeck1(), viewCardButtons, deck1.size() - 1, 1, {18, 14}, {26, 38}, true); //deck1 support Card
						drawer.DrawCardButtonOnGrid(regularGame->GetDeck2(), viewCardButtons, deck1.size() - 1, 2, {38, 14}, {46, 38}, true); //deck2 support Card
					}
					drawer.DrawRectangleOnGrid({1, 43.0f - ((42.0f/deck1.size()) * regularGame->GetPlayer1Score())}, {6, 43}, RED); //Left Score Column
					drawer.DrawRectangleOnGrid({58, 43.0f - ((42.0f/deck1.size()) * regularGame->GetPlayer2Score())}, {63, 43}, BLUE); //Right Score Column Outline
					drawer.DrawRectangleLinesOnGrid({1, 1}, {6, 43}, BLACK, 5); //Left Score Column Outline
					drawer.DrawRectangleLinesOnGrid({58, 1}, {63, 43}, BLACK, 5); //Right Score Column Outline
					drawer.DrawRectangleLinesOnGrid({1, 44}, {63, 52}, BLACK, 5); //Text Box
					drawer.DrawTextSWrappedOnGrid(regularGame->GetDialog(), {1, 44}, {63, 52}, {UIDrawer::CENTERX, UIDrawer::CENTERY}, 5); //Dialog in Text Box
					drawer.DrawRectangleOnGrid(drawer.REC_START, drawer.REC_END, drawer.REC_COLOR); //Rectangle behind buttons
					drawer.DrawButtonRowOnGrid(gameButtons, drawer.REC_BTN_START1, drawer.REC_BTN_END2); //Buttons
					//Draw Star on who is playing
					if (regularGame->GetPlayerInPlay() == RegularGame::PLAYER1) {
						if (regularGame->GetCurrCardRole() == RegularGame::C_MAIN) drawer.DrawStarOnGrid({7, 8});
						else drawer.DrawStarOnGrid({18, 14});
					} else {
						if (regularGame->GetCurrCardRole() == RegularGame::C_MAIN) drawer.DrawStarOnGrid({57, 8});
						else drawer.DrawStarOnGrid({46, 14});
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
					drawer.DrawCardOnGrid(dummyDeck, cardEditVars.cardClickedOn, {7, 8}, {17, 38}, true);
					drawer.DrawTextSOnGrid(stats, {18, 8}, {25, 38}, {UIDrawer::LEFTX, UIDrawer::CENTERY});
					
					//Draw how many points the Deck has on the screen
					drawer.DrawTextSOnGrid("Points Left " + to_string(cardEditVars.remainingPoints) + "/" +  to_string((cardEditVars.playerEditing == 0 ? deck1 : deck2).GetTotalPoints()), 
					{7, 1}, {57, 7}, {UIDrawer::CENTERX, UIDrawer::CENTERY}, 5);
					
					//Draws the settings buttons
					drawer.DrawButtonOnGrid(cardEditButtons, 0, dummyDeck[cardEditVars.cardClickedOn]->GetColorStr(), {41, 10}, {57, 17});
					drawer.DrawButtonOnGrid(cardEditButtons, 1, dummyDeck[cardEditVars.cardClickedOn]->GetAttributeStr(), {41, 19}, {57, 26});
					drawer.DrawButtonOnGrid(cardEditButtons, 2, to_string(dummyDeck[cardEditVars.cardClickedOn]->GetNumber()), {41, 28}, {57, 35});
					drawer.DrawButtonOnGrid(cardEditButtons, 3, dummyDeck[cardEditVars.cardClickedOn]->GetSpellStr(), {41, 37}, {57, 44});
					
					//Draws the back button at the bottom of the screen
					drawer.DrawRectangleOnGrid(drawer.REC_START, drawer.REC_END, drawer.REC_COLOR); //Rectangle behind buttons
					drawer.DrawButtonRowOnGrid(cardEditScreenButtons, drawer.REC_BTN_START1, drawer.REC_BTN_END2); //back button
					
					break;
				}
				case VIEWCARD: {
					Deck *deck;
					if (viewCardVars.deckNum == 0) deck = &regularGame->GetDeck1();
					else deck = &regularGame->GetDeck2();
					
					//String for stats
					string stats = "Power: " + to_string(deck->at(viewCardVars.cardIndex)->GetPower()) +
						"\nMagical Power: " + to_string(deck->at(viewCardVars.cardIndex)->GetMagicalPower()) +
						"\nHealth: " + to_string(deck->at(viewCardVars.cardIndex)->GetHealth()) + "/" + to_string(deck->at(viewCardVars.cardIndex)->GetHealthT()) +
						"\nPhysical Resistance: " + deck->at(viewCardVars.cardIndex)->GetPhysicalResistanceStr(2) +
						"\nMagical Resistance: " + deck->at(viewCardVars.cardIndex)->GetMagicalResistanceStr(2) +
						"\nAbility: " + deck->at(viewCardVars.cardIndex)->GetAbilityStr() +
						"\nSpell: " + deck->at(viewCardVars.cardIndex)->GetSpellStr();
						
					//Draws the Card on the screen and its stats
					drawer.DrawCardOnGrid(*deck, viewCardVars.cardIndex, {7, 8}, {17, 38}, true);
					drawer.DrawTextSOnGrid(stats, {18, 8}, {25, 38}, {UIDrawer::LEFTX, UIDrawer::CENTERY});
					
					//Draws the back button at the bottom of the screen
					drawer.DrawRectangleOnGrid(drawer.REC_START, drawer.REC_END, drawer.REC_COLOR); //Rectangle behind buttons
					drawer.DrawButtonRowOnGrid(singleBackButton, drawer.REC_BTN_START1, drawer.REC_BTN_END2); //back button
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
    for (int i = 0; i < fonts.size(); ++i) {
		UnloadFont(fonts[i].font);
	}
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
