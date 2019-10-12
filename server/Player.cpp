//
// Created by tabis on 12/10/2019.
//

#include <QtCore/QSettings>
#include "Player.hpp"

GuessGame::Player::Player(const std::string &name, unsigned int numberToFind, unsigned int triesLeft) :
        _name(name),
        _numberToFind(numberToFind),
        _triesLeft(triesLeft)
{
    _infinite = !_triesLeft;
    _startDate = QDate::currentDate();
};

void GuessGame::GameManager::addPlayer(const std::string &name, unsigned int index, unsigned int nbrToFind, unsigned int limit)
{
    if (index < _players.size() && _players.at(index).getName() == DEFAULT_CLIENT_NAME)
        return;
    for (auto &it : _players)
        if (it.getName() == name && name != DEFAULT_CLIENT_NAME)
            return;
    _players.emplace_back(Player(name, nbrToFind, limit));
}

GuessGame::GameManager::GameManager(const std::string &configFile)
{
    Data::JSONPacket parser;
    QJsonArray config = parser.getJSONFromFile(configFile);

    _configFilePath = config.first().toObject()["error"] == "yes" ? DEFAULT_CONFIG_FILE : configFile;
}

bool GuessGame::GameManager::isPlayerInList(const std::string &name) const
{
    for (auto &it : _players)
        if (it.getName() == name)
            return true;
    return false;
}

void GuessGame::GameManager::deletePlayerAtIndex(unsigned int index)
{
    _players.erase(_players.begin() + index);
}
