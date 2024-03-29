//
// Created by tabis on 11/10/2019.
//

#pragma once

#define DEFAULT_DEBUG   false
#define DEFAULT_AUTO    false
#define DEFAULT_PORT    "4242"
#define DEFAULT_IP      "127.0.0.1"
#define DEFAULT_NAME    "Unknown"

#include <iostream>

#include <QtCore>
#include <QtWebSockets/QWebSocket>

#include "JSONPacket.hpp"

namespace GuessGame {
    class Client : public QObject {
    Q_OBJECT
    public:
        explicit Client(int ac, char *av[], QObject *parent = nullptr);

        void setupClientApp(QCoreApplication &app);
        void assignParametersToClient();
        bool checkIdentification(QJsonObject &json);
        int run() { return _app.exec();};
    Q_SIGNALS:
        void closed();


    private Q_SLOTS:
        void onConnected();
        void onBinaryMessageReceived(const QByteArray &message);

    private:
        unsigned int generateRandomAnswer();

        Data::JSONPacket _packetCreator;
        QCommandLineParser _appParser;
        QWebSocket _webSocket;
        QCoreApplication _app;
        QUrl _url;

        std::pair<unsigned int, unsigned int> _bounds;
        bool _debug;
        bool _auto;
        std::string _port;
        std::string _name;
        std::string _ip;
    };
}