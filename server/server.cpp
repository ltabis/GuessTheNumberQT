//
// Created by tabis on 10/10/2019.
//

#include "server.hpp"

/*
 * Constructors
 */
GuessGame::Server::Server(unsigned int bound_x, unsigned int bound_y, unsigned int limit) : _bounds(std::make_pair(bound_x, bound_y)), _limit(limit), _status(IS_INACTIVE) {}
GuessGame::Server::Server(const std::pair<unsigned int, unsigned int> &bounds, unsigned int limit) : _bounds(bounds), _limit(limit), _status(IS_INACTIVE) {}
GuessGame::Server::Server(int ac, const char * const *av) : _status(IS_INACTIVE), _bounds(std::make_pair(0, 0)), _limit(0) {}

GuessGame::Server::Server(quint16 port, bool debug, QObject *parent) :
_pWebSocketServer(new QWebSocketServer(QStringLiteral("GuessTheNumber Server"),QWebSocketServer::NonSecureMode, this)),
_status(IS_INACTIVE),
_bounds(std::make_pair(0, 0)),
_limit(0),
_debug(debug)
{
    if (_pWebSocketServer->listen(QHostAddress::Any, 4242)) {
        connect(_pWebSocketServer, &QWebSocketServer::newConnection,this, &Server::onNewConnection);
        connect(_pWebSocketServer, &QWebSocketServer::closed, this, &Server::closed);
    }
}

/*
 * Run the server
 */
void GuessGame::Server::run()
{
    std::cout << "Server started ..." << std::endl;
    _status = IS_RUNNING;
}

/*
 * Stop the server
 */
int GuessGame::Server::stop()
{
    if (_status >= IS_INACTIVE)
        return IS_INACTIVE;
    return _status;
}

/*
 * Callback when the server receive a new connexion request from a client
 */
void GuessGame::Server::onNewConnection()
{
    QWebSocket *pSocket = _pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &Server::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &Server::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &Server::socketDisconnected);

    _clients << pSocket;
}

/*
 *
 */
void GuessGame::Server::processTextMessage(QString message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (_debug)
        qDebug() << "Message received:" << message;
    if (pClient)
        pClient->sendTextMessage(message);
}

void GuessGame::Server::processBinaryMessage(QByteArray message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (_debug)
        qDebug() << "Binary Message received:" << message;
    if (pClient) {
        pClient->sendBinaryMessage(message);
    }
}

void GuessGame::Server::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (_debug)
        qDebug() << "socketDisconnected:" << pClient;
    if (pClient) {
        _clients.removeAll(pClient);
        pClient->deleteLater();
    }
}
