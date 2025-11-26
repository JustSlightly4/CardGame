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
#include <unordered_map>
#include "raylib.h"
#include "Card.h"
#include "Deck.h"
#include "Globals.h"
#include "Buttons.h"

class RegularGame {
    public:
        //Public Enums
        enum players {
            PLAYER1 = 0,
            PLAYER2 = 1,
        };

        enum cardRoles {
            C_MAIN = 0,
            C_SUPPORT = 0,
        };

        //Public Functions
        RegularGame(Deck &player1, Deck &player2, int currNumCards, int currDeckStrength, bool isDeck1AI, bool isDeck2AI);
        void PlayRegularGame(Card::actions nextDecision);
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
        Deck &GetPlayer();
        Deck &GetOpponent();

    protected:
        //Private Variables
        Deck deck1;
        Deck deck2;
        Deck* player;
        Deck* opponent;
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

        //Dispatch Table
        /*
        * This is a hash table that takes in a players actions and then performs that action
        * accordingly in O(1) time.
        * unordered_map<Card::actions, function<returnType(arguments)>>
        * [this] Capture Clause that captures all the class variables but not any local variables
        * () Function Arguments. Doesn't need any.
        * {Card::Key, function to return}
        */
        unordered_map<Card::actions, function<string()>> performAction = {
            {Card::ATTACK, [this](){ return PhysicalAttack(); }},
            {Card::CASTSPELL, [this](){ return MagicalAttack(); }},
            {Card::CHARGE, [this](){ return Charge(); }},
            {Card::SWAP, [this](){ return Swap(); }},
            {Card::FLASK, [this](){ return UseFlask(); }},
            {Card::ERROR, [this](){ return ""; }}
        };

        //Private Functions
        float GetCardSourceX(Card *card, float cardWidth);
        Rectangle GetCardSourceRec(Card *card, float cardWidth, float cardHeight);
        string ApplyAbility();
        string NullifyOpponentsAbility();
        string PhysicalAttack();
        string MagicalAttack();
        string Swap();
        string Charge();
        string UseFlask();
        Card::actions MakeAIDecisionDumb();

};

#endif