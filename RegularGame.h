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
#include "Card.h"
#include "Deck.h"
#include "Globals.h"
#include "Buttons.h"

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

        //Public Functions
        RegularGame(Deck &player1, Deck &player2, int currNumCards, int currDeckStrength, bool isDeck1AI, bool isDeck2AI);
        void PlayRegularGame(SingleButtonGroup &buttons);
        int GetCardInPlay();
        int GetTurn();
        int GetRound();
        players GetPlayerInPlay();
        cardRoles GetCurrCardRole();
        int GetPlayer1Score();
        int GetPlayer2Score();
        string GetDialog();
        int GetAmtActions();
        bool GetGameStarted();
        bool GetGameEnded();
        int GetNumCards();
        int GetDeckStrength();
        bool GetDeck1AI();
        bool GetDeck2AI();
        bool GetFirstTurnFrame();
        int GetMaxSwaps();
        int GetMaxCharges();
        int GetMaxFlask();
        Deck &GetDeck1();
        Deck &GetDeck2();

    protected:
        //Private Variables
        int cardInPlay;
        int turn;
        int round;
        players playerInPlay;
        cardRoles currCardRole;
        int player1Score;
        int player2Score;
        string dialog;
        int amtActions;
        bool gameStarted;
        bool gameEnded;
        int numCards;
        int deckStrength;
        bool deck1AI;
        bool deck2AI;
        bool firstTurnFrame;
        static constexpr int maxSwaps = 2;
        static constexpr int maxCharges = 2;
        static constexpr int maxFlasks = 2;
        Deck deck1;
        Deck deck2;

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