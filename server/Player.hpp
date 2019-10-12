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
        Player(const std::string &name, unsigned int numberToFind, unsigned int triesLeft);

        void subTries() { _triesLeft--; };

        std::string getName() const { return _name; };
        unsigned int getTriesLeft() const { return _triesLeft; };
        unsigned int getNumberToFind() const { return _numberToFind; };
    private:
        bool _infinite;
        std::string _name;
        unsigned int _triesLeft;
        unsigned int _numberToFind;
    };

    class GameManager {
    public:
        GameManager(const std::string &configFile = DEFAULT_CONFIG_FILE);
        ~GameManager();

        unsigned int getPlayerNumberToFindAtIndex(unsigned int index) const { return _players.at(index).getNumberToFind(); }
        unsigned int getPlayerTriesLeftAtIndex(unsigned int index) const { return _players.at(index).getTriesLeft(); }
        std::string getPlayerNameAtIndex(unsigned int index) const { return _players.at(index).getName(); }
        void subTriesOfPlayerAtIndex(unsigned int index) {_players.at(index).subTries();};
        void addPlayer(const std::string &name, unsigned int index, unsigned int nbrToFind, unsigned int limit);
        bool isPlayerInList(const std::string &name) const;
        unsigned int getPlayerNumber() const {return _players.size();};

        void deletePlayerAtIndex(unsigned int index);
    private:
        std::vector<Player> _players;
        std::string _configFilePath;
    };
}