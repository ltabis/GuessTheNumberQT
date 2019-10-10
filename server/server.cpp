//
// Created by tabis on 10/10/2019.
//

#include "server.hpp"

void GuessGame::Server::run()
{
    _status = IS_RUNNING;
}

GuessGame::Server::Server(unsigned int bound_x, unsigned int bound_y, unsigned int limit) : _bounds(std::make_pair(bound_x, bound_y)), _limit(limit), _status(IS_INACTIVE) {}
GuessGame::Server::Server(const std::pair<unsigned int, unsigned int> &bounds, unsigned int limit) : _bounds(bounds), _limit(limit), _status(IS_INACTIVE) {}
GuessGame::Server::Server(int ac, const char * const *av) : _status(IS_INACTIVE)
{
    // Got to check the args passed as parameters
    (void)ac;
    (void)av;
}

int GuessGame::Server::stop()
{
    if (_status >= IS_INACTIVE)
        return IS_INACTIVE;
    return _status;
}
