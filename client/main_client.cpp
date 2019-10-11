//
// Created by tabis on 10/10/2019.
//

#include "Client.hpp"
#include "Exception.hpp"
#include <QApplication>

int main(int argc, char **argv)
{
    QCoreApplication a(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("GuessTheNumber Client");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption dbgOption(QStringList() << "d" << "debug",{"Debug output [default: off]."});

    parser.addOption(dbgOption);
    parser.process(a);

    bool debug = parser.isSet(dbgOption);

    try {
        // hardcoded, to change
        GuessGame::Client client(QUrl(QStringLiteral("ws://localhost:4242")), debug);
        QObject::connect(&client, &GuessGame::Client::closed, &a, &QCoreApplication::quit);
        QCoreApplication app(argc, argv);

        return app.exec();
    } catch (Log::Exception &exception) {
        exception.debugErrorMessage();
    }
    return 0;
}