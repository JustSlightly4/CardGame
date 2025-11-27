/*
 * Eric Ryan Montgomery
 * 11/26/2025
 * For CardGameUI
 * GameDataManager for saving and loading data
 */

#ifndef GAMEDATAMANAGERH
#define GAMEDATAMANAGERH
#include <iostream>
#include <fstream>
#include <emscripten.h>
#include "include/json.hpp"
#include "Deck.h"
#include "Card.h"
using namespace std;
using json = nlohmann::json;

class GameDataManager {
    public:
        GameDataManager();
        void SaveDecks(string path, Deck &deck1, Deck &deck2);
        void DownloadDeck(string path);
        void LoadDecks(string path, Deck &deck1, Deck &deck2);
};
#endif