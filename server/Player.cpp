//
// Created by tabis on 12/10/2019.
//

#include <QtCore/QSettings>
#include "Player.hpp"

GuessGame::Player::~Player()
{

}

void GuessGame::GameManager::addPlayer(const std::string &name, unsigned int index)
{
    if (index < _players.size() && _players.at(index).getName() == DEFAULT_CLIENT_NAME)
        return;
    for (auto &it : _players)
        if (it.getName() == name && name != DEFAULT_CLIENT_NAME)
            return;
    _players.emplace_back(Player(name));
}

GuessGame::GameManager::GameManager(const std::string &configFile)
{
    Data::JSONPacket parser;
    QJsonObject config = parser.getJSONFromFile(configFile);

    _configFilePath = config["error"] == "yes" ? DEFAULT_CONFIG_FILE : configFile;
}

GuessGame::GameManager::~GameManager()
{
    Data::JSONPacket parser;
    QJsonObject obj;

    for (auto &it : _players)
        obj.insert({ it.getName().c_str() }, (int)it.getScore());
    parser.writeJSONToFile(_configFilePath, obj);
}


bool GuessGame::GameManager::isPlayerInList(const std::string &name) const
{
    for (auto &it : _players)
        if (it.getName() == name)
            return true;
    return false;
}