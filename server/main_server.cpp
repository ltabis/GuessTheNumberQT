//
// Created by tabis on 10/10/2019.
//

#include <memory>
#include "Exception.hpp"
#include "server.hpp"

int main(int ac, char *av[])
{
    QCoreApplication app(ac, av);
    QCommandLineParser parser;
    QStringList argList;
    bool debug = DEFAULT_DEBUG;
    int port = DEFAULT_PORT;

    QCoreApplication::setApplicationName("GuessTheNumberServer");
    QCoreApplication::setApplicationVersion("1.0");
    parser.setApplicationDescription(QCoreApplication::translate("main","Server program for the GuessTheNumber game network."));
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption dbgOption({"d", "debug"}, {"Debug output [default: off]."});
    QCommandLineOption bdsOption({"b", "bounds"}, {"Bounds X,Y of the number to find [default: [1,100]]."}, {"bounds"});
    QCommandLineOption limOption({"l", "limit"}, {"Number of tries before game over [default: unlimited]."}, {"limit"});

    parser.addOption(dbgOption);
    parser.addOption(bdsOption);
    parser.addOption(limOption);
    parser.process(app);

    try {
        debug = parser.isSet("debug");
        std::unique_ptr<GuessGame::IServer> server(new GuessGame::Server(port, debug));

        server->run();
        if (debug)
            std::cout << "Waiting for a connection ..." << std::endl;
        return app.exec();
    } catch (Log::Exception &exception) {
        exception.debugErrorMessage();
    }
    return 0;
}
