/*
 * Eric Ryan Montgomery
 * 09/21/2025
 * Build Command: g++ -Wall -o out CardGame.cpp UIDrawer.cpp RegularGame.cpp Buttons.cpp Card.cpp Deck.cpp StateVariables.cpp GameDataManager.cpp Globals.cpp -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm
 * Web Build Command: cmd /c em++ -Wall CardGame.cpp UIDrawer.cpp RegularGame.cpp Buttons.cpp Card.cpp Deck.cpp StateVariables.cpp GameDataManager.cpp Globals.cpp -o index.html -I include/ -L lib/ -lraylib -s USE_GLFW=3 -s FULL_ES2=1 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s ASYNCIFY=1 -s ASYNCIFY_STACK_SIZE=1048576 --preload-file textures@/textures --preload-file fonts@/fonts
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
#include "GameDataManager.h"
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
	GameDataManager dataManager;
    
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

	//Deck and card Definitions
    //Dummy Deck has to be defined instead of just a single Card because decks handle textures
    Deck dummyDeck(preGameVars.numCards, cardTexture);
    Deck deck1(preGameVars.numCards, cardTexture, false, preGameVars.deckStrength);
    Deck deck2(preGameVars.numCards, cardTexture, false, preGameVars.deckStrength);
    
    //Variable for the main window to close
    bool closeWindow = false;
	
	Card::actions decision = Card::ERROR;
	
	//Button Definitions
    SingleButtonGroup setupButtons(buttonTexture);
		setupButtons.AddButton("Rules", [&](){
			currentScreen = RULES;
			previousScreen = SETUP;
		});
		setupButtons.AddButton("Info", [&](){
			currentScreen = SKILLS;
			previousScreen = SETUP;
		});
		setupButtons.AddButton("Settings", [&](){
			currentScreen = SETTINGS;
			previousScreen = SETUP;
		});
		setupButtons.AddButton("Create Deck 1", [&](){
			deck1 = Deck(preGameVars.numCards, cardTexture, true, preGameVars.deckStrength);
		});
		setupButtons.AddButton("Create Deck 2", [&](){
			deck2 = Deck(preGameVars.numCards, cardTexture, true, preGameVars.deckStrength);
		});
		setupButtons.AddButton("Start", [&](){
			regularGame = make_unique<RegularGame>(deck1, deck2, preGameVars.numCards, preGameVars.deckStrength, preGameVars.deck1AI, preGameVars.deck2AI);
			currentScreen = GAME;
			previousScreen = SETUP;
		});
		setupButtons.AddButton("Exit", [&](){
			closeWindow = true;
		});
		setupButtons.AddButton("Save Decks to Web", [&](){
			dataManager.SaveDecks("/decks/decks.json", deck1, deck2);
		});
		setupButtons.AddButton("Load Decks from Web", [&](){
			dataManager.LoadDecks("/decks/decks.json", deck1, deck2);
		});
		setupButtons.AddButton("Save Decks to Local", [&](){
			dataManager.DownloadDeck("/decks/decks.json");
		});
		setupButtons.AddButton("Load Decks from Local", [&](){

		});
	DoubleButtonGroup settingsButtons(buttonTexture);
		settingsButtons.AddButton(to_string(preGameVars.numCards), "Number of Cards", "+", "-", [&](){
			int currButtonIndex = settingsButtons.GetButtonIndex("Number of Cards");
			++preGameVars.numCards;
			if (preGameVars.numCards >= 7) {
				preGameVars.numCards = 7;
				settingsButtons[currButtonIndex].leftButton.SetFunctionality(false);
			} else {
				settingsButtons[currButtonIndex].rightButton.SetFunctionality(true);
			}
			settingsButtons[currButtonIndex].SetValue(to_string(preGameVars.numCards));
			settingsChanged = true;
		}, [&](){
			int currButtonIndex = settingsButtons.GetButtonIndex("Number of Cards");
			--preGameVars.numCards;
			if (preGameVars.numCards <= 3) {
				preGameVars.numCards = 3;
				settingsButtons[currButtonIndex].rightButton.SetFunctionality(false);
			} else {
				settingsButtons[currButtonIndex].leftButton.SetFunctionality(true);
			}
			settingsButtons[currButtonIndex].SetValue(to_string(preGameVars.numCards));
			settingsChanged = true;
		});
		settingsButtons.AddButton(to_string(preGameVars.deckStrength), "Deck Strength", "+", "-", [&](){
			int currButtonIndex = settingsButtons.GetButtonIndex("Deck Strength");
			++preGameVars.deckStrength;
			if (preGameVars.deckStrength >= 10) {
				preGameVars.deckStrength = 10;
				settingsButtons[currButtonIndex].leftButton.SetFunctionality(false);
			} else {
				settingsButtons[currButtonIndex].rightButton.SetFunctionality(true);
			}
			settingsButtons[currButtonIndex].SetValue(to_string(preGameVars.deckStrength));
			settingsChanged = true;
		}, [&](){
			int currButtonIndex = settingsButtons.GetButtonIndex("Deck Strength");
			--preGameVars.deckStrength;
			if (preGameVars.deckStrength <= 1) {
				preGameVars.deckStrength = 1;
				settingsButtons[currButtonIndex].rightButton.SetFunctionality(false);
			} else {
				settingsButtons[currButtonIndex].leftButton.SetFunctionality(true);
			}
			settingsButtons[currButtonIndex].SetValue(to_string(preGameVars.deckStrength));
			settingsChanged = true;
		});
		settingsButtons.AddButton("false", "Is Player 2 AI?", "true", "false", [&](){
			int currButtonIndex = settingsButtons.GetButtonIndex("Is Player 2 AI?");
			preGameVars.deck2AI = true;
			settingsButtons[currButtonIndex].SetValue("true");
		}, [&](){
			int currButtonIndex = settingsButtons.GetButtonIndex("Is Player 2 AI?");
			preGameVars.deck2AI = false;
			settingsButtons[currButtonIndex].SetValue("false");
		});
		settingsButtons.AddButton("Montserrat", "font", "<-", "->", [&](){
			int currButtonIndex = settingsButtons.GetButtonIndex("font");
			++drawer.currentFont;
			if (drawer.currentFont == fonts.end()) drawer.currentFont = fonts.begin(); // wrap around
			settingsButtons[currButtonIndex].SetValue(drawer.currentFont->name);
		}, [&](){
			int currButtonIndex = settingsButtons.GetButtonIndex("font");
			if (drawer.currentFont == fonts.begin()) {
				drawer.currentFont = prev(fonts.end());
			} else {
				--drawer.currentFont;
			}
			settingsButtons[currButtonIndex].SetValue(drawer.currentFont->name);
		});
	SingleButtonGroup gameButtons(buttonTexture);
		gameButtons.AddButton("Rules", [&](){
			currentScreen = RULES;
			previousScreen = GAME;
		});
		gameButtons.AddButton("Attack", [&](){
			decision = Card::ATTACK;
		});
		gameButtons.AddButton("Cast Spell", [&](){
			decision = Card::CASTSPELL;
		});
		gameButtons.AddButton("Swap", [&](){
			decision = Card::SWAP;
		});
		gameButtons.AddButton("Charge", [&](){
			decision = Card::CHARGE;
		});
		gameButtons.AddButton("Use Flask", [&](){
			decision = Card::FLASK;
		});
		gameButtons.AddButton("Main Menu", [&](){
			regularGame.reset();
			currentScreen = SETUP;
			previousScreen = GAME;
			gameButtons.SetFunctionality(true, 0, gameButtons.GetSize()-1);
		});
	DoubleButtonGroup cardEditButtons(buttonTexture);
		cardEditButtons.AddButton("", "Color", "+", "-", [&](){
			if (cardEditVars.remainingPoints <= 0) return;
			if (cardEditVars.chosenColor != Card::cols[Card::cols.size() - 1]) {
				cardEditVars.chosenColor = Card::cols[cardEditVars.chosenColor + 1];
				--cardEditVars.remainingPoints;
				cardEditVars.cardEdited = true;
			}
			if (cardEditVars.chosenColor == Card::cols[Card::cols.size() - 1]) {
				cardEditButtons["Color"].leftButton.SetFunctionality(false);
			} else {
				cardEditButtons["Color"].rightButton.SetFunctionality(true);
			}
		}, [&](){
			if (cardEditVars.chosenColor != Card::cols[0]) {
				cardEditVars.chosenColor = Card::cols[cardEditVars.chosenColor - 1];
				++cardEditVars.remainingPoints;
				cardEditVars.cardEdited = true;
			}
			if (cardEditVars.chosenColor == Card::cols[0]) {
				cardEditButtons["Color"].rightButton.SetFunctionality(false);
			} else {
				cardEditButtons["Color"].leftButton.SetFunctionality(true);
			}
		});
		cardEditButtons.AddButton("", "Attribute", "+", "-", [](){}, [](){});
		cardEditButtons.AddButton("", "Power Level", "+", "-", [](){}, [](){});
		cardEditButtons.AddButton("", "Spell", "+", "-", [](){}, [](){});
	SingleButtonGroup deck1EditButtons(invisTexture);
	SingleButtonGroup deck2EditButtons(invisTexture);
		for (int i = 0; i < preGameVars.numCards; ++i) {
			deck1EditButtons.AddButton("", [&, i](){
				currentScreen = EDITCARD;
				previousScreen = SETUP;
				cardEditVars.Set(deck1, i, 0);
				*dummyDeck[i] = *deck1[i];
				cardEditButtons["Color"].SetValue(dummyDeck[i]->GetColorStr());
				cardEditButtons["Attribute"].SetValue(dummyDeck[i]->GetAttributeStr());
				cardEditButtons["Power Level"].SetValue(to_string(dummyDeck[i]->GetNumber()));
				cardEditButtons["Spell"].SetValue(dummyDeck[i]->GetSpellStr());
			});
			deck2EditButtons.AddButton("", [&, i](){
				currentScreen = EDITCARD;
				previousScreen = SETUP;
				cardEditVars.Set(deck2, i, 1);
				*dummyDeck[i] = *deck2[i];
				cardEditButtons["Color"].SetValue(dummyDeck[i]->GetColorStr());
				cardEditButtons["Attribute"].SetValue(dummyDeck[i]->GetAttributeStr());
				cardEditButtons["Power Level"].SetValue(to_string(dummyDeck[i]->GetNumber()));
				cardEditButtons["Spell"].SetValue(dummyDeck[i]->GetSpellStr());
			});
		}
	SingleButtonGroup viewCardButtons(invisTexture);
		viewCardButtons.AddButton("", [&](){
			viewCardVars.Set(regularGame->GetRound(), 0); //Deck1 Main Card
			currentScreen = VIEWCARD;
			previousScreen = GAME;
		});
		viewCardButtons.AddButton("", [&](){
			viewCardVars.Set(regularGame->GetNumCards() - 1, 0); //Deck1 Support Card
			currentScreen = VIEWCARD;
			previousScreen = GAME;
		});
		viewCardButtons.AddButton("", [&](){
			viewCardVars.Set(regularGame->GetNumCards() - 1, 1); //Deck2 Main Card
			currentScreen = VIEWCARD;
			previousScreen = GAME;
		});
		viewCardButtons.AddButton("", [&](){
			viewCardVars.Set(regularGame->GetRound(), 1); //Deck2 Support Card
			currentScreen = VIEWCARD;
			previousScreen = GAME;
		});
	SingleButtonGroup cardEditScreenButtons(buttonTexture);
		cardEditScreenButtons.AddButton("Rules", [&](){
			currentScreen = RULES;
			previousScreen = EDITCARD;
		});
		cardEditScreenButtons.AddButton("Abilities", [&](){
			currentScreen = SKILLS;
			previousScreen = EDITCARD;
		});
		cardEditScreenButtons.AddButton("Wipe", [&](){
			cardEditVars.remainingPoints += (cardEditVars.chosenColor + cardEditVars.chosenAtt + cardEditVars.chosenPowerLevel);
			cardEditVars.chosenColor = Card::cols[0];
			cardEditVars.chosenAtt = Card::atts[0];
			cardEditVars.chosenPowerLevel = 0;
			cardEditVars.chosenSpell = Card::FORCE;
		});
		cardEditScreenButtons.AddButton("Accept", [&](){
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
		});
		cardEditScreenButtons.AddButton("Cancel", [&](){
			currentScreen = SETUP;
			previousScreen = EDITCARD;
			drawer.scrollOffset = 0;
			dummyDeck = Deck(preGameVars.numCards, cardTexture);
			cardEditVars.Reset();
		});
	SingleButtonGroup singleBackButton(buttonTexture);
		singleBackButton.AddButton("Back", [&](){
			currentScreen = previousScreen;
			previousScreen = RULES; //FIXME
			drawer.scrollOffset = 0;
		});
	SingleButtonGroup settingsBackButton(buttonTexture);
		settingsBackButton.AddButton("Back", [&](){
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
					deck1EditButtons.AddButton("", [&, i](){
						currentScreen = EDITCARD;
						previousScreen = SETUP;
						cardEditVars.Set(deck1, i, 0);
						*dummyDeck[i] = *deck1[i];
					});
					deck2EditButtons.AddButton("", [&, i](){
						currentScreen = EDITCARD;
						previousScreen = SETUP;
						cardEditVars.Set(deck2, i, 1);
						*dummyDeck[i] = *deck2[i];
					});
				}
				settingsChanged = false;
			}
		});
    
	//Main program loop
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
				setupButtons.AnimationLogic(mousePoint); //Provides the animation logic for the button groups
				
				deck1EditButtons.AnimationLogic(mousePoint);
				deck2EditButtons.AnimationLogic(mousePoint);
				
				break;
			}
			case RULES: {
				
				//Enables the logic for the back button
				singleBackButton.AnimationLogic(mousePoint);
                
				break;
			}
			case SKILLS: {

				//Enables the logic for the back button
				singleBackButton.AnimationLogic(mousePoint);

				break;
			}
			case SETTINGS: {

				//Enables the logic for the back button
				settingsBackButton.AnimationLogic(mousePoint);
				settingsButtons.AnimationLogic(mousePoint);

				break;
			}
			case GAME: {

				decision = Card::ERROR;

				//Allows animation and logic for buttons
				gameButtons.AnimationLogic(mousePoint);
				viewCardButtons.AnimationLogic(mousePoint);

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
				
				//Play Game if not the end of a game
				regularGame->PlayRegularGame(decision);
				
				break;
			}
			case EDITCARD: {
				//Enables the logic for the back button
				cardEditScreenButtons.AnimationLogic(mousePoint);
				cardEditButtons.AnimationLogic(mousePoint);
				
				/*
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
				*/
				
				//Edits the displayed Card not the original
				if (cardEditVars.cardEdited == true) {
					*dummyDeck[cardEditVars.cardClickedOn] = Card(cardEditVars.chosenColor, cardEditVars.chosenAtt, cardEditVars.chosenPowerLevel);
					dummyDeck[cardEditVars.cardClickedOn]->SetSpell(cardEditVars.chosenSpell);
					cardEditButtons["Color"].SetValue(dummyDeck[cardEditVars.cardClickedOn]->GetColorStr());
					cardEditButtons["Attribute"].SetValue(dummyDeck[cardEditVars.cardClickedOn]->GetAttributeStr());
					cardEditButtons["Power Level"].SetValue(to_string(dummyDeck[cardEditVars.cardClickedOn]->GetNumber()));
					cardEditButtons["Spell"].SetValue(dummyDeck[cardEditVars.cardClickedOn]->GetSpellStr());
				}
				break;
			}
			case VIEWCARD: {
				singleBackButton.AnimationLogic(mousePoint);
				
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
					drawer.DrawButtonRowOnGrid(setupButtons, drawer.REC_BTN_START, drawer.REC_BTN_END);
					
					break;
				}
				case RULES: {
					//float DrawTextSWrapped(string text, Rectangle dest, Color tint, float fontSize, Alignment orientation, int lineThickness)
					drawer.maxScroll = drawer.DrawTextSWrapped(gamerules, {0, -drawer.scrollOffset, 63 * drawer.widthSegment, drawer.screenDimensions.y}, drawer.textColor, drawer.currentFont->fontSize, {UIDrawer::LEFTX, UIDrawer::UPY}, 5)
						- (drawer.screenDimensions.y - (11 * drawer.heightSegment));
					
					//Draws the back button at the bottom of the screen
					drawer.DrawRectangleOnGrid(drawer.REC_START, drawer.REC_END, drawer.REC_COLOR); //Rectangle behind buttons
					drawer.DrawButtonRowOnGrid(singleBackButton, drawer.REC_BTN_START, drawer.REC_BTN_END); //back button
					break;
				}
				case SKILLS: {
					//float DrawTextSWrapped(string text, Rectangle dest, Color tint, float fontSize, Alignment orientation, int lineThickness)
					drawer.maxScroll = drawer.DrawTextSWrapped(skills, {0, -drawer.scrollOffset, 63 * drawer.widthSegment, drawer.screenDimensions.y}, drawer.textColor, drawer.currentFont->fontSize, {UIDrawer::LEFTX, UIDrawer::UPY}, 5)
						- (drawer.screenDimensions.y - (11 * drawer.heightSegment));
					
					//Draws the back button at the bottom of the screen
					drawer.DrawRectangleOnGrid(drawer.REC_START, drawer.REC_END, drawer.REC_COLOR); //Rectangle behind buttons
					drawer.DrawButtonRowOnGrid(singleBackButton, drawer.REC_BTN_START, drawer.REC_BTN_END); //back button
					break;
				}
				case SETTINGS: {
					//Draws the settings buttons
					drawer.DrawButtonColumnOnGrid(settingsButtons, {24, 9}, {40, 49});
					
					//Draws the back button at the bottom of the screen
					drawer.DrawRectangleOnGrid(drawer.REC_START, drawer.REC_END, drawer.REC_COLOR); //Rectangle behind buttons
					drawer.DrawButtonRowOnGrid(settingsBackButton, drawer.REC_BTN_START, drawer.REC_BTN_END); //back button
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
					drawer.DrawButtonRowOnGrid(gameButtons, drawer.REC_BTN_START, drawer.REC_BTN_END); //Buttons
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
					drawer.DrawButtonOnGrid(cardEditButtons, 0, {41, 10}, {57, 17});
					drawer.DrawButtonOnGrid(cardEditButtons, 1, {41, 19}, {57, 26});
					drawer.DrawButtonOnGrid(cardEditButtons, 2, {41, 28}, {57, 35});
					drawer.DrawButtonOnGrid(cardEditButtons, 3, {41, 37}, {57, 44});
					
					//Draws the back button at the bottom of the screen
					drawer.DrawRectangleOnGrid(drawer.REC_START, drawer.REC_END, drawer.REC_COLOR); //Rectangle behind buttons
					drawer.DrawButtonRowOnGrid(cardEditScreenButtons, drawer.REC_BTN_START, drawer.REC_BTN_END); //back button
					
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
					drawer.DrawButtonRowOnGrid(singleBackButton, drawer.REC_BTN_START, drawer.REC_BTN_END); //back button
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
