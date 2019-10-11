//
// Created by tabis on 10/10/2019.
//

#include <memory>
#include "Exception.hpp"
#include "Server.hpp"

int main(int ac, char *av[])
{
    QCoreApplication app(ac, av);

    try {
        std::unique_ptr<GuessGame::IServer> server(new GuessGame::Server(app));

        server->run();
        return app.exec();
    } catch (Log::Exception &exception) {
        exception.debugErrorMessage();
    }
    return 0;
}
