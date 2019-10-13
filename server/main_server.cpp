//
// Created by tabis on 10/10/2019.
//

#include <memory>
#include "Exception.hpp"
#include "Server.hpp"

int main(int ac, char *av[])
{
    try {
        std::unique_ptr<GuessGame::IServer> server(new GuessGame::Server(ac, av));

        return server->run();
    } catch (Log::Exception &exception) {
        exception.debugErrorMessage();
    }
    return 0;
}
