//
// Created by tabis on 11/10/2019.
//

#include "Client.hpp"

#include <QtCore/QDebug>

GuessGame::Client::Client(QCoreApplication &app, QObject *parent) :
QObject(parent),
_debug(DEFAULT_DEBUG),
_auto(DEFAULT_AUTO),
_port(DEFAULT_PORT),
_ip(DEFAULT_IP),
_name(DEFAULT_NAME)
{
    setupClientApp(app);
    std::string urlString("ws://" + _ip + ":" + _port);
    _url = QUrl(urlString.c_str());
    if (_debug)
        qDebug() << "[Client] WebSocket server:" << _url;
    connect(&_webSocket, &QWebSocket::connected, this, &Client::onConnected);
    connect(&_webSocket, &QWebSocket::disconnected, this, &Client::closed);
    _webSocket.open(QUrl(_url));
}

/*
 * Setup all the parameters passed in the app
 */
void GuessGame::Client::setupClientApp(QCoreApplication &app)
{
    QCoreApplication::setApplicationName("GuessTheNumberClient");
    QCoreApplication::setApplicationVersion("1.0");
    _appParser.setApplicationDescription({"Client program for the GuessTheNumber game."});
    _appParser.addVersionOption();
    QCommandLineOption dbgOption({"d", "debug"}, {"Debug output [default: off]."});
    QCommandLineOption hostOption({"h", "host"}, {"IP of the server [default: 127.0.0.1]."}, {"ip"});
    QCommandLineOption portOption({"p", "port"}, {"Port to connect to [default: 4242]."}, {"port"});
    QCommandLineOption autoOption({"a", "auto"}, {"Solve the game automatically."});
    QCommandLineOption nameOption({"n", "name"}, {"Client identity."}, {"name"});

    _appParser.addOption(dbgOption);
    _appParser.addOption(hostOption);
    _appParser.addOption(portOption);
    _appParser.addOption(autoOption);
    _appParser.addOption(nameOption);
    _appParser.process(app);
    assignParametersToClient();
}

void GuessGame::Client::assignParametersToClient()
{
    bool debug = _appParser.isSet("debug");
    bool autoVal = _appParser.isSet("auto");
    std::string port = _appParser.value("port").toStdString();
    std::string name = _appParser.value("name").toStdString();
    std::string host = _appParser.value("host").toStdString();

    _debug = debug ? debug : _debug;
    _auto = autoVal ? autoVal : _auto;
    _port = !port.empty() ? port : _port;
    _name = !name.empty() ? name : _name;
    _ip = !host.empty() ? host : _ip;
    if (_debug) {
        qDebug() << "[Client] Client options parsed.";
        qDebug() << "[Client] debug activated.";
        qDebug() << "[Client] auto mode : [" << (_auto ? "on" : "off") << "]";
        qDebug() << "[Client] port : [" << _port.c_str() << "]";
        qDebug() << "[Client] Player name : [" << _name.c_str() << "]";
        qDebug() << "[Client] host : [" << _ip.c_str() << "]";
    }
}

void GuessGame::Client::onConnected()
{
    if (_debug)
        qDebug() << "[Client] WebSocket connected.";
    connect(&_webSocket, &QWebSocket::binaryMessageReceived,
            this, &Client::onBinaryMessageReceived);
}

void GuessGame::Client::onBinaryMessageReceived(const QByteArray &message)
{
    QJsonObject json = _packetCreator.UnpackToJson(message);
    QString answer;
    std::string tmp;


    if (checkIdentification(json) || json[TURN_MESSAGE].toArray()[0].toString().toStdString() == "no")
        return;
    else
        std::cout << json[INFO_MESSAGE].toString().toStdString() << std::endl;
    if (json[TURN_MESSAGE].toArray()[0].toString().toStdString() == WON_MESSAGE) {
        _webSocket.close();
        return;
    }
    while (!answer.toInt()) {
        std::cout << "Please input your number : ";
        std::cin >> tmp;
        answer = tmp.c_str();
    }
    _webSocket.sendBinaryMessage(_packetCreator.createJSONPacket(QList<QList<std::string>>({{NAME_MESSAGE, _name}, {ANSWER_MESSAGE, answer.toStdString()}})));
}

bool GuessGame::Client::checkIdentification(QJsonObject &json)
{
    if (json[CONFIRM_CONNECTION].toArray()[0].toString() == "Ok") {
        QByteArray message = _packetCreator.createJSONIdentificationPacket(_name);
        if (_debug)
            qDebug() << "[Client] Message received." << json[CONFIRM_CONNECTION].toArray()[0].toString();
        std::cout << "Server says : connection successful." << std::endl;
        _webSocket.sendBinaryMessage(message);
        return true;
    } else {
        if (_debug)
            qDebug() << "[Client] Message received." << json[INFO_MESSAGE];
        std::cout << "Server says : " << json[INFO_MESSAGE].toArray()[0].toString().toStdString() << std::endl;
    }
    return false;
}
