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
    std::unique_ptr<GuessGame::IServer> server(new GuessGame::Server(4242, true));
    QStringList argList;

    QCoreApplication::setApplicationName("GuessTheNumber Server");
    QCoreApplication::setApplicationVersion("1.0");
    parser.setApplicationDescription(QCoreApplication::translate("main","Server program for the GuessTheNumber game network."));
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption dbgOption(QStringList() << "d" << "debug",QCoreApplication::translate("main", "Debug output [default: off]."));
    QCommandLineOption bdsOption(QStringList() << "b" << "bounds",QCoreApplication::translate("main", "Bounds of the number to find [default: [1, 100]]."));
    QCommandLineOption limOption(QStringList() << "l" << "limit",QCoreApplication::translate("main", "Number of tries before game over [default: unlimited]."));
    parser.addOption(dbgOption);
    parser.addOption(bdsOption);
    parser.addOption(limOption);
    parser.process(app);
    try {
        server->run();
        std::cout << "Waiting for a connection ..." << std::endl;
        while (server->getStatus() == GuessGame::IS_RUNNING) {
        }
    } catch (Log::Exception &exception) {
        exception.debugErrorMessage();
    }
    return 0;
}
