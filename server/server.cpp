//
// Created by tabis on 10/10/2019.
//

#include "server.hpp"

void GuessGame::Server::run()
{
    while (true) {}
}

GuessGame::Server::Server(unsigned int bound_x, unsigned int bound_y, unsigned int limit) : _bounds(std::make_pair(bound_x, bound_y)), _limit(limit) {}
GuessGame::Server::Server(const std::pair<unsigned int, unsigned int> &bounds, unsigned int limit) : _bounds(bounds), _limit(limit) {}
