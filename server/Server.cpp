//
// Created by tabis on 10/10/2019.
//

#include "Exception.hpp"
#include "Server.hpp"

/*
 * Constructor
 */
GuessGame::Server::Server(QCoreApplication &app, QObject *parent) :
QObject(parent),
_pWebSocketServer(new QWebSocketServer(QStringLiteral("GuessTheNumber Server"),QWebSocketServer::NonSecureMode, this)),
_status(IS_INACTIVE),
_limit(INFINITE_LIMIT),
_debug(DEFAULT_DEBUG)
{
    setupServerApp(app);
    if (_pWebSocketServer->listen(QHostAddress::Any, DEFAULT_PORT)) {
        if (_debug)
            qDebug() << "[Server] Socket has been setuped correctly.";
        connect(_pWebSocketServer, &QWebSocketServer::newConnection,this, &Server::onNewConnection);
        connect(_pWebSocketServer, &QWebSocketServer::closed, this, &Server::closed);
    } else
        throw Log::Exception("Server web socket hasn't been initialised properly", "Server constructor");
}

/*
 * Destructor
 */
GuessGame::Server::~Server()
{
    _pWebSocketServer->close();
    qDeleteAll(_clients.begin(), _clients.end());
}

/*
 * Run the server
 */
void GuessGame::Server::run()
{
    if (_debug)
        qDebug() << "[Server] Server started, waiting for clients ...";
    _status = IS_RUNNING;
}

/*
 * Stop the server
 */
int GuessGame::Server::stop()
{
    if (_debug)
        qDebug() << "[Server] The server has been stopped.";
    if (_status >= IS_INACTIVE)
        return IS_INACTIVE;
    return _status;
}

/*
 * Setup all the parameters passed in the app
 */
void GuessGame::Server::setupServerApp(QCoreApplication &app)
{
    QCoreApplication::setApplicationName("GuessTheNumberServer");
    QCoreApplication::setApplicationVersion("1.0");
    _appParser.setApplicationDescription({"Server program for the GuessTheNumber game network."});
    _appParser.addHelpOption();
    _appParser.addVersionOption();
    QCommandLineOption dbgOption({"d", "debug"}, {"Debug output [default: off]."});
    QCommandLineOption bdsOption({"b", "bounds"}, {"Bounds X,Y of the number to find [default: [1,100]]."}, {"bounds"});
    QCommandLineOption limOption({"l", "limit"}, {"Number of tries before game over [default: unlimited]."}, {"limit"});

    _appParser.addOption(dbgOption);
    _appParser.addOption(bdsOption);
    _appParser.addOption(limOption);
    _appParser.process(app);
    assignParametersToClient();
}

void GuessGame::Server::assignParametersToClient()
{
    bool debug = _appParser.isSet("debug");
    QStringList list = _appParser.values("bounds");
    int limit = _appParser.value("limit").toInt();

    _debug = debug ? debug : _debug;
    if (list.size() == 2 || list.empty()) {
        _bounds.first = list.size() == 2 ? list.at(0).toInt() : DEFAULT_BOUNDX;
        _bounds.second = list.size() == 2 ? list.at(1).toInt() : DEFAULT_BOUNDY;
    }
    _limit = limit > 0 ? _limit : limit;
    if (_bounds.first >= _bounds.second || _bounds.first <= 0 || _bounds.second <= 0 || (list.size() != 2 && !list.empty()))
        throw Log::Exception("Bad formatted bounds", "setupServerApp");
    if (_debug) {
        qDebug() << "[Server] Server options parsed.";
        qDebug() << "[Server] debug activated.";
        qDebug() << "[Server] bounds : [" << _bounds.first << "," << _bounds.second << "]";
        qDebug() << "[Server] limit : [" << (_limit ? std::to_string(_limit).c_str() : "Infinite") << "]";
    }
}

/*
 * Callback when the server receive a new connexion request from a client
 */
void GuessGame::Server::onNewConnection()
{
    QWebSocket *pSocket = _pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &Server::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &Server::socketDisconnected);
    _clients << pSocket;
    QByteArray obj = _packetCreator.createJSONPacket(QList<QList<std::string>>({{CONFIRM_CONNECTION, "Ok"}, {TURN_MESSAGE, "no"}}));
    if (_debug) {
        qDebug() << "[Server] A new client as just connected !";
        qDebug() << "[Server] sending confirmed connection" << obj;
    }
    pSocket->sendBinaryMessage(obj);
}

/*
 *  Send a byte array to the client
 */
void GuessGame::Server::processBinaryMessage(const QByteArray &message)
{
    bool startOfGame = false;
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    QJsonObject packet = _packetCreator.UnpackToJson(message);

    if (_debug)
        qDebug() << "[Server] Binary Message received :" << message;
    for (auto &it : packet.keys())
        if (it.toStdString() == CONNEXION_REQUEST) {
            handleClients(packet[CONNEXION_REQUEST].toString().toStdString(), pClient);
            startOfGame = true;
        }
    handleGames(packet, pClient, startOfGame);
}

void GuessGame::Server::handleClients(const std::string &name, QWebSocket *pClient)
{
    _manager.addPlayer(name, _clients.indexOf(pClient));
}

void GuessGame::Server::handleGames(const QJsonObject &data, QWebSocket *pClient, bool startOfGame)
{
    if (startOfGame) {
        std::string format("Welcome to the game ! Guess the number between " +
        std::to_string(_bounds.first) + " and " + std::to_string(_bounds.second) + ". It is your turn, you have" +
        (_limit ? std::to_string(_limit) + " tries left." : " unlimited tries."));
        QByteArray message = _packetCreator.createJSONPacket(QList<QList<std::string>>({{INFO_MESSAGE, format}, {TURN_MESSAGE, "yes"}}));
        pClient->sendBinaryMessage(message);
    } else {
        QByteArray message = _packetCreator.createJSONPacket(QList<QList<std::string>>({{INFO_MESSAGE, "It is your turn, you have" +
        (_limit ? std::to_string(_limit) + " tries left." : " unlimited tries.")}, {TURN_MESSAGE, "yes"}}));
        pClient->sendBinaryMessage(message);
    }
}

/*
 * Delete client from our list when disconnected
 */
void GuessGame::Server::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

    if (_debug)
        qDebug() << "[Server] socket disconnected :" << pClient;
    if (pClient) {
        _clients.removeAll(pClient);
        pClient->deleteLater();
    }
}