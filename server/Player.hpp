//
// Created by tabis on 12/10/2019.
//

#pragma once

#include <string>
#include <vector>
#include <fstream>

#include <QFile>

#include "JSONPacket.hpp"

#define DEFAULT_CONFIG_FILE "../scores.json"
#define DEFAULT_CLIENT_NAME "Unknown"

namespace GuessGame {
    class Player {
    public:
        Player(const std::string &name, unsigned int numberToFind, unsigned int triesLeft);
        ~Player() = default;

        void subTries() { _triesLeft--; };

        std::string getName() const { return _name; };
        unsigned int getTriesLeft() const { return _triesLeft; };
        unsigned int getNumberToFind() const { return _numberToFind; };
        QDate getDate() const { return _startDate; };
        bool isInfinite() const { return _infinite; };
    private:
        bool _infinite;
        std::string _name;
        unsigned int _triesLeft;
        unsigned int _numberToFind;
        QDate _startDate;
    };

    class GameManager {
    public:
        GameManager(const std::string &configFile = DEFAULT_CONFIG_FILE);
        ~GameManager() = default;

        unsigned int getPlayerNumberToFindAtIndex(unsigned int index) const { return _players[index].getNumberToFind(); }
        unsigned int getPlayerTriesLeftAtIndex(unsigned int index) const { return _players[index].getTriesLeft(); }
        std::string getPlayerNameAtIndex(unsigned int index) const { return _players[index].getName(); }
        bool getPlayerInfiniteAtIndex(unsigned int index) const { return _players[index].isInfinite(); }
        QDate getPlayerDateAtIndex(unsigned int index) const { return _players[index].getDate(); }
        void subTriesOfPlayerAtIndex(unsigned int index) {_players[index].subTries();};
        void addPlayer(const std::string &name, unsigned int index, unsigned int nbrToFind, unsigned int limit);
        bool isPlayerInList(const std::string &name) const;
        unsigned int getPlayerNumber() const {return _players.size();};

        void deletePlayerAtIndex(unsigned int index);
    private:
        QList<Player> _players;
        std::string _configFilePath;
    };
}