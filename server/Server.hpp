//
// Created by tabis on 10/10/2019.
//

#pragma once

#include <map>
#include <chrono>
#include <utility>
#include <iostream>

#include <QtCore/QList>
#include <QtWebSockets>
#include <QtCore/QObject>
#include <QtCore/QByteArray>

#include "Player.hpp"
#include "IServer.hpp"
#include "JSONPacket.hpp"

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

namespace GuessGame {
    class Server : public QObject, public IServer {
    Q_OBJECT
    public:
        /// \brief constructor
        /// \param bound_x minimum bound value
        /// \param bound_y maximum bound value
        /// \param limit limit of turn for the player
        Server(int ac, char *av[], QObject *parent = nullptr);
        ~Server();

        // Inheritance methods
        /// \brief runs the server
        int run() override;
        /// \brief stops the server
        /// \return the code that made the server stop
        int stop() override;
        /// \brief server status
        /// Gets the status of the server
        /// \return the status code of the server
        int getStatus() const override {return _status;};

        // Regular methods
        void setupServerApp(QCoreApplication &app);
        void assignParametersToClient();

    Q_SIGNALS:
        void closed();

    private slots:
        void onNewConnection();
        void processBinaryMessage(const QByteArray &message);
        void socketDisconnected();

    private:
        void handleClients(const std::string &name, QWebSocket *pClient);
        void handleGames(const QJsonObject &data, QWebSocket *pClient, bool startOfGame = false);
        void checkIfWin(const QJsonObject &data, QWebSocket *pClient);
        void checkDistance(const QJsonObject &data, QWebSocket *pClient, unsigned int clientIdx);
        void deletePlayer(unsigned int clientIdx, QWebSocket *pClient);
        void displayPlayerScore(unsigned int clientIdx, QWebSocket *pClient);
        void writePlayerScoreInConfigFile(unsigned int clientIdx, int status);
        GameManager _manager;

        QWebSocketServer *_pWebSocketServer;
        Data::JSONPacket _packetCreator;
        QCommandLineParser _appParser;
        QList<QWebSocket *> _clients;
        QJsonArray _playersConfig;
        QCoreApplication _app;

        std::pair<unsigned int, unsigned int> _bounds;
        unsigned int _limit;
        e_code _status;
        bool _debug;
    };
}
