/*
 * Eric Ryan Montgomery
 * 11/25/2025
 * For CardGameUI
 * Regular Game Class
 */
 
#ifndef REGULARGAMEH
#define REGULARGAMEH
#include <iostream>
#include <string>
#include <vector>
#include "raylib.h"
#include "cards.h"
#include "deckofcards.h"
#include "globals.h"
#include "buttons.h"

class RegularGame {
    public:
        //Public Enums
        enum players {
            PLAYER1,
            PLAYER2,
        };

        enum cardRoles {
            C_MAIN,
            C_SUPPORT,
        };

        //Public Variables
        int who = 0; //This is the index for the Card in play
        int turn = 0;
        int round = 0;
        players playerInPlay = PLAYER1; //This is the Deck that is in play
        cardRoles currCardRole = C_MAIN; //This is the role that the current Card is in
        int player1Score = 0;
        int player2Score = 0;
        string dialog = "";
        int amtActions = 0;
        bool gameStarted = false;
        bool gameEnded = false;

        int numCards = 5;
        int deckStrength = 7;
        bool deck1AI = false;
        bool deck2AI = false;
        bool firstTurnFrame = true;
        /*
        * static constexpr makes these varable const and compile time known
        * It also prevents errors when doing something like this:
        * GameVars gameVars1 = gameVars2;
        * because these variable can't be changed
        */
        static constexpr int maxSwaps = 2;
        static constexpr int maxCharges = 2;
        static constexpr int maxFlasks = 2;

        //Public Functions
        void Reset();
        void PlayRegularGame(Deck &deck1, Deck &deck2, SingleButtonGroup &buttons);

    private:
        //Private Functions
        void PlayRegularGameTurn(Deck &player, Deck &opponent, SingleButtonGroup &buttons);
        float GetCardSourceX(Card *card, float cardWidth);
        Rectangle GetCardSourceRec(Card *card, float cardWidth, float cardHeight);
        string ApplyAbility(Deck &player, Deck &opponent, int pos, int mainPos);
        string NullifyOpponentsAbility(Card &player, Card &opponent);
        string PhysicalAttack(Card *player, Card *opponent, Card *mainCard);
        string MagicalAttack(Card *player, Card *opponent, Card *mainCard);
        Card::actions MakeAIDecisionDumb(Deck &player, Deck &opponent);

};

#endif