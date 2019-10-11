//
// Created by tabis on 10/10/2019.
//

#pragma once

#include <utility>
#include <iostream>

#include <QtWebSockets>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>

#include "IServer.hpp"

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

#define DEFAULT_DEBUG   false
#define DEFAULT_PORT    4242

namespace GuessGame {
    class Server : public QObject, public IServer {
    Q_OBJECT
    public:
        /// \brief constructor
        /// \param bound_x minimum bound value
        /// \param bound_y maximum bound value
        /// \param limit limit of turn for the player
//        Server(unsigned int bound_x, unsigned int bound_y, unsigned int limit);
//        Server(const std::pair<unsigned int, unsigned int> &bounds, unsigned int limit);
//        Server(int ac, const char * const *av);
        Server(quint16 port, bool debug = false, QObject *parent = nullptr, std::pair<unsigned int, unsigned int > bounds = std::make_pair(1, 100));
        ~Server();

        // Inheritance methods
        /// \brief runs the server
        void run() override;
        /// \brief stops the server
        /// \return the code that made the server stop
        int stop() override;
        /// \brief server status
        /// Gets the status of the server
        /// \return the status code of the server
        int getStatus() const override {return _status;};

    Q_SIGNALS:
        void closed();

    private slots:
        void onNewConnection();
        void processTextMessage(QString message);
        void processBinaryMessage(QByteArray message);
        void socketDisconnected();

        // Server methods
    private:
        QWebSocketServer *_pWebSocketServer;
        QList<QWebSocket *> _clients;
        QCommandLineParser _appParser;

        std::pair<unsigned int, unsigned int> _bounds;
        unsigned int _limit;
        e_code _status;
        // to replace with debug object
        bool _debug;
    };
}
