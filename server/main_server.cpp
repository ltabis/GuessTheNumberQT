//
// Created by tabis on 10/10/2019.
//

#include <memory>
#include "Exception.hpp"
#include "server.hpp"

int main(int ac, const char * const *av)
{
    try {
        std::unique_ptr<GuessGame::IServer> server(new GuessGame::Server(ac, av));
        server->run();
        while (server->getStatus() == GuessGame::IS_RUNNING)
            std::cout << "Server running ..." << std::endl;
    } catch (Log::Exception &exception) {
        exception.debugErrorMessage();
    }
    return 0;
}