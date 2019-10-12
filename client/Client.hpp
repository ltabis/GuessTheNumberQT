//
// Created by tabis on 11/10/2019.
//

#pragma once

#define DEFAULT_DEBUG   false
#define DEFAULT_AUTO    false
#define DEFAULT_PORT    4242
#define DEFAULT_IP      "127.0.0.1"
#define DEFAULT_NAME    "Unknown"

// QUrl(QStringLiteral("ws://localhost:4242")

#include <QtCore>
#include <QtWebSockets/QWebSocket>

namespace GuessGame {
    class Client : public QObject {
    Q_OBJECT
    public:
        explicit Client(QCoreApplication &app, QObject *parent = nullptr);

        void setupClientApp(QCoreApplication &app);
        void assignParametersToClient();
    Q_SIGNALS:
        void closed();

    private Q_SLOTS:
        void onConnected();
        void onTextMessageReceived(QString message);

    private:
        QWebSocket _webSocket;
        QCommandLineParser _appParser;
        QUrl _url;

        bool _debug;
        bool _auto;
        int _port;
        std::string _name;
        std::string _ip;
    };
}