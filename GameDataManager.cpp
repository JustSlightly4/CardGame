/*
* Eric Ryan Montgomery
* 11/26/2025
* For CardGameUI
* GameDataManager for saving and loading data
*/

#include "GameDataManager.h"

GameDataManager::GameDataManager() {
    //The constructor mounts a file system on the web for persistent storage of game data
    //The EM_ASM means that this block is written in JavaScript
    EM_ASM(
        {
            // This checks to see if the folder "decks" exist
            // If it doesn't, create it
            if (!FS.analyzePath('/decks').exists) {
                FS.mkdir('/decks');
            }

            // Mount IDBFS
            // As far as I can tell this literally mounts a drive/folder
            // like you would do on your personal computer
            FS.mount(IDBFS, {}, '/decks');

            // The data has to be synced
            FS.syncfs(true, function (err) {
                if (err) {
                    console.error("IDBFS load error:", err);
                } else {
                    console.log("IDBFS loaded successfully");
                }
            });
        }
    );
}

void GameDataManager::SaveDecks(string path, Deck &deck1, Deck &deck2) {

    json j;

    // Save deck1
    j["deck1"]["cards"] = json::array();
    for (int i = 0; i < deck1.size(); ++i) {
        j["deck1"]["cards"].push_back({
            {"color",     deck1[i]->GetColor()},
            {"attribute", deck1[i]->GetAttribute()},
            {"number",     deck1[i]->GetNumber()},
            {"spell",     deck1[i]->GetSpell()}
        });
    }
    j["deck1"]["remainingPoints"] = deck1.GetRemainingPoints();

    // Save deck2
    j["deck2"]["cards"] = json::array();
    for (int i = 0; i < deck2.size(); ++i) {
        j["deck2"]["cards"].push_back({
            {"color",     deck2[i]->GetColor()},
            {"attribute", deck2[i]->GetAttribute()},
            {"number",     deck2[i]->GetNumber()},
            {"spell",     deck2[i]->GetSpell()}
        });
    }
    j["deck2"]["remainingPoints"] = deck1.GetRemainingPoints();

    //Write the json data to the actual file
    std::ofstream file(path);
    if (!file.is_open()) cout << "Failed to save file!" << endl;
    else file << j.dump(4);
    file.close();

    // Sync to persistent storage
    //The EM_ASM means that this block is written in JavaScript
    EM_ASM(
        FS.syncfs(false, function (err) {
            if (err) console.error("IDBFS save error:", err);
            else console.log("IDBFS save completed");
        });
    );
}

void GameDataManager::LoadDecks(string path, Deck &deck1, Deck &deck2) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "Failed to open: " << path << std::endl;
        return;
    }

    json j;
    try {
        file >> j;
    } catch (std::exception &e) {
        std::cout << "JSON parse failed: " << e.what() << std::endl;
        return;
    }

    int index = 0;
    for (auto &card : j["deck1"]["cards"]) {
        if (index >= deck1.size()) break; //Safety incase more cards are saved than deck size
        *deck1[index] = Card(card["color"], card["attribute"], card["number"]);
        deck1[index]->SetSpell(card["spell"]);
        ++index;
    }
    deck1.SetRemainingPoints(j["deck1"]["remainingPoints"]);

    index = 0;
    for (auto &card : j["deck2"]["cards"]) {
        if (index >= deck2.size()) break; //Safety incase more cards are saved than deck size
        *deck2[index] = Card(card["color"], card["attribute"], card["number"]);
        deck2[index]->SetSpell(card["spell"]);
        deck2.SetRemainingPoints(0);
        ++index;
    }
    deck2.SetRemainingPoints(j["deck1"]["remainingPoints"]);

}

void GameDataManager::DownloadDeck(string path) {
    //The EM_ASM means that this block is written in JavaScript
    EM_ASM(
        {
            var jsPath = UTF8ToString($0);
            if (!FS.analyzePath(jsPath).exists) return;
            const data = FS.readFile(jsPath, { encoding: "binary" });

            const blob = new Blob([data], { type: "application/json" });
            const url = URL.createObjectURL(blob);

            const a = document.createElement("a");
            a.href = url;
            a.download = "decks.json";
            a.click();
            URL.revokeObjectURL(url);
        },
        path.c_str() //This is the argument for $0
    );
}