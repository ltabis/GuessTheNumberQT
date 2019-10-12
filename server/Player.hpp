//
// Created by tabis on 12/10/2019.
//

#pragma once

#include <string>
#include <vector>
#include <fstream>

#include <QFile>

#include "JSONPacket.hpp"

#define DEFAULT_CONFIG_FILE "./game/scores.txt"
#define DEFAULT_CLIENT_NAME "Unknown"

namespace GuessGame {
    class Player {
    public:
        Player(const std::string &name, unsigned int numberToFind) : _name(name), _score(0), _numberToFind() {};
        ~Player();

        void addScore(unsigned int score) {_score += score;};
        std::string getName() const { return _name;};
        unsigned int getScore() const { return _score;};
        unsigned int getTriesLeft() const { return _triesLeft;};
    private:
        std::string _name;
        unsigned int _score;
        unsigned int _triesLeft;
        unsigned int _numberToFind;
    };

    class GameManager {
    public:
        GameManager(const std::string &configFile = DEFAULT_CONFIG_FILE);
        ~GameManager();

        void addPlayer(const std::string &name, unsigned int index, unsigned int nbrToFind);
        bool isPlayerInList(const std::string &name) const;
        unsigned int getPlayerNumber() {return _players.size();};
    private:
        std::vector<Player> _players;
        std::string _configFilePath;
    };
}