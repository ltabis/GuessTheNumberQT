//
// Created by tabis on 10/10/2019.
//

#include "Client.hpp"
#include "Exception.hpp"

int main(int ac, char **av)
{
    QCoreApplication app(ac, av);

    try {
        GuessGame::Client client(app);
        QObject::connect(&client, &GuessGame::Client::closed, &app, &QCoreApplication::quit);

        return app.exec();
    } catch (Log::Exception &exception) {
        exception.debugErrorMessage();
    }
    return 0;
}