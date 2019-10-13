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
        _playersConfig = _packetCreator.getJSONFromFile(DEFAULT_CONFIG_FILE);
        if (_debug) {
            qDebug() << "[Server] Socket has been setuped correctly.";
            qDebug() << "[Server] Player history loaded : " << _playersConfig;
        }
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
    _limit = limit > 0 ? limit : _limit;
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
    QByteArray obj = _packetCreator.createJSONPacket(
    QList<QList<std::string>>({{CONFIRM_CONNECTION, "Ok"},
                                    {TURN_MESSAGE, "no"},
                                    {BOUND_MESSAGE, std::to_string(_bounds.first), std::to_string(_bounds.second)}}));
    if (_debug) {
        qDebug() << "[Server] A new client as just connected !";
        qDebug() << "[Server] sending confirmed connection.";
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
    std::seed_seq seed{std::chrono::system_clock::now().time_since_epoch().count()};
    QRandomGenerator generator(seed);
    unsigned int nbrToFind = generator.bounded(_bounds.first, _bounds.second);

    if (_debug)
        qDebug() << "[Server] number to find : " << nbrToFind;
    _manager.addPlayer(name, _clients.indexOf(pClient), nbrToFind, _limit);
}

void GuessGame::Server::handleGames(const QJsonObject &data, QWebSocket *pClient, bool startOfGame)
{
    if (startOfGame) {
        std::string format("Welcome to the game ! Guess the number between " +
        std::to_string(_bounds.first) + " and " + std::to_string(_bounds.second) + ". It is your turn, you have " +
        (_limit ? std::to_string(_limit) + " tries left." : " unlimited tries."));
        QByteArray message = _packetCreator.createJSONPacket(
        QList<QList<std::string>>({{INFO_MESSAGE, format}, {TURN_MESSAGE, "yes"}}));
        pClient->sendBinaryMessage(message);
    } else
        checkIfWin(data, pClient);
}

/*
 * Delete client from our list when disconnected
 */
void GuessGame::Server::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    int clientIdx = _clients.indexOf(pClient);

    if (_debug)
        qDebug() << "[Server] socket disconnected :" << pClient;
    if (pClient && clientIdx >= 0)
        deletePlayer(clientIdx, pClient, 0);
}

void GuessGame::Server::checkIfWin(const QJsonObject &data, QWebSocket *pClient)
{
    unsigned int clientIdx = _clients.indexOf(pClient);

    if (!_manager.getPlayerTriesLeftAtIndex(clientIdx) && _limit) {
        QByteArray message = _packetCreator.createJSONPacket(QList<QList<std::string>>(
        {{INFO_MESSAGE, "You've lost... The number was " +
        std::to_string(_manager.getPlayerNumberToFindAtIndex(clientIdx)) + "."}, {TURN_MESSAGE, "no"}}));
        pClient->sendBinaryMessage(message);
        deletePlayer(clientIdx, pClient, 1);
    } else if (data[ANSWER_MESSAGE].toArray()[0].toString().toInt() == _manager.getPlayerNumberToFindAtIndex(clientIdx)) {
        displayPlayerScore(clientIdx, pClient);
        deletePlayer(clientIdx, pClient, 2);
    } else
        checkDistance(data, pClient, clientIdx);
}

void GuessGame::Server::checkDistance(const QJsonObject &data, QWebSocket *pClient, unsigned int clientIdx)
{
    unsigned int triesLeft = _manager.getPlayerTriesLeftAtIndex(clientIdx);
    bool isInfinite = _manager.getPlayerInfiniteAtIndex(clientIdx);

    if (data[ANSWER_MESSAGE].toArray()[0].toString().toInt() > _manager.getPlayerNumberToFindAtIndex(clientIdx)) {
        QByteArray message = _packetCreator.createJSONPacket(QList<QList<std::string>>(
        {{INFO_MESSAGE, "Less " +
        (isInfinite ? "!" : "! (" + std::to_string(triesLeft - 1) + " tries left.)")}, {TURN_MESSAGE, "yes"}}));
        pClient->sendBinaryMessage(message);
    } else if (data[ANSWER_MESSAGE].toArray()[0].toString().toInt() < _manager.getPlayerNumberToFindAtIndex(clientIdx)) {
        QByteArray message = _packetCreator.createJSONPacket(QList<QList<std::string>>(
        {{INFO_MESSAGE, "More " +
        (isInfinite ? "!" : "! (" + std::to_string(triesLeft - 1) + " tries left.)")}, {TURN_MESSAGE, "yes"}}));
        pClient->sendBinaryMessage(message);
    }
    _manager.subTriesOfPlayerAtIndex(clientIdx);
}

void GuessGame::Server::deletePlayer(unsigned int clientIdx, QWebSocket *pClient, int status)
{
    if (_manager.getPlayerNameAtIndex(clientIdx) != DEFAULT_CLIENT_NAME) {
        QJsonObject playerConfig =_packetCreator.createPlayerConfig(_manager.getPlayerNameAtIndex(clientIdx),
                                          _manager.getPlayerTriesLeftAtIndex(clientIdx),
                                          status,
                                          _bounds,
                                          _manager.getPlayerDateAtIndex(clientIdx),
                                          _limit);
        _playersConfig.push_back(playerConfig);
    }
    _packetCreator.writeJSONToFile(DEFAULT_CONFIG_FILE, _playersConfig);
    _manager.deletePlayerAtIndex(clientIdx);
    _clients.removeAll(pClient);
    pClient->deleteLater();
}

void GuessGame::Server::displayPlayerScore(unsigned int clientIdx, QWebSocket *pClient)
{
    QByteArray message;
    std::string format;
    std::vector<unsigned int> scores;

    for (unsigned int i = 0; i < _playersConfig.size(); ++i)
        if (_playersConfig[i].toObject()[NAME_MESSAGE].toString().toStdString() ==
        _manager.getPlayerNameAtIndex(clientIdx))
            scores.push_back(_playersConfig[i].toObject()["score"].toString().toInt());
    std::sort(scores.begin(), scores.end(), std::greater<unsigned int>());
    if (_manager.getPlayerNameAtIndex(clientIdx) != DEFAULT_CLIENT_NAME)
        for (unsigned int i = 0; i < 5 && i < scores.size(); ++i)
            format += (std::to_string(i + 1) + " : " + std::to_string(scores.at(i)) + "\n");
    message = _packetCreator.createJSONPacket(QList<QList<std::string>>(
    {{INFO_MESSAGE, "You've won! The number was indeed " +
    std::to_string(_manager.getPlayerNumberToFindAtIndex(clientIdx)) + ".\n" + format},
    {TURN_MESSAGE, END_MESSAGE}}));
    pClient->sendBinaryMessage(message);
}
